//
// Created by SleepyFish on 17.09.2026.
// Project: mcclone
//

#include "ResourcePackRepository.h"

#include "IResourcePack.h"
#include "data/IMetadataSerializer.h"
#include "FolderResourcePack.h"
#include "FileResourcePack.h"
#include "data/PackMetadataSection.h"
#include "../settings/GameSettings.h"
#include "../../util/HttpUtil.h"
#include "../../debug/Logger.h"

#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <regex>

void ResourcePackRepository::Entry::updateResourcePack() {
    if (std::filesystem::is_directory(this->resourcePackFile)) {
        this->reResourcePack = new FolderResourcePack(this->resourcePackFile);
    } else {
        this->reResourcePack = new FileResourcePack(this->resourcePackFile);
    }

    std::any metadata = this->reResourcePack->getPackMetadata(*this->metadataSerializer_, "pack");
    if (metadata.has_value()) {
        this->rePackMetadataSection = std::make_shared<PackMetadataSection>(std::any_cast<PackMetadataSection>(metadata));
    }

    this->closeResourcePack();
}

void ResourcePackRepository::Entry::closeResourcePack() {
    if (this->reResourcePack != nullptr) {
        auto *filepack = dynamic_cast<FileResourcePack *>(this->reResourcePack);
        if (filepack != nullptr) {
            filepack->close();
        }
    }
}

std::string ResourcePackRepository::Entry::getResourcePackName() const {
    if (this->reResourcePack == nullptr) return "";
    return this->reResourcePack->getPackName();
}

std::string ResourcePackRepository::Entry::getTexturePackDescription() const {
    if (this->rePackMetadataSection == nullptr)
        return "\u00A7cInvalid pack.mcmeta (or missing 'pack' section)";
    return this->rePackMetadataSection->getPackDescription();
}

int ResourcePackRepository::Entry::getPackFormat() const {
    if (this->rePackMetadataSection == nullptr) return -1;
    return this->rePackMetadataSection->getPackFormat();
}

std::string ResourcePackRepository::Entry::toString() const {
    std::ostringstream oss;
    oss << this->resourcePackFile.filename().string() << ":"
        << (std::filesystem::is_directory(this->resourcePackFile) ? "folder" : "zip") << ":"
        << std::filesystem::last_write_time(this->resourcePackFile).time_since_epoch().count();
    return oss.str();
}

// ── Repository ───────────────────────────────────────────────────────

bool ResourcePackRepository::isValidResourcePack(const std::filesystem::path &path) {
    if (std::filesystem::is_regular_file(path) && path.extension() == ".zip")
        return true;
    if (std::filesystem::is_directory(path) && std::filesystem::exists(path / "pack.mcmeta"))
        return true;
    return false;
}

// ← 5 params now — reads pack lists from GameSettings, same as Java
ResourcePackRepository::ResourcePackRepository(
        const std::filesystem::path &dirResourcePacksIn,
        const std::filesystem::path &dirServerResourcePacksIn,
        IResourcePack *defaultResourcePack,
        IMetadataSerializer *metadataSerializer,
        GameSettings *settings
) : dirResourcePacks(dirResourcePacksIn),
    dirServerResourcePacks(dirServerResourcePacksIn),
    rprDefaultResourcePack(defaultResourcePack),
    rprMetadataSerializer(metadataSerializer)
{
    this->fixDirResourcePacks();
    this->updateRepositoryEntriesAll();

    const auto &selectedPacks      = settings->resourcePacks;
    const auto &incompatiblePacks  = settings->incompatibleResourcePacks;

    // Always include "default" pack if it exists in resourcepacks folder
    bool hasDefaultPack = false;
    for (const auto &entry : this->repositoryEntriesAll) {
        if (entry.getResourcePackName() == "default") {
            hasDefaultPack = true;
            break;
        }
    }
    if (hasDefaultPack) {
        // Add default pack at the beginning (highest priority)
        for (const auto &entry : this->repositoryEntriesAll) {
            if (entry.getResourcePackName() == "default") {
                this->repositoryEntries.insert(this->repositoryEntries.begin(), entry);
                break;
            }
        }
    }

    for (const auto &packName : selectedPacks) {
        for (const auto &entry : this->repositoryEntriesAll) {
            if (entry.getResourcePackName() == packName) {
                if (entry.getPackFormat() == 1 ||
                    std::find(incompatiblePacks.begin(), incompatiblePacks.end(), packName) !=
                    incompatiblePacks.end()) {
                    this->repositoryEntries.push_back(entry);
                } else {
                    Logger::warn("Removed selected resource pack {} because it's no longer compatible", packName);
                }
                break;
            }
        }
    }
}

void ResourcePackRepository::fixDirResourcePacks() {
    if (std::filesystem::exists(this->dirResourcePacks)) {
        if (!std::filesystem::is_directory(this->dirResourcePacks)) {
            std::filesystem::remove(this->dirResourcePacks);
            if (!std::filesystem::create_directories(this->dirResourcePacks)) {
                Logger::warn("Unable to recreate resourcepack folder, it exists but is not a directory: {}",
                             this->dirResourcePacks.string());
            }
        }
    } else {
        if (!std::filesystem::create_directories(this->dirResourcePacks)) {
            Logger::warn("Unable to create resourcepack folder: {}", this->dirResourcePacks.string());
        }
    }
}

std::vector<std::filesystem::path> ResourcePackRepository::getResourcePackFiles() const {
    std::vector<std::filesystem::path> files;
    if (!std::filesystem::is_directory(this->dirResourcePacks))
        return files;

    for (const auto &entry : std::filesystem::directory_iterator(this->dirResourcePacks)) {
        if (isValidResourcePack(entry.path()))
            files.push_back(entry.path());
    }
    return files;
}

void ResourcePackRepository::updateRepositoryEntriesAll() {
    std::vector<Entry> newList;

    for (const auto &file : this->getResourcePackFiles()) {
        Entry entry(file, this->rprMetadataSerializer);

        auto existing = std::find(this->repositoryEntriesAll.begin(),
                                  this->repositoryEntriesAll.end(), entry);

        if (existing == this->repositoryEntriesAll.end()) {
            try {
                entry.updateResourcePack();
                newList.push_back(entry);
            } catch (const std::exception &e) {
                Logger::warn("Failed to load resource pack {}: {}", file.string(), e.what());
            }
        } else {
            newList.push_back(*existing);
        }
    }

    for (auto &old : this->repositoryEntriesAll) {
        if (std::find(newList.begin(), newList.end(), old) == newList.end())
            old.closeResourcePack();
    }

    this->repositoryEntriesAll = newList;
}

std::vector<ResourcePackRepository::Entry> ResourcePackRepository::getRepositoryEntriesAll() const {
    return this->repositoryEntriesAll;
}

std::vector<ResourcePackRepository::Entry> ResourcePackRepository::getRepositoryEntries() const {
    return this->repositoryEntries;
}

void ResourcePackRepository::setRepositories(const std::vector<Entry> &repositories) {
    this->repositoryEntries.clear();
    this->repositoryEntries = repositories;
}

void ResourcePackRepository::setResourcePackInstance(const std::filesystem::path &resourceFile) {
    this->resourcePackInstance = new FileResourcePack(resourceFile);
    // Minecraft::getMinecraft()->scheduleResourcesRefresh();
}

void ResourcePackRepository::clearResourcePack() {
    std::lock_guard<std::recursive_mutex> guard(this->lock);

    if (this->downloadingPacks.valid()) {
        this->downloadingPacks = {};
    }

    if (this->resourcePackInstance != nullptr) {
        this->resourcePackInstance = nullptr;
        // Minecraft::getMinecraft()->scheduleResourcesRefresh();
    }
}

void ResourcePackRepository::deleteOldServerResourcePacks() {
    std::vector<std::pair<std::filesystem::file_time_type, std::filesystem::path>> files;

    for (const auto &entry : std::filesystem::directory_iterator(this->dirServerResourcePacks)) {
        if (entry.is_regular_file()) {                          // ← was listing directories too
            files.emplace_back(std::filesystem::last_write_time(entry.path()), entry.path());
        }
    }

    std::sort(files.begin(), files.end(),
              [](const auto &a, const auto &b) { return a.first > b.first; });

    int i = 0;
    for (const auto &[time, path] : files) {
        if (i++ >= 10) {
            Logger::log("Deleting old server resource pack {}", path.filename().string());
            std::error_code ec;
            std::filesystem::remove(path, ec);
        }
    }
}

std::future<void> ResourcePackRepository::downloadResourcePack(const std::string &url, const std::string &hash) {
    std::string s;
    static const std::regex sha1Regex("^[a-f0-9]{40}$");
    if (std::regex_match(hash, sha1Regex)) {
        s = hash;
    } else {
        s = "legacy";
    }

    std::filesystem::path file1 = this->dirServerResourcePacks / s;

    std::lock_guard<std::recursive_mutex> lockGuard(this->lock);  // ← recursive, safe with clearResourcePack

    this->clearResourcePack();  // ← re-entrant lock, no deadlock now

    if (std::filesystem::exists(file1) && hash.length() == 40) {
        try {
            std::string fileHash = HttpUtil::sha1File(file1);

            if (fileHash == hash) {
                this->setResourcePackInstance(file1);
                std::promise<void> promise;
                promise.set_value();
                return promise.get_future();
            }

            Logger::warn("File {} had wrong hash (expected {}, found {}). Deleting it.",
                         file1.string(), hash, fileHash);
        } catch (const std::ios_base::failure &e) {
            Logger::warn("File {} couldn't be hashed. Deleting it. ({})",
                         file1.string(), e.what());
        }
        std::filesystem::remove(file1);
    }

    this->deleteOldServerResourcePacks();

    std::unordered_map<std::string, std::string> sessionInfo;

    return std::async(std::launch::async, [this, file1, url, sessionInfo = std::move(sessionInfo)]() mutable {
        auto downloadFuture = HttpUtil::downloadResourcePack(file1, url, sessionInfo, 52428800, nullptr);
        downloadFuture.get();
        this->setResourcePackInstance(file1);
    });
}
