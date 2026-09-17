//
// Created by SleepyFish on 17.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_FILERESOURCEPACK_H
#define MCCLONE_FILERESOURCEPACK_H

#include "AbstractResourcePack.h"

#include <filesystem>
#include <unordered_set>
#include <string>
#include <memory>

#include <miniz.h>

/**
 * @author SleepyFish
 * @brief File resource pack
 */
class FileResourcePack : public AbstractResourcePack {

private:

    mz_zip_archive *resourcePackZipFile{};

    mz_zip_archive *getResourcePackZipFile();

public:

    explicit FileResourcePack(const std::filesystem::path &resourcePackFile) :
        AbstractResourcePack(resourcePackFile)
    {}

    ~FileResourcePack() {
        this->close();
    }

    std::unordered_set<std::string> getResourceDomains() const override;

    void close();

protected:

    std::unique_ptr<std::istream> getInputStreamByName(const std::string &name) override;

    bool hasResourceName(const std::string &name) override;

};


#endif //MCCLONE_FILERESOURCEPACK_H
