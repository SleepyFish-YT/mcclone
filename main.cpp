//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#ifndef MCCLONE_MAIN_CPP
#define MCCLONE_MAIN_CPP

#include "pcb.h"

#include "src/me/sleepyfish/client/main/Main.h"
#include "src/me/sleepyfish/debug/Logger.h"

#include <consoleapi3.h>
#include <filesystem>

/**
 * @author SleepyFish
 * @return Exit code
 */
int main(int argc, char* argv[]) {
    if (argc == 0 || argv[0] == nullptr) {
        return MCCLONE_ERR_ARGUMENTS;
    }

    Main mainInstance;

    // get executable path without file name
    const std::filesystem::path executable_path = std::filesystem::path(argv[0]).parent_path();

    if (executable_path.empty() || !std::filesystem::is_directory(executable_path)) {
        Logger::error("Failed to get executable path");
        return MCCLONE_ERR_EXECUTE;
    }

    Logger::init(std::filesystem::path(executable_path / "logs"));
    if (!Logger::isInitialized()) {
        Logger::error("Failed to initialize logger");
        return MCCLONE_ERR_LOGGER;
    }

    // pass console window handle to main instance
    mainInstance.setConsoleWindow(::GetConsoleWindow());
    const int exitCode = mainInstance.main(argc, argv, executable_path);

    Logger::log("Exit code: " + std::string(MCCLONE_ERR_NAME_FUNC(exitCode)));
    Logger::close();

    return exitCode;
}


#endif //MCCLONE_MAIN_CPP
