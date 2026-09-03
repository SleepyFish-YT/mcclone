//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_MATERIALS_H
#define MCCLONE_MATERIALS_H

#include "Material.h"
#include "MapColors.h"

#include "MaterialTransparent.h"
#include "MaterialLiquid.h"
#include "MaterialLogic.h"
#include "MaterialPortal.h"

/**
 * @author SleepyFish
 * @brief Material list
 */
class Materials {

protected:

    /**
     * @author SleepyFish
     * @brief Web material
     */
    class WebMaterial : public Material {

    public:

        WebMaterial() : Material(MapColors::clothColor)
        {
            this->setRequiresTool();
            this->setNoPushMobility();
        }

        bool blocksMovement() {
            return false;
        }

    };

public:

    static const inline Material grass = Material(MapColors::grassColor);
    static const inline Material ground = Material(MapColors::dirtColor);
    static const inline Material wood = (Material(MapColors::woodColor)).setBurning();
    static const inline Material rock = (Material(MapColors::stoneColor)).setRequiresTool();
    static const inline Material iron = (Material(MapColors::ironColor)).setRequiresTool();
    static const inline Material anvil = (Material(MapColors::ironColor)).setRequiresTool().setImmovableMobility();
    static const inline Material water = (MaterialLiquid(MapColors::waterColor)).setNoPushMobility();
    static const inline Material lava = (MaterialLiquid(MapColors::tntColor)).setNoPushMobility();
    static const inline Material leaves = (Material(MapColors::foliageColor)).setBurning().setTranslucent().setNoPushMobility();
    static const inline Material plants = (MaterialLogic(MapColors::foliageColor)).setNoPushMobility();
    static const inline Material vine = (MaterialLogic(MapColors::foliageColor)).setBurning().setNoPushMobility().setReplaceable();
    static const inline Material sponge = Material(MapColors::yellowColor);
    static const inline Material cloth = (Material(MapColors::clothColor)).setBurning();
    static const inline Material fire = (MaterialTransparent(MapColors::airColor)).setNoPushMobility();
    static const inline Material sand = Material(MapColors::sandColor);
    static const inline Material circuits = (MaterialLogic(MapColors::airColor)).setNoPushMobility();
    static const inline Material carpet = (MaterialLogic(MapColors::clothColor)).setBurning();
    static const inline Material glass = (Material(MapColors::airColor)).setTranslucent().setAdventureModeExempt();
    static const inline Material redstoneLight = (Material(MapColors::airColor)).setAdventureModeExempt();
    static const inline Material tnt = (Material(MapColors::tntColor)).setBurning().setTranslucent();
    static const inline Material coral = (Material(MapColors::foliageColor)).setNoPushMobility();
    static const inline Material ice = (Material(MapColors::iceColor)).setTranslucent().setAdventureModeExempt();
    static const inline Material packedIce = (Material(MapColors::iceColor)).setAdventureModeExempt();
    static const inline Material snow = (MaterialLogic(MapColors::snowColor)).setReplaceable().setTranslucent().setRequiresTool().setNoPushMobility();
    static const inline Material craftedSnow = (Material(MapColors::snowColor)).setRequiresTool();
    static const inline Material cactus = (Material(MapColors::foliageColor)).setTranslucent().setNoPushMobility();
    static const inline Material clay = Material(MapColors::clayColor);
    static const inline Material gourd = (Material(MapColors::foliageColor)).setNoPushMobility();
    static const inline Material dragonEgg = (Material(MapColors::foliageColor)).setNoPushMobility();
    static const inline Material portal = (MaterialPortal(MapColors::airColor)).setImmovableMobility();
    static const inline Material cake = (Material(MapColors::airColor)).setNoPushMobility();
    static const inline Material piston = (Material(MapColors::stoneColor)).setImmovableMobility();
    static const inline Material barrier = (Material(MapColors::airColor)).setRequiresTool().setImmovableMobility();
    static const inline Material web = WebMaterial();

};

#endif //MCCLONE_MATERIALS_H
