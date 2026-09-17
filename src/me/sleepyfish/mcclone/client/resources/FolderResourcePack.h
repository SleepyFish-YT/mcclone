//
// Created by SleepyFish on 17.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_FOLDERRESOURCEPACK_H
#define MCCLONE_FOLDERRESOURCEPACK_H

#include "AbstractResourcePack.h"

#include <filesystem>
#include <unordered_set>
#include <string>

/**
 * @author SleepyFish
 * @brief Resource pack based on a folder
 */
class FolderResourcePack : public AbstractResourcePack {

public:

    explicit FolderResourcePack(const std::filesystem::path &resourcePackFile) :
        AbstractResourcePack(resourcePackFile)
    {}

    ~FolderResourcePack() = default;

    std::unordered_set<std::string> getResourceDomains() const override;

protected:

    std::unique_ptr<std::istream> getInputStreamByName(const std::string &name) override;

    bool hasResourceName(const std::string &name) override;

};


#endif //MCCLONE_FOLDERRESOURCEPACK_H
