//
// Created by SleepyFish on 07.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ENUMCHATFORMATTING_H
#define MCCLONE_ENUMCHATFORMATTING_H

#include <string>
#include <vector>
#include <unordered_map>
#include <regex>
#include <algorithm>
#include <optional>

/**
 * @author SleepyFish
 * @brief Chat formatting enum
 */
enum class EnumChatFormatting {

    BLACK = 0,
    DARK_BLUE,
    DARK_GREEN,
    DARK_AQUA,
    DARK_RED,
    DARK_PURPLE,
    GOLD,
    GRAY,
    DARK_GRAY,
    BLUE,
    GREEN,
    AQUA,
    RED,
    LIGHT_PURPLE,
    YELLOW,
    WHITE,
    OBFUSCATED,
    BOLD,
    STRIKETHROUGH,
    UNDERLINE,
    ITALIC,
    RESET

};


inline const std::string CHAT_FORMATTING_SECTION = "\u00a7";

/**
 * @author SleepyFish
 */
struct ChatFormattingInfo {

    std::string name;

    char formattingCode;

    bool fancyStyling;

    int colorIndex;

    std::string controlString;

};

inline const ChatFormattingInfo& getChatFormattingInfo(EnumChatFormatting fmt) {
    static const ChatFormattingInfo infos[] = {
            { "BLACK",         '0', false, 0,  CHAT_FORMATTING_SECTION + '0' },
            { "DARK_BLUE",     '1', false, 1,  CHAT_FORMATTING_SECTION + '1' },
            { "DARK_GREEN",    '2', false, 2,  CHAT_FORMATTING_SECTION + '2' },
            { "DARK_AQUA",     '3', false, 3,  CHAT_FORMATTING_SECTION + '3' },
            { "DARK_RED",      '4', false, 4,  CHAT_FORMATTING_SECTION + '4' },
            { "DARK_PURPLE",   '5', false, 5,  CHAT_FORMATTING_SECTION + '5' },
            { "GOLD",          '6', false, 6,  CHAT_FORMATTING_SECTION + '6' },
            { "GRAY",          '7', false, 7,  CHAT_FORMATTING_SECTION + '7' },
            { "DARK_GRAY",     '8', false, 8,  CHAT_FORMATTING_SECTION + '8' },
            { "BLUE",          '9', false, 9,  CHAT_FORMATTING_SECTION + '9' },
            { "GREEN",         'a', false, 10, CHAT_FORMATTING_SECTION + 'a' },
            { "AQUA",          'b', false, 11, CHAT_FORMATTING_SECTION + 'b' },
            { "RED",           'c', false, 12, CHAT_FORMATTING_SECTION + 'c' },
            { "LIGHT_PURPLE",  'd', false, 13, CHAT_FORMATTING_SECTION + 'd' },
            { "YELLOW",        'e', false, 14, CHAT_FORMATTING_SECTION + 'e' },
            { "WHITE",         'f', false, 15, CHAT_FORMATTING_SECTION + 'f' },
            { "OBFUSCATED",    'k', true,  -1, CHAT_FORMATTING_SECTION + 'k' },
            { "BOLD",          'l', true,  -1, CHAT_FORMATTING_SECTION + 'l' },
            { "STRIKETHROUGH", 'm', true,  -1, CHAT_FORMATTING_SECTION + 'm' },
            { "UNDERLINE",     'n', true,  -1, CHAT_FORMATTING_SECTION + 'n' },
            { "ITALIC",        'o', true,  -1, CHAT_FORMATTING_SECTION + 'o' },
            { "RESET",         'r', false, -1, CHAT_FORMATTING_SECTION + 'r' },
    };

    return infos[static_cast<int>(fmt)];
}

inline bool isFancyStyling(EnumChatFormatting fmt) {
    return getChatFormattingInfo(fmt).fancyStyling;
}

inline bool isColor(EnumChatFormatting fmt) {
    return !isFancyStyling(fmt) && fmt != EnumChatFormatting::RESET;
}

inline int getColorIndex(EnumChatFormatting fmt) {
    return getChatFormattingInfo(fmt).colorIndex;
}

inline std::string getFriendlyName(EnumChatFormatting fmt) {
    std::string name = getChatFormattingInfo(fmt).name;
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    return name;
}

inline std::string toString(EnumChatFormatting fmt) {
    return getChatFormattingInfo(fmt).controlString;
}

inline std::string getTextWithoutFormattingCodes(const std::string& text) {
    // might need to change back to 'pattern("(?i)\u00a7[0-9A-FK-OR]", std::regex::icase)'
    static const std::regex pattern("(?i)" + CHAT_FORMATTING_SECTION + "[0-9A-FK-OR]", std::regex::icase);
    return std::regex_replace(text, pattern, "");
}

inline std::optional<EnumChatFormatting> getValueByName(const std::string& friendlyName) {
    std::string key;
    for (char c : friendlyName) {
        if (std::isalpha(c)) {
            key += std::tolower(c);
        }
    }

    static const EnumChatFormatting all[] = {
            EnumChatFormatting::BLACK, EnumChatFormatting::DARK_BLUE, EnumChatFormatting::DARK_GREEN,
            EnumChatFormatting::DARK_AQUA, EnumChatFormatting::DARK_RED, EnumChatFormatting::DARK_PURPLE,
            EnumChatFormatting::GOLD, EnumChatFormatting::GRAY, EnumChatFormatting::DARK_GRAY,
            EnumChatFormatting::BLUE, EnumChatFormatting::GREEN, EnumChatFormatting::AQUA,
            EnumChatFormatting::RED, EnumChatFormatting::LIGHT_PURPLE, EnumChatFormatting::YELLOW,
            EnumChatFormatting::WHITE, EnumChatFormatting::OBFUSCATED, EnumChatFormatting::BOLD,
            EnumChatFormatting::STRIKETHROUGH, EnumChatFormatting::UNDERLINE, EnumChatFormatting::ITALIC,
            EnumChatFormatting::RESET
    };

    for (auto fmt : all) {
        std::string candidate = getFriendlyName(fmt);
        candidate.erase(std::remove_if(candidate.begin(), candidate.end(), [](char c) {
            return !std::isalpha(c);
        }), candidate.end());

        if (candidate == key) {
            return fmt;
        }
    }

    return std::nullopt;
}

inline std::optional<EnumChatFormatting> getByColorIndex(int index) {
    if (index < 0) {
        return EnumChatFormatting::RESET;
    }

    static const EnumChatFormatting colors[] = {
            EnumChatFormatting::BLACK, EnumChatFormatting::DARK_BLUE, EnumChatFormatting::DARK_GREEN,
            EnumChatFormatting::DARK_AQUA, EnumChatFormatting::DARK_RED, EnumChatFormatting::DARK_PURPLE,
            EnumChatFormatting::GOLD, EnumChatFormatting::GRAY, EnumChatFormatting::DARK_GRAY,
            EnumChatFormatting::BLUE, EnumChatFormatting::GREEN, EnumChatFormatting::AQUA,
            EnumChatFormatting::RED, EnumChatFormatting::LIGHT_PURPLE, EnumChatFormatting::YELLOW,
            EnumChatFormatting::WHITE
    };

    for (auto fmt : colors) {
        if (getChatFormattingInfo(fmt).colorIndex == index) {
            return fmt;
        }
    }

    return std::nullopt;
}

inline std::vector<std::string> getValidValues(bool includeColors, bool includeFancy) {
    std::vector<std::string> result;

    static const EnumChatFormatting all[] = {
            EnumChatFormatting::BLACK, EnumChatFormatting::DARK_BLUE, EnumChatFormatting::DARK_GREEN,
            EnumChatFormatting::DARK_AQUA, EnumChatFormatting::DARK_RED, EnumChatFormatting::DARK_PURPLE,
            EnumChatFormatting::GOLD, EnumChatFormatting::GRAY, EnumChatFormatting::DARK_GRAY,
            EnumChatFormatting::BLUE, EnumChatFormatting::GREEN, EnumChatFormatting::AQUA,
            EnumChatFormatting::RED, EnumChatFormatting::LIGHT_PURPLE, EnumChatFormatting::YELLOW,
            EnumChatFormatting::WHITE, EnumChatFormatting::OBFUSCATED, EnumChatFormatting::BOLD,
            EnumChatFormatting::STRIKETHROUGH, EnumChatFormatting::UNDERLINE, EnumChatFormatting::ITALIC,
            EnumChatFormatting::RESET
    };

    for (auto fmt : all) {
        if ((!isColor(fmt) || includeColors) && (!isFancyStyling(fmt) || includeFancy)) {
            result.push_back(getFriendlyName(fmt));
        }
    }

    return result;
}


#endif //MCCLONE_ENUMCHATFORMATTING_H
