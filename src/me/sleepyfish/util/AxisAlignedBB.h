//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_AXISALIGNEDBB_H
#define MCCLONE_AXISALIGNEDBB_H

#include "BlockPos.h"
#include "Vec3.h"
#include "MovingObjectPosition.h"
#include "EnumFacing.h"

#include <algorithm>
#include <string>
#include <cmath>
#include <optional>

/**
 * @author SleepyFish
 * @brief AxisAlignedBB class
 */
class AxisAlignedBB {

    const double minX;

    const double minY;

    const double minZ;

    const double maxX;

    const double maxY;

    const double maxZ;

    AxisAlignedBB(const double x1, const double y1, const double z1, const double x2, const double y2, const double z2) :
        minX(std::min(x1, x2)),
        minY(std::min(y1, y2)),
        minZ(std::min(z1, z2)),
        maxX(std::max(x1, x2)),
        maxY(std::max(y1, y2)),
        maxZ(std::max(z1, z2))
    {}

    AxisAlignedBB(const BlockPos& pos1, const BlockPos& pos2) :
        minX(pos1.x),
        minY(pos1.y),
        minZ(pos1.z),
        maxX(pos2.x),
        maxY(pos2.y),
        maxZ(pos2.z)
    {}

    static AxisAlignedBB fromBounds(const double x1, const double y1, const double z1, const double x2, const double y2, const double z2) {
        return {
            std::min(x1, x2), std::min(y1, y2), std::min(z1, z2),
            std::max(x1, x2), std::max(y1, y2), std::max(z1, z2)
        };
    }

    AxisAlignedBB addCoord(const double x, const double y, const double z) const {
        double d0 = this->minX, d1 = this->minY, d2 = this->minZ;
        double d3 = this->maxX, d4 = this->maxY, d5 = this->maxZ;

        if (x < 0.0) d0 += x; else if (x > 0.0) d3 += x;
        if (y < 0.0) d1 += y; else if (y > 0.0) d4 += y;
        if (z < 0.0) d2 += z; else if (z > 0.0) d5 += z;

        return {
            d0, d1, d2,
            d3, d4, d5
        };
    }

    AxisAlignedBB expand(const double x, const double y, const double z) const {
        return {
            this->minX - x, this->minY - y, this->minZ - z,
            this->maxX + x, this->maxY + y, this->maxZ + z
        };
    }

    AxisAlignedBB contract(const double x, const double y, const double z) const {
        return {
            this->minX + x, this->minY + y, this->minZ + z,
            this->maxX - x, this->maxY - y, this->maxZ - z
        };
    }

    AxisAlignedBB offset(const double x, const double y, const double z) const {
        return {
            this->minX + x, this->minY + y, this->minZ + z,
            this->maxX + x, this->maxY + y, this->maxZ + z
        };
    }

    AxisAlignedBB union_(const AxisAlignedBB& other) const {
        return {
            std::min(this->minX, other.minX), std::min(this->minY, other.minY), std::min(this->minZ, other.minZ),
            std::max(this->maxX, other.maxX), std::max(this->maxY, other.maxY), std::max(this->maxZ, other.maxZ)
        };
    }

    double calculateXOffset(const AxisAlignedBB& other, double offsetX) const {
        if (other.maxY > this->minY && other.minY < this->maxY && other.maxZ > this->minZ && other.minZ < this->maxZ) {
            if (offsetX > 0.0 && other.maxX <= this->minX) {
                const double d1 = this->minX - other.maxX;
                if (d1 < offsetX) offsetX = d1;
            } else if (offsetX < 0.0 && other.minX >= this->maxX) {
                const double d0 = this->maxX - other.minX;
                if (d0 > offsetX) offsetX = d0;
            }
        }

        return offsetX;
    }

    double calculateYOffset(const AxisAlignedBB& other, double offsetY) const {
        if (other.maxX > this->minX && other.minX < this->maxX && other.maxZ > this->minZ && other.minZ < this->maxZ) {
            if (offsetY > 0.0 && other.maxY <= this->minY) {
                const double d1 = this->minY - other.maxY;
                if (d1 < offsetY) offsetY = d1;
            } else if (offsetY < 0.0 && other.minY >= this->maxY) {
                const double d0 = this->maxY - other.minY;
                if (d0 > offsetY) offsetY = d0;
            }
        }

        return offsetY;
    }

    double calculateZOffset(const AxisAlignedBB& other, double offsetZ) const {
        if (other.maxX > this->minX && other.minX < this->maxX && other.maxY > this->minY && other.minY < this->maxY) {
            if (offsetZ > 0.0 && other.maxZ <= this->minZ) {
                const double d1 = this->minZ - other.maxZ;
                if (d1 < offsetZ) offsetZ = d1;
            } else if (offsetZ < 0.0 && other.minZ >= this->maxZ) {
                const double d0 = this->maxZ - other.minZ;
                if (d0 > offsetZ) offsetZ = d0;
            }
        }

        return offsetZ;
    }

    bool intersectsWith(const AxisAlignedBB& other) const {
        return other.maxX > this->minX && other.minX < this->maxX &&
               other.maxY > this->minY && other.minY < this->maxY &&
               other.maxZ > this->minZ && other.minZ < this->maxZ;
    }

    bool isVecInside(const Vec3& vec) const {
        return vec.xCoord > this->minX && vec.xCoord < this->maxX &&
               vec.yCoord > this->minY && vec.yCoord < this->maxY &&
               vec.zCoord > this->minZ && vec.zCoord < this->maxZ;
    }

    double getAverageEdgeLength() const {
        return ((this->maxX - this->minX) + (this->maxY - this->minY) + (this->maxZ - this->minZ)) / 3.0;
    }

    std::optional<MovingObjectPosition> calculateIntercept(const Vec3& vecA, const Vec3& vecB) const {
        auto getIfInYZ = [&](std::optional<Vec3> v) -> std::optional<Vec3> {
            if (!v) return std::nullopt;
            return isVecInYZ(*v) ? v : std::nullopt;
        };

        auto getIfInXZ = [&](std::optional<Vec3> v) -> std::optional<Vec3> {
            if (!v) return std::nullopt;
            return isVecInXZ(*v) ? v : std::nullopt;
        };

        auto getIfInXY = [&](std::optional<Vec3> v) -> std::optional<Vec3> {
            if (!v) return std::nullopt;
            return isVecInXY(*v) ? v : std::nullopt;
        };

        const std::optional<Vec3> vec3  = getIfInYZ(vecA.getIntermediateWithXValue(vecB, this->minX));
        const std::optional<Vec3> vec31 = getIfInYZ(vecA.getIntermediateWithXValue(vecB, this->maxX));
        const std::optional<Vec3> vec32 = getIfInXZ(vecA.getIntermediateWithYValue(vecB, this->minY));
        const std::optional<Vec3> vec33 = getIfInXZ(vecA.getIntermediateWithYValue(vecB, this->maxY));
        const std::optional<Vec3> vec34 = getIfInXY(vecA.getIntermediateWithZValue(vecB, this->minZ));
        const std::optional<Vec3> vec35 = getIfInXY(vecA.getIntermediateWithZValue(vecB, this->maxZ));

        const std::optional<Vec3>* closestOpt = nullptr;
        auto tryUpdate = [&](const std::optional<Vec3>& v) {
            if (!v) return;
            if (!closestOpt || vecA.squareDistanceTo(*v) < vecA.squareDistanceTo(**closestOpt))
                closestOpt = &v;
        };

        tryUpdate(vec3);
        tryUpdate(vec31);
        tryUpdate(vec32);
        tryUpdate(vec33);
        tryUpdate(vec34);
        tryUpdate(vec35);

        if (!closestOpt) return std::nullopt;

        const EnumFacing facing =
                closestOpt == &vec3  ? EnumFacing(EnumFacing::WEST)  :
                closestOpt == &vec31 ? EnumFacing(EnumFacing::EAST)  :
                closestOpt == &vec32 ? EnumFacing(EnumFacing::DOWN)  :
                closestOpt == &vec33 ? EnumFacing(EnumFacing::UP)    :
                closestOpt == &vec34 ? EnumFacing(EnumFacing::NORTH) :
                EnumFacing(EnumFacing::SOUTH);

        return MovingObjectPosition(**closestOpt, facing);
    }

    bool hasNaN() const {
        return std::isnan(this->minX) || std::isnan(this->minY) || std::isnan(this->minZ) ||
               std::isnan(this->maxX) || std::isnan(this->maxY) || std::isnan(this->maxZ);
    }

    std::string toString() const {
        return "box[" + std::to_string(this->minX) + ", " + std::to_string(this->minY) + ", " + std::to_string(this->minZ) +
               " -> "  + std::to_string(this->maxX) + ", " + std::to_string(this->maxY) + ", " + std::to_string(this->maxZ) + "]";
    }

private:

    bool isVecInYZ(const Vec3& vec) const {
        return vec.yCoord >= this->minY && vec.yCoord <= this->maxY &&
               vec.zCoord >= this->minZ && vec.zCoord <= this->maxZ;
    }

    bool isVecInXZ(const Vec3& vec) const {
        return vec.xCoord >= this->minX && vec.xCoord <= this->maxX &&
               vec.zCoord >= this->minZ && vec.zCoord <= this->maxZ;
    }

    bool isVecInXY(const Vec3& vec) const {
        return vec.xCoord >= this->minX && vec.xCoord <= this->maxX &&
               vec.yCoord >= this->minY && vec.yCoord <= this->maxY;
    }

};


#endif //MCCLONE_AXISALIGNEDBB_H
