//
// Created by SleepyFish on 11.09.2026.
// Project: mcclone
//

// not FULLY implemented yet.

#include "TextureAtlasSprite.h"

#include "../../../util/ResourceLocation.h"
#include "../../resources/data/AnimationMetadataSection.h"
#include "../../resources/data/AnimationFrame.h"
#include "../../../../sava/BufferedImage.h"
#include "TextureUtil.h"
// #include "CrashReport.h"
#include "../../../util/ReportedException.h"
#include "TextureClock.h"
#include "TextureCompass.h"

#include <algorithm>
#include <stdexcept>

std::string TextureAtlasSprite::locationNameClock   = "builtin/clock";
std::string TextureAtlasSprite::locationNameCompass = "builtin/compass";

TextureAtlasSprite::TextureAtlasSprite(const std::string& spriteName) :
    iconName(spriteName)
{}

TextureAtlasSprite* TextureAtlasSprite::makeAtlasSprite(const ResourceLocation& loc) {
    std::string s = loc.toString();
    if (s == locationNameClock)   return new TextureClock(s);
    if (s == locationNameCompass) return new TextureCompass(s);
    return new TextureAtlasSprite(s);
}

void TextureAtlasSprite::setLocationNameClock(const std::string& clockName) {
    locationNameClock = clockName;
}
void TextureAtlasSprite::setLocationNameCompass(const std::string& compassName) {
    locationNameCompass = compassName;
}

void TextureAtlasSprite::initSprite(int inX, int inY, int originInX, int originInY, bool rotatedIn) {
    originX = originInX;
    originY = originInY;
    rotated = rotatedIn;

    const float f  = (double) 0.01f / static_cast<float>(inX);
    const float f1 = (double) 0.01f / static_cast<float>(inY);

    minU = static_cast<float>(originInX) / static_cast<float>(inX) + f;
    maxU = static_cast<float>(originInX + width)  / static_cast<float>(inX) - f;
    minV = static_cast<float>(originInY) / static_cast<float>(inY) + f1;
    maxV = static_cast<float>(originInY + height) / static_cast<float>(inY) - f1;
}

void TextureAtlasSprite::copyFrom(const TextureAtlasSprite& a) {
    originX = a.originX;
    originY = a.originY;
    width = a.width;
    height = a.height;
    rotated = a.rotated;
    minU = a.minU; maxU = a.maxU;
    minV = a.minV; maxV = a.maxV;
    glSpriteTextureId = a.glSpriteTextureId;
    indexInMap = a.indexInMap;
}

void TextureAtlasSprite::updateAnimation() {
    ++tickCounter;

    if (tickCounter >= animationMetadata->getFrameTimeSingle(frameCounter)) {
        int i = animationMetadata->getFrameIndex(frameCounter);
        int j = (animationMetadata->getFrameCount() == 0)
                ? static_cast<int>(framesTextureData.size())
                : animationMetadata->getFrameCount();

        frameCounter = (frameCounter + 1) % j;
        tickCounter  = 0;
        int k = animationMetadata->getFrameIndex(frameCounter);

        if (i != k && k >= 0 && k < static_cast<int>(framesTextureData.size())) {
            TextureUtil::uploadTextureMipmap_(framesTextureData[k], width, height, originX, originY, false, false);
        }
    } else if (animationMetadata->isInterpolate()) {
        updateAnimationInterpolated();
    }
}

void TextureAtlasSprite::updateAnimationInterpolated() {
    double d0 = 1.0 - static_cast<double>(tickCounter) / static_cast<double>(animationMetadata->getFrameTimeSingle(frameCounter));

    int i = animationMetadata->getFrameIndex(frameCounter);
    int j = (animationMetadata->getFrameCount() == 0) ? static_cast<int>(framesTextureData.size()) : animationMetadata->getFrameCount();
    int k = animationMetadata->getFrameIndex((frameCounter + 1) % j);

    if (i == k || k < 0 || k >= static_cast<int>(framesTextureData.size()))
        return;

    const FrameData& aint  = framesTextureData[i];
    const FrameData& aint1 = framesTextureData[k];

    // if (interpolatedFrameData == null || length mismatch) new int[aint.length][]
    if (!interpolatedFrameDataValid || interpolatedFrameData.size() != aint.size()) {
        interpolatedFrameData.assign(aint.size(), MipmapLevel{});
        interpolatedFrameDataValid = true;
    }

    for (size_t l = 0; l < aint.size(); ++l) {
        if (interpolatedFrameData[l].empty() && !aint[l].empty()) {
            interpolatedFrameData[l].resize(aint[l].size());
        }

        if (l < aint1.size() && aint1[l].size() == aint[l].size()) {
            for (size_t i1 = 0; i1 < aint[l].size(); ++i1) {
                int j1 = aint[l][i1];
                int k1 = aint1[l][i1];

                int l1 = static_cast<int>(((j1 & 0xFF0000) >> 16) * d0 + ((k1 & 0xFF0000) >> 16) * (1.0 - d0));
                int i2 = static_cast<int>(((j1 & 0x00FF00) >>  8) * d0 + ((k1 & 0x00FF00) >>  8) * (1.0 - d0));
                int j2 = static_cast<int>(( j1 & 0x0000FF)        * d0 + ( k1 & 0x0000FF)        * (1.0 - d0));

                interpolatedFrameData[l][i1] = (j1 & static_cast<int>(0xFF000000)) | (l1 << 16) | (i2 << 8) | j2;
            }
        }
    }

    TextureUtil::uploadTextureMipmap_(interpolatedFrameData, width, height, originX, originY, false, false);
}

void TextureAtlasSprite::loadSprite(BufferedImage* const* images, int imageCount, const AnimationMetadataSection* meta) {
    resetSprite();

    const int i = images[0]->width;
    const int j = images[0]->height;
    width  = i;
    height = j;

    FrameData aint(static_cast<size_t>(imageCount));

    for (int k = 0; k < imageCount; ++k) {
        BufferedImage* img = images[k];
        if (img == nullptr) continue;

        if (k > 0 && (img->width != (i >> k) || img->height != (j >> k))) {
            throw std::runtime_error(
                    "Unable to load miplevel: " + std::to_string(k) +
                    ", image is size: " + std::to_string(img->width) + "x" +
                    std::to_string(img->height) + ", expected " +
                    std::to_string(i >> k) + "x" + std::to_string(j >> k));
        }

        aint[k].resize(static_cast<size_t>(img->width * img->height));
        img->getRGB(0, 0, img->width, img->height, aint[k].data(), 0, img->width);
    }

    if (meta == nullptr) {
        if (j != i) {
            throw std::runtime_error("broken aspect ratio and not an animation");
        }
        framesTextureData.push_back(std::move(aint));
    } else {
        const int j1 = j / i;
        const int k1 = i;
        const int l  = i;
        height = width;

        if (meta->getFrameCount() > 0) {
            for (int i1 : meta->getFrameIndexSet()) {
                if (i1 >= j1) {
                    throw std::runtime_error("invalid frameindex " + std::to_string(i1));
                }
                allocateFrameTextureData(i1);
                framesTextureData[i1] = getFrameTextureData(aint, k1, l, i1);
            }

            animationMetadata = const_cast<AnimationMetadataSection*>(meta);
        } else {
            std::vector<AnimationFrame> list;
            list.reserve(j1);
            for (int l1 = 0; l1 < j1; ++l1) {
                framesTextureData.push_back(getFrameTextureData(aint, k1, l, l1));
                list.emplace_back(l1, -1);
            }
            animationMetadata = new AnimationMetadataSection(list, width, height, meta->getFrameTime(), meta->isInterpolate());
        }
    }
}

void TextureAtlasSprite::generateMipmaps(int level) {
    FrameList list;

    for (size_t i = 0; i < framesTextureData.size(); ++i) {
        const FrameData& aint = framesTextureData[i];
        if (aint.empty()) continue; // "null" frame

        try {
            list.push_back(TextureUtil::generateMipmapData_(level, width, aint));
        } catch (const std::exception& e) {
            throw std::runtime_error(
                    std::string("Generating mipmaps for frame ") +
                    std::to_string(i) + ": " + e.what());
        }
    }

    setFramesTextureData(list);
}

void TextureAtlasSprite::allocateFrameTextureData(int index) {
    if (static_cast<int>(framesTextureData.size()) <= index) {
        for (int i = static_cast<int>(framesTextureData.size()); i <= index; ++i) {
            framesTextureData.emplace_back();
        }
    }
}

FrameData TextureAtlasSprite::getFrameTextureData(const FrameData& data, int rows, int columns, int frameIndex) {
    FrameData aint(data.size());

    for (size_t i = 0; i < data.size(); ++i) {
        const MipmapLevel& aint1 = data[i];
        if (aint1.empty()) continue;

        const size_t len = static_cast<size_t>((rows >> i) * (columns >> i));
        aint[i].resize(len);
        std::copy_n(aint1.begin() + static_cast<ptrdiff_t>(frameIndex * len), len, aint[i].begin());
    }

    return aint;
}

void TextureAtlasSprite::resetSprite() {
    animationMetadata = nullptr;
    setFramesTextureData(FrameList{});
    frameCounter = 0;
    tickCounter  = 0;
    interpolatedFrameDataValid = false;
    interpolatedFrameData.clear();
}

std::string TextureAtlasSprite::toString() const {
    return "TextureAtlasSprite{name='" + iconName + '\''
           + ", frameCount=" + std::to_string(framesTextureData.size())
           + ", rotated=" + (rotated ? "true" : "false")
           + ", x=" + std::to_string(originX)
           + ", y=" + std::to_string(originY)
           + ", height=" + std::to_string(height)
           + ", width="  + std::to_string(width)
           + ", u0=" + std::to_string(minU)
           + ", u1=" + std::to_string(maxU)
           + ", v0=" + std::to_string(minV)
           + ", v1=" + std::to_string(maxV) + '}';
}
