//
// Created by SleepyFish on 12.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_DYNAMICTEXTURE_H
#define MCCLONE_DYNAMICTEXTURE_H

#include "AbstractTexture.h"
#include "TextureUtil.h"
#include <vector>

class BufferedImage;
class IResourceManager;

/**
 * @author SleepyFish
 * @brief Dynamic texture
 */
class DynamicTexture : public AbstractTexture {

private:

    int width;
    int height;
    std::vector<int> dynamicTextureData;

public:

    explicit DynamicTexture(const BufferedImage& bufferedImage);
    DynamicTexture(int textureWidth, int textureHeight);

    void loadTexture(IResourceManager& resourceManager) override;

    void updateDynamicTexture();

    // returns a mutable reference so callers can modify pixel data
    // then call updateDynamicTexture() to push changes to GPU
    std::vector<int>& getTextureData();

};


#endif //MCCLONE_DYNAMICTEXTURE_H
