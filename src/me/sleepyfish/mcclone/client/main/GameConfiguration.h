//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#ifndef MCCLONE_GAMECONFIGURATION_H
#define MCCLONE_GAMECONFIGURATION_H

#include <filesystem>
#include <string>
#include <vector>
#include <cstdint>

/**
 * @author SleepyFish
 * @brief This class is used to configure the game
 */
struct GameConfiguration {

    /**
     * @author SleepyFish
     */
    struct Display {
        int width;
        int height;
        bool fullscreen;
        bool showGlErrors;
    } displayInformation{};

    /**
     * @author SleepyFish
     */
    struct Folder {
        std::filesystem::path mcDataDir;
        std::filesystem::path resourcePacksDir;
        std::filesystem::path assetsDir;
        std::filesystem::path assetIndexFile;
    } folderInformation{};

    /**
     * @author SleepyFish
     */
    struct Game {
        bool isDemo;
        std::string version;
    } gameInformation{};

    /**
     * @author SleepyFish
     */
    struct Server {
        std::string serverName;
        uint16_t serverPort;
    } serverInformation{};

    /**
     * @author SleepyFish
     */
    struct User {
        std::string username;
    } userInformation{};

    bool debugMode{};

    std::vector<std::string> arguments{};

};


#endif //MCCLONE_GAMECONFIGURATION_H
