//
// Created by SleepyFish on 17.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_RESOURCEPACKREPOSITORY_H
#define MCCLONE_RESOURCEPACKREPOSITORY_H

#include <string>
#include <vector>
#include <filesystem>
#include <mutex>
#include <memory>
#include <future>
#include <functional>

class IResourcePack;
class IMetadataSerializer;
class PackMetadataSection;
class GameSettings;

class ResourcePackRepository {

public:

    class Entry {

    private:

        std::filesystem::path resourcePackFile;
        IResourcePack *reResourcePack{};
        PackMetadataSection *rePackMetadataSection{};
        IMetadataSerializer *metadataSerializer_{};

    public:

        explicit Entry(const std::filesystem::path &resourcePackFile, IMetadataSerializer *metadataSerializer)
                : resourcePackFile(resourcePackFile), metadataSerializer_(metadataSerializer) {}

        ~Entry() = default;

        void updateResourcePack();
        void closeResourcePack();

        IResourcePack *getResourcePack() const noexcept { return this->reResourcePack; }
        std::string getResourcePackName() const;
        std::string getTexturePackDescription() const;
        int getPackFormat() const;
        std::string toString() const;

        bool operator==(const Entry &other) const {
            return this->toString() == other.toString();
        }
    };

private:

    static bool isValidResourcePack(const std::filesystem::path &path);

    std::filesystem::path dirResourcePacks;
    std::filesystem::path dirServerResourcePacks;
    const IResourcePack *rprDefaultResourcePack;
    IMetadataSerializer *rprMetadataSerializer;
    IResourcePack *resourcePackInstance{};
    std::recursive_mutex lock; // ← recursive, mirrors Java's ReentrantLock
    std::vector<Entry> repositoryEntriesAll{};
    std::vector<Entry> repositoryEntries{};
    std::future<void> downloadingPacks;

public:

    ResourcePackRepository(
            const std::filesystem::path &dirResourcePacksIn,
            const std::filesystem::path &dirServerResourcePacksIn,
            IResourcePack *defaultResourcePack,
            IMetadataSerializer *metadataSerializer,
            GameSettings *settings
    );

    ~ResourcePackRepository() = default;

    void updateRepositoryEntriesAll();
    std::vector<Entry> getRepositoryEntriesAll() const;
    std::vector<Entry> getRepositoryEntries() const;
    void setRepositories(const std::vector<Entry> &repositories);

    std::filesystem::path getDirResourcePacks() const noexcept { return this->dirResourcePacks; }

    std::future<void> downloadResourcePack(const std::string &url, const std::string &hash);

    IResourcePack *getResourcePackInstance() const noexcept { return this->resourcePackInstance; }

    void setResourcePackInstance(const std::filesystem::path &resourceFile);
    void clearResourcePack();

private:

    void fixDirResourcePacks();
    std::vector<std::filesystem::path> getResourcePackFiles() const;
    void deleteOldServerResourcePacks();

};

#endif //MCCLONE_RESOURCEPACKREPOSITORY_H
