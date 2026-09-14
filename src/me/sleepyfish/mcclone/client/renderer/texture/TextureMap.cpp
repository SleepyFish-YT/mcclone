//
// Created by SleepyFish on 14.09.2026.
// Project: mcclone
//

#include "TextureMap.h"

#include "TextureAtlasSprite.h"
#include "TextureUtil.h"
#include "IIconCreator.h"
#include "Stitcher.h"
#include "DynamicTexture.h"
#include "../StitcherException.h"
#include "../../resources/IResourceManager.h"
#include "../../resources/IResource.h"
#include "../../resources/data/TextureMetadataSection.h"
#include "../../resources/data/AnimationMetadataSection.h"
#include "../../../util/ResourceLocation.h"
#include "../../../util/MathHelper.h"
#include "../../../util/ReportedException.h"
#include "../../../debug/Logger.h"
#include "../GlStateManager.h"

#include "../../../../sava/BufferedImage.h"

#include <algorithm>
#include <format>
#include <any>

const ResourceLocation TextureMap::LOCATION_MISSING_TEXTURE("missingno");
const ResourceLocation TextureMap::LOCATION_BLOCKS_TEXTURE("textures/atlas/blocks.png");

TextureMap::TextureMap(const std::string& basePath) :
    TextureMap(basePath, nullptr)
{}

TextureMap::TextureMap(const std::string& basePath, IIconCreator* iconCreator) :
    basePath(basePath),
    iconCreator(iconCreator),
    missingImage(new TextureAtlasSprite("missingno"))
{}

void TextureMap::initMissingImage() {
    const std::vector<int>& missingData = TextureUtil::missingTexture->getTextureData();

    this->missingImage->setIconWidth(16);
    this->missingImage->setIconHeight(16);

    std::vector<std::vector<std::vector<int>>> frames;
    std::vector<std::vector<int>> mipData(this->mipmapLevels + 1);
    mipData[0] = missingData;
    frames.push_back(mipData);

    this->missingImage->setFramesTextureData(frames);
}

ResourceLocation TextureMap::completeResourceLocation(const ResourceLocation& location, int mipmapLevel) {
    if (mipmapLevel == 0) {
        return ResourceLocation(
                location.getResourceDomain(),
                std::format("{}/{}{}", this->basePath, location.getResourcePath(), ".png")
        );
    } else {
        return ResourceLocation(
                location.getResourceDomain(),
                std::format("{}/mipmaps/{}.{}{}", this->basePath, location.getResourcePath(), mipmapLevel, ".png")
        );
    }
}

// -------------------------------------------------------------------------
// Public API
// -------------------------------------------------------------------------

void TextureMap::loadTexture(IResourceManager& resourceManager) {
    if (this->iconCreator != nullptr) {
        this->loadSprites(resourceManager, this->iconCreator);
    }
}

void TextureMap::loadSprites(IResourceManager& resourceManager, IIconCreator* iconCreator) {
    this->mapRegisteredSprites.clear();
    iconCreator->registerSprites(*this);
    this->initMissingImage();
    this->deleteGlTexture();
    this->loadTextureAtlas(resourceManager);
}

void TextureMap::loadTextureAtlas(IResourceManager& resourceManager) {
    const int maxSize = GlStateManager::_getGLMaximumTextureSize();
    Stitcher stitcher(maxSize, maxSize, true, 0, this->mipmapLevels);

    this->mapUploadedSprites.clear();
    this->listAnimatedSprites.clear();

    int minSpriteSize = INT_MAX;
    int maxMipDivisor = 1 << this->mipmapLevels;

    for (auto& [name, sprite] : this->mapRegisteredSprites) {
        ResourceLocation location(sprite->getIconName());
        ResourceLocation fullLocation = this->completeResourceLocation(location, 0);

        try {
            auto iResource = resourceManager.getResource(fullLocation);
            auto inputStream = iResource->getInputStream();

            int w0, h0;
            std::vector<std::vector<int>> mipImages(1 + this->mipmapLevels);
            mipImages[0] = TextureUtil::readImageData_(*inputStream, w0, h0);

            // texture metadata
            std::any texMeta = iResource->getMetadata("texture");
            if (texMeta.has_value()) {
                auto* texSection = std::any_cast<TextureMetadataSection>(&texMeta);
                if (texSection) {
                    const auto& mipmapList = texSection->getListMipmaps();
                    if (!mipmapList.empty()) {
                        if (MathHelper::roundUpToPowerOfTwo(w0) != w0 ||
                            MathHelper::roundUpToPowerOfTwo(h0) != h0) {
                            throw std::runtime_error("Unable to load extra miplevels, source-texture is not power of two");
                        }
                    }

                    for (int mipLevel : mipmapList) {
                        if (mipLevel > 0 && mipLevel < (int)mipImages.size() - 1 && mipImages[mipLevel].empty()) {
                            ResourceLocation mipLocation = this->completeResourceLocation(location, mipLevel);
                            try {
                                auto mipResource = resourceManager.getResource(mipLocation);
                                auto mipStream   = mipResource->getInputStream();
                                int mw, mh;
                                mipImages[mipLevel] = TextureUtil::readImageData_(*mipStream, mw, mh);
                            } catch (std::ios_base::failure& e) {
                                Logger::error("Unable to load miplevel {} from: {} ({})", mipLevel, mipLocation.toString(), e.what());
                            }
                        }
                    }
                }
            }

            // animation metadata
            std::any animMeta = iResource->getMetadata("animation");
            AnimationMetadataSection* animSection = nullptr;
            if (animMeta.has_value())
                animSection = std::any_cast<AnimationMetadataSection>(&animMeta);

            std::vector<BufferedImage> images;
            for (int mipIdx = 0; mipIdx < (int)mipImages.size(); ++mipIdx)
                images.emplace_back(mipImages[mipIdx], w0 >> mipIdx, h0 >> mipIdx);

            std::vector<BufferedImage*> ptrs;
            for (auto& img : images) ptrs.push_back(&img);

            sprite->loadSprite(ptrs.data(), (int)ptrs.size(), animSection);

        } catch (std::runtime_error& e) {
            Logger::error("Unable to parse metadata from {} ({})", fullLocation.toString(), e.what());
            continue;
        } catch (std::ios_base::failure& e) {
            Logger::error("Using missing texture, unable to load {} ({})", fullLocation.toString(), e.what());
            continue;
        }

        minSpriteSize = std::min(minSpriteSize, std::min(sprite->getIconWidth(), sprite->getIconHeight()));

        int lowestOneBit = std::min(
                sprite->getIconWidth()  & -sprite->getIconWidth(),
                sprite->getIconHeight() & -sprite->getIconHeight()
        );

        if (lowestOneBit < maxMipDivisor) {
            Logger::warn(
                    "Texture {} with size {}x{} limits mip level from {} to {}",
                    fullLocation.toString(),
                    sprite->getIconWidth(), sprite->getIconHeight(),
                    MathHelper::calculateLogBaseTwo(maxMipDivisor),
                    MathHelper::calculateLogBaseTwo(lowestOneBit)
            );
            maxMipDivisor = lowestOneBit;
        }

        stitcher.addSprite(sprite);
    }

    // clamp mipmap levels
    int effectiveMip = MathHelper::calculateLogBaseTwo(std::min(minSpriteSize, maxMipDivisor));
    if (effectiveMip < this->mipmapLevels) {
        Logger::warn(
                "{}: dropping miplevel from {} to {}, because of minimum power of two: {}",
                this->basePath, this->mipmapLevels, effectiveMip,
                std::min(minSpriteSize, maxMipDivisor)
        );
        this->mipmapLevels = effectiveMip;
    }

    // generate mipmaps
    for (auto& [name, sprite] : this->mapRegisteredSprites) {
        try {
            sprite->generateMipmaps(this->mipmapLevels);
        } catch (std::exception& e) {
            throw ReportedException(
                    "Applying mipmap to " + sprite->getIconName() + ": " + e.what()
            );
        }
    }

    this->missingImage->generateMipmaps(this->mipmapLevels);
    stitcher.addSprite(this->missingImage);

    try {
        stitcher.doStitch();
    } catch (StitcherException& e) {
        throw;
    }

    Logger::log("Created: {}x{} {}-atlas",
                 stitcher.getCurrentWidth(), stitcher.getCurrentHeight(), this->basePath);

    TextureUtil::allocateTextureImpl_(
            this->getGlTextureId(),
            this->mipmapLevels,
            stitcher.getCurrentWidth(),
            stitcher.getCurrentHeight()
    );

    // upload stitched sprites
    std::unordered_map<std::string, TextureAtlasSprite*> remaining = this->mapRegisteredSprites;

    for (TextureAtlasSprite* sprite : stitcher.getStitchSlots()) {
        const std::string& spriteName = sprite->getIconName();
        remaining.erase(spriteName);
        this->mapUploadedSprites[spriteName] = sprite;

        try {
            TextureUtil::uploadTextureMipmap_(
                    sprite->getFrameTextureData(0),
                    sprite->getIconWidth(),
                    sprite->getIconHeight(),
                    sprite->getOriginX(),
                    sprite->getOriginY(),
                    false, false
            );
        } catch (std::exception& e) {
            throw ReportedException(
                    "Stitching texture atlas " + this->basePath + " sprite " + spriteName + ": " + e.what()
            );
        }

        if (sprite->hasAnimationMetadata()) {
            this->listAnimatedSprites.push_back(sprite);
        }
    }

    // copy missing image into unstitched sprites
    for (auto& [name, sprite] : remaining) {
        sprite->copyFrom(*this->missingImage);
    }
}

TextureAtlasSprite* TextureMap::getAtlasSprite(const std::string& iconName) {
    auto it = this->mapUploadedSprites.find(iconName);
    return it != this->mapUploadedSprites.end() ? it->second : this->missingImage;
}

TextureAtlasSprite* TextureMap::registerSprite(const ResourceLocation& location) {
    if (location.toString().empty())
        throw std::invalid_argument("Location cannot be null!");

    auto it = this->mapRegisteredSprites.find(location.toString());
    if (it != this->mapRegisteredSprites.end())
        return it->second;

    TextureAtlasSprite* sprite = TextureAtlasSprite::makeAtlasSprite(location);
    this->mapRegisteredSprites[location.toString()] = sprite;
    return sprite;
}

TextureAtlasSprite* TextureMap::getMissingSprite() {
    return this->missingImage;
}

void TextureMap::updateAnimations() {
    TextureUtil::bindTexture_(this->getGlTextureId());
    for (TextureAtlasSprite* sprite : this->listAnimatedSprites)
        sprite->updateAnimation();
}

void TextureMap::tick() {
    this->updateAnimations();
}

void TextureMap::setMipmapLevels(int levels) {
    this->mipmapLevels = levels;
}
