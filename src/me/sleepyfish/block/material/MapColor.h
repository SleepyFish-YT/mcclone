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

    static constexpr int ALPHA_MAP[] = {180, 220, 255, 135};

    int colorValue;

    const int colorIndex;

     int getMapColor(const int lightLevel) const {
         const int alpha = (lightLevel < 4) ? ALPHA_MAP[lightLevel] : 0;
         if (alpha == 0) return 0;

         const int r_i = ((this->colorValue >> 16) & 0xFF);
         const int g_i = ((this->colorValue >> 8) & 0xFF);
         const int b_i = (this->colorValue & 0xFF);

         const int r = r_i * alpha / 255;
         const int g = g_i * alpha / 255;
         const int b = b_i * alpha / 255;

         return (0xFF000000) | (r << 16) | (g << 8) | b;
     }

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
