//
// Created by SleepyFish on 15.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_SIMPLERELOADABLERESOURCEMANAGER_H
#define MCCLONE_SIMPLERELOADABLERESOURCEMANAGER_H

#include "IReloadableResourceManager.h"
#include "IResourceManagerReloadListener.h"
#include "FallbackResourceManager.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <memory>

class IResourcePack;
class IMetadataSerializer;
class ResourceLocation;

/**
 * @author SleepyFish
 * @brief Simple reloadable resource manager
 */
class SimpleReloadableResourceManager : public IReloadableResourceManager {

private:

    std::unordered_map<std::string, FallbackResourceManager *> domainResourceManagers;
    std::vector<IResourceManagerReloadListener *> reloadListeners;
    std::unordered_set<std::string> setResourceDomains;
    const IMetadataSerializer *rmMetadataSerializer;

    void clearResources();
    void notifyReloadListeners();

public:

    explicit SimpleReloadableResourceManager(const IMetadataSerializer *rmMetadataSerializer);

    ~SimpleReloadableResourceManager();

    void reloadResourcePack(IResourcePack *resourcePack);

    const std::unordered_set<std::string>& getResourceDomains() const override;

    IResource *getResource(const ResourceLocation &location) override; // throws std::ios_base::failure

    std::vector<IResource *>
    getAllResources(const ResourceLocation &location) override; // throws std::ios_base::failure

    void reloadResources(const std::vector<std::shared_ptr<IResourcePack>>& resourcesPacksList) override;

    void registerReloadListener(IResourceManagerReloadListener& reloadListener) override;

};


#endif //MCCLONE_SIMPLERELOADABLERESOURCEMANAGER_H
