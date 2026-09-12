//
// Created by SleepyFish on 12.09.2026.
// Project: mcclone
//

#include "DynamicTexture.h"
#include "TextureUtil.h"

#include "../../../../sava/BufferedImage.h"

DynamicTexture::DynamicTexture(const BufferedImage& bufferedImage) :
    DynamicTexture(bufferedImage.width, bufferedImage.height)
{
    dynamicTextureData.assign(
            bufferedImage.pixels.begin(),
            bufferedImage.pixels.end()
    );
    updateDynamicTexture();
}

DynamicTexture::DynamicTexture(int textureWidth, int textureHeight) :
    width(textureWidth),
    height(textureHeight),
    dynamicTextureData(textureWidth * textureHeight, 0)
{
    TextureUtil::allocateTexture_(this->getGlTextureId(), textureWidth, textureHeight);
}

void DynamicTexture::loadTexture(IResourceManager& resourceManager) {
    // Intentionally empty — matches Java implementation
}

void DynamicTexture::updateDynamicTexture() {
    TextureUtil::uploadTexture_(this->getGlTextureId(), dynamicTextureData.data(), width, height);
}

std::vector<int>& DynamicTexture::getTextureData() {
    return dynamicTextureData;
}
