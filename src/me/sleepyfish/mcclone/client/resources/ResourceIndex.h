//
// Created by SleepyFish on 16.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_RESOURCEINDEX_H
#define MCCLONE_RESOURCEINDEX_H

#include <unordered_map>
#include <string>
#include <filesystem>

/**
 * @author SleepyFish
 * @brief Resource index
 */
class ResourceIndex {

private:

    std::unordered_map<std::string, std::filesystem::path> resourceMap;

public:

    ResourceIndex(const std::filesystem::path &assetsDir, const std::string &indexName);

    const std::unordered_map<std::string, std::filesystem::path> &getResourceMap() const;

};


#endif //MCCLONE_RESOURCEINDEX_H
