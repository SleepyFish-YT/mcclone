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

    std::unordered_map<std::string, ITextureObject *> mapTextureObjects;
    std::vector<ITickableTexture *> listTickables;
    std::unordered_map<std::string, int> mapTextureCounters;
    IResourceManager *resourceManager;

public:

    explicit TextureManager(IResourceManager *resourceManager) : resourceManager(resourceManager) {}

    void bindTexture(const ResourceLocation &resource);

    bool loadTickableTexture(const ResourceLocation &textureLocation, ITickableTextureObject *textureObj);

    bool loadTexture(const ResourceLocation &textureLocation, ITextureObject *textureObj);

    ITextureObject *getTexture(const ResourceLocation &textureLocation);

    ResourceLocation *getDynamicTextureLocation(std::string name, DynamicTexture *texture);

    void deleteTexture(const ResourceLocation &textureLocation);

    void update() override;

    void onResourceManagerReload(IResourceManager &resourceManager) override;

};


#endif //MCCLONE_TEXTUREMANAGER_H
