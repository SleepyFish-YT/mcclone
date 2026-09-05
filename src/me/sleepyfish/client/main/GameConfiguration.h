//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#ifndef MCCLONE_GAMECONFIGURATION_H
#define MCCLONE_GAMECONFIGURATION_H

#include <filesystem>
#include <string>
#include <filesystem>

/**
 * @author SleepyFish
 * @brief This class is used to configure the game
 */
struct GameConfiguration {

    /**
     * @author SleepyFish
     */
    struct DisplayInformation {
        int width;
        int height;
        bool fullscreen;
        bool showGlErrors;
    } displayInformation;

    /**
     * @author SleepyFish
     */
    struct FolderInformation {
        std::filesystem::path mcDataDir;
        std::filesystem::path resourcePacksDir;
        std::filesystem::path assetsDir;
        std::filesystem::path assetIndex;
    } folderInformation;

    /**
     * @author SleepyFish
     */
    struct GameInformation {
        bool isDemo;
        std::string version;
    } gameInformation;

    /**
     * @author SleepyFish
     */
    struct ServerInformation {
        std::string serverName;
        uint16_t serverPort;
    } serverInformation;

    /**
     * @author SleepyFish
     */
    struct UserInformation {
        std::string username;
    } userInformation;

    bool debugMode;

};


#endif //MCCLONE_GAMECONFIGURATION_H
