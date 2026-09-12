//
// Created by SleepyFish on 12.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_IRESOURCEMANAGERRELOADLISTENER_H
#define MCCLONE_IRESOURCEMANAGERRELOADLISTENER_H

class IResourceManager;

/**
 * @author SleepyFish
 * @brief Resource manager reload listener interface
 */
class IResourceManagerReloadListener {

public:

    virtual ~IResourceManagerReloadListener() = default;

    virtual void onResourceManagerReload(IResourceManager& resourceManager) = 0;

};


#endif //MCCLONE_IRESOURCEMANAGERRELOADLISTENER_H
