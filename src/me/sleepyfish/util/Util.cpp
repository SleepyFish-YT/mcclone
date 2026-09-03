//
// Created by SleepyFish on 02.09.2026.
// Project: mcclone
//

#include "Util.h"

#include "../debug/Logger.h"

#ifdef _WIN32
    #include <windows.h>
#elif __APPLE__
    #include <TargetConditionals.h>
    #include <sys/utsname.h>
#elif __linux__
    #include <sys/utsname.h>
#endif

#include <memory>
#include <stdexcept>

Util::EnumOS Util::getOSType() {
    std::string os = []() {
#ifdef _WIN32
        return std::string("windows");
#elif __APPLE__
        return std::string("mac");
#elif __linux__
        return std::string("linux");
#elif __sun
        return std::string("solaris");
#else
        return std::string("unknown");
#endif
    }();

    if (os == "unknown") {
#ifdef _WIN32
        os = "windows";
#else
        struct utsname unameData;
        if (::uname(&unameData) == 0) {
            std::string sysname = unameData.sysname;
            if (sysname == "Linux") os = "linux";
            else if (sysname == "Darwin") os = "mac";
            else if (sysname == "SunOS") os = "solaris";
            else os = "unknown";
        }
#endif
    }

    if (os == "windows") return EnumOS::WINDOWS;
    if (os == "mac" || os == "darwin") return EnumOS::OSX;
    if (os == "solaris" || os == "sunos") return EnumOS::SOLARIS;
    if (os == "linux" || os == "unix") return EnumOS::LINUX;
    return EnumOS::UNKNOWN;
}

template<typename V>
V Util::runTask(std::shared_future<V> &task) {
    try {
        // wait for the task to be ready
        task.wait();
        return task.get();
    } catch (const std::future_error& e) {
        Logger::fatal("Error executing task", e.what());
        throw;
    } catch (const std::bad_alloc& e) {
        Logger::fatal("Out of memory", e.what());
        throw;
    } catch (const std::exception& e) {
        Logger::fatal("Error executing task", e.what());
        return V{};
    }
}

template<typename V>
V Util::runTask(std::future<V>& task) {
    try {
        // wait for the task to be ready
        task.wait();
        return task.get();
    } catch (const std::future_error& e) {
        Logger::fatal("Error executing task", e.what());
        throw;
    } catch (const std::bad_alloc& e) {
        Logger::fatal("Out of memory", e.what());
        throw;
    } catch (const std::exception& e) {
        Logger::fatal("Error executing task", e.what());
        return V{};
    }
}
