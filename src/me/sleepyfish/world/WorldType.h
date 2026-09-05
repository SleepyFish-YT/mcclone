//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_WORLDTYPE_H
#define MCCLONE_WORLDTYPE_H

#include <string>
#include <array>
#include <algorithm>
#include <stdexcept>

/**
 * @author SleepyFish
 * @brief World type
 */
class WorldType {

private:

    int worldTypeId;

    std::string worldType;

    int generatorVersion;

    bool canBeCreated = true;

    bool isWorldTypeVersioned = false;

    bool hasNotificationData = false;

    WorldType(int id, const std::string& name, int version = 0) noexcept :
        worldTypeId(id),
        worldType(name),
        generatorVersion(version),
        canBeCreated(true)
    {
        WorldType::worldTypes[id] = this;
    }

    WorldType& setVersioned() noexcept {
        this->isWorldTypeVersioned = true;
        return *this;
    }

    WorldType& setCanBeCreated(bool enable) noexcept {
        this->canBeCreated = enable;
        return *this;
    }

    WorldType& setNotificationData() noexcept {
        this->hasNotificationData = true;
        return *this;
    }

public:

    static std::array<WorldType*, 8> worldTypes;



    static WorldType DEFAULT;

    static WorldType FLAT;

    static WorldType LARGE_BIOMES;

    static WorldType AMPLIFIED;

    static WorldType CUSTOMIZED;

    static WorldType DEBUG_WORLD;

    static WorldType DEFAULT_1_1;

    std::string getWorldTypeName() const noexcept {
        return this->worldType;
    }

    std::string getTranslateName() const {
        return "generator." + this->worldType;
    }

    std::string getTranslatedInfo() const {
        return this->getTranslateName() + ".info";
    }

    int getGeneratorVersion() const noexcept {
        return this->generatorVersion;
    }

    WorldType* getWorldTypeForGeneratorVersion(int version) noexcept {
        return this == &DEFAULT && version == 0 ? &DEFAULT_1_1 : this;
    }

    bool getCanBeCreated() const noexcept {
        return this->canBeCreated;
    }

    bool isVersioned() const noexcept {
        return this->isWorldTypeVersioned;
    }

    bool showWorldInfoNotice() const noexcept {
        return this->hasNotificationData;
    }

    int getWorldTypeID() const noexcept {
        return this->worldTypeId;
    }

    static WorldType* parseWorldType(const std::string& type) {
        for (WorldType* wt : worldTypes) {
            if (wt != nullptr) {
                std::string name = wt->worldType;
                std::string lower = type;
                std::transform(name.begin(),  name.end(),  name.begin(),  ::tolower);
                std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
                if (name == lower) return wt;
            }
        }
        return nullptr;
    }

};


inline std::array<WorldType*, 8> WorldType::worldTypes = {};



inline WorldType WorldType::DEFAULT      = WorldType(0, "default", 1).setVersioned();

inline WorldType WorldType::FLAT         = WorldType(1, "flat");

inline WorldType WorldType::LARGE_BIOMES = WorldType(2, "largeBiomes");

inline WorldType WorldType::AMPLIFIED    = WorldType(3, "amplified").setNotificationData();

inline WorldType WorldType::CUSTOMIZED   = WorldType(4, "customized");

inline WorldType WorldType::DEBUG_WORLD  = WorldType(5, "debug_all_block_states");

inline WorldType WorldType::DEFAULT_1_1  = WorldType(8, "default_1_1", 0).setCanBeCreated(false);


#endif //MCCLONE_WORLDTYPE_H
