//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ICAMERA_H
#define MCCLONE_ICAMERA_H

#include "../../../util/AxisAlignedBB.h"

/**
 * @author SleepyFish
 * @brief Camera interface
 */
class ICamera {

public:

    virtual bool isBoundingBoxInFrustum(AxisAlignedBB axisAlignedBB) = 0;

    virtual void setPosition(double x, double y, double z) = 0;

};

#endif //MCCLONE_ICAMERA_H
