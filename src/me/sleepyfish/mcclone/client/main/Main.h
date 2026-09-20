//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#ifndef MCCLONE_MAIN_H
#define MCCLONE_MAIN_H

#include <vector>
#include <string>
#include <thread>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#endif //_WIN32

#include <glm/ext/vector_int2.hpp>

class GameConfiguration;

/**
 * @author SleepyFish
 * @brief This class is the main entry point of the application
 */
class Main {

private:

    ::glm::ivec2 screenSize;

    std::string majorVersion;

    std::string minorVersion;

    std::string patchVersion;

    std::vector<std::string> arguments;

    GameConfiguration *gameConfiguration;

public:

    Main();

    ~Main();

    static inline const std::string AUTHOR = MCCLONE_AUTHOR;

    /**
     * @author SleepyFish
     * @brief This function starts the main window and holds the main loop
     * @args Accepted argument list:
     * @args --debug
     * @args --demo
     * @args --ignoreGlErrors
     * @args --resolution [width] [height]
     * @args --resolution fullscreen
     * @returns Exit code
     */
    int main(int arg_count, char *arg_vals[], const std::filesystem::path &exec_path);

    std::string getVersion() const;

#ifdef _WIN32
    using NativeWindow = HWND;
#elif __linux__ || __APPLE__
    using NativeWindow = int; // file descriptor
#endif //_WIN32

private:

    NativeWindow consoleWindow;

public:

    void setConsoleWindow(NativeWindow wnd) noexcept { consoleWindow = wnd; }
    NativeWindow getConsoleWindow() const noexcept { return consoleWindow; }

};


#endif //MCCLONE_MAIN_H
