//
// Created by SleepyFish on 14.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_TEXTUREMANAGER_H
#define MCCLONE_TEXTUREMANAGER_H

#include "ITickableTexture.h"
#include "../../resources/IResourceManagerReloadListener.h"

#include <unordered_map>
#include <vector>
#include <string>

class ResourceLocation;
class ITextureObject;
class IResourceManager;
class ITickableTextureObject;
class DynamicTexture;

/**
 * @author SleepyFish
 * @brief Texture manager
 */
class TextureManager : public ITickableTexture, public IResourceManagerReloadListener {

private:

    std::unordered_map<ResourceLocation *, ITextureObject *> mapTextureObjects;
    std::vector<ITickableTexture *> listTickables;
    std::unordered_map<std::string, int> mapTextureCounters;
    IResourceManager *resourceManager;

public:

    TextureManager(IResourceManager *resourceManager) :
        resourceManager(resourceManager)
    {}

    void bindTexture(ResourceLocation &resource);

    bool loadTickableTexture(ResourceLocation &textureLocation, ITickableTextureObject *textureObj);

    bool loadTexture(ResourceLocation &textureLocation, ITextureObject *textureObj);

    ITextureObject *getTexture(ResourceLocation &textureLocation);

    ResourceLocation *getDynamicTextureLocation(std::string name, DynamicTexture *texture);

    void tick() override;

    void deleteTexture(ResourceLocation &textureLocation);

    void onResourceManagerReload(IResourceManager &resourceManager) override;

};


#endif //MCCLONE_TEXTUREMANAGER_H
