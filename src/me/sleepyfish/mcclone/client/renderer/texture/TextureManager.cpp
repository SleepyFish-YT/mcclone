//
// Created by SleepyFish on 14.09.2026.
// Project: mcclone
//

#include "TextureManager.h"

#include "../../../util/ResourceLocation.h"
#include "ITextureObject.h"
#include "ITickableTextureObject.h"
#include "SimpleTexture.h"
#include "DynamicTexture.h"
#include "TextureUtil.h"
#include "../../../debug/Logger.h"
#include "../../../util/ReportedException.h"

void TextureManager::bindTexture(const ResourceLocation &resource) {
    auto it = this->mapTextureObjects.find(resource.toString());

    ITextureObject *textureObj = (it != this->mapTextureObjects.end()) ? it->second : nullptr;

    if (textureObj == nullptr) {
        textureObj = new SimpleTexture(resource);
        this->loadTexture(resource, textureObj);
    }

    TextureUtil::bindTexture_(textureObj->getGlTextureId());
}

bool TextureManager::loadTickableTexture(const ResourceLocation &textureLocation, ITickableTextureObject *textureObj) {
    if (this->loadTexture(textureLocation, textureObj)) {
        this->listTickables.push_back(textureObj);
        return true;
    }
    return false;
}

bool TextureManager::loadTexture(const ResourceLocation &textureLocation, ITextureObject *textureObj) {
    bool flag = true;

    try {
        textureObj->loadTexture(*this->resourceManager);
    } catch (std::ios_base::failure &e) {
        Logger::warn("Failed loading texture: ({}) {}", textureLocation.toString(), e.what());
        textureObj = TextureUtil::missingTexture;
        flag = false;
    } catch (std::exception&) {
        throw ReportedException(nullptr);
    }

    this->mapTextureObjects[textureLocation.toString()] = textureObj;
    return flag;
}

ITextureObject *TextureManager::getTexture(const ResourceLocation &textureLocation) {
    auto it = this->mapTextureObjects.find(textureLocation.toString());
    return (it != this->mapTextureObjects.end()) ? it->second : nullptr;
}

ResourceLocation *TextureManager::getDynamicTextureLocation(std::string name, DynamicTexture *texture) {
    int &counter = this->mapTextureCounters[name];
    if (counter == 0) {
        counter = 1;
    } else {
        counter++;
    }

    auto *location = new ResourceLocation(std::format("dynamic/{}_{}", name, counter));
    this->loadTexture(*location, texture);
    return location;
}

void TextureManager::update() {
    for (ITickableTexture *tickableTexture : this->listTickables) {
        tickableTexture->update();
    }
}

void TextureManager::deleteTexture(const ResourceLocation &textureLocation) {
    ITextureObject *textureObj = this->getTexture(textureLocation);

    if (textureObj != nullptr) {
        TextureUtil::deleteTexture_(textureObj->getGlTextureId());
    }
}

void TextureManager::onResourceManagerReload(IResourceManager &resourceManager) {
    for (auto &[key, textureObj] : this->mapTextureObjects) {
        ResourceLocation location(key);
        this->loadTexture(location, textureObj);
    }
}
