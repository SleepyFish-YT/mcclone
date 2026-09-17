//
// Created by SleepyFish on 17.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_CRASHREPORT_H
#define MCCLONE_CRASHREPORT_H

#include "../../sava/StackTraceElement.h"

#include <string>
#include <filesystem>
#include <vector>
#include <sstream>
#include <memory>

class CrashReportCategory;
class Throwable;

/**
 * @author SleepyFish
 * @brief This class is used to report Throwable's
 */
class CrashReport {

private:

    const std::string description;
    const Throwable *cause;
    std::unique_ptr<CrashReportCategory> theReportCategory;
    std::vector<CrashReportCategory*> crashReportSections;
    std::filesystem::path reportPath;
    bool firstCategoryInCrashReport;
    std::vector<StackTraceElement> stacktrace{};

public:

    CrashReport(const std::string &description, Throwable *cause);

    ~CrashReport();

    std::string getDescription() const noexcept {
        return this->description;
    }

    const Throwable *getCrashCause() const noexcept {
        return this->cause;
    }

    void getSectionsInStringBuilder(std::ostringstream &builder) const noexcept;

    std::string getCauseStackTraceOrString() const;

    std::string getCompleteReport() const;

    std::filesystem::path getReportPath() const noexcept {
        return this->reportPath;
    }

    bool saveToFile(const std::filesystem::path &path);

    CrashReportCategory* getReportCategory() const noexcept;

    CrashReportCategory* makeCategory(const std::string &name);

    CrashReportCategory* makeCategoryDepth(const std::string &name, int stacktraceLength);

    static CrashReport* makeCrashReport(Throwable *cause, const std::string &description);

private:

    void populateEnvironment();

    static std::string getWittyComment();

};


#endif //MCCLONE_CRASHREPORT_H
