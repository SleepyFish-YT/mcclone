//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ANVILCONVERTEREXCEPTION_H
#define MCCLONE_ANVILCONVERTEREXCEPTION_H

#include <stdexcept>
#include <string>

/**
 * @author SleepyFish
 */
class AnvilConverterException : public std::exception {

private:

    const std::string message;

public:

    explicit AnvilConverterException(const std::string& exceptionMessage) :
        message(exceptionMessage)
    {}

    const char* what() const noexcept override {
        return this->message.c_str();
    }

};


#endif //MCCLONE_ANVILCONVERTEREXCEPTION_H
