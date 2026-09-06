//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_COLORIZERFOLIAGE_H
#define MCCLONE_COLORIZERFOLIAGE_H

#include <array>

/**
 * @author SleepyFish
 * @brief Foliage colorizer
 */
class ColorizerFoliage {

private:

    static inline std::array<int, 65536> foliageBuffer{};

public:

    static void setFoliageBiomeColorizer(const std::array<int, 65536>& source) {
        ColorizerFoliage::foliageBuffer = source;
    }

    static int getFoliageColor(const double p1, double p2) {
        const double p3 = p2 * p1;
        const int i = (int) ((1.0 - p1) * 255.0);
        const int j = (int) ((1.0 - p3) * 255.0);
        return ColorizerFoliage::foliageBuffer[j << 8 | i];
    }

    static int getFoliageColorPine() {
        return 6396257;
    }

    static int getFoliageColorBirch() {
        return 8431445;
    }

    static int getFoliageColorBasic() {
        return 4764952;
    }

};


#endif //MCCLONE_COLORIZERFOLIAGE_H
