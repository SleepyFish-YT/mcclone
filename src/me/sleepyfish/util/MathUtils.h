//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_MATHUTILS_H
#define MCCLONE_MATHUTILS_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif //M_PI

#include <array>
#include <cmath>
#include <numeric>
#include <span>
#include <numbers>

/**
 * @author SleepyFish
 */
struct MathUtils {

private:

    static std::array<float, 65536> buildAsinTable() {
        std::array<float, 65536> table{};

        for (int i = 0; i < 65536; ++i) {
            table[i] = std::asin(static_cast<double>(i) / 32767.5 - 1.0);
        }

        for (int j = -1; j < 2; ++j) {
            table[static_cast<int>((static_cast<double>(j) + 1.0) * 32767.5) & 65535] = std::asin(j);
        }

        return table;
    }

    static inline const std::array<float, 65536> ASIN_TABLE = MathUtils::buildAsinTable();

public:

    static constexpr float PI = std::numbers::pi_v<float>;
    static constexpr float PI2 = std::numbers::pi_v<float> * 2.0f;
    static constexpr float PId2 = std::numbers::pi_v<float> / 2.0f;

    static float asin(float value) {
        return ASIN_TABLE[static_cast<int>((static_cast<double>(value + 1.0f)) * 32767.5) & 65535];
    }

    static float acos(float value) {
        return PId2 - ASIN_TABLE[static_cast<int>((static_cast<double>(value + 1.0f)) * 32767.5) & 65535];
    }

    static int getAverage(std::span<const int> vals) {
        if (vals.empty()) return 0;
        return getSum(vals) / static_cast<int>(vals.size());
    }

    static int getSum(std::span<const int> vals) {
        if (vals.empty()) return 0;
        return std::reduce(vals.begin(), vals.end(), 0);
    }

    static int roundDownToPowerOfTwo(int val) noexcept {
        // next power of two
        int i = 1;
        while (i < val) i <<= 1;
        return val == i ? i : i >> 1;
    }

    static bool equalsDelta(float f1, float f2, float delta) {
        return std::abs(f1 - f2) <= delta;
    }

    static float toDeg(float angle) noexcept {
        return angle * 180.0f / PI;
    }

    static float toRad(float angle) noexcept {
        return angle / 180.0f * PI;
    }

    static float roundToFloat(double d) {
        return static_cast<float>(std::round(d * 1.0E8) / 1.0E8);
    }

};


#endif //MCCLONE_MATHUTILS_H
