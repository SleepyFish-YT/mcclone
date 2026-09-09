//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_MCCLONEEXCEPTION_H
#define MCCLONE_MCCLONEEXCEPTION_H

#include <exception>
#include <string>

/**
 * @author SleepyFish
 */
class McCloneException : public std::exception {

private:

    const std::string message;

public:

    explicit McCloneException(std::string message) :
        message(std::move(message))
    {}

    const char* what() const noexcept override {
        return this->message.c_str();
    }

};

#endif //MCCLONE_MCCLONEEXCEPTION_H
