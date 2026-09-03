//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_COMMANDEXCEPTION_H
#define MCCLONE_COMMANDEXCEPTION_H

#include <stdexcept>
#include <vector>
#include <any>
#include <string>

/**
 * @author SleepyFish
 * @brief Command exception base
 */
class CommandException : public std::exception {

private:

    const std::string message;

public:

    const std::vector<std::any> errorObjects;

    template<typename... Args>
    CommandException(const std::string& msg, Args&&... args) :
        message(msg),
        errorObjects({ std::any(std::forward<Args>(args))... })
    {}

    const char* what() const noexcept override {
        return this->message.c_str();
    }

};


#endif //MCCLONE_COMMANDEXCEPTION_H
