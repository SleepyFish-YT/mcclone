#include <windows.h>

#include "src/me/sleepyfish/client/main/Main.h"
#include "src/me/sleepyfish/debug/Logger.h"

/**
 * @author SleepyFish
 * @return Exit code
 */
int main(int argc, char* argv[]) {
    Main mainInstance;

    // Get executable path without file name
    const std::filesystem::path executablePath = std::filesystem::path(argv[0]).parent_path();

    if (executablePath.empty() || !std::filesystem::is_directory(executablePath)) {
        Logger::error("Failed to get executable path");
        return -1;
    }

    // This should never happen!
    if (!std::filesystem::is_directory(executablePath)) {
        Logger::error("Executable path is not a directory");
        return -1;
    }

    Logger::init(executablePath / "logs");
    if (!Logger::isInitialized()) {
        Logger::error("Failed to initialize logger");
        return -1;
    }

    // Pass console window handle to main instance even tho since 1.0.0 it is not used.
    mainInstance.setConsoleWindow(::GetConsoleWindow());
    return mainInstance.main(argc, argv, executablePath);
}

