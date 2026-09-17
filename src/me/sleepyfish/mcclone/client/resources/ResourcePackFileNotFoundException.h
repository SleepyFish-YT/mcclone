//
// Created by SleepyFish on 17.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_RESOURCEPACKFILENOTFOUNDEXCEPTION_H
#define MCCLONE_RESOURCEPACKFILENOTFOUNDEXCEPTION_H

#include <string>
#include <exception>
#include <filesystem>

/**
 * @author SleepyFish
 * @brief Exception thrown when a resource pack file is not found
 */
class ResourcePackFileNotFoundException : public std::exception {

private:

    std::string message;
    const std::filesystem::path resourcePackFile;

public:

    explicit ResourcePackFileNotFoundException(const std::filesystem::path &resourcePackFile, const std::string &message) :
        message(message),
        resourcePackFile(resourcePackFile)
    {}

    const char *what() const noexcept override {
        return message.c_str();
    }

    const std::filesystem::path &getResourcePackFile() const {
        return this->resourcePackFile;
    }

};


#endif //MCCLONE_RESOURCEPACKFILENOTFOUNDEXCEPTION_H
