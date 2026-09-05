//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ENUMWORLDBLOCKLAYER_H
#define MCCLONE_ENUMWORLDBLOCKLAYER_H

#include <string>
#include <string_view>

namespace {

    /**
     * @author SleepyFish
     * @brief Enum for world block layers
     */
    enum class EnumWorldBlockLayer {
        SOLID = 0,
        CUTOUT_MIPPED,
        CUTOUT,
        TRANSLUCENT
    };

    inline std::string_view toString(EnumWorldBlockLayer layer) {
        switch (layer) {
            case EnumWorldBlockLayer::SOLID: return "Solid";
            case EnumWorldBlockLayer::CUTOUT_MIPPED: return "Mipped Cutout";
            case EnumWorldBlockLayer::CUTOUT: return "Cutout";
            case EnumWorldBlockLayer::TRANSLUCENT: return "Translucent";
            default: return "Unknown";
        }
    }

};


#endif //MCCLONE_ENUMWORLDBLOCKLAYER_H
