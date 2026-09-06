//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_COMMANDNOTFOUNDEXCEPTION_H
#define MCCLONE_COMMANDNOTFOUNDEXCEPTION_H

#include "CommandException.h"

/**
 * @author SleepyFish
 */
class CommandNotFoundException : public CommandException {

public:

    CommandNotFoundException() :
        CommandException("commands.generic.notFound")
    {}

    template<typename... Args>
    explicit CommandNotFoundException(const std::string& message, Args&&... args) :
        CommandException(message, std::forward<Args>(args)...)
    {}

};


#endif //MCCLONE_COMMANDNOTFOUNDEXCEPTION_H
