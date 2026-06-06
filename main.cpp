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
    std::string executablePath = argv[0];
    std::string::size_type pos = executablePath.find_last_of("\\/");
    if (pos != std::string::npos) {
        executablePath = executablePath.substr(0, pos);
    }

    if (executablePath.empty()) {
        Logger::error("Failed to get executable path");
        return -1;
    }

    // This should never happen!
    if (!std::filesystem::is_directory(executablePath)) {
        Logger::error("Executable path is not a directory");
        return -1;
    }

    Logger::init((std::filesystem::path(executablePath) / "logs"));
    if (!Logger::isInitialized()) {
        Logger::error("Failed to initialize logger");
        return -1;
    }

    // Pass console window handle to main instance
    mainInstance.setConsoleWindow(GetConsoleWindow());

    return mainInstance.main(argc, argv, executablePath);
}

