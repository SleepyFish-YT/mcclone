//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_RESOURCELOCATION_H
#define MCCLONE_RESOURCELOCATION_H

#include <string>
#include <stdexcept>
#include <functional>
#include <cctype>

/**
 * @author SleepyFish
 * @brief A class representing a resource location
 */
class ResourceLocation {

protected:

    std::string resourceDomain;

    std::string resourcePath;

    static std::string ToLowerCase(std::string str) {
        for (char& c : str)
            c = std::tolower(static_cast<unsigned char>(c));
        return str;
    }

public:

    static std::pair<std::string, std::string> SplitObjectName(const std::string& toSplit) {
        std::string domain;
        std::string path = toSplit;

        const size_t i = toSplit.find(':');

        if (i != std::string::npos) {
            path = toSplit.substr(i + 1);
            if (i > 1)
                domain = toSplit.substr(0, i);
        }

        return { domain, path };
    }

    explicit ResourceLocation(const std::string& resourceName) {
        auto [domain, path] = SplitObjectName(resourceName);
        this->resourceDomain = domain.empty() ? "mcclone" : ToLowerCase(domain);
        this->resourcePath = path;

        if (this->resourcePath.empty())
            throw std::invalid_argument("resourcePath must not be null/empty");
    }

    ResourceLocation(const std::string& resourceDomainIn, const std::string& resourcePathIn) {
        this->resourceDomain = resourceDomainIn.empty() ? "mcclone" : ToLowerCase(resourceDomainIn);
        this->resourcePath = resourcePathIn;

        if (this->resourcePath.empty())
            throw std::invalid_argument("resourcePath must not be null/empty");
    }

    std::string getResourcePath() const noexcept {
        return this->resourcePath;
    }

    std::string getResourceDomain() const noexcept {
        return this->resourceDomain;
    }

    std::string toString() const {
        return this->resourceDomain + ':' + this->resourcePath;
    }

    bool operator==(const ResourceLocation& other) const {
        return this->resourceDomain == other.resourceDomain && this->resourcePath == other.resourcePath;
    }

    bool operator!=(const ResourceLocation& other) const {
        return !(*this == other);
    }

    struct Hash {
        std::size_t operator()(const ResourceLocation& rl) const {
            std::size_t h1 = std::hash<std::string>{}(rl.resourceDomain);
            std::size_t h2 = std::hash<std::string>{}(rl.resourcePath);
            return 31 * h1 + h2;
        }
    };

};

#endif //MCCLONE_RESOURCELOCATION_H
