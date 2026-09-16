//
// Created by SleepyFish on 14.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_TEXTUREMAP_H
#define MCCLONE_TEXTUREMAP_H

#include "AbstractTexture.h"
#include "ITickableTextureObject.h"

#include <string>
#include <vector>
#include <unordered_map>

class TextureAtlasSprite;
class IResourceManager;
class IIconCreator;
class ResourceLocation;

/**
 * @author SleepyFish
 * @brief Texture atlas map
 */
class TextureMap : public AbstractTexture, public ITickableTextureObject {

public:

    static const ResourceLocation LOCATION_MISSING_TEXTURE;
    static const ResourceLocation LOCATION_BLOCKS_TEXTURE;

    explicit TextureMap(const std::string &basePath);

    TextureMap(const std::string &basePath, IIconCreator *iconCreator);

    void loadTexture(IResourceManager &resourceManager) override;
    void loadSprites(IResourceManager &resourceManager, IIconCreator *iconCreator);
    void loadTextureAtlas(IResourceManager &resourceManager);

    TextureAtlasSprite *getAtlasSprite(const std::string &iconName);
    TextureAtlasSprite *registerSprite(const ResourceLocation &location);
    TextureAtlasSprite *getMissingSprite();

    void updateAnimations();
    void update() override;

    void setMipmapLevels(int levels);

private:

    std::vector<TextureAtlasSprite *> listAnimatedSprites;
    std::unordered_map<std::string, TextureAtlasSprite *> mapRegisteredSprites;
    std::unordered_map<std::string, TextureAtlasSprite *> mapUploadedSprites;

    std::string basePath;
    IIconCreator *iconCreator;
    int mipmapLevels = 0;
    TextureAtlasSprite *missingImage;

    void initMissingImage();

    ResourceLocation completeResourceLocation(const ResourceLocation &location, int mipmapLevel);

};


#endif //MCCLONE_TEXTUREMAP_H
