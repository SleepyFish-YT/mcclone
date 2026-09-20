//
// Created by SleepyFish on 16.09.2026.
// Project: mcclone
//

#include "ResourceIndex.h"

#include "../../debug/Logger.h"

#include <cctype>
#include <fstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

namespace {

    bool isSha1Hex(const std::string &s) noexcept {
        if (s.size() != 40) return false;
        for (unsigned char c : s)
            if (!std::isxdigit(c)) return false;
        return true;
    }

    bool ensureDir(const std::filesystem::path &dir, const char *what) {
        std::error_code ec;
        if (std::filesystem::is_directory(dir, ec))
            return true;

        if (std::filesystem::create_directories(dir, ec)) {
            Logger::log("Created the {} directory: {}", what, dir.string());
            return true;
        }

        Logger::error("Can't create the {} directory: {} ({})", what, dir.string(), ec.message());
        return false;
    }

} // namespace

ResourceIndex::ResourceIndex(const std::filesystem::path &assetsDir, const std::string &indexName) {
    if (indexName.empty()) {
        Logger::error("ResourceIndex: empty index name");
        throw std::invalid_argument("ResourceIndex: empty index name");
    }

    std::error_code ec;
    if (!std::filesystem::exists(assetsDir, ec)) {
        Logger::error("Can't find the resource assets directory: {}", assetsDir.string());
        throw std::runtime_error("Missing assets directory: " + assetsDir.string());
    }

    const std::filesystem::path objectsDir = assetsDir / "objects";
    const std::filesystem::path indexDir   = assetsDir / "indexes";
    const std::filesystem::path indexFile  = indexDir / (indexName + ".json");

    if (!ensureDir(objectsDir, "resource objects"))
        throw std::runtime_error("Can't create directory: " + objectsDir.string());

    if (!ensureDir(indexDir, "resource index"))
        throw std::runtime_error("Can't create directory: " + indexDir.string());

    std::ifstream stream(indexFile, std::ios::binary);
    if (!stream) {
        Logger::error("Can't open {}. Copy it from your original and paste it inside: {}", indexFile.filename().string(), indexDir.string());
        throw std::runtime_error("Can't open resource index: " + indexFile.string());
    }

    if (stream.peek() == std::ifstream::traits_type::eof()) {
        Logger::error("{} is empty. Copy it from your original and paste it inside: {}", indexFile.filename().string(), indexDir.string());
        throw std::runtime_error("Resource index is empty: " + indexFile.string());
    }

    try {
        const ::nlohmann::json j = ::nlohmann::json::parse(stream);

        const auto objects = j.find("objects");
        if (objects == j.end() || !objects->is_object()) {
            Logger::error("No \"objects\" entry in {}", indexFile.string());
            throw std::runtime_error("No \"objects\" in " + indexFile.filename().string());
        }

        this->resourceMap.reserve(objects->size());

        bool found_icon_16x16 = false;

        size_t skipped = 0;
        for (auto it = objects->begin(); it != objects->end(); ++it) {
            if (!it->is_object()) { ++skipped; continue; }

            const auto h = it->find("hash");
            if (h == it->end() || !h->is_string()) { ++skipped; continue; }

            const auto &hash = h->get_ref<const std::string &>();
            if (!isSha1Hex(hash)) {
                ++skipped;
                Logger::error("Invalid hash {} in {}", hash, indexFile.filename().string());
                continue;
            }

            // security check for first hash
            if (!found_icon_16x16) {
                if (hash == "bdf48ef6b5d0d23bbb02e17d04865216179f510a") {
                    found_icon_16x16 = true;
                }
            }

            std::string key = it.key(); // one copy
            if (const auto slash = key.find('/'); slash != std::string::npos) {
                key[slash] = ':'; // "minecraft/x" -> "minecraft:x" in place
            }

            this->resourceMap.emplace(std::move(key), objectsDir / hash.substr(0, 2) / hash);
        }

        if (!found_icon_16x16) {
            Logger::error("Failed security check: No icon_16x16 in {}", indexFile.filename().string());
            throw std::runtime_error("Failed asset_index.json security check");
        }

        if (skipped > 0)
            Logger::error("Skipped {} invalid entries in {}", skipped, indexFile.filename().string());

    } catch (const ::nlohmann::json::parse_error &e) {
        Logger::error("Unable to parse resource index file: {} {}", indexFile.string(), e.what());
        throw;
    }
}

const std::unordered_map<std::string, std::filesystem::path> &ResourceIndex::getResourceMap() const {
    return this->resourceMap;
}
