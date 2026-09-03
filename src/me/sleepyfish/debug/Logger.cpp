//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#include "Logger.h"

#include <iostream>
#include <ctime>
#ifdef _WIN32
#include <windows.h>
#endif //_WIN32
#include <debugapi.h>

std::string Logger::getPrefix() {
    std::time_t t = std::time(nullptr);
    std::tm* tm = std::localtime(&t);
    char buffer[16];
    std::strftime(buffer, sizeof(buffer), "[%H:%M:%S] ", tm);
    return { buffer };
}

void Logger::writeToFile(const std::string& txt) {
    if (!Logger::initialized) {
        // fail silently
        return;
    }

    try {
        if (!std::filesystem::exists(Logger::logFolderPath)) {
            if (!std::filesystem::create_directory(Logger::logFolderPath)) {
                Logger::error("Failed to create log folder while writing to file");
                return;
            }
        }

        std::time_t t = std::time(nullptr);
        std::tm *tm = std::localtime(&t);

        std::ostringstream ss;
        ss << (tm->tm_year + 1900) << "-"
           << std::setw(2) << std::setfill('0') << (tm->tm_mon + 1) << "-"
           << std::setw(2) << std::setfill('0') << tm->tm_mday;

        std::string fileName = ss.str() + ".log";
        Logger::logFile = std::ofstream(Logger::logFolderPath / fileName, std::ios::app);
        Logger::logFile << txt << std::endl;
    } catch (const std::exception& e) {
        // if this fails to write to the file it is very likely to fail 100 more times after.
        // so to prevent a crash caused by stackTraceOverflow, we do not write the error to the file (this method)
        Logger::error("Failed to write to log file: " + std::string(e.what()), false);
    }
}

void Logger::init(const std::filesystem::path& logFolder) {
    if (Logger::initialized) {
        Logger::error("Logger already initialized");
        return;
    }

    try {
        Logger::logFolderPath = logFolder;

        if (!std::filesystem::exists(Logger::logFolderPath)) {
            if (!std::filesystem::create_directory(Logger::logFolderPath)) {
                Logger::error("Failed to create log folder");
                return;
            }

            Logger::log("Log folder created");
        }
    } catch (const std::exception& e) {
        Logger::error("Failed to initialize logger: " + std::string(e.what()));
        return;
    }

    Logger::writeToFile(Logger::getPrefix() + "[main/INFO]: Logger initialized");
    Logger::initialized = true;
}

void Logger::close() {
    if (Logger::initialized) {
        Logger::logFile.close();

        if (Logger::logFile.fail()) {
            Logger::error("Failed to close log file");
        } else {
            Logger::initialized = false;
        }
    }
}

void Logger::log(const std::string& txt, bool writeToFile) {
    std::string formatted = Logger::getPrefix() + "[main/INFO]: " + txt;
    std::cout << formatted << std::endl;
    ::OutputDebugStringA((formatted + "\n").c_str());

    if (writeToFile) {
        Logger::writeToFile(formatted);
    }
}

void Logger::warn(const std::string& txt, bool writeToFile) {
    std::string formatted = Logger::getPrefix() + "[main/WARN]: " + txt;
    std::cerr << formatted << std::endl;
    ::OutputDebugStringA((formatted + "\n").c_str());

    if (writeToFile) {
        Logger::writeToFile(formatted);
    }
}

void Logger::error(const std::string& txt, bool writeToFile) {
    std::string formatted = Logger::getPrefix() + "[main/ERROR]: " + txt;
    std::cerr << formatted << std::endl;
    ::OutputDebugStringA((formatted + "\n").c_str());

    if (writeToFile) {
        Logger::writeToFile(formatted);
    }
}

void Logger::fatal(const std::string& txt, bool writeToFile) {
    std::string formatted = Logger::getPrefix() + "[main/FATAL]: " + txt;
    std::cerr << formatted << std::endl;
    ::OutputDebugStringA((formatted + "\n").c_str());

    if (writeToFile) {
        Logger::writeToFile(formatted);
    }
}

void Logger::trace(const std::string& txt, bool writeToFile) {
    std::string formatted = Logger::getPrefix() + "[main/TRACE]: " + txt;
    std::cout << formatted << std::endl;
    ::OutputDebugStringA((formatted + "\n").c_str());

    if (writeToFile) {
        Logger::writeToFile(formatted);
    }
}

bool Logger::isInitialized() {
    return Logger::initialized;
}
