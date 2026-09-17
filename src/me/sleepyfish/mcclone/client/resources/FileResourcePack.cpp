//
// Created by SleepyFish on 17.09.2026.
// Project: mcclone
//

#include "FileResourcePack.h"

#include "ResourcePackFileNotFoundException.h"

#include <sstream>
#include <algorithm>
#include <iostream>

mz_zip_archive *FileResourcePack::getResourcePackZipFile() {
    if (this->resourcePackZipFile == nullptr) {
        this->resourcePackZipFile = new mz_zip_archive{};
        mz_zip_zero_struct(this->resourcePackZipFile);

        if (!mz_zip_reader_init_file(this->resourcePackZipFile, this->resourcePackFile.string().c_str(), 0)) {
            delete this->resourcePackZipFile;
            this->resourcePackZipFile = nullptr;
            throw std::ios_base::failure("Could not open zip file: " + this->resourcePackFile.string());
        }
    }

    return this->resourcePackZipFile;
}

std::unique_ptr<std::istream> FileResourcePack::getInputStreamByName(const std::string &name) {
    mz_zip_archive *zip = this->getResourcePackZipFile();

    int index = mz_zip_reader_locate_file(zip, name.c_str(), nullptr, 0);
    if (index < 0) {
        throw ResourcePackFileNotFoundException(this->resourcePackFile, name);
    }

    size_t size = 0;
    void *data = mz_zip_reader_extract_to_heap(zip, index, &size, 0);

    if (data == nullptr) {
        throw std::ios_base::failure("Could not extract resource: " + name);
    }

    // copy into a string and wrap in a stringstream
    std::string buffer(static_cast<char*>(data), size);
    mz_free(data);

    return std::make_unique<std::istringstream>(std::move(buffer), std::ios::binary);
}

bool FileResourcePack::hasResourceName(const std::string &name) {
    try {
        mz_zip_archive *zip = this->getResourcePackZipFile();
        return mz_zip_reader_locate_file(zip, name.c_str(), nullptr, 0) >= 0;
    } catch (...) {
        return false;
    }
}

std::unordered_set<std::string> FileResourcePack::getResourceDomains() const {
    mz_zip_archive *zip = nullptr;

    try {
        zip = const_cast<FileResourcePack*>(this)->getResourcePackZipFile();
    } catch (...) {
        return {};
    }

    std::unordered_set<std::string> domains;
    mz_uint fileCount = mz_zip_reader_get_num_files(zip);

    for (mz_uint i = 0; i < fileCount; ++i) {
        mz_zip_archive_file_stat stat;
        if (!mz_zip_reader_file_stat(zip, i, &stat)) {
            continue;
        }

        std::string entryName = stat.m_filename;

        if (entryName.rfind("assets/", 0) != 0) {
            continue;
        }

        // split on '/' up to 3 parts - mirrors Guava's entryNameSplitter
        std::vector<std::string> parts;
        std::stringstream ss(entryName);
        std::string part;
        while (std::getline(ss, part, '/') && parts.size() < 3) {
            if (!part.empty()) {
                parts.push_back(part);
            }
        }

        // parts[0] = "assets", parts[1] = domain
        if (parts.size() > 1) {
            std::string domain = parts[1];
            std::string lower = domain;
            std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

            if (domain != lower) {
                this->logNameNotLowercase(domain);
            } else {
                domains.insert(domain);
            }
        }
    }

    return domains;
}

void FileResourcePack::close() {
    if (this->resourcePackZipFile != nullptr) {
        mz_zip_reader_end(this->resourcePackZipFile);
        delete this->resourcePackZipFile;
        this->resourcePackZipFile = nullptr;
    }
}
