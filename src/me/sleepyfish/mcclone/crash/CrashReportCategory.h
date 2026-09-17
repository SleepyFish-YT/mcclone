//
// Created by SleepyFish on 17.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_CRASHREPORTCATEGORY_H
#define MCCLONE_CRASHREPORTCATEGORY_H

#include "../../sava/StackTraceElement.h"

#include <string>
#include <vector>
#include <functional>
#include <sstream>

class CrashReport;

/**
 * @author SleepyFish
 * @brief Crash report category
 */
class CrashReportCategory {

private:

    struct Entry {
        std::string key;
        std::string value;

        Entry(const std::string &key, const std::string &value) : key(key), value(value) {}

        const std::string &getKey() const noexcept { return this->key; }

        const std::string &getValue() const noexcept { return this->value; }
    };

    CrashReport *crashReport;
    std::string name;
    std::vector<Entry> children;
    std::vector<StackTraceElement> stackTrace{};

public:

    CrashReportCategory() = default;

    CrashReportCategory(CrashReport *report, const std::string &name) :
        crashReport(report),
        name(name)
    {}

    ~CrashReportCategory() = default;

    static std::string getCoordinateInfo(double x, double y, double z);

    static std::string getCoordinateInfo(int x, int y, int z);

    void addCrashSectionCallable(const std::string &sectionName, std::function<std::string()> callable);

    void addCrashSection(const std::string &sectionName, const std::string &value);

    void addCrashSectionThrowable(const std::string &sectionName, const std::exception &throwable);

    int getPrunedStackTrace(int size);

    bool firstTwoElementsOfStackTraceMatch(const StackTraceElement *s1, const StackTraceElement *s2);

    void trimStackTraceEntriesFromBottom(int amount);

    void appendToStringBuilder(std::ostringstream &builder) const;

    std::vector<StackTraceElement> getStackTrace() const noexcept;

};


#endif //MCCLONE_CRASHREPORTCATEGORY_H
