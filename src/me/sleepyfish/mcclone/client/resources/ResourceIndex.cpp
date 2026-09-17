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

    if (!std::filesystem::exists(assetsDir)) {
        Logger::error("Can't find the resource assets directory: {}", assetsDir.string());
        return;
    }

    std::filesystem::path objectsDir = assetsDir / "objects";
    if (!std::filesystem::exists(objectsDir)) {
        if (std::filesystem::create_directory(objectsDir)) {
            Logger::log("Created the resource objects directory: {}", objectsDir.string());
        } else {
            Logger::error("Can't create the resource objects directory: {}", objectsDir.string());
            return;
        }
    }

    std::filesystem::path indexDir = assetsDir / "indexes";
    if (!std::filesystem::exists(indexDir)) {
        if (std::filesystem::create_directory(indexDir)) {
            Logger::log("Created the resource index directory: {}", indexDir.string());
        } else {
            Logger::error("Can't create the resource index directory: {}", indexDir.string());
            return;
        }
    }

    std::filesystem::path indexFile = indexDir / (indexName + ".json");
    if (!std::filesystem::exists(indexFile)) {
        std::ofstream stream(indexFile);
        if (!stream.is_open()) {
            Logger::error("Can't create the resource index file: {}", indexFile.string());
            return;
        }
        Logger::log("Created the resource index file: {}", indexFile.string());
    }

    std::ifstream stream(indexFile);
    if (!stream.is_open()) {
        Logger::error("Can't find the resource index file: {}", indexFile.string());
        return;
    }

    try {
        ::nlohmann::json j = ::nlohmann::json::parse(stream);

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
    } catch (const ::nlohmann::json::parse_error& e) {
        Logger::error("Unable to parse resource index file: {} {}", indexFile.string(), e.what());
    }
}

const std::unordered_map<std::string, std::filesystem::path> &ResourceIndex::getResourceMap() const {
    return this->resourceMap;
}
