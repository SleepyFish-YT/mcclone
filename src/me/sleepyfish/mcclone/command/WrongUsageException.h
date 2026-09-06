//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_WRONGUSAGEEXCEPTION_H
#define MCCLONE_WRONGUSAGEEXCEPTION_H

#include "SyntaxErrorException.h"

/**
 * @author SleepyFish
 */
class WrongUsageException : public SyntaxErrorException {

public:

    template<typename... Args>
    explicit WrongUsageException(const std::string& message, Args&&... args) :
        SyntaxErrorException(message, std::forward<Args>(args)...)
    {}

};


#endif //MCCLONE_WRONGUSAGEEXCEPTION_H
