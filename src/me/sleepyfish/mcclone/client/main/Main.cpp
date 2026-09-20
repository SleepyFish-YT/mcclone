//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#include "../../../../../../pcb.h"

#include "Main.h"

#include "GameConfiguration.h"
#include "../Minecraft.h"
#include "../../debug/Logger.h"
#include "../../../sava/window/OpenGLWindow.h"
#include "../../../sava/window/RenderInformation.h"
#include "../../../sava/SavaUtil.h"

Main::Main() {
    // misc
    this->arguments = {};

#if _WIN32
    this->consoleWindow = nullptr;
#elif __linux__ || __APPLE__
    this->consoleWindow = ::stdout;
#endif //_WIN32

    // settings
    this->gameConfiguration = new GameConfiguration();

    // screen
    this->screenSize = {};

    // version
#ifdef MCCLONE_VERSION
    this->majorVersion = std::to_string(MCCLONE_VERSION_MAJOR);
    this->minorVersion = std::to_string(MCCLONE_VERSION_MINOR);
    this->patchVersion = std::to_string(MCCLONE_VERSION_PATCH);
#else
    this->majorVersion = "-0";
    this->minorVersion = "-0";
    this->patchVersion = "-0";
#endif //MCCLONE_VERSION
}

int Main::main(int arg_count, char *arg_vals[], const std::filesystem::path &gameDir_dir) {
    this->arguments = std::vector<std::string>(arg_vals, arg_vals + arg_count);

    if (!::glfwInit()) {
        Logger::error("Failed to initialize GLFW (glfwInit)");
        return MCCLONE_ERR_RESOLUTION;
    }

    if (::GLFWmonitor *m = ::glfwGetPrimaryMonitor()) {
        if (const ::GLFWvidmode *vm = ::glfwGetVideoMode(m))
            this->screenSize = { vm->width, vm->height };
    }

    if (this->screenSize.x <= 0 || this->screenSize.y <= 0) {
        Logger::error("Failed to retrieve screen resolution");
        return MCCLONE_ERR_RESOLUTION;
    }

    ::glm::ivec2 windowSize = {1050, 700};

    // clion shows this is unused but it is used. (search for this->gameConfiguration =) to find it.
    bool args_showGlErrors = true;
    bool args_isFullscreen = false;
    bool args_isDemo = false;
    bool args_isDebug = false;

    // check arguments
    {
        std::string args_str; // initializing this is useless performance waste
        for (const std::string &arg : this->arguments) {
            args_str += arg + " ";
        }

        Logger::log("Arguments: " + args_str);

        for (int i = 0; i < this->arguments.size(); i++) {
            const std::string &arg = this->arguments[i];

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

        if (!args_isDebug && this->consoleWindow) {
#ifdef _WIN32
            ::ShowWindow(this->consoleWindow, SW_HIDE);
#endif //_WIN32
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
            if (std::filesystem::create_directory(resourcepacks_dir)) {
                Logger::log("Created resourcepacks folder");
            } else {
                Logger::error("Can't create resourcepacks folder");
                return MCCLONE_ERR_RESOURCEPACKS;
            }
        }

        std::filesystem::path assets_dir(gameDir_dir / "assets");
        if (!std::filesystem::exists(assets_dir)) {
            if (std::filesystem::create_directory(assets_dir)) {
                Logger::log("Created assets folder");
            } else {
                Logger::error("Can't create assets folder");
                return MCCLONE_ERR_ASSETS;
            }
        }

        std::filesystem::path assets_index_dir(assets_dir / "indexes");
        if (!std::filesystem::exists(assets_dir)) {
            if (std::filesystem::create_directory(assets_dir)) {
                Logger::log("Created assets index folder");
            } else {
                Logger::error("Can't create assets index folder");
                return MCCLONE_ERR_ASSETS_INDEX;
            }
        }

        this->gameConfiguration = new GameConfiguration(
                GameConfiguration::Display(windowSize.x, windowSize.y, args_isFullscreen, args_showGlErrors),
                GameConfiguration::Folder(gameDir_dir, resourcepacks_dir, assets_dir, (assets_index_dir / "asset_index.json")),
                GameConfiguration::Game(args_isDemo, this->getVersion()),
                GameConfiguration::Server("testName.de", 3333),
                GameConfiguration::User(SavaUtil::StringUtil::GetRandomPlayerName()),
                args_isDebug,
                this->arguments
        );
    } catch (const std::exception &e) {
        Logger::error("Failed to create game configuration: " + std::string(e.what()));
        return MCCLONE_ERR_GAME_CONFIG;
    }

    auto minecraft = std::make_unique<Minecraft>(this->gameConfiguration);

    Logger::log("Starting game as: {}", this->gameConfiguration->userInformation.username);

    // if im correct, std::move should be used here, to move it from local to OpenGLWindow, since it is not used after this.
    std::string title = "McClone [" + this->getVersion() + "] (C++20) by " + Main::AUTHOR;
    auto glWindow = std::make_unique<OpenGLWindow>(this->gameConfiguration->displayInformation, std::move(title), std::move(minecraft));
    if (!glWindow->init()) {
        return MCCLONE_ERR_OPENGL_INIT;
    }

    glWindow->thread_start();
    glWindow->thread_run();

    return MCCLONE_ERR_NONE;
}

Main::~Main() = default;

std::string Main::getVersion() const {
    return this->majorVersion + "." + this->minorVersion + "." + this->patchVersion;
}
