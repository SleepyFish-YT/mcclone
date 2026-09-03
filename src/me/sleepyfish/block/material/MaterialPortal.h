//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_MATERIALPORTAL_H
#define MCCLONE_MATERIALPORTAL_H

#include "Material.h"

/**
 * @author SleepyFish
 * @brief Portal material
 */
class MaterialPortal : public Material {

public:

    explicit MaterialPortal(const MapColor& color) :
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

#endif //MCCLONE_MATERIALPORTAL_H
