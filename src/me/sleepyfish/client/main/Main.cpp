//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#include "../../../../../pcb.h"

#include "Main.h"

#include "../../debug/Logger.h"
#include "../../render/OpenGLWindow.h"

#ifdef _WIN32
#include <windows.h>
#endif //_WIN32

Main::Main() {
    // misc
    this->arguments = {};
    this->consoleWindow = nullptr;

    // settings
    this->gameConfiguration = {};

    // openGL
    this->renderContext = {};
    this->screenSize = {};

    // version
    this->majorVersion = "1";
    this->minorVersion = "0";
    this->patchVersion = "2";
}

int Main::main(int argc, char* argv[], const std::filesystem::path& gameDir_dir) {
    this->arguments = std::vector<std::string>(argv, argv + argc);

#ifdef _WIN32
    this->screenSize = { ::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN) };
    if (this->screenSize.x <= 0 || this->screenSize.y <= 0) {
        Logger::error("Failed to retrieve screen resolution");
        return MCCLONE_ERR_RESOLUTION;
    }
#endif //_WIN32

    glm::ivec2 windowSize = {1050, 700};

    // clion shows this is unused but it is used. (search for this->gameConfiguration =) to find it.
    bool args_showGlErrors = true;
    bool args_isFullscreen = false;
    bool args_isDemo = false;
    bool args_isDebug = false;

    // check arguments
    {
        std::string args_str; // initializing this is useless performance waste
        for (const std::string& arg : this->arguments) {
            args_str += arg + " ";
        }
        Logger::log("Arguments: " + args_str);

        for (int i = 0; i < this->arguments.size(); i++) {
            const std::string& arg = this->arguments[i];

            if (arg == "--debug") {
                Logger::log("Debug mode enabled");
                args_isDebug = true;
            }

            if (arg == "--ignoreGlErrors") {
                args_showGlErrors = false;
            }

            if (arg == "--demo") {
                args_isDemo = true;
            }

            if (arg == "--resolution") {
                if (i + 1 >= this->arguments.size()) {
                    Logger::error("Missing arguments for --resolution (usage: --resolution <width> <height> or --resolution fullscreen)");
                    return MCCLONE_ERR_ARGUMENTS;
                }

                if (this->arguments[i + 1] == "fullscreen") {
                    windowSize.x = this->screenSize.x;
                    windowSize.y = this->screenSize.y;
                    args_isFullscreen = true;
                    i += 1; // skip "fullscreen"
                } else {
                    if (i + 2 >= this->arguments.size()) {
                        Logger::error("Missing arguments for --resolution (usage: --resolution <width> <height>)");
                        return MCCLONE_ERR_ARGUMENTS;
                    }

                    try {
                        windowSize.x = std::stoi(this->arguments[i + 1]);
                        windowSize.y = std::stoi(this->arguments[i + 2]);
                    } catch (const std::exception&) {
                        Logger::error("Invalid arguments for --resolution (must be integers or 'fullscreen')");
                        return MCCLONE_ERR_ARGUMENTS;
                    }

                    i += 2; // skip width and height
                }
            }
        }

        if (!args_isDebug) {
            ::ShowWindow(this->consoleWindow, SW_HIDE);
        }
    }

    // security checks
    {
        if (windowSize.x <= 0 || windowSize.y <= 0) {
            Logger::error("Both window dimensions must be greater than 0");
            return MCCLONE_ERR_DIMENSIONS;
        }

        if (windowSize.x > this->screenSize.x + 8 || windowSize.y > this->screenSize.y + 8) { // + 8 to account other things
            Logger::error("Window size does not fit on screen");
            return MCCLONE_ERR_DIMENSIONS;
        }
    }

    try {
        std::filesystem::path resourcepacks_dir(gameDir_dir / "resourcepacks");
        if (!std::filesystem::exists(resourcepacks_dir)) {
            std::filesystem::create_directory(resourcepacks_dir);
            Logger::log("Created resourcepacks folder");
        }

        std::filesystem::path assets_dir(gameDir_dir / "assets");
        if (!std::filesystem::exists(assets_dir)) {
            std::filesystem::create_directory(assets_dir);
            Logger::log("Created assets folder");
        }

        std::filesystem::path assetIndex_file(assets_dir / "asset_index.json");
        {
            if (!std::filesystem::exists(assetIndex_file)) {
                std::ofstream file(assetIndex_file);
                {
                    file << "{}";
                }
                file.close();

                if (file.fail()) {
                    Logger::log("Failed to create asset_index.json");
                }

                if (!file.is_open()) {
                    Logger::log("asset_index.json created");
                }
            } else {
                // Logger::log("asset_index.json already exists");
            }
        }

        this->gameConfiguration = {
                GameConfiguration::DisplayInformation(windowSize.x, windowSize.y, args_isFullscreen, args_showGlErrors),
                GameConfiguration::FolderInformation(gameDir_dir, resourcepacks_dir, assets_dir, assetIndex_file),
                GameConfiguration::GameInformation(args_isDemo, this->getVersion()),
                GameConfiguration::ServerInformation("testName.de", 3333),
                GameConfiguration::UserInformation("username"),
                args_isDebug,
                this->arguments
        };
    } catch (const std::exception& e) {
        Logger::error("Failed to create game configuration: " + std::string(e.what()));
        return MCCLONE_ERR_GAME_CONFIG;
    }

    Minecraft minecraft {this->gameConfiguration};

    // if im correct, std::move should be used here, to move it from local to OpenGLWindow, since it is not used after this.
    std::string title = "McClone [" + this->getVersion() + "] (C++20) by " + Main::AUTHOR;
    OpenGLWindow glWindow {this->gameConfiguration.displayInformation, std::move(title), &minecraft};
    if (!glWindow.init()) {
        return -1;
    }

    glWindow.execute();
    glWindow.stop();

    return MCCLONE_ERR_NONE;
}

std::string Main::getVersion() const {
    return this->majorVersion + "." + this->minorVersion + "." + this->patchVersion;
}

