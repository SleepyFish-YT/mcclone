//
// Created by SleepyFish on 10.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_TEXTUREOFFSET_H
#define MCCLONE_TEXTUREOFFSET_H

/**
 * @author SleepyFish
 * @brief Texture offset
 */
class TextureOffset {

public:

    int offsetX;
    int offsetY;

    TextureOffset() = default;

    TextureOffset(int offsetX, int offsetY) :
        offsetX(offsetX),
        offsetY(offsetY)
    {}

};


#endif //MCCLONE_TEXTUREOFFSET_H
