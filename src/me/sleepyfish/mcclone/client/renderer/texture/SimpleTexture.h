//
// Created by SleepyFish on 14.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_SIMPLETEXTURE_H
#define MCCLONE_SIMPLETEXTURE_H

#include "AbstractTexture.h"
#include "../../../util/ResourceLocation.h"

#include <utility>

class IResourceManager;

/**
 * @author SleepyFish
 * @brief Simple texture base
 */
class SimpleTexture : public AbstractTexture {

protected:

    ResourceLocation textureLocation;

public:

    explicit SimpleTexture(ResourceLocation  textureLocation) :
        textureLocation(std::move(textureLocation))
    {}

    virtual ~SimpleTexture() = default;

    void loadTexture(IResourceManager& resourceManager) override; // throws std::ios_base::failure

};


#endif //MCCLONE_SIMPLETEXTURE_H
