//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_NUMBERINVALIDEXCEPTION_H
#define MCCLONE_NUMBERINVALIDEXCEPTION_H

#include "CommandException.h"

/**
 * @author SleepyFish
 */
class NumberInvalidException : public CommandException {

public:

    NumberInvalidException() :
        CommandException("commands.generic.num.invalid")
    {}

    template<typename... Args>
    explicit NumberInvalidException(const std::string& message, Args&&... args) :
        CommandException(message, std::forward<Args>(args)...)
    {}

    explicit NumberInvalidException(const std::string& message, const std::vector<std::string>& params) :
        CommandException(message, params)
    {}

};


#endif //MCCLONE_NUMBERINVALIDEXCEPTION_H
