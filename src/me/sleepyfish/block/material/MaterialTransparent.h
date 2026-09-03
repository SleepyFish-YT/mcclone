//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_MATERIALTRANSPARENT_H
#define MCCLONE_MATERIALTRANSPARENT_H

#include "Material.h"

/**
 * @author SleepyFish
 * @brief Transparent material
 */
class MaterialTransparent : public Material {

public:

    explicit MaterialTransparent(const MapColor& color) :
        Material(color)
    {
        this->setReplaceable();
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


#endif //MCCLONE_MATERIALTRANSPARENT_H
