//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#ifndef MCCLONE_MAIN_CPP
#define MCCLONE_MAIN_CPP

#include "src/me/sleepyfish/client/main/Main.h"
#include "src/me/sleepyfish/debug/Logger.h"

#include <consoleapi3.h>
#include <filesystem>

/**
 * @author SleepyFish
 * @return Exit code
 */
int main(int argc, char* argv[]) {
    Main mainInstance;

    // get executable path without file name
    const std::filesystem::path executablePath = std::filesystem::path(argv[0]).parent_path();

    if (executablePath.empty() || !std::filesystem::is_directory(executablePath)) {
        Logger::error("Failed to get executable path");
        return -1;
    }

    Logger::init(executablePath / "logs");
    if (!Logger::isInitialized()) {
        Logger::error("Failed to initialize logger");
        return -1;
    }

    // pass console window handle to main instance
    mainInstance.setConsoleWindow(::GetConsoleWindow());
    return mainInstance.main(argc, argv, executablePath);
}


#endif //MCCLONE_MAIN_CPP
