//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_MATERIALLIQUID_H
#define MCCLONE_MATERIALLIQUID_H

#include "Material.h"

/**
 * @author SleepyFish
 * @brief Liquid material
 */
class MaterialLiquid : public Material {

public:

    explicit MaterialLiquid(const MapColor& color) :
            Material(color)
    {
        this->setReplaceable();
        this->setNoPushMobility();
    }

    bool isLiquid() const override {
        return true;
    }

    bool blocksMovement() const override {
        return false;
    }

    bool isSolid() const override {
        return false;
    }

};


#endif //MCCLONE_MATERIALLIQUID_H
