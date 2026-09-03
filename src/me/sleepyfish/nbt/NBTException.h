//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_NBTEXCEPTION_H
#define MCCLONE_NBTEXCEPTION_H

#include <stdexcept>
#include <string>

/**
 * @author SleepyFish
 * @brief Exception thrown when an error occurs while reading NBT data
 */
class NBTException : public std::exception {

private:

    const std::string message;

public:

    explicit NBTException(const std::string& message) :
        std::exception(),
        message(message)
    {}

    const char* what() const override {
        return this->message.c_str();
    }

};


#endif //MCCLONE_NBTEXCEPTION_H
