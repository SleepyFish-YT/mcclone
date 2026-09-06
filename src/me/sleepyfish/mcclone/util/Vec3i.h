//
// Created by SleepyFish on 02.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_VEC3I_H
#define MCCLONE_VEC3I_H

#include <string>
#include <format>
#include <cmath>

/**
 * @author SleepyFish
 * @brief 3 integer vector class
 */
class Vec3i {

public:

    const int x;
    const int y;
    const int z;

    Vec3i(int xIn, int yIn, int zIn) noexcept :
        x(xIn),
        y(yIn),
        z(zIn)
    {}

    Vec3i(double xIn, double yIn, double zIn) :
        x(static_cast<int>(std::floor(xIn))),
        y(static_cast<int>(std::floor(yIn))),
        z(static_cast<int>(std::floor(zIn)))
    {}

    bool operator==(const Vec3i& other) const = default;

    bool operator<(const Vec3i& other) const noexcept {
        if (this->y != other.y) return this->y < other.y;
        if (this->z != other.z) return this->z < other.z;
        return this->x < other.x;
    }

    Vec3i crossProduct(const Vec3i& vec) const noexcept {
        return {
                this->y * vec.z - this->z * vec.y,
                this->z * vec.x - this->x * vec.z,
                this->x * vec.y - this->y * vec.x
        };
    }

    double distanceSq(double toX, double toY, double toZ) const noexcept {
        double d0 = static_cast<double>(this->x) - toX;
        double d1 = static_cast<double>(this->y) - toY;
        double d2 = static_cast<double>(this->z) - toZ;
        return d0 * d0 + d1 * d1 + d2 * d2;
    }

    double distanceSqToCenter(double xIn, double yIn, double zIn) const noexcept {
        double d0 = static_cast<double>(this->x) + 0.5 - xIn;
        double d1 = static_cast<double>(this->y) + 0.5 - yIn;
        double d2 = static_cast<double>(this->z) + 0.5 - zIn;
        return d0 * d0 + d1 * d1 + d2 * d2;
    }

    double distanceSq(const Vec3i& to) const noexcept {
        return this->distanceSq(to.x, to.y, to.z);
    }

    std::string toString() const {
        return std::format("Vec3i{{x={}, y={}, z={}}}", this->x, this->y, this->z);
    }

};

#endif //MCCLONE_VEC3I_H
