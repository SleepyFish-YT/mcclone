//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_STRINGUTILS_H
#define MCCLONE_STRINGUTILS_H

#include <string>
#include <regex>

/**
 * @author SleepyFish
 * @brief std::string utility class
 */
class StringUtils {

public:

    static inline const std::regex PATTERN_CONTROL_CODE = std::regex("§[0-9A-FK-OR]", std::regex::icase);

    static std::string TicksToElapsedTime(int ticks) {
        int i = ticks / 20;
        const int j = i / 60;
        i = i % 60;
        return std::to_string(j) + (i < 10 ? ":0" : ":") + std::to_string(i);
    }

    static std::string StripControlCodes(const std::string& str) {
        return std::regex_replace(str, StringUtils::PATTERN_CONTROL_CODE, "");
    }

    static bool IsNullOrEmpty(const std::string& str) noexcept {
        return str.empty();
    }

};


#endif //MCCLONE_STRINGUTILS_H
