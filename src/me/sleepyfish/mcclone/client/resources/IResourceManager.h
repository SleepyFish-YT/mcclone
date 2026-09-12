//
// Created by SleepyFish on 12.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_IRESOURCEMANAGER_H
#define MCCLONE_IRESOURCEMANAGER_H

#include <string>
#include <vector>
#include <unordered_set>

class ResourceLocation;
class IResource;

/**
 * @author SleepyFish
 * @brief Resource manager interface
 */
class IResourceManager {

public:

    virtual ~IResourceManager() = default;

    virtual const std::unordered_set<std::string>& getResourceDomains() const = 0;

    virtual IResource* getResource(const ResourceLocation& location) = 0;

    virtual std::vector<IResource*> getAllResources(const ResourceLocation& location) = 0;

};


#endif //MCCLONE_IRESOURCEMANAGER_H
