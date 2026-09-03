//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#ifndef MCCLONE_MAIN_H
#define MCCLONE_MAIN_H

#include "../../render/RenderInformation.h"
#include "../../debug/Logger.h"
#include "GameConfiguration.h"

#include <windows.h>
#include <vector>
#include <string>
#include <thread>
#include <glm/ext/vector_int2.hpp>

/**
 * @author SleepyFish
 * @brief This class is the main entry point of the application
 */
class Main {

private:

    ::HWND consoleWindow;

    glm::ivec2 screenSize;

    std::string majorVersion;

    std::string minorVersion;

    std::string patchVersion;

    std::vector<std::string> arguments;

    GameConfiguration gameConfiguration;

    RenderInformation renderContext;

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
    int main(int argc, char* argv[], std::filesystem::path executablePath);

    std::string getVersion() const;

    void setConsoleWindow(::HWND wnd) {
        this->consoleWindow = wnd;
    }

    ::HWND getConsoleWindow() {
        return this->consoleWindow;
    }

};


#endif //MCCLONE_MAIN_H
