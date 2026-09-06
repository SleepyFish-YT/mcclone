//
// Created by SleepyFish on 06.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_THREADQUICKEXITEXCEPTION_H
#define MCCLONE_THREADQUICKEXITEXCEPTION_H

#include <exception>

/**
 * @author SleepyFish
 */
class ThreadQuickExitException final : public std::exception {

public:

    static ThreadQuickExitException INSTANCE;

    const char* what() const noexcept override {
        return "ThreadQuickExitException";
    }

    // deleted to enforce singleton pattern
    ThreadQuickExitException(const ThreadQuickExitException&) = delete;
    ThreadQuickExitException& operator=(const ThreadQuickExitException&) = delete;

private:

    ThreadQuickExitException() = default;

};


inline ThreadQuickExitException ThreadQuickExitException::INSTANCE;


#endif //MCCLONE_THREADQUICKEXITEXCEPTION_H
