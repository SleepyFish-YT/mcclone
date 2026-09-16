//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#include "Logger.h"

#include <iostream>
#include <ctime>

#ifdef _WIN32
#include <windows.h>
#include <debugapi.h>
#endif //_WIN32

std::string Logger::getPrefix() {
    std::time_t t = std::time(nullptr);
    std::tm *tm = std::localtime(&t);
    char buffer[16];
    std::strftime(buffer, sizeof(buffer), "[%H:%M:%S] ", tm);
    return { buffer };
}

void Logger::init(std::filesystem::path log_dir_path) {
    if (Logger::initialized) {
        Logger::error("Logger already initialized");
        return;
    }

    try {
        Logger::logFolderPath = std::move(log_dir_path);

        if (!std::filesystem::exists(Logger::logFolderPath)) {
            if (!std::filesystem::create_directory(Logger::logFolderPath)) {
                Logger::error("Failed to create log folder");
                return;
            }
        }

        std::time_t t = std::time(nullptr);
        std::tm *tm = std::localtime(&t);
        std::ostringstream ss;
        ss << (tm->tm_year + 1900) << "-"
           << std::setw(2) << std::setfill('0') << (tm->tm_mon + 1) << "-"
           << std::setw(2) << std::setfill('0') << tm->tm_mday;

        Logger::logFile.open(Logger::logFolderPath / (ss.str() + ".log"), std::ios::app);
        if (!Logger::logFile.is_open()) {
            Logger::error("Failed to open log file");
            return;
        }

        Logger::initialized = true;
        Logger::writeToFile(Logger::getPrefix() + "[main/INFO]: Logger initialized");
    } catch (const std::exception &e) {
        Logger::error("Failed to initialize logger: " + std::string(e.what()));
    }
}

void Logger::writeToFile(const std::string &txt) {
    if (!Logger::initialized || !Logger::logFile.is_open()) {
        return;
    }

    try {
        if (!std::filesystem::exists(Logger::logFolderPath)) {
            if (!std::filesystem::create_directory(Logger::logFolderPath)) {
                Logger::error("Failed to create log folder while writing to file");
                return;
            }
        }

        Logger::logFile << txt << "\n";
        Logger::logFile.flush();
        ::OutputDebugStringA((txt + "\n").c_str());
    } catch (const std::exception &e) {
        Logger::error("Failed to write to log file: " + std::string(e.what()), false);
    }
}

void Logger::close() {
    if (Logger::initialized) {
        Logger::logFile.flush();
        Logger::logFile.close();

        if (Logger::logFile.fail()) {
            Logger::error("Failed to close log file", false);
        } else {
            Logger::initialized = false;
        }
    }
}

void Logger::log(const std::string &txt, bool writeToFile) {
    std::string formatted = Logger::getPrefix() + "[main/INFO]: " + txt;
    std::cout << formatted << std::endl;

    if (writeToFile) {
        Logger::writeToFile(formatted);
    }
}

void Logger::warn(const std::string &txt, bool writeToFile) {
    std::string formatted = Logger::getPrefix() + "[main/WARN]: " + txt;
    std::cerr << formatted << std::endl;

    if (writeToFile) {
        Logger::writeToFile(formatted);
    }
}

void Logger::error(const std::string &txt, bool writeToFile) {
    std::string formatted = Logger::getPrefix() + "[main/ERROR]: " + txt;
    std::cerr << formatted << std::endl;

    if (writeToFile) {
        Logger::writeToFile(formatted);
    }
}

void Logger::fatal(const std::string &txt, bool writeToFile) {
    std::string formatted = Logger::getPrefix() + "[main/FATAL]: " + txt;
    std::cerr << formatted << std::endl;

    if (writeToFile) {
        Logger::writeToFile(formatted);
    }
}

void Logger::trace(const std::string &txt, bool writeToFile) {
    std::string formatted = Logger::getPrefix() + "[main/TRACE]: " + txt;
    std::cout << formatted << std::endl;

    if (writeToFile) {
        Logger::writeToFile(formatted);
    }
}

bool Logger::isInitialized() noexcept {
    return Logger::initialized;
}
