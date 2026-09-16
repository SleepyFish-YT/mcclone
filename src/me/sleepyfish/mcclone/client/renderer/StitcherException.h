//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_STITCHEREXCEPTION_H
#define MCCLONE_STITCHEREXCEPTION_H

#include <exception>

/**
 * @author SleepyFish
 * @brief Exception thrown by the stitcher
 */
class StitcherException : public std::exception {

public:

    StitcherException( const std::string &message) :
        std::exception(message.c_str())
    {}

};


#endif //MCCLONE_STITCHEREXCEPTION_H
