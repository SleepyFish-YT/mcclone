//
// Created by SleepyFish on 17.09.2026.
// Project: mcclone
//

#include "CrashReport.h"

#include "CrashReportCategory.h"
#include "../../sava/Throwable.h"

#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <stdexcept>

CrashReport::CrashReport(const std::string &description, Throwable *cause) :
    description(description),
    cause(cause),
    firstCategoryInCrashReport(true)
{
    this->theReportCategory = std::make_unique<CrashReportCategory>(this, "System Details");
    this->populateEnvironment();
}

CrashReport::~CrashReport() {
    for (auto *section: this->crashReportSections) {
        delete section;
    }
}

void CrashReport::populateEnvironment() {
#ifdef MCCLONE_VERSION
    std::string maj = std::to_string(MCCLONE_VERSION_MAJOR);
    std::string min = std::to_string(MCCLONE_VERSION_MINOR);
    std::string pat = std::to_string(MCCLONE_VERSION_PATCH);
#else
    std::string maj = "-0";
    std::string min = "-0";
    std::string pat = "-0";
#endif //MCCLONE_VERSION

    this->theReportCategory->addCrashSectionCallable("McClone Version", [maj, min, pat]() -> std::string {
        return maj + "." + min + "." + pat;
    });

#ifdef _WIN32
    std::string osName = "Windows";
#elif __APPLE__
    std::string osName = "macOS";
#elif __linux__
    std::string osName = "Linux";
#else
    std::string osName = "Unknown";
#endif //_WIN32

    this->theReportCategory->addCrashSectionCallable("Operating System", [osName]() -> std::string {
        return osName;
    });

    this->theReportCategory->addCrashSectionCallable("C++ Standard", []() -> std::string {
        return std::to_string(__cplusplus);
    });
}

void CrashReport::getSectionsInStringBuilder(std::ostringstream &builder) const noexcept {
    // if stacktrace is empty, but we have sections, grab first element of first section
    if (this->stacktrace.empty() && !this->crashReportSections.empty()) {
        auto firstTrace = this->crashReportSections[0]->getStackTrace();
        if (!firstTrace.empty()) {
            const_cast<CrashReport *>(this)->stacktrace = {firstTrace[0]};
        }
    }

    if (!this->stacktrace.empty()) {
        builder << "-- Head --\n";
        builder << "Stacktrace:\n";

        for (const auto &element: this->stacktrace) {
            builder << "\tat " << element.toString() << "\n";
        }

        builder << "\n";
    }

    for (const auto *section: this->crashReportSections) {
        section->appendToStringBuilder(builder);
        builder << "\n\n";
    }

    this->theReportCategory->appendToStringBuilder(builder);
}

std::string CrashReport::getCauseStackTraceOrString() const {
    if (this->cause == nullptr) {
        return "No cause available";
    }

    std::ostringstream oss;
    oss << this->cause->type << ": " << this->cause->message << "\n";

    for (const auto &frame: this->cause->stacktrace) {
        oss << "\tat " << frame.toString() << "\n";
    }

    return oss.str();
}

std::string CrashReport::getCompleteReport() const {
    std::ostringstream sb;

    sb << "---- McClone Crash Report ----\n";
    sb << "// " << getWittyComment() << "\n\n";

    // timestamp
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    sb << "Time: " << std::ctime(&time);

    sb << "Description: " << this->description << "\n\n";
    sb << this->getCauseStackTraceOrString();
    sb << "\n\nA detailed walkthrough of the error, its code path and all known details is as follows:\n";

    for (int i = 0; i < 87; ++i) {
        sb << "-";
    }

    sb << "\n\n";
    this->getSectionsInStringBuilder(sb);

    return sb.str();
}

bool CrashReport::saveToFile(const std::filesystem::path &path) {
    if (!this->reportPath.empty()) {
        return false;
    }

    std::filesystem::create_directories(path.parent_path());

    try {
        std::ofstream file(path);
        if (!file.is_open()) {
            return false;
        }

        file << this->getCompleteReport();
        file.close();
        this->reportPath = path;
        return true;
    } catch (const std::exception &e) {
        std::cerr << "Could not save crash report to " << path << ": " << e.what() << "\n";
        return false;
    }
}

CrashReportCategory *CrashReport::getReportCategory() const noexcept {
    return this->theReportCategory.get();
}

CrashReportCategory *CrashReport::makeCategory(const std::string &name) {
    return this->makeCategoryDepth(name, 1);
}

CrashReportCategory *CrashReport::makeCategoryDepth(const std::string &name, int stacktraceLength) {
    auto *category = new CrashReportCategory(this, name);

    if (this->firstCategoryInCrashReport) {
        int i = category->getPrunedStackTrace(stacktraceLength);
        const auto &causeTrace = this->cause->stacktrace;
        int j = static_cast<int>(causeTrace.size()) - i;

        if (j < 0) {
            std::cout << "Negative index in crash report handler (" << causeTrace.size() << "/" << i << ")\n";
        }

        const StackTraceElement *elem0 = nullptr;
        const StackTraceElement *elem1 = nullptr;

        if (!causeTrace.empty() && 0 <= j && j < static_cast<int>(causeTrace.size())) {
            elem0 = &causeTrace[j];

            int idx1 = static_cast<int>(causeTrace.size()) + 1 - i;
            if (idx1 < static_cast<int>(causeTrace.size())) {
                elem1 = &causeTrace[idx1];
            }
        }

        this->firstCategoryInCrashReport = category->firstTwoElementsOfStackTraceMatch(elem0, elem1);

        if (i > 0 && !this->crashReportSections.empty()) {
            this->crashReportSections.back()->trimStackTraceEntriesFromBottom(i);
        } else if (!causeTrace.empty() && static_cast<int>(causeTrace.size()) >= i && 0 <= j &&
                   j < static_cast<int>(causeTrace.size())) {
            this->stacktrace = std::vector<StackTraceElement>(causeTrace.begin(), causeTrace.begin() + j);
        } else {
            this->firstCategoryInCrashReport = false;
        }
    }

    this->crashReportSections.push_back(category);
    return category;
}

std::string CrashReport::getWittyComment() {
    static const std::vector<std::string> comments = {
        "Who set us up the TNT?",
        "Everything's going to plan. No, really, that was supposed to happen.",
        "Uh... Did I do that?", "Oops.", "Why did you do that?",
        "I feel sad now :(", "My bad.", "I'm sorry, Sleepy.",
        "I let you down. Sorry :(",
        "On the bright side, I bought you a primogem!",
        "Daisy, daisy...", "Oh - I know what I did wrong!",
        "Hey, that tickles! Hehehe!", "I blame Johannes.",
        "You should try our sister game, McClonePlusPlusProMaxUltra!",
        "Don't be sad. I'll do better next time, I promise!",
        "Don't be sad, have a hug! <3",
        "I just don't know what went wrong :(", "Shall we play a game?",
        "Quite honestly, I wouldn't worry myself about that.", "Sorry :(",
        "Surprise! Haha. Well, this is awkward.",
        "Would you like a cupcake?",
        "Hi. I'm McClone, and I'm a crashaholic.", "Ooh. Shiny.",
        "This doesn't make any sense!", "Why is it breaking :(",
        "Don't do that.", "Ouch. That hurt :(", "You're mean.",
        "This is a token for 1 free hug.",
        "There are four lights!", "But it works on my machine."
    };

    try {
        auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        return comments[now % comments.size()];
    } catch (...) {
        return "Witty comment unavailable :(";
    }
}

CrashReport *CrashReport::makeCrashReport(Throwable *cause, const std::string &description) {
    // mirrors java: if cause is a reportedException, extract its report
    // otherwise create a new one
    // reportedException port would go here once you have it
    return new CrashReport(description, cause);
}
