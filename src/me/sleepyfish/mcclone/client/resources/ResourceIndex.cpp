//
// Created by SleepyFish on 16.09.2026.
// Project: mcclone
//

#include "ResourceIndex.h"

#include "../../debug/Logger.h"

#include <fstream>

#include <nlohmann/json.hpp>

ResourceIndex::ResourceIndex(const std::filesystem::path &assetsDir, const std::string &indexName) {
    if (indexName.empty())
        return;

    std::filesystem::path objectsDir = assetsDir / "objects";
    std::filesystem::path indexFile  = assetsDir / "indexes" / (indexName + ".json");

    std::ifstream stream(indexFile);
    if (!stream.is_open()) {
        Logger::error("Can't find the resource index file: {}", indexFile.string());
        return;
    }

    try {
        nlohmann::json j = nlohmann::json::parse(stream);

        if (!j.contains("objects") || !j["objects"].is_object())
            return;

        for (auto& [key, value] : j["objects"].items()) {
            if (!value.is_object() || !value.contains("hash"))
                continue;

            std::string hash = value["hash"].get<std::string>();

            std::string resourceKey;
            auto slashPos = key.find('/');
            if (slashPos == std::string::npos) {
                resourceKey = key;
            } else {
                resourceKey = key.substr(0, slashPos) + ":" + key.substr(slashPos + 1);
            }

            std::filesystem::path filePath = objectsDir / hash.substr(0, 2) / hash;
            this->resourceMap[resourceKey] = filePath;
        }
    } catch (const nlohmann::json::parse_error& e) {
        Logger::error("Unable to parse resource index file: {}", indexFile.string());
    }
}

const std::unordered_map<std::string, std::filesystem::path> &ResourceIndex::getResourceMap() const {
    return this->resourceMap;
}
