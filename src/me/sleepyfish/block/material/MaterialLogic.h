//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_MATERIALLOGIC_H
#define MCCLONE_MATERIALLOGIC_H

#include "Material.h"

/**
 * @author SleepyFish
 * @brief Logic material
 */
class MaterialLogic : public Material {

public:

    explicit MaterialLogic(const MapColor& color) :
        Material(color)
    {
        this->setAdventureModeExempt();
    }

    bool isSolid() const override {
        return false;
    }

    bool blocksLight() const override {
        return false;
    }

    bool blocksMovement() const override {
        return false;
    }

};

#endif //MCCLONE_MATERIALLOGIC_H
