//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_CLIPPINGHELPER_H
#define MCCLONE_CLIPPINGHELPER_H

#include <algorithm>

/**
 * @author SleepyFish
 * @brief Clipping helper
 */
class ClippingHelper {

public:

    float frustum[6][4]{};
    float projectionMatrix[16]{};
    float modelviewMatrix[16]{};
    float clippingMatrix[16]{};
    bool disabled = false;

    virtual ~ClippingHelper() = default;

    virtual bool isBoxInFrustum(double minX, double minY, double minZ, double maxX, double maxY, double maxZ) noexcept {
        if (this->disabled) {
            return true;
        }

        auto f = static_cast<float>(minX);
        auto f1 = static_cast<float>(minY);
        auto f2 = static_cast<float>(minZ);
        auto f3 = static_cast<float>(maxX);
        auto f4 = static_cast<float>(maxY);
        auto f5 = static_cast<float>(maxZ);

        const auto isOutside = [&](const float* plane) {
            const float f6 = plane[0], f7 = plane[1], f8 = plane[2], f9 = plane[3];

            return (f6 * f  + f7 * f1 + f8 * f2 + f9 <= 0.0f &&
                    f6 * f3 + f7 * f1 + f8 * f2 + f9 <= 0.0f &&
                    f6 * f  + f7 * f4 + f8 * f2 + f9 <= 0.0f &&
                    f6 * f3 + f7 * f4 + f8 * f2 + f9 <= 0.0f &&
                    f6 * f  + f7 * f1 + f8 * f5 + f9 <= 0.0f &&
                    f6 * f3 + f7 * f1 + f8 * f5 + f9 <= 0.0f &&
                    f6 * f  + f7 * f4 + f8 * f5 + f9 <= 0.0f &&
                    f6 * f3 + f7 * f4 + f8 * f5 + f9 <= 0.0f);
        };

        return !std::ranges::any_of(this->frustum, isOutside);
    }

    bool isBoxInFrustumFully(double minX, double minY, double minZ, double maxX, double maxY, double maxZ) noexcept {
        if (this->disabled) {
            return true;
        }

        auto f = (float) minX;
        auto f1 = (float) minY;
        auto f2 = (float) minZ;
        auto f3 = (float) maxX;
        auto f4 = (float) maxY;
        auto f5 = (float) maxZ;

        for (int i = 0; i < 6; ++i) {
            const float* p = frustum[i];
            float f6 = p[0], f7 = p[1], f8 = p[2], f9 = p[3];

            if (i < 4) {
                if (f6 * f  + f7 * f1 + f8 * f2 + f9 <= 0.0f ||
                    f6 * f3 + f7 * f1 + f8 * f2 + f9 <= 0.0f ||
                    f6 * f  + f7 * f4 + f8 * f2 + f9 <= 0.0f ||
                    f6 * f3 + f7 * f4 + f8 * f2 + f9 <= 0.0f ||
                    f6 * f  + f7 * f1 + f8 * f5 + f9 <= 0.0f ||
                    f6 * f3 + f7 * f1 + f8 * f5 + f9 <= 0.0f ||
                    f6 * f  + f7 * f4 + f8 * f5 + f9 <= 0.0f ||
                    f6 * f3 + f7 * f4 + f8 * f5 + f9 <= 0.0f)
                {
                    return false;
                }
            } else {
                if (f6 * f  + f7 * f1 + f8 * f2 + f9 <= 0.0f &&
                    f6 * f3 + f7 * f1 + f8 * f2 + f9 <= 0.0f &&
                    f6 * f  + f7 * f4 + f8 * f2 + f9 <= 0.0f &&
                    f6 * f3 + f7 * f4 + f8 * f2 + f9 <= 0.0f &&
                    f6 * f  + f7 * f1 + f8 * f5 + f9 <= 0.0f &&
                    f6 * f3 + f7 * f1 + f8 * f5 + f9 <= 0.0f &&
                    f6 * f  + f7 * f4 + f8 * f5 + f9 <= 0.0f &&
                    f6 * f3 + f7 * f4 + f8 * f5 + f9 <= 0.0f)
                {
                    return false;
                }
            }
        }

        return true;
    }

private:

    float dot(const float* plane, float x, float y, float z) noexcept {
        return plane[0] * x + plane[1] * y + plane[2] * z + plane[3];
    }

};


#endif //MCCLONE_CLIPPINGHELPER_H
