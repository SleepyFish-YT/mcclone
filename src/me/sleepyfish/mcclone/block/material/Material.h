//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_MATERIAL_H
#define MCCLONE_MATERIAL_H

#include "MapColor.h"

/**
 * @author SleepyFish
 * @brief Material base class
 */
class Material {

private:

    bool canBurn;

    bool replaceable;

    bool isTranslucent;

    const MapColor materialMapColor;

    bool requiresNoTool = true;

    int mobilityFlag;

public:

    explicit Material(const MapColor& color) :
        materialMapColor(color)
    {}

    virtual bool isLiquid() const {
        return false;
    }

    virtual bool isSolid() const {
        return true;
    }

    virtual bool blocksLight() const {
        return true;
    }

    virtual bool blocksMovement() const {
        return true;
    }

    MapColor getMaterialMapColor() const {
        return this->materialMapColor;
    }
    
    Material setAdventureModeExempt() const {
        return *this;
    }

    Material setTranslucent() {
        this->isTranslucent = true;
        return *this;
    }

    Material setRequiresTool() {
        this->requiresNoTool = false;
        return *this;
    }

    Material setBurning() {
        this->canBurn = true;
        return *this;
    }

    Material setNoPushMobility() {
        this->mobilityFlag = 1;
        return *this;
    }

    Material setImmovableMobility() {
        this->mobilityFlag = 2;
        return *this;
    }

    Material setReplaceable() {
        this->replaceable = true;
        return *this;
    }

};


#endif //MCCLONE_MATERIAL_H
