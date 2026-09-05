//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_BLOCKPOS_H
#define MCCLONE_BLOCKPOS_H

#include "Vec3i.h"
#include "Vec3.h"
#include "EnumFacing.h"
#include "MathHelper.h"

#include <cmath>
#include <functional>

/**
 * @author SleepyFish
 */
class BlockPos : public Vec3i {

private:

    static inline const int NUM_X_BITS = 1 + MathHelper::calculateLogBaseTwo(MathHelper::roundUpToPowerOfTwo(30000000));
    static inline const int NUM_Z_BITS = NUM_X_BITS;
    static inline const int NUM_Y_BITS = 64 - NUM_X_BITS - NUM_Z_BITS;
    static inline const int Y_SHIFT = NUM_Z_BITS;
    static inline const int X_SHIFT = Y_SHIFT + NUM_Y_BITS;
    static inline const long X_MASK = (1L << NUM_X_BITS) - 1L;
    static inline const long Y_MASK = (1L << NUM_Y_BITS) - 1L;
    static inline const long Z_MASK = (1L << NUM_Z_BITS) - 1L;

public:

    BlockPos(int x, int y, int z) noexcept :
        Vec3i(x, y, z)
    {}

    BlockPos(double x, double y, double z) noexcept :
        Vec3i((int) x, (int) y, (int) z)
    {}

    explicit BlockPos(const Vec3& source) noexcept :
        Vec3i((int) source.xCoord, (int) source.yCoord, (int) source.zCoord)
    {}

    explicit BlockPos(const Vec3i& source) noexcept :
        Vec3i(source.x, source.y, source.z)
    {}

    BlockPos add(double x, double y, double z) const noexcept {
        return (x == 0.0 && y == 0.0 && z == 0.0) ? *this : BlockPos((double) this->x + x, (double) this->y + y, (double) this->z + z);
    }

    BlockPos add(int x, int y, int z) const noexcept {
        return (x == 0 && y == 0 && z == 0) ? *this : BlockPos(this->x + x, this->y + y, this->z + z);
    }

    BlockPos add(const Vec3i& vec) const noexcept {
        return (vec.x == 0 && vec.y == 0 && vec.z == 0) ? *this : BlockPos(this->x + vec.x, this->y + vec.y, this->z + vec.z);
    }

    BlockPos subtract(const Vec3i& vec) const noexcept {
        return (vec.x == 0 && vec.y == 0 && vec.z == 0) ? *this : BlockPos(this->x - vec.x, this->y - vec.y, this->z - vec.z);
    }

    BlockPos up(int n = 1) const noexcept { return this->offset(EnumFacing::UP, n); }

    BlockPos down(int n = 1) const { return this->offset(EnumFacing::DOWN, n); }

    BlockPos north(int n = 1) const { return this->offset(EnumFacing::NORTH, n); }

    BlockPos south(int n = 1) const { return this->offset(EnumFacing::SOUTH, n); }

    BlockPos west(int n = 1) const { return this->offset(EnumFacing::WEST, n); }

    BlockPos east(int n = 1) const { return this->offset(EnumFacing::EAST, n); }

    BlockPos offset(EnumFacing facing) const noexcept { return this->offset(facing, 1); }

    BlockPos offset(EnumFacing facing, int n) const noexcept {
        return n == 0 ? *this : BlockPos(
                this->x + facing.getFrontOffsetX() * n,
                this->y + facing.getFrontOffsetY() * n,
                this->z + facing.getFrontOffsetZ() * n
        );
    }

    BlockPos crossProduct(const Vec3i& vec) const noexcept {
        return {
            this->y * vec.z - this->z * vec.y,
            this->z * vec.x - this->x * vec.z,
            this->x * vec.y - this->y * vec.x
        };
    }

    long toLong() const noexcept {
        return ((long) this->x & X_MASK) << X_SHIFT |
               ((long) this->y & Y_MASK) << Y_SHIFT |
               ((long) this->z & Z_MASK);
    }

    static BlockPos fromLong(long serialized) noexcept {
        const int i = (int) (serialized << 64 - X_SHIFT - NUM_X_BITS >> 64 - NUM_X_BITS);
        const int j = (int) (serialized << 64 - Y_SHIFT - NUM_Y_BITS >> 64 - NUM_Y_BITS);
        const int k = (int) (serialized << 64 - NUM_Z_BITS >> 64 - NUM_Z_BITS);
        return {i, j, k};
    }

    static std::vector<BlockPos> getAllInBox(const BlockPos& from, const BlockPos& to) {
        const BlockPos min(
                std::min(from.x, to.x),
                std::min(from.y, to.y),
                std::min(from.z, to.z)
        );
        const BlockPos max(
                std::max(from.x, to.x),
                std::max(from.y, to.y),
                std::max(from.z, to.z)
        );

        std::vector<BlockPos> result;
        for (int k = min.z; k <= max.z; ++k)
            for (int j = min.y; j <= max.y; ++j)
                for (int i = min.x; i <= max.x; ++i)
                    result.emplace_back(i, j, k);
        return result;
    }

    static const BlockPos ORIGIN;

};

inline const BlockPos BlockPos::ORIGIN(0, 0, 0);

class MutableBlockPos : public BlockPos {

public:

    int x, y, z;

    MutableBlockPos() noexcept :
        BlockPos(0, 0, 0),
        x(0),
        y(0),
        z(0)
    {}

    MutableBlockPos(int x_, int y_, int z_) noexcept :
        BlockPos(0, 0, 0),
        x(x_),
        y(y_),
        z(z_)
    {}

    MutableBlockPos& set(int xIn, int yIn, int zIn) noexcept {
        this->x = xIn;
        this->y = yIn;
        this->z = zIn;
        return *this;
    }

};


#endif //MCCLONE_BLOCKPOS_H
