//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_WORLDSETTINGS_H
#define MCCLONE_WORLDSETTINGS_H

#include "../entity/player/PlayerCapabilities.h"
#include "WorldType.h"

#include <string>
#include <stdexcept>
#include <utility>

/**
 * @author SleepyFish
 */
class WorldSettings {

public:

    enum class GameType {
        NOT_SET   = -1,
        SURVIVAL  =  0,
        CREATIVE  =  1,
        ADVENTURE =  2,
        SPECTATOR =  3
    };

    static int getGameTypeID(GameType type) {
        return static_cast<int>(type);
    }

    static std::string getGameTypeName(GameType type) {
        switch (type) {
            case GameType::NOT_SET:   return "";
            case GameType::SURVIVAL:  return "survival";
            case GameType::CREATIVE:  return "creative";
            case GameType::ADVENTURE: return "adventure";
            case GameType::SPECTATOR: return "spectator";
            default: throw std::runtime_error("Unknown GameType");
        }
    }

    static GameType getGameTypeByID(int id) {
        switch (id) {
            case -1: return GameType::NOT_SET;
            case 0: return GameType::SURVIVAL;
            case 1: return GameType::CREATIVE;
            case 2: return GameType::ADVENTURE;
            case 3: return GameType::SPECTATOR;
            default: return GameType::SURVIVAL;
        }
    }

    static GameType getGameTypeByName(const std::string& name) {
        if (name == "survival") return GameType::SURVIVAL;
        if (name == "creative") return GameType::CREATIVE;
        if (name == "adventure") return GameType::ADVENTURE;
        if (name == "spectator") return GameType::SPECTATOR;
        return GameType::SURVIVAL;
    }

    static bool isAdventure(GameType type) {
        return type == GameType::ADVENTURE || type == GameType::SPECTATOR;
    }

    static bool isCreative(GameType type) {
        return type == GameType::CREATIVE;
    }

    static bool isSurvivalOrAdventure(GameType type) {
        return type == GameType::SURVIVAL || type == GameType::ADVENTURE;
    }

    static void configurePlayerCapabilities(GameType type, PlayerCapabilities& capabilities) {
        if (type == GameType::CREATIVE) {
            capabilities.allowFlying    = true;
            capabilities.isCreativeMode = true;
            capabilities.disableDamage  = true;
            capabilities.isFlying       = false;
        } else if (type == GameType::SPECTATOR) {
            capabilities.allowFlying    = true;
            capabilities.isCreativeMode = false;
            capabilities.disableDamage  = true;
            capabilities.isFlying       = true;
        } else {
            capabilities.allowFlying    = false;
            capabilities.isCreativeMode = false;
            capabilities.disableDamage  = false;
            capabilities.isFlying       = false;
        }
        capabilities.allowEdit = !isAdventure(type);
    }

private:

    long long seed = 0ll;

    GameType theGameType;

    bool mapFeaturesEnabled;

    bool hardcoreEnabled;

    WorldType terrainType;

    bool commandsAllowed;

    bool bonusChestEnabled;

    std::string worldName;

    WorldSettings(long long seed, WorldSettings::GameType gameType, bool enableMapFeatures, bool hardcoreMode, WorldType worldTypeIn) :
        terrainType(std::move(worldTypeIn))
    {
        this->worldName = "";
        this->seed = seed;
        this->theGameType = gameType;
        this->mapFeaturesEnabled = enableMapFeatures;
        this->hardcoreEnabled = hardcoreMode;
    }

    WorldSettings enableBonusChest() {
        this->bonusChestEnabled = true;
        return *this;
    }

    WorldSettings enableCommands() {
        this->commandsAllowed = true;
        return *this;
    }

    WorldSettings setWorldName(std::string name) {
        this->worldName = std::move(name);
        return *this;
    }

    bool isBonusChestEnabled() const {
        return this->bonusChestEnabled;
    }

    long long getSeed() const {
        return this->seed;
    }

    WorldSettings::GameType getGameType() const {
        return this->theGameType;
    }

    bool getHardcoreEnabled() const {
        return this->hardcoreEnabled;
    }

    bool isMapFeaturesEnabled() const {
        return this->mapFeaturesEnabled;
    }

    WorldType getTerrainType() const {
        return this->terrainType;
    }

    bool areCommandsAllowed() const {
        return this->commandsAllowed;
    }

    WorldSettings::GameType getGameTypeById(const int id) const {
        return WorldSettings::getGameTypeByID(id);
    }

    std::string getWorldName() const {
        return this->worldName;
    }

};

#endif //MCCLONE_WORLDSETTINGS_H
