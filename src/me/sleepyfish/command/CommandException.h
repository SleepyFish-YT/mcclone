//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_COMMANDEXCEPTION_H
#define MCCLONE_COMMANDEXCEPTION_H

#include <stdexcept>
#include <utility>
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
    explicit CommandException(std::string  msg, Args&&... args) :
        message(std::move(msg)),
        errorObjects({ std::any(std::forward<Args>(args))... })
    {}

    explicit CommandException(std::string  message, const std::vector<std::string>& params) :
        message(std::move(message)),
        errorObjects({ std::any(params) })
    {}

    const char* what() const noexcept override {
        return this->message.c_str();
    }

};


#endif //MCCLONE_COMMANDEXCEPTION_H
