//
// Created by SleepyFish on 17.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_THROWABLE_H
#define MCCLONE_THROWABLE_H

#include "StackTraceElement.h"

#include <string>
#include <vector>

/**
 * @author SleepyFish - SleepyAVA
 * @version 1.0
 * @brief Throwable interface
 */
struct Throwable {
    std::string message{};
    std::string type{};
    Throwable *cause{};
    std::vector<StackTraceElement> stacktrace{};
};


#endif //MCCLONE_THROWABLE_H
