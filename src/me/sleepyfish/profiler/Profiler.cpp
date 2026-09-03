//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#include "Profiler.h"

#include "../debug/Logger.h"

Profiler::Profiler() {
    this->profilingEnabled = false;
    this->profilerGlobalEnabled = true;
    this->profilerLocalEnabled = this->profilerGlobalEnabled;
    this->profilingSection = "";
}

void Profiler::clearProfiling() {
    this->profilingMap.clear();
    this->profilingSection = "";
    this->sectionList.clear();
    this->timestampList.clear();
    this->profilerLocalEnabled = this->profilerGlobalEnabled;
}

void Profiler::startSection(const std::string& name) {
    if (!this->profilerLocalEnabled) return;
    if (!this->profilingEnabled) return;

    if (!this->profilingSection.empty()) {
        this->profilingSection += ".";
    }

    this->profilingSection += name;
    this->sectionList.push_back(this->profilingSection);

    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
    this->timestampList.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(now).count());
}

void Profiler::endSection() {
    if (!this->profilerLocalEnabled) return;
    if (!this->profilingEnabled) return;

    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
    long long i = std::chrono::duration_cast<std::chrono::nanoseconds>(now).count();
    long long j = this->timestampList.back();
    this->timestampList.pop_back();

    long long k = i - j;

    std::string currentSection = this->sectionList.back();
    this->sectionList.pop_back();

    if (this->profilingMap.count(currentSection)) {
        this->profilingMap[currentSection] += k;
    } else {
        this->profilingMap[currentSection] = k;
    }

    if (k > 100000000LL) {
        Logger::warn("Something's taking too long! '" + currentSection + "' took approx " + std::to_string((double) k / 1000000.0) + " ms");
    }

    this->profilingSection = !this->sectionList.empty() ? this->sectionList.back() : "";
}

void Profiler::endStartSection(const std::string& name) {
    if (!this->profilerLocalEnabled) return;
    this->endSection();
    this->startSection(name);
}

std::string Profiler::getNameOfLastSection() const {
    return this->sectionList.empty() ? "[UNKNOWN]" : this->sectionList.back();
}

std::vector<Profiler::Result> Profiler::getProfilingData(const std::string& profilerName) {
    if (!this->profilingEnabled) return {};

    long long i = this->profilingMap.count("root") ? this->profilingMap.at("root") : 0LL;
    long long j = this->profilingMap.count(profilerName) ? this->profilingMap.at(profilerName) : -1LL;

    std::vector<Result> list;
    std::string prefix = profilerName.empty() ? "" : profilerName + ".";

    long long k = 0LL;
    for (const auto& [key, val] : this->profilingMap) {
        if (key.length() > prefix.length()
            && key.starts_with(prefix)
            && key.find('.', prefix.length() + 1) == std::string::npos) {
            k += val;
        }
    }

    float f = (float) k;
    if (k < j) k = j;
    if (i < k) i = k;

    for (const auto& [key, val] : this->profilingMap) {
        if (key.length() > prefix.length()
            && key.starts_with(prefix)
            && key.find('.', prefix.length() + 1) == std::string::npos
        ) {
            double d0 = (double) val * 100.0 / (double) k;
            double d1 = (double) val * 100.0 / (double) i;
            std::string sectionName = key.substr(prefix.length());
            list.emplace_back(sectionName, d0, d1);
        }
    }

    // Decay all values by 95%
    for (auto& [key, val] : this->profilingMap) {
        val = val * 950LL / 1000LL;
    }

    if ((float) k > f) {
        double unspecifiedUse = (double) ((float) k - f) * 100.0 / (double) k;
        double unspecifiedTotal = (double) ((float) k - f) * 100.0 / (double) i;
        list.emplace_back("unspecified", unspecifiedUse, unspecifiedTotal);
    }

    std::sort(list.begin(), list.end());
    list.insert(list.begin(), Result(profilerName, 100.0, (double) k * 100.0 / (double) i));

    return list;
}

void Profiler::printProfilerSection(const std::string &section) {
    auto data = getProfilingData(section);
    if (data.empty() || !this->profilerLocalEnabled) return;

    Logger::log("  " + section);
    for (int i = 1; i < data.size(); i++) {
        const auto& result = data[i];

        int barWidth = 30;
        int filled = (int) (result.usePercentage / 100.0 * barWidth);
        filled = std::clamp(filled, 0, barWidth);

        std::string bar = "[" + std::string(filled, '#') + std::string(barWidth - filled, '-') + "]";
        std::string line = "  " + bar + " " + result.name;

        while (line.length() < 52) line += " ";
        line += std::to_string((int) result.usePercentage) + "." + std::to_string((int) (result.usePercentage * 10) % 10) + "%";

        Logger::log(line);
    }
}
