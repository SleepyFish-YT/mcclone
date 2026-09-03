//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_MAPCOLOR_H
#define MCCLONE_MAPCOLOR_H

#include <stdexcept>

/**
 * @author SleepyFish
 * @brief Map color
 */
class MapColor {

public:

    static constexpr int ALPHA_FULL = 255;
    static constexpr int ALPHA_NORMAL = 220;
    static constexpr int ALPHA_DIM = 180;
    static constexpr int ALPHA_DARK = 135;

    int colorValue;

    const int colorIndex;

     int getMapColor(const int lightLevel) const {
         int alpha;
         switch (lightLevel) {
             case 0: alpha = ALPHA_DIM; break;
             case 1: alpha = ALPHA_NORMAL; break;
             case 2: alpha = ALPHA_FULL; break;
             case 3: alpha = ALPHA_DARK; break;
             default: alpha = 0; break;
         }

         if (alpha == 0) return 0;

         const int red = (this->colorValue >> 16 & 255) * alpha / 255;
         const int green = (this->colorValue >> 8 & 255) * alpha / 255;
         const int blue = (this->colorValue & 255) * alpha / 255;

         return (0xFF << 24) | (red << 16) | (green << 8) | blue;
     }

private:

    MapColor(const int index, const int color) :
            colorIndex(index),
            colorValue(color)
    {
        if (index < 0 || index > 63) {
            throw std::out_of_range("Map color ID must be between 0 and 63 (inclusive)");
        }
    }

};

#endif //MCCLONE_MAPCOLOR_H
