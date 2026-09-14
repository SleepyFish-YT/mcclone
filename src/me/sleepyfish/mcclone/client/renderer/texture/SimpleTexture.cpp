//
// Created by SleepyFish on 14.09.2026.
// Project: mcclone
//

#include "SimpleTexture.h"
#include "../../resources/IResource.h"
#include "../../resources/IResourceManager.h"
#include "../../../util/ResourceLocation.h"
#include "TextureUtil.h"
#include "../../resources/data/TextureMetadataSection.h"
#include "../../../debug/Logger.h"

#include <exception>
#include <istream>

#ifdef MCCLONE_DEBUG
#include <iostream>
#endif //MCCLONE_DEBUG

void SimpleTexture::loadTexture(IResourceManager& resourceManager) {
    this->deleteGlTexture();

    try {
        IResource* iResource = resourceManager.getResource(this->textureLocation);
        auto inputStream = iResource->getInputStream();

        if (!inputStream || !*inputStream) {
            Logger::warn("Failed reading texture: {} (Input stream is null)", this->textureLocation.toString());
            return;
        }

        int width, height;
        auto pixels = TextureUtil::readImageData_(*inputStream, width, height);
        bool flag = false;
        bool flag1 = false;

        if (iResource->hasMetadata()) {
            try {
                std::any metaDataSection = iResource->getMetadata("texture");
                if (metaDataSection.has_value()) {
                    auto* textureMetaDataSection = std::any_cast<TextureMetadataSection>(&metaDataSection);

                    if (textureMetaDataSection) {
                        flag = textureMetaDataSection->getTextureBlur();
                        flag1 = textureMetaDataSection->getTextureClamp();
                    }
                }

            } catch (std::exception &e) {
                Logger::warn("Failed reading metadata of: {} ({})", this->textureLocation.toString(), e.what());
            }
        }

        TextureUtil::uploadTextureImageAllocate_(this->glTextureId, pixels.data(), width, height, flag, flag1);
    } catch (std::ios_base::failure &e) {
        Logger::warn("Failed reading texture: {} ({})", this->textureLocation.toString(), e.what());
    }
}
