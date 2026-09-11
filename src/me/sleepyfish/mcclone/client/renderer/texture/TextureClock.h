//
// Created by SleepyFish on 11.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_TEXTURECLOCK_H
#define MCCLONE_TEXTURECLOCK_H

#include "TextureAtlasSprite.h"

/**
 * @author SleepyFish
 * @brief Texture clock
 */
class TextureClock : public TextureAtlasSprite {

private:

    double currentAngle{};
    double angleDelta{};

public:

    TextureClock(const std::string& iconName) :
        TextureAtlasSprite(iconName)
    {}

    void updateAnimation() override;

};


#endif //MCCLONE_TEXTURECLOCK_H
