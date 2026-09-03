//
// Created by SleepyFish on 02.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_UTIL_H
#define MCCLONE_UTIL_H

#include <string>
#include <future>

/**
 * @author SleepyFish
 * @brief Utility class
 */
class Util {

public:

    enum class EnumOS {
        LINUX = 0,
        SOLARIS,
        WINDOWS,
        OSX,
        UNKNOWN
    };

    static EnumOS getOSType();

    template<typename V>
    static V runTask(std::future<V>& task);

    template<typename V>
    static V runTask(std::shared_future<V>& task);

};


#endif //MCCLONE_UTIL_H
