//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_CHATCOMPONENTTRANSLATIONFORMATEXCEPTION_H
#define MCCLONE_CHATCOMPONENTTRANSLATIONFORMATEXCEPTION_H

#include <stdexcept>
#include <string>
#include <format>

class ChatComponentTranslation;

/**
 * @auhor SleepyFish
 */
class ChatComponentTranslationFormatException : public std::invalid_argument {

public:

    ChatComponentTranslationFormatException(const ChatComponentTranslation& component, const std::string& message) :
        //std::invalid_argument(std::format("Error parsing: {}: {}", component.toString(), message))
        std::invalid_argument(std::format("Error parsing: {}: {}", "component.toString()", message))
    {}

    ChatComponentTranslationFormatException(const ChatComponentTranslation& component, int index) :
        //std::invalid_argument(std::format("Invalid index {} requested for {}", index, component.toString()))
        std::invalid_argument(std::format("Invalid index {} requested for {}", index, "component.toString()"))
    {}

    ChatComponentTranslationFormatException(const ChatComponentTranslation& component, const std::exception& cause) :
        //std::invalid_argument(std::format("Error while parsing: {} (caused by: {})", component.toString(), cause.what()))
        std::invalid_argument(std::format("Error while parsing: {} (caused by: {})", "component.toString()", cause.what()))
    {}


};

#endif //MCCLONE_CHATCOMPONENTTRANSLATIONFORMATEXCEPTION_H
