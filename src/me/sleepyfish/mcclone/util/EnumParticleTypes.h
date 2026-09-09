//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ENUMPARTICLETYPES_H
#define MCCLONE_ENUMPARTICLETYPES_H

#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <optional>

/**
 * @author SleepyFish
 * @brief Particle types enum
 */
enum class EnumParticleTypes {
    EXPLOSION_NORMAL = 0,
    EXPLOSION_LARGE,
    EXPLOSION_HUGE,
    FIREWORKS_SPARK,
    WATER_BUBBLE,
    WATER_SPLASH,
    WATER_WAKE,
    SUSPENDED,
    SUSPENDED_DEPTH,
    CRIT,
    CRIT_MAGIC,
    SMOKE_NORMAL,
    SMOKE_LARGE,
    SPELL,
    SPELL_INSTANT,
    SPELL_MOB,
    SPELL_MOB_AMBIENT,
    SPELL_WITCH,
    DRIP_WATER,
    DRIP_LAVA,
    VILLAGER_ANGRY,
    VILLAGER_HAPPY,
    TOWN_AURA,
    NOTE,
    PORTAL,
    ENCHANTMENT_TABLE,
    FLAME,
    LAVA,
    FOOTSTEP,
    CLOUD,
    REDSTONE,
    SNOWBALL,
    SNOW_SHOVEL,
    SLIME,
    HEART,
    BARRIER,
    ITEM_CRACK,
    BLOCK_CRACK,
    BLOCK_DUST,
    WATER_DROP,
    ITEM_TAKE,
    MOB_APPEARANCE
};

struct ParticleInfo {

    std::string_view particleName;
    int particleID;
    bool shouldIgnoreRange;
    int argumentCount;

};

inline const ParticleInfo& getParticleInfo(EnumParticleTypes type) {
    static const ParticleInfo infos[] = {
            { "explode",           0,  true,  0 },
            { "largeexplode",      1,  true,  0 },
            { "hugeexplosion",     2,  true,  0 },
            { "fireworksSpark",    3,  false, 0 },
            { "bubble",            4,  false, 0 },
            { "splash",            5,  false, 0 },
            { "wake",              6,  false, 0 },
            { "suspended",         7,  false, 0 },
            { "depthsuspend",      8,  false, 0 },
            { "crit",              9,  false, 0 },
            { "magicCrit",         10, false, 0 },
            { "smoke",             11, false, 0 },
            { "largesmoke",        12, false, 0 },
            { "spell",             13, false, 0 },
            { "instantSpell",      14, false, 0 },
            { "mobSpell",          15, false, 0 },
            { "mobSpellAmbient",   16, false, 0 },
            { "witchMagic",        17, false, 0 },
            { "dripWater",         18, false, 0 },
            { "dripLava",          19, false, 0 },
            { "angryVillager",     20, false, 0 },
            { "happyVillager",     21, false, 0 },
            { "townaura",          22, false, 0 },
            { "note",              23, false, 0 },
            { "portal",            24, false, 0 },
            { "enchantmenttable",  25, false, 0 },
            { "flame",             26, false, 0 },
            { "lava",              27, false, 0 },
            { "footstep",          28, false, 0 },
            { "cloud",             29, false, 0 },
            { "reddust",           30, false, 0 },
            { "snowballpoof",      31, false, 0 },
            { "snowshovel",        32, false, 0 },
            { "slime",             33, false, 0 },
            { "heart",             34, false, 0 },
            { "barrier",           35, false, 0 },
            { "iconcrack_",        36, false, 2 },
            { "blockcrack_",       37, false, 1 },
            { "blockdust_",        38, false, 1 },
            { "droplet",           39, false, 0 },
            { "take",              40, false, 0 },
            { "mobappearance",     41, true,  0 },
    };

    return infos[static_cast<int>(type)];
}

inline std::optional<EnumParticleTypes> getParticleFromId(int id) {
    if (id < 0 || id > 41) return std::nullopt;
    return static_cast<EnumParticleTypes>(id);
}

inline bool hasArguments(EnumParticleTypes type) noexcept {
    return getParticleInfo(type).argumentCount > 0;
}

inline const std::vector<std::string>& getParticleNames() {
    static const std::vector<std::string> names = []() {
        std::vector<std::string> result;

        for (int i = 0; i <= 41; ++i) {
            const auto& info = getParticleInfo(static_cast<EnumParticleTypes>(i));
            std::string_view name = info.particleName;
            if (!name.ends_with('_'))
                result.emplace_back(name);
        }

        return result;
    }();

    return names;
}

#endif //MCCLONE_ENUMPARTICLETYPES_H
