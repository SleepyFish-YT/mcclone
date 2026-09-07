//
// Created by SleepyFish on 07.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ENUMRARITY_H
#define MCCLONE_ENUMRARITY_H

#include "../util/EnumChatFormatting.h"

/**
 * @author SleepyFish
 * @brief Item rarity enum
 */
enum class EnumRarity {

    COMMON = 0,
    UNCOMMON,
    RARE,
    EPIC

};

namespace {

    struct RarityInfo {

        EnumChatFormatting rarityColor;

        std::string rarityName;

    };

    inline const RarityInfo& getRarityInfo(EnumRarity rarity) {
        static const RarityInfo infos[] = {
                {EnumChatFormatting::WHITE, "Common"},
                {EnumChatFormatting::YELLOW, "Uncommon"},
                {EnumChatFormatting::AQUA, "Rare"},
                {EnumChatFormatting::LIGHT_PURPLE, "Epic"},
        };

        return infos[static_cast<int>(rarity)];
    }

};


#endif //MCCLONE_ENUMRARITY_H
