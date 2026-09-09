//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_FRUSTUM_H
#define MCCLONE_FRUSTUM_H

#include "ICamera.h"
#include "ClippingHelper.h"
#include "ClippingHelperImpl.h"
#include "../../../util/AxisAlignedBB.h"

/**
 * @author SleepyFish
 * @brief Frustum class
 */
class Frustum : public ICamera {

private:

    ClippingHelper* clippingHelper;
    double xPosition;
    double yPosition;
    double zPosition;


public:

    Frustum() noexcept :
        clippingHelper(ClippingHelperImpl::getInstance()),
        xPosition(0.0),
        yPosition(0.0),
        zPosition(0.0)
    {}

    explicit Frustum(ClippingHelper* clippingHelper) noexcept :
        clippingHelper(clippingHelper),
        xPosition(0.0),
        yPosition(0.0),
        zPosition(0.0)
    {}

    void setPosition(double x, double y, double z) override {
        this->xPosition = x;
        this->yPosition = y;
        this->zPosition = z;
    }

    bool isBoxInFrustum(double minX, double minY, double minZ, double maxX, double maxY, double maxZ) noexcept {
        return this->clippingHelper->isBoxInFrustum(
                minX - this->xPosition, minY - this->yPosition, minZ - this->zPosition,
                maxX - this->xPosition, maxY - this->yPosition, maxZ - this->zPosition
        );
    }

    bool isBoundingBoxInFrustum(const AxisAlignedBB& aabb) override {
        return this->isBoxInFrustum(aabb.minX, aabb.minY, aabb.minZ, aabb.maxX, aabb.maxY, aabb.maxZ);
    }

    bool isBoxInFrustumFully(double minX, double minY, double minZ, double maxX, double maxY, double maxZ) {
        return this->clippingHelper->isBoxInFrustumFully(
                minX - this->xPosition, minY - this->yPosition, minZ - this->zPosition,
                maxX - this->xPosition, maxY - this->yPosition, maxZ - this->zPosition
        );
    }

};


#endif //MCCLONE_FRUSTUM_H
