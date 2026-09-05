//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_SOUNDCATEGORY_H
#define MCCLONE_SOUNDCATEGORY_H

#include <string>
#include <unordered_map>
#include <stdexcept>

// namespace { is basically doing nothing in code if i remember correctly, so its just here to make the code more readable
namespace {

    /**
     * @author SleepyFish
     */
    enum class SoundCategory {
        MASTER = 0,
        MUSIC,
        RECORDS,
        WEATHER,
        BLOCKS,
        MOBS,
        ANIMALS,
        PLAYERS,
        AMBIENT
    };

    /**
     * @author SleepyFish
     */
    struct SoundCategoryInfo {
        SoundCategory category;
        std::string name;
        int id;
    };

    inline const SoundCategoryInfo SOUND_CATEGORY_ENTRIES[] = {
            { SoundCategory::MASTER,  "master",  0 },
            { SoundCategory::MUSIC,   "music",   1 },
            { SoundCategory::RECORDS, "record",  2 },
            { SoundCategory::WEATHER, "weather", 3 },
            { SoundCategory::BLOCKS,  "block",   4 },
            { SoundCategory::MOBS,    "hostile", 5 },
            { SoundCategory::ANIMALS, "neutral", 6 },
            { SoundCategory::PLAYERS, "player",  7 },
            { SoundCategory::AMBIENT, "ambient", 8 }
    };

    inline const std::unordered_map<std::string, SoundCategory>& getSoundCategoryByName() {
        static std::unordered_map<std::string, SoundCategory> NAME_CATEGORY_MAP;
        static std::unordered_map<int, SoundCategory> ID_CATEGORY_MAP;
        static bool initialized = false;

        if (!initialized) {
            for (const auto& entry : SOUND_CATEGORY_ENTRIES) {
                if (NAME_CATEGORY_MAP.count(entry.name) || ID_CATEGORY_MAP.count(entry.id)) {
                    throw std::runtime_error("Clash in Sound Category ID & Name pools! Cannot insert " + entry.name);
                }
                NAME_CATEGORY_MAP[entry.name] = entry.category;
                ID_CATEGORY_MAP[entry.id] = entry.category;
            }
            initialized = true;
        }

        return NAME_CATEGORY_MAP;
    }

    inline std::string getSoundCategoryName(SoundCategory category) {
        for (const auto& entry : SOUND_CATEGORY_ENTRIES)
            if (entry.category == category)
                return entry.name;
        throw std::runtime_error("Unknown SoundCategory");
    }

    inline int getSoundCategoryId(SoundCategory category) {
        for (const auto& entry : SOUND_CATEGORY_ENTRIES)
            if (entry.category == category)
                return entry.id;
        throw std::runtime_error("Unknown SoundCategory");
    }

    inline SoundCategory* getSoundCategory(const std::string& name) {
        auto& map = getSoundCategoryByName();
        auto it = map.find(name);
        if (it != map.end())
            return const_cast<SoundCategory*>(&it->second);
        return nullptr;
    }

};


#endif //MCCLONE_SOUNDCATEGORY_H
