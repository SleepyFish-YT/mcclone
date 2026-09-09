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

        const auto minXf = static_cast<float>(minX);
        const auto minYf = static_cast<float>(minY);
        const auto minZf = static_cast<float>(minZ);
        const auto maxXf = static_cast<float>(maxX);
        const auto maxYf = static_cast<float>(maxY);
        const auto maxZf = static_cast<float>(maxZ);

        // check negative vertex (closest to plane). if outside, the whole box is outside.
        const auto isOutside = [&](const auto& plane) {
            const float a = plane[0], b = plane[1], c = plane[2], d = plane[3];

            const float px = (a >= 0.0f) ? maxXf : minXf;
            const float py = (b >= 0.0f) ? maxYf : minYf;
            const float pz = (c >= 0.0f) ? maxZf : minZf;

            return (a * px + b * py + c * pz + d <= 0.0f);
        };

        return !std::ranges::any_of(this->frustum, isOutside);
    }

    bool isBoxInFrustumFully(double minX, double minY, double minZ, double maxX, double maxY, double maxZ) noexcept {
        if (this->disabled) return true;

        const auto minXf = static_cast<float>(minX);
        const auto minYf = static_cast<float>(minY);
        const auto minZf = static_cast<float>(minZ);
        const auto maxXf = static_cast<float>(maxX);
        const auto maxYf = static_cast<float>(maxY);
        const auto maxZf = static_cast<float>(maxZ);

        // check negative vertex (furthest from plane). if outside, the box is partially outside.
        const auto isPartiallyOutside = [&](const auto& plane) {
            const float a = plane[0], b = plane[1], c = plane[2], d = plane[3];

            const float nx = (a >= 0.0f) ? minXf : maxXf;
            const float ny = (b >= 0.0f) ? minYf : maxYf;
            const float nz = (c >= 0.0f) ? minZf : maxZf;

            return (a * nx + b * ny + c * nz + d < 0.0f);
        };

        return !std::ranges::any_of(this->frustum, isPartiallyOutside);
    }

};


#endif //MCCLONE_CLIPPINGHELPER_H
