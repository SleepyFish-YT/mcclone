//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_CHUNKCOORDINTPAIR_H
#define MCCLONE_CHUNKCOORDINTPAIR_H

#include "../util/BlockPos.h"

#include <cstdint>
#include <string>

/**
 * @author SleepyFish
 * @brief Chunk coordinate pair
 */
class ChunkCoordIntPair {

private:

    mutable int cachedHashCode = 0;

public:

    const int chunkXPos;
    const int chunkZPos;

    ChunkCoordIntPair(int x, int z) noexcept :
        chunkXPos(x),
        chunkZPos(z)
    {}

    static int64_t chunkXZ2Int(int x, int z) noexcept {
        return ((int64_t) x & 0xFFFFFFFFl) | (((int64_t) z & 0xFFFFFFFFl) << 32);
    }

    int hashCode() const noexcept {
        if (this->cachedHashCode == 0) {
            int i = 1664525 * this->chunkXPos + 1013904223;
            int j = 1664525 * (this->chunkZPos ^ -559038737) + 1013904223;
            this->cachedHashCode = i ^ j;
        }
        return this->cachedHashCode;
    }

    bool operator==(const ChunkCoordIntPair &other) const noexcept {
        return this->chunkXPos == other.chunkXPos && this->chunkZPos == other.chunkZPos;
    }

    bool operator!=(const ChunkCoordIntPair &other) const noexcept {
        return !(*this == other);
    }

    int getCenterXPos() const noexcept {
        return (this->chunkXPos << 4) + 8;
    }

    int getCenterZPosition() const noexcept {
        return (this->chunkZPos << 4) + 8;
    }

    int getXStart() const noexcept {
        return this->chunkXPos << 4;
    }

    int getZStart() const noexcept {
        return this->chunkZPos << 4;
    }

    int getXEnd() const noexcept {
        return (this->chunkXPos << 4) + 15;
    }

    int getZEnd() const noexcept {
        return (this->chunkZPos << 4) + 15;
    }

    BlockPos getBlock(int x, int y, int z) const noexcept {
        return {(this->chunkXPos << 4) + x, y, (this->chunkZPos << 4) + z};
    }

    BlockPos getCenterBlock(int y) const noexcept {
        return {this->getCenterXPos(), y, this->getCenterZPosition()};
    }

    std::string toString() const {
        return "[" + std::to_string(this->chunkXPos) + ", " + std::to_string(this->chunkZPos) + "]";
    }

};

namespace std {

    template<>
    struct hash<ChunkCoordIntPair> {
        size_t operator()(const ChunkCoordIntPair &c) const noexcept {
            return c.hashCode();
        }
    };

}


#endif //MCCLONE_CHUNKCOORDINTPAIR_H
