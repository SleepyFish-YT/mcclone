//
// Created by SleepyFish on 17.09.2026.
// Project: mcclone
//

#include "CrashReportCategory.h"

#include "CrashReport.h"

#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")
#elif __linux__ || __APPLE__
#include <execinfo.h>
#include <cxxabi.h>
#endif //_WIN32

std::string CrashReportCategory::getCoordinateInfo(double x, double y, double z) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << x << "," << y << "," << z;
    oss << " - " << getCoordinateInfo(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z));
    return oss.str();
}

std::string CrashReportCategory::getCoordinateInfo(int x, int y, int z) {
    std::ostringstream sb;

    // world coords
    try {
        sb << "World: (" << x << "," << y << "," << z << ")";
    } catch (...) {
        sb << "(Error finding world loc)";
    }

    sb << ", ";

    // chunk coords
    try {
        int chunkX = x >> 4;
        int chunkZ = z >> 4;
        int localX = x & 15;
        int localY = y >> 4;
        int localZ = z & 15;
        int blockMinX = chunkX << 4;
        int blockMinZ = chunkZ << 4;
        int blockMaxX = ((chunkX + 1) << 4) - 1;
        int blockMaxZ = ((chunkZ + 1) << 4) - 1;

        sb << "Chunk: (at " << localX << "," << localY << "," << localZ
           << " in " << chunkX << "," << chunkZ
           << "; contains blocks " << blockMinX << ",0," << blockMinZ
           << " to " << blockMaxX << ",255," << blockMaxZ << ")";
    } catch (...) {
        sb << "(Error finding chunk loc)";
    }

    sb << ", ";

    // region coords
    try {
        int regionX   = x >> 9;
        int regionZ   = z >> 9;
        int chunkMinX = regionX << 5;
        int chunkMinZ = regionZ << 5;
        int chunkMaxX = ((regionX + 1) << 5) - 1;
        int chunkMaxZ = ((regionZ + 1) << 5) - 1;
        int blockMinX = regionX << 9;
        int blockMinZ = regionZ << 9;
        int blockMaxX = ((regionX + 1) << 9) - 1;
        int blockMaxZ = ((regionZ + 1) << 9) - 1;

        sb << "Region: (" << regionX << "," << regionZ
           << "; contains chunks " << chunkMinX << "," << chunkMinZ
           << " to " << chunkMaxX << "," << chunkMaxZ
           << ", blocks " << blockMinX << ",0," << blockMinZ
           << " to " << blockMaxX << ",255," << blockMaxZ << ")";
    } catch (...) {
        sb << "(Error finding world loc)";
    }

    return sb.str();
}

void CrashReportCategory::addCrashSectionCallable(const std::string &sectionName, std::function<std::string()> callable) {
    try {
        this->addCrashSection(sectionName, callable());
    } catch (const std::exception &e) {
        this->addCrashSectionThrowable(sectionName, e);
    }
}

void CrashReportCategory::addCrashSection(const std::string &sectionName, const std::string &value) {
    this->children.emplace_back(sectionName, value);
}

void CrashReportCategory::addCrashSectionThrowable(const std::string &sectionName, const std::exception &throwable) {
    std::string value = std::string("~~ERROR~~ ") + throwable.what();
    this->children.emplace_back(sectionName, value);
}

int CrashReportCategory::getPrunedStackTrace(int size) {
#ifdef _WIN32
    const int MAX_FRAMES = 64;
    void *frames[MAX_FRAMES];
    ::HANDLE process = ::GetCurrentProcess();
    ::SymInitialize(process, nullptr, TRUE);

    ::USHORT frameCount = ::RtlCaptureStackBackTrace(0, MAX_FRAMES, frames, nullptr);

    // skip first 3 + size frames to mirror Java's behaviour
    int skip = 3 + size;
    if (frameCount <= skip) {
        return 0;
    }

    this->stackTrace.clear();

    char symbolBuffer[sizeof(::SYMBOL_INFO) + MAX_SYM_NAME * sizeof(::TCHAR)];
    ::SYMBOL_INFO *symbol = reinterpret_cast<::SYMBOL_INFO*>(symbolBuffer);
    symbol->SizeOfStruct = sizeof(::SYMBOL_INFO);
    symbol->MaxNameLen = MAX_SYM_NAME;

    ::IMAGEHLP_LINE64 line;
    line.SizeOfStruct = sizeof(::IMAGEHLP_LINE64);

    for (::USHORT i = skip; i < frameCount; ++i) {
        ::StackTraceElement element;
        ::DWORD64 address = reinterpret_cast<::DWORD64>(frames[i]);

        if (::SymFromAddr(process, address, nullptr, symbol)) {
            element.methodName = symbol->Name;
            element.className = ""; // not available without extra parsing
        }

        ::DWORD displacement;
        if (::SymGetLineFromAddr64(process, address, &displacement, &line)) {
            element.fileName = line.FileName;
            element.lineNumber = static_cast<int>(line.LineNumber);
        }

        this->stackTrace.push_back(element);
    }
#elif __linux__ || __APPLE__
    const int MAX_FRAMES = 64;
    void *frames[MAX_FRAMES];

    int frameCount = ::backtrace(frames, MAX_FRAMES);
    char **symbols  = ::backtrace_symbols(frames, frameCount);

    // skip first 3 + size frames to mirror Java's behaviour
    int skip = 3 + size;
    if (::frameCount <= skip || symbols == nullptr) {
        ::free(symbols);
        return 0;
    }

    this->stackTrace.clear();

    for (int i = skip; i < frameCount; ++i) {
        StackTraceElement element;
        std::string raw(symbols[i]);

        // demangle the symbol name
        // raw format on Linux:  ./binary(_ZN3foo3barEv+0x1a) [0xaddr]
        // raw format on macOS:  0 binary 0xaddr _ZN3foo3barEv + 10
        std::size_t start = raw.find('(');
        std::size_t end   = raw.find('+', start);

        if (start != std::string::npos && end != std::string::npos) {
            std::string mangled = raw.substr(start + 1, end - start - 1);
            int status = 0;
            char *demangled = abi::__cxa_demangle(mangled.c_str(), nullptr, nullptr, &status);

            element.methodName = (status == 0 && demangled) ? demangled : mangled;
            ::free(demangled);
        } else {
            element.methodName = raw;
        }

        element.fileName   = raw;
        element.lineNumber = -1; // not available without addr2line or libdw

        this->stackTrace.push_back(element);
    }

    ::free(symbols);
#endif //_WIN32

    return static_cast<int>(this->stackTrace.size());
}

bool CrashReportCategory::firstTwoElementsOfStackTraceMatch(const StackTraceElement *s1, const StackTraceElement *s2) {
    if (this->stackTrace.empty() || s1 == nullptr) {
        return false;
    }

    const StackTraceElement &first = this->stackTrace[0];

    if (first.isNativeMethod() != s1->isNativeMethod()
        || first.className  != s1->className
        || first.fileName   != s1->fileName
        || first.methodName != s1->methodName) {
        return false;
    }

    // mirrors: if (s2 == null == this.stackTrace.length > 1) return false
    if ((s2 == nullptr) == (this->stackTrace.size() > 1)) {
        return false;
    }

    if (s2 != nullptr && this->stackTrace.size() > 1) {
        const StackTraceElement &second = this->stackTrace[1];
        if (second.className  != s2->className
            || second.fileName   != s2->fileName
            || second.methodName != s2->methodName) {
            return false;
        }
    }

    this->stackTrace[0] = *s1;
    return true;
}

void CrashReportCategory::trimStackTraceEntriesFromBottom(int amount) {
    if (amount >= static_cast<int>(this->stackTrace.size())) {
        this->stackTrace.clear();
        return;
    }

    this->stackTrace.erase(
            this->stackTrace.end() - amount,
            this->stackTrace.end()
    );
}

void CrashReportCategory::appendToStringBuilder(std::ostringstream &builder) const {
    builder << "-- " << this->name << " --\n";
    builder << "Details:";

    for (const auto &entry : this->children) {
        builder << "\n\t" << entry.getKey() << ": " << entry.getValue();
    }

    if (!this->stackTrace.empty()) {
        builder << "\nStacktrace:";
        for (const auto &element : this->stackTrace) {
            builder << "\n\tat " << element.toString();
        }
    }
}

std::vector<StackTraceElement> CrashReportCategory::getStackTrace() const noexcept {
    return this->stackTrace;
}