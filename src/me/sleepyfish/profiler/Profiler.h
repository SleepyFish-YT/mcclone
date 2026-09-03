//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#ifndef MCCLONE_PROFILER_H
#define MCCLONE_PROFILER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <chrono>

/**
 * @author SleepyFish
 * @brief Profiler to measure performance
 */
class Profiler {

public:

    struct Result {

        std::string name;
        double usePercentage;
        double totalUsePercentage;

        Result(const std::string& name, double usePercentage, double totalUsePercentage) {
            this->name = name;
            this->usePercentage = usePercentage;
            this->totalUsePercentage = totalUsePercentage;
        }

        bool operator<(const Result& other) const {
            if (other.usePercentage != this->usePercentage) {
                return other.usePercentage < this->usePercentage;
            }

            return other.name < this->name;
        }

        int getColor() const {
            return (std::hash<std::string>{}(this->name) & 11184810) + 4473924;
        }

    };

    bool profilingEnabled;
    bool profilerGlobalEnabled;

    Profiler();

    void clearProfiling();
    void startSection(const std::string& name);
    void endSection();
    void endStartSection(const std::string& name);
    std::string getNameOfLastSection() const;
    std::vector<Result> getProfilingData(const std::string& profilerName);
    void printProfilerSection(const std::string& profilerName);

private:

    std::vector<std::string> sectionList;
    std::vector<long long> timestampList;
    std::string profilingSection;
    std::unordered_map<std::string, long long> profilingMap;
    bool profilerLocalEnabled;

};

#endif //MCCLONE_PROFILER_H
