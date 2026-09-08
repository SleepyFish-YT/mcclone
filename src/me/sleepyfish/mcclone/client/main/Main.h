//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#ifndef MCCLONE_MAIN_H
#define MCCLONE_MAIN_H

#include "../../debug/Logger.h"

#include <vector>
#include <string>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#endif //_WIN32

#include <glm/ext/vector_int2.hpp>

class RenderInformation;
class GameConfiguration;

/**
 * @author SleepyFish
 * @brief This class is the main entry point of the application
 */
class Main {

private:
#ifdef _WIN32
    ::HWND consoleWindow;
#endif //_WIN32
    glm::ivec2 screenSize;

    std::string majorVersion;

    std::string minorVersion;

    std::string patchVersion;

    std::vector<std::string> arguments;

    GameConfiguration* gameConfiguration;

    RenderInformation* renderContext;

    std::thread updateThread;

    std::thread renderThread;

public:

    Main();

    static inline const std::string AUTHOR = "SleepyFish";

    /**
     * @author SleepyFish
     * @brief This function starts the main window and holds the main loop
     * @returns Exit code
     */
    int main(int arg_count, char* arg_vals[], const std::filesystem::path& exec_path);

    std::string getVersion() const;
#ifdef _WIN32
    void setConsoleWindow(::HWND wnd) noexcept {
        this->consoleWindow = wnd;
    }

    ::HWND getConsoleWindow() const noexcept {
        return this->consoleWindow;
    }
#endif //_WIN32
};


#endif //MCCLONE_MAIN_H
