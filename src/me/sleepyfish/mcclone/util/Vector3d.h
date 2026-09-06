//
// Created by SleepyFish on 02.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_VECTOR3D_H
#define MCCLONE_VECTOR3D_H

/**
 * @author SleepyFish
 * @brief 3 double vector class
 */
class Vector3d {

public:

    double x, y, z;

    Vector3d() noexcept {
        this->x = 0.0;
        this->y = 0.0;
        this->z = 0.0;
    }

};


#endif //MCCLONE_VECTOR3D_H
