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

    static constexpr uint16_t ALPHA_FULL = 255;
    static constexpr uint16_t ALPHA_NORMAL = 220;
    static constexpr uint16_t ALPHA_DIM = 180;
    static constexpr uint16_t ALPHA_DARK = 135;

    uint32_t colorValue;

    const uint8_t colorIndex;

     int getMapColor(const uint8_t lightLevel) const {
         static constexpr uint16_t alphaMap[] = {ALPHA_DIM, ALPHA_NORMAL, ALPHA_FULL, ALPHA_DARK};
         const uint16_t alpha = (lightLevel < 4) ? alphaMap[lightLevel] : 0;
         if (alpha == 0) return 0;

         const uint32_t r = ((this->colorValue >> 16) & 0xFF);
         const uint32_t g = ((this->colorValue >> 8) & 0xFF);
         const uint32_t b = (this->colorValue & 0xFF);

         const uint32_t red = r * alpha / 255;
         const uint32_t green = g * alpha / 255;
         const uint32_t blue = b * alpha / 255;

         return (0xFF000000) | (red << 16) | (green << 8) | blue;
     }

    MapColor(const uint8_t index, const uint32_t color) :
        colorIndex(index),
        colorValue(color)
    {
        if (index < 0 || index > 63) {
            throw std::out_of_range("Map color ID must be between 0 and 63 (inclusive)");
        }
    }

};

#endif //MCCLONE_MAPCOLOR_H
