//
// Created by SleepyFish on 11.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_TEXTURECOMPASS_H
#define MCCLONE_TEXTURECOMPASS_H

#include "TextureAtlasSprite.h"

class World;

/**
 * @author SleepyFish
 * @brief Texture compass
 */
class TextureCompass : public TextureAtlasSprite {

private:

    double currentAngle{};
    double angleDelta{};

public:

    TextureCompass(const std::string& iconName) :
        TextureAtlasSprite(iconName)
    {}

    void updateAnimation() override;

    void updateCompass(World* worldIn, double xIn, double zIn, double angle, bool reqWorld, bool setAngle);

};


#endif //MCCLONE_TEXTURECOMPASS_H
