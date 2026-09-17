//
// Created by SleepyFish on 17.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_STACKTRACEELEMENT_H
#define MCCLONE_STACKTRACEELEMENT_H

#include <string>
#include <sstream>

/**
 * @author SleepyFish - SleepyAVA
 * @version 1.0
 * @brief Stack trace element
 */
struct StackTraceElement {

    std::string classLoaderName{};
    std::string moduleName{};
    std::string moduleVersion{};
    std::string className{};
    std::string methodName{};
    std::string fileName{};
    int lineNumber{};
    bool nativeMethod{};

    static constexpr const char* UNKNOWN_SOURCE = "Unknown Source";
    static constexpr const char* NATIVE_METHOD = "Native Method";

    bool isNativeMethod() const noexcept {
        return this->nativeMethod;
    }

    std::string toString() const {
        std::ostringstream sb;

        if (!classLoaderName.empty()) {
            sb << classLoaderName << '/';
        }

        if (!moduleName.empty()) {
            sb << moduleName;
            if (!moduleVersion.empty()) {
                sb << '@' << moduleVersion;
            }
        }

        if (sb.tellp() > 0) {
            sb << '/';
        }

        sb << className << '.' << methodName << '(';

        if (isNativeMethod()) {
            sb << NATIVE_METHOD;
        } else if (fileName.empty()) {
            sb << UNKNOWN_SOURCE;
        } else {
            sb << fileName;
            if (lineNumber >= 0) {
                sb << ':' << lineNumber;
            }
        }

        sb << ')';
        return sb.str();
    }

};


#endif //MCCLONE_STACKTRACEELEMENT_H
