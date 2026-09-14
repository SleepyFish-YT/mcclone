//
// Created by SleepyFish on 14.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_LAYEREDTEXTURE_H
#define MCCLONE_LAYEREDTEXTURE_H

#include "AbstractTexture.h"

#include <vector>
#include <string>

/**
 * @author SleepyFish
 * @brief Layered texture
 */
class LayeredTexture : public AbstractTexture {

public:

    std::vector<std::string> layeredTextureNames;

    explicit LayeredTexture(std::initializer_list<std::string> textureNames) :
        layeredTextureNames(textureNames)
    {}

    void loadTexture(IResourceManager &resourceManager) override;

};


#endif //MCCLONE_LAYEREDTEXTURE_H
