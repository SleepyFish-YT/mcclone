//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_SYNTAXERROREXCEPTION_H
#define MCCLONE_SYNTAXERROREXCEPTION_H

#include "CommandException.h"

/**
 * @author SleepyFish
 */
class SyntaxErrorException : public CommandException {

public:

    SyntaxErrorException() :
        CommandException("commands.generic.syntax")
    {}

    template<typename... Args>
    explicit SyntaxErrorException(const std::string& message, Args&&... args) :
        CommandException(message, std::forward<Args>(args)...)
    {}

};


#endif //MCCLONE_SYNTAXERROREXCEPTION_H
