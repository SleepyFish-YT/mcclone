//
// Created by SleepyFish on 15.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_FALLBACKRESOURCEMANAGER_H
#define MCCLONE_FALLBACKRESOURCEMANAGER_H

#include "IResourceManager.h"

#include <vector>
#include <string>
#include <istream>

class IResourcePack;
class IMetadataSerializer;
class ResourceLocation;
class IResource;

/**
 * @author SleepyFish
 * @brief Fallback resource manager
 */
class FallbackResourceManager : public IResourceManager {

protected:

    std::vector<IResourcePack *> resourcePacks;

private:

    const IMetadataSerializer *frmMetadataSerializer;

    std::istream *getInputStream(const ResourceLocation &location, IResourcePack *resourcePack); // throws std::ios_base::failure

public:

    explicit FallbackResourceManager(const IMetadataSerializer *frmMetadataSerializer);

    ~FallbackResourceManager() override = default;

    void addResourcePack(IResourcePack *resourcePack);

    IResource *getResource(const ResourceLocation &location) override; // throws std::ios_base::failure

    std::vector<IResource *> getAllResources(const ResourceLocation &location) override; // throws std::ios_base::failure

    static ResourceLocation getLocationMcmeta(const ResourceLocation &location);

    const std::unordered_set<std::string>& getResourceDomains() const override;

};


#endif //MCCLONE_FALLBACKRESOURCEMANAGER_H
