//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#include "Main.h"
#include "../../debug/Logger.h"
#include "../../render/OpenGLWindow.h"

#include <windows.h>

Main::Main() {
    // Misc
    this->arguments = {};
    this->consoleWindow = nullptr;

    // Settings
    this->gameConfiguration = {};

    // OpenGL
    this->renderContext = {};
    this->screenSize = {};

    // Version
    this->majorVersion = "1";
    this->minorVersion = "0";
    this->patchVersion = "0";
}

int Main::main(int argc, char* argv[], std::filesystem::path executablePath) {
    this->arguments = std::vector<std::string>(argv, argv + argc);
    this->screenSize = {GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)};

    glm::ivec2 windowSize = {1050, 700};
    bool showGlErrors = true;
    bool fullscreen = false;
    bool isDemo = false;
    bool debug = false;

    // Check arguments
    {
        std::string args_str = "";
        for (const std::string& arg : this->arguments) {
            args_str += arg + " ";
        }
        Logger::log("Arguments: " + args_str);

        for (int i = 0; i < this->arguments.size(); i++) {
            const std::string& arg = this->arguments[i];

            if (arg == "--debug") {
                Logger::log("Debug mode enabled");
                debug = true;
            }

            if (arg == "--ignoreGlErrors") {
                showGlErrors = false;
            }

            if (arg == "--demo") {
                isDemo = true;
            }

            if (arg == "--resolution") {
                if (i + 1 >= this->arguments.size()) {
                    Logger::error("Missing arguments for --resolution (usage: --resolution <width> <height> or --resolution fullscreen)");
                    return -1;
                }

                if (this->arguments[i + 1] == "fullscreen") {
                    windowSize.x = this->screenSize.x;
                    windowSize.y = this->screenSize.y;
                    fullscreen = true;
                    i += 1; // skip "fullscreen"
                } else {
                    if (i + 2 >= this->arguments.size()) {
                        Logger::error("Missing arguments for --resolution (usage: --resolution <width> <height>)");
                        return -1;
                    }

                    try {
                        windowSize.x = std::stoi(this->arguments[i + 1]);
                        windowSize.y = std::stoi(this->arguments[i + 2]);
                    } catch (const std::exception&) {
                        Logger::error("Invalid arguments for --resolution (must be integers or 'fullscreen')");
                        return -1;
                    }

                    i += 2; // skip width and height
                }
            }
        }

        if (!debug) {
            ShowWindow(this->consoleWindow, SW_HIDE);
        }
    }

    // Security checks
    {
        if (windowSize.x <= 0 || windowSize.y <= 0) {
            Logger::error("Both window dimensions must be greater than 0");
            return -1;
        }

        if (windowSize.x > this->screenSize.x || windowSize.y > this->screenSize.y) {
            Logger::error("Window size does not fit on screen");
            return -1;
        }
    }

    // IntelliJ IDEA Project: src (C:/Users/Luca/Desktop/-/-/java/mcp1.8.9op/src)
    try {
        std::filesystem::path gameDir = executablePath; // std::filesystem::current_path();

        std::filesystem::path gameDirRessourcepacks = gameDir / "ressourcepacks";
        if (!std::filesystem::exists(gameDirRessourcepacks)) {
            std::filesystem::create_directory(gameDirRessourcepacks);
            Logger::log("Created ressourcepacks folder");
        }

        std::filesystem::path gameDirAssets = gameDir / "assets";
        if (!std::filesystem::exists(gameDirAssets)) {
            std::filesystem::create_directory(gameDirAssets);
            Logger::log("Created assets folder");
        }

        std::filesystem::path assetIndex = gameDirAssets / "asset_index.json";
        {
            if (!std::filesystem::exists(assetIndex)) {
                std::ofstream file(assetIndex);
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
                GameConfiguration::DisplayInformation(windowSize.x, windowSize.y, fullscreen, showGlErrors),
                GameConfiguration::FolderInformation(gameDir, gameDirRessourcepacks, gameDirAssets, assetIndex),
                GameConfiguration::GameInformation(isDemo, this->getVersion()),
                GameConfiguration::ServerInformation("testName.de", 3333),
                GameConfiguration::UserInformation("username"),
                debug
        };
    } catch (const std::exception& e) {
        Logger::error("Failed to create game configuration: " + std::string(e.what()));
        return -1;
    }

    Minecraft minecraft {this->gameConfiguration};

    std::string title = "McClone [" + this->getVersion() + "] (C++20) by SleepyFish";
    OpenGLWindow glWindow {this->gameConfiguration.displayInformation, title, &minecraft};
    if (!glWindow.init()) {
        return -1;
    }

    glWindow.execute();

    return 0;
}

std::string Main::getVersion() const {
    return this->majorVersion + "." + this->minorVersion + "." + this->patchVersion;
}

