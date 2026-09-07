//
// Created by SleepyFish on 07.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ENUMPLAYERMODELPARTS_H
#define MCCLONE_ENUMPLAYERMODELPARTS_H

#include <string>

/**
 * @author SleepyFish
 */
enum class EnumPlayerModelParts {

    CAPE = 0,
    JACKET,
    LEFT_SLEEVE,
    RIGHT_SLEEVE,
    LEFT_PANTS_LEG,
    RIGHT_PANTS_LEG,
    HAT

};

namespace {

    struct PlayerModelPartInfo {

        int partId;

        int partMask;

        std::string partName;

        std::string translationKey;

    };



    inline PlayerModelPartInfo getPlayerModelPartInfo(EnumPlayerModelParts part) {
        switch (part) {
            case EnumPlayerModelParts::CAPE: return { 0, 1 << 0, "cape", "options.modelPart.cape"};
            case EnumPlayerModelParts::JACKET: return { 1, 1 << 1, "jacket", "options.modelPart.jacket"};
            case EnumPlayerModelParts::LEFT_SLEEVE: return { 2, 1 << 2, "left_sleeve", "options.modelPart.left_sleeve"};
            case EnumPlayerModelParts::RIGHT_SLEEVE: return { 3, 1 << 3, "right_sleeve", "options.modelPart.right_sleeve"};
            case EnumPlayerModelParts::LEFT_PANTS_LEG: return { 4, 1 << 4, "left_pants_leg", "options.modelPart.left_pants_leg"};
            case EnumPlayerModelParts::RIGHT_PANTS_LEG: return { 5, 1 << 5, "right_pants_leg", "options.modelPart.right_pants_leg"};
            case EnumPlayerModelParts::HAT: return { 6, 1 << 6, "hat", "options.modelPart.hat"};
        }
        return { 0, 1, "cape", "options.modelPart.cape" };
    }

};


#endif //MCCLONE_ENUMPLAYERMODELPARTS_H
