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

// not FULLY implemented yet.

void TextureManager::bindTexture(ResourceLocation &resource) {
    ITextureObject *textureObj = this->mapTextureObjects.at(&resource);

    if (textureObj == nullptr) {
        textureObj = new SimpleTexture(resource);
        this->loadTexture(resource, textureObj);
    }

    TextureUtil::bindTexture_(textureObj->getGlTextureId());
}

bool TextureManager::loadTickableTexture(ResourceLocation &textureLocation, ITickableTextureObject *textureObj) {
    if (this->loadTexture(textureLocation, textureObj)) {
        this->listTickables.push_back(textureObj);
        return true;
    } else {
        return false;
    }
}

bool TextureManager::loadTexture(ResourceLocation &textureLocation, ITextureObject *textureObj) {
    bool flag = true;

    try {
        textureObj->loadTexture(*this->resourceManager);
    } catch (std::ios_base::failure &e) {
        Logger::warn("Failed loading texture: ({}) {}", textureLocation.toString(), e.what());
        textureObj = TextureUtil::missingTexture;
        this->mapTextureObjects[&textureLocation] = textureObj;
        flag = false;
    } catch (std::exception &e) {
        // CrashReport report = CrashReport::makeCrashReport(throwable, "Registering texture");
        // CrashReportCategory category = report.makeCategory("Resource location being registered");
        // category.addCrashSection("Resource location", textureLocation);
        // category.addCrashSectionCallable("Texture object class", new Callable<String>() {
        //     public String call() {
        //             return textureObj.getClass().getName();
        //     }
        // });
        throw new ReportedException(/*report*/nullptr);
    }

    this->mapTextureObjects[&textureLocation] = textureObj;
    return flag;
}

ITextureObject *TextureManager::getTexture(ResourceLocation &textureLocation) {
    return this->mapTextureObjects.at(&textureLocation);
}

ResourceLocation *TextureManager::getDynamicTextureLocation(std::string name, DynamicTexture *texture) {
    int integer = this->mapTextureCounters.at(name);
    if (integer == 0) {
        integer = 1;
    } else {
        integer = integer + 1;
    }

    this->mapTextureCounters[name] = integer;

    ResourceLocation *location = new ResourceLocation(std::format("dynamic/%s_%d", name, integer));
    this->loadTexture(*location, texture);
    return location;
}

void TextureManager::tick() {
    for (ITickableTexture *tickableTexture: this->listTickables) {
        tickableTexture->tick();
    }
}

void TextureManager::deleteTexture(ResourceLocation &textureLocation) {
    ITextureObject *textureObj = this->getTexture(textureLocation);

    if (textureObj != nullptr) {
        TextureUtil::deleteTexture_(textureObj->getGlTextureId());
    }
}

void TextureManager::onResourceManagerReload(IResourceManager &resourceManager) {
    for (auto entry: this->mapTextureObjects) {
        this->loadTexture(*entry.first, entry.second);
    }
}
