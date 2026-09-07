//
// Created by SleepyFish on 07.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_IPROJECTILE_H
#define MCCLONE_IPROJECTILE_H

/**
 * @author SleepyFish
 * @brief Projectile interface
 */
class IProjectile {

public:

    virtual ~IProjectile() = default;

    virtual void setThrowableHeading(double x, double y, double z, float velocity, float inaccuracy) = 0;

};


#endif //MCCLONE_IPROJECTILE_H
