//
// Created by SleepyFish on 12.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_IRELOADABLERESOURCEMANAGER_H
#define MCCLONE_IRELOADABLERESOURCEMANAGER_H

#include "IResourceManager.h"

#include <vector>
#include <memory>

class IResourcePack;
class IResourceManagerReloadListener;

/**
 * @author SleepyFish
 * @brief Resource manager interface
 */
class IReloadableResourceManager : public IResourceManager {

public:

    virtual ~IReloadableResourceManager() = default;

    virtual void reloadResources(const std::vector<std::shared_ptr<IResourcePack>>& resourcePacksList) = 0;

    virtual void registerReloadListener(IResourceManagerReloadListener& reloadListener) = 0;

};


#endif //MCCLONE_IRELOADABLERESOURCEMANAGER_H
