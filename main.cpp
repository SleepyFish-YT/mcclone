//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#ifndef MCCLONE_MAIN_CPP
#define MCCLONE_MAIN_CPP

#include "pcb.h"

#include "src/me/sleepyfish/mcclone/client/main/Main.h"
#include "src/me/sleepyfish/mcclone/debug/Logger.h"

#ifdef _WIN32
#include <consoleapi3.h>
#endif //_WIN32

#include <filesystem>

/**
 * @author SleepyFish
 * @version 1.6
 * @brief This is the main entry point of the application
 * @returns Exit code
 */
int main(int arg_count, char* arg_vals[]) {
    if (arg_count == 0 || arg_vals[0] == nullptr) {
        return MCCLONE_ERR_ARGUMENTS;
    }

    Main main_instance{};

    // get executable path without file name
    const std::filesystem::path executable_path = std::filesystem::path(arg_vals[0]).parent_path();

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
#ifdef _WIN32
    main_instance.setConsoleWindow(::GetConsoleWindow());
#endif //_WIN32

    const int exit_code = main_instance.main(arg_count, arg_vals, executable_path);

    Logger::log("Exit code: {}", MCCLONE_ERR_NAME_FUNC(exit_code));
    Logger::close();

    return exit_code;
}


#endif //MCCLONE_MAIN_CPP
