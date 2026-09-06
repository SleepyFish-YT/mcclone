//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ENTITYNOTFOUNDEXCEPTION_H
#define MCCLONE_ENTITYNOTFOUNDEXCEPTION_H

#include "CommandException.h"

/**
 * @author SleepyFish
 */
class EntityNotFoundException : public CommandException {

public:

    EntityNotFoundException() :
        CommandException("commands.generic.entity.notFound")
    {}

    template<typename... Args>
    explicit EntityNotFoundException(const std::string& message, Args&&... args) :
        CommandException(message, std::forward<Args>(args)...)
    {}

};


#endif //MCCLONE_ENTITYNOTFOUNDEXCEPTION_H
