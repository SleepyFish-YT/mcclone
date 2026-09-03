//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_CHATALLOWEDCHARACTERS_H
#define MCCLONE_CHATALLOWEDCHARACTERS_H

#include <string>
#include <array>

/**
 * @author SleepyFish
 */
class ChatAllowedCharacters {

public:

    static constexpr std::array<char, 15> AllowedCharactersArray = {
            '/', '\n', '\r', '\t', '\0', '\f', '`', '?', '*', '\\', '<', '>', '|', '"', ':'
    };

    static bool IsAllowedCharacter(unsigned char character) {
        return character != 167 && character >= 32 && character != 127;
    }

    static std::string FilterAllowedCharacters(const std::string& input) {
        std::string result;
        result.reserve(input.size());

        for (const char c : input) {
            if (IsAllowedCharacter(static_cast<unsigned char>(c)))
                result += c;
        }

        return result;
    }

};

#endif //MCCLONE_CHATALLOWEDCHARACTERS_H
