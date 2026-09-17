//
// Created by SleepyFish on 17.09.2026.
// Project: mcclone
//

#include "FolderResourcePack.h"

#include <fstream>
#include <algorithm>
#include <iostream>

std::unique_ptr<std::istream> FolderResourcePack::getInputStreamByName(const std::string &name) {
    std::filesystem::path filePath = this->resourcePackFile / name;
    auto stream = std::make_unique<std::ifstream>(filePath, std::ios::binary);

    if (!stream->is_open()) {
        throw std::ios_base::failure("Could not open resource: " + filePath.string());
    }

    return stream;
}

bool FolderResourcePack::hasResourceName(const std::string &name) {
    return std::filesystem::is_regular_file(this->resourcePackFile / name);
}

std::unordered_set<std::string> FolderResourcePack::getResourceDomains() const {
    std::unordered_set<std::string> domains;
    std::filesystem::path assetsDir = this->resourcePackFile / "assets";

    if (!std::filesystem::is_directory(assetsDir)) {
        return domains;
    }

    for (const auto &entry : std::filesystem::directory_iterator(assetsDir)) {
        if (!std::filesystem::is_directory(entry)) {
            continue;
        }

        std::string name = entry.path().filename().string();
        std::string lower = name;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

        if (name != lower) {
            this->logNameNotLowercase(name);
        } else {
            domains.insert(name);
        }
    }

    return domains;
}
