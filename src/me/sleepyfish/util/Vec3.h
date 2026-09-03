//
// Created by SleepyFish on 02.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_VEC3_H
#define MCCLONE_VEC3_H

#include "Vec3i.h"

#include <cmath>
#include <optional>
#include <format>

/**
 * @author SleepyFish
 * @brief 3 double vector class
 */
class Vec3 {

public:

    const double xCoord;
    const double yCoord;
    const double zCoord;

    Vec3(double x, double y, double z) noexcept :
        xCoord(x == -0.0 ? 0.0 : x),
        yCoord(y == -0.0 ? 0.0 : y),
        zCoord(z == -0.0 ? 0.0 : z)
    {}

    Vec3(const Vec3i &vec) noexcept :
        Vec3(vec.x, vec.y, vec.z)
    {}

    Vec3 subtractReverse(const Vec3 &vec) const noexcept {
        return {
            vec.xCoord - this->xCoord,
            vec.yCoord - this->yCoord,
            vec.zCoord - zCoord
        };
    }

    Vec3 normalize() const {
        const double d0 = std::sqrt(
                this->xCoord * this->xCoord + this->yCoord * this->yCoord + this->zCoord * this->zCoord
        );
        return d0 < 1.0E-4 ? Vec3(0.0, 0.0, 0.0) : Vec3(this->xCoord / d0, this->yCoord / d0, this->zCoord / d0);
    }

    double dotProduct(const Vec3 &vec) const noexcept {
        return this->xCoord * vec.xCoord + this->yCoord * vec.yCoord + this->zCoord * vec.zCoord;
    }

    Vec3 crossProduct(const Vec3 &vec) const  noexcept {
        return {
            this->yCoord * vec.zCoord - this->zCoord * vec.yCoord,
            this->zCoord * vec.xCoord - this->xCoord * vec.zCoord,
            this->xCoord * vec.yCoord - this->yCoord * vec.xCoord
        };
    }

    Vec3 subtract(const Vec3 &vec) const noexcept {
        return this->subtract(vec.xCoord, vec.yCoord, vec.zCoord);
    }

    Vec3 subtract(double x, double y, double z) const noexcept {
        return this->addVector(-x, -y, -z);
    }

    Vec3 add(const Vec3 &vec) const noexcept {
        return this->addVector(vec.xCoord, vec.yCoord, vec.zCoord);
    }

    Vec3 addVector(double x, double y, double z) const {
        return {
            this->xCoord + x,
            this->yCoord + y,
            this->zCoord + z
        };
    }

    double distanceTo(const Vec3 &vec) const {
        const double d0 = vec.xCoord - this->xCoord;
        const double d1 = vec.yCoord - this->yCoord;
        const double d2 = vec.zCoord - this->zCoord;
        return std::sqrt(d0 * d0 + d1 * d1 + d2 * d2);
    }

    double squareDistanceTo(const Vec3 &vec) const noexcept {
        const double d0 = vec.xCoord - this->xCoord;
        const double d1 = vec.yCoord - this->yCoord;
        const double d2 = vec.zCoord - this->zCoord;
        return d0 * d0 + d1 * d1 + d2 * d2;
    }

    double lengthVector() const {
        return std::sqrt(this->xCoord * this->xCoord + this->yCoord * this->yCoord + this->zCoord * this->zCoord);
    }

    std::optional<Vec3> getIntermediateWithXValue(const Vec3 &vec, double x) const {
        const double d0 = vec.xCoord - this->xCoord;
        const double d1 = vec.yCoord - this->yCoord;
        const double d2 = vec.zCoord - this->zCoord;

        if (d0 * d0 < 1.0000000116860974E-7)
            return std::nullopt;

        const double d3 = (x - this->xCoord) / d0;
        return d3 >= 0.0 && d3 <= 1.0 ? std::optional<Vec3>(
                Vec3(this->xCoord + d0 * d3,
                     this->yCoord + d1 * d3,
                     this->zCoord + d2 * d3
                )) : std::nullopt;
    }

    std::optional<Vec3> getIntermediateWithYValue(const Vec3 &vec, double y) const {
        const double d0 = vec.xCoord - this->xCoord;
        const double d1 = vec.yCoord - this->yCoord;
        const double d2 = vec.zCoord - this->zCoord;

        if (d1 * d1 < 1.0000000116860974E-7)
            return std::nullopt;

        const double d3 = (y - this->yCoord) / d1;
        return d3 >= 0.0 && d3 <= 1.0 ? std::optional<Vec3>(
                Vec3(this->xCoord + d0 * d3,
                     this->yCoord + d1 * d3,
                     this->zCoord + d2 * d3
                )) : std::nullopt;
    }

    std::optional<Vec3> getIntermediateWithZValue(const Vec3 &vec, double z) const {
        const double d0 = vec.xCoord - this->xCoord;
        const double d1 = vec.yCoord - this->yCoord;
        const double d2 = vec.zCoord - this->zCoord;

        if (d2 * d2 < 1.0000000116860974E-7)
            return std::nullopt;

        const double d3 = (z - this->zCoord) / d2;
        return d3 >= 0.0 && d3 <= 1.0 ? std::optional<Vec3>(
                Vec3(this->xCoord + d0 * d3,
                     this->yCoord + d1 * d3,
                     this->zCoord + d2 * d3
                )) : std::nullopt;
    }

    Vec3 rotatePitch(float pitch) const {
        const float f = std::cos(pitch);
        const float f1 = std::sin(pitch);

        return {
            this->xCoord,
            this->yCoord * f + this->zCoord * f1,
            this->zCoord * f - this->yCoord * f1
        };
    }

    Vec3 rotateYaw(float yaw) const {
        const float f = std::cos(yaw);
        const float f1 = std::sin(yaw);

        return {
            this->xCoord * f + this->zCoord * f1,
            this->yCoord,
            this->zCoord * f - this->xCoord * f1
        };
    }

    std::string toString() const {
        return std::format("({}, {}, {})", this->xCoord, this->yCoord, this->zCoord);
    }


};

#endif //MCCLONE_VEC3_H
