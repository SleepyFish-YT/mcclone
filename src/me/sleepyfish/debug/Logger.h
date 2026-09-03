//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#ifndef MCCLONE_LOGGER_H
#define MCCLONE_LOGGER_H

#include <string>
#include <filesystem>
#include <format>
#include <fstream>

/**
 * @author SleepyFish
 * @brief This class is used to log debug information
 */
class Logger {

private:

    static inline bool initialized = false;

    static inline std::filesystem::path logFolderPath {};

    static inline std::ofstream logFile {};

    static std::string getPrefix();

    static void writeToFile(const std::string& txt);

public:

    static void init(const std::filesystem::path& logFolder);

    static void close();

    static void log(const std::string& txt, bool writeToFile = true);

    static void warn(const std::string& txt, bool writeToFile = true);

    static void error(const std::string& txt, bool writeToFile = true);

    static void fatal(const std::string& txt, bool writeToFile = true);

    /**
     * @brief This function is used to log trace information without writing to the file
     */
    static void trace(const std::string& txt, bool writeToFile = false);

    template<typename... Args>
    static void log(const std::string& fmt, Args&&... args) {
        Logger::log(std::vformat(fmt, std::make_format_args(args...)));
    }

    template<typename... Args>
    static void warn(const std::string& fmt, Args&&... args) {
        Logger::warn(std::vformat(fmt, std::make_format_args(args...)));
    }

    template<typename... Args>
    static void error(const std::string& fmt, Args&&... args) {
        Logger::error(std::vformat(fmt, std::make_format_args(args...)));
    }

    template<typename... Args>
    static void fatal(const std::string& fmt, Args&&... args) {
        Logger::fatal(std::vformat(fmt, std::make_format_args(args...)));
    }

    /**
     * @brief This function is used to log trace information without writing to the file
     */
    template<typename... Args>
    static void trace(const std::string& fmt, Args&&... args) {
        Logger::trace(std::vformat(fmt, std::make_format_args(args...)));
    }

    static bool isInitialized();

};


#endif //MCCLONE_LOGGER_H
