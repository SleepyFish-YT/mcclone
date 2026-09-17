//
// Created by SleepyFish on 14.09.2026.
// Project: mcclone
//

#include "LayeredTexture.h"
#include "TextureUtil.h"
#include "../../resources/IResourceManager.h"
#include "../../resources/IResource.h"
#include "../../../util/ResourceLocation.h"
#include "../../../debug/Logger.h"

#include <istream>
#include <vector>

void LayeredTexture::loadTexture(IResourceManager &resourceManager) {
    this->deleteGlTexture();

    int width = 0, height = 0;
    std::vector<int> composited;

    try {
        for (const std::string &name : this->layeredTextureNames) {
            if (name.empty()) continue;

            IResource *iResource = resourceManager.getResource(ResourceLocation(name));
            auto inputStream = iResource->getInputStream();

            if (!inputStream || !*inputStream) {
                Logger::warn("Failed reading layer: {} (Input stream is null)", name);
                continue;
            }

            int layerWidth, layerHeight;
            auto pixels = TextureUtil::readImageData_(*inputStream, layerWidth, layerHeight);

            if (composited.empty()) {
                width      = layerWidth;
                height     = layerHeight;
                composited = pixels;
            } else {
                // composite layer on top - java uses Graphics.drawImage which is essentially alpha-over
                if (layerWidth != width || layerHeight != height) {
                    Logger::warn("Layer {} has mismatched dimensions, skipping", name);
                    continue;
                }

                for (int i = 0; i < width * height; ++i) {
                    int src = pixels[i];
                    int srcA = (src >> 24) & 0xFF;

                    if (srcA == 0) continue; // fully transparent, skip

                    if (srcA == 255) {
                        composited[i] = src; // fully opaque, just overwrite
                    } else {
                        // alpha-over blending
                        int dstA = (composited[i] >> 24) & 0xFF;
                        int dstR = (composited[i] >> 16) & 0xFF;
                        int dstG = (composited[i] >>  8) & 0xFF;
                        int dstB =  composited[i]        & 0xFF;

                        int srcR = (src >> 16) & 0xFF;
                        int srcG = (src >>  8) & 0xFF;
                        int srcB =  src        & 0xFF;

                        float sa = srcA / 255.0f;
                        float da = dstA / 255.0f;
                        float oa = sa + da * (1.0f - sa);

                        int outR = static_cast<int>((srcR * sa + dstR * da * (1.0f - sa)) / oa);
                        int outG = static_cast<int>((srcG * sa + dstG * da * (1.0f - sa)) / oa);
                        int outB = static_cast<int>((srcB * sa + dstB * da * (1.0f - sa)) / oa);
                        int outA = static_cast<int>(oa * 255.0f);

                        composited[i] = (outA << 24) | (outR << 16) | (outG << 8) | outB;
                    }
                }
            }
        }
    } catch (std::ios_base::failure& e) {
        Logger::error("Couldn't load layered image ({})", e.what());
        return;
    }

    if (composited.empty()) return;

    TextureUtil::uploadTextureImage_(this->glTextureId, composited.data(), width, height);
}