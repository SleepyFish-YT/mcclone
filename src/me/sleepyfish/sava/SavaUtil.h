//
// Created by SleepyFish on 07.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_SAVAUTIL_H
#define MCCLONE_SAVAUTIL_H

#include <string>
#include <algorithm>
#include <cctype> // std::tolower

/**
 * @author SleepyFish - SleepyAVA
 * @version 1.0
 * @brief Util main class
 */
class SavaUtil {

public:

    /**
     * @author SleepyFish - SleepyAVA
     * @version 1.1
     * @brief String util class
     */
    class StringUtil {

    public:

        static std::string ToLowerCase(const std::string& str) {
            std::string result;
            result.reserve(str.size());

            for (char c : str) {
                result.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
            }

            return result;
        }

        static std::string ToUpperCase(const std::string& str) {
            std::string result;
            result.reserve(str.size());

            for (char c : str) {
                result.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(c))));
            }

            return result;
        }

        static bool StartsWith(const std::string& str, const std::string& prefix, bool ignoreCase = false) {
            if (prefix.size() > str.size()) {
                return false;
            }

            if (ignoreCase) {
                return std::equal(prefix.begin(), prefix.end(), str.begin(), [](char a, char b) {
                    return std::tolower(static_cast<unsigned char>(a)) == std::tolower(static_cast<unsigned char>(b));
                });
            }

            return str.starts_with(prefix);
        }

        static std::string GetRandomPlayerName();

    };

};


#endif //MCCLONE_SAVAUTIL_H
