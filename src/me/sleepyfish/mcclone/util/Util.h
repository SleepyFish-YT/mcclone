//
// Created by SleepyFish on 02.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_UTIL_H
#define MCCLONE_UTIL_H

#include "../debug/Logger.h"

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
    static V runTaskPkg(std::packaged_task<V>& task);

    template<typename V>
    static void runTaskFunc(std::packaged_task<V()>& task) {
        try {
            task();
        } catch (const std::future_error& e) {
            Logger::fatal("Error executing task", e.what());
            throw;
        } catch (const std::bad_alloc& e) {
            Logger::fatal("Out of memory", e.what());
            throw;
        } catch (const std::exception& e) {
            Logger::fatal("Error executing task", e.what());
        }
    }

};


#endif //MCCLONE_UTIL_H
