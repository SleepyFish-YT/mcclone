//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_PLAYERNOTFOUNDEXCEPTION_H
#define MCCLONE_PLAYERNOTFOUNDEXCEPTION_H

#include "CommandException.h"

/**
 * @author SleepyFish
 */
class PlayerNotFoundException : public CommandException {

public:

    PlayerNotFoundException() :
        CommandException("commands.generic.player.notFound")
    {}

    template<typename... Args>
    explicit PlayerNotFoundException(const std::string& message, Args&&... args) :
        CommandException(message, std::forward<Args>(args)...)
    {}

};


#endif //MCCLONE_PLAYERNOTFOUNDEXCEPTION_H
