//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_MOVINGOBJECTPOSITION_H
#define MCCLONE_MOVINGOBJECTPOSITION_H

#include "Vec3.h"
#include "BlockPos.h"
#include "EnumFacing.h"

#include <string>

class Entity;

/**
 * @author SleepyFish
 */
class MovingObjectPosition {

public:

    enum class MovingObjectType {
        MISS = 0,
        BLOCK,
        ENTITY
    };

    MovingObjectType typeOfHit;
    EnumFacing sideHit;
    Vec3 hitVec;
    Entity *entityHit = nullptr;

private:

    BlockPos blockPos;

public:

    MovingObjectPosition(const Vec3 &hitVecIn, EnumFacing facing, const BlockPos &blockPosIn) :
        typeOfHit(MovingObjectType::BLOCK),
        sideHit(facing),
        hitVec(hitVecIn.xCoord, hitVecIn.yCoord, hitVecIn.zCoord),
        blockPos(blockPosIn)
    {}

    MovingObjectPosition(const Vec3 &hitVecIn, EnumFacing facing) :
        typeOfHit(MovingObjectType::BLOCK),
        sideHit(facing),
        hitVec(hitVecIn.xCoord, hitVecIn.yCoord, hitVecIn.zCoord),
        blockPos(BlockPos::ORIGIN)
    {}

    MovingObjectPosition(MovingObjectType typeOfHitIn, const Vec3 &hitVecIn, EnumFacing sideHitIn, const BlockPos &blockPosIn) :
        typeOfHit(typeOfHitIn),
        sideHit(sideHitIn),
        hitVec(hitVecIn.xCoord, hitVecIn.yCoord, hitVecIn.zCoord),
        blockPos(blockPosIn)
    {}

    MovingObjectPosition(Entity *entityHitIn, const Vec3 &hitVecIn) :
        typeOfHit(MovingObjectType::ENTITY),
        sideHit(EnumFacing::NORTH),
        hitVec(hitVecIn),
        entityHit(entityHitIn),
        blockPos(BlockPos::ORIGIN)
    {}

    const BlockPos &getBlockPos() const {
        return this->blockPos;
    }

    std::string toString() const {
        return "HitResult{type=" + std::to_string((int) this->typeOfHit) +
               ", blockpos=" + this->blockPos.toString() +
               ", f=" + this->sideHit.toString() +
               ", pos=" + this->hitVec.toString() + '}';
    }

};


#endif //MCCLONE_MOVINGOBJECTPOSITION_H
