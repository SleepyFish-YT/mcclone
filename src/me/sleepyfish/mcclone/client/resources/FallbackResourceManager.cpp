//
// Created by SleepyFish on 15.09.2026.
// Project: mcclone
//

#include "FallbackResourceManager.h"

#include "IResourcePack.h"
#include "IResource.h"
#include "SimpleResource.h"
#include "../../util/ResourceLocation.h"
#include "../../debug/Logger.h"

#include <stdexcept>

FallbackResourceManager::FallbackResourceManager(const IMetadataSerializer* frmMetadataSerializer) :
    frmMetadataSerializer(frmMetadataSerializer)
{}

void FallbackResourceManager::addResourcePack(IResourcePack* resourcePack) {
    this->resourcePacks.push_back(resourcePack);
}

IResource* FallbackResourceManager::getResource(const ResourceLocation& location) {
    IResourcePack* metaPack = nullptr;
    ResourceLocation metaLocation = getLocationMcmeta(location);

    for (int i = (int)this->resourcePacks.size() - 1; i >= 0; --i) {
        IResourcePack* pack = this->resourcePacks[i];

        if (metaPack == nullptr && pack->resourceExists(metaLocation)) {
            metaPack = pack;
        }

        if (pack->resourceExists(location)) {
            std::istream* metaStream = nullptr;

            if (metaPack != nullptr) {
                metaStream = this->getInputStream(metaLocation, metaPack);
            }

            return new SimpleResource(
                    pack->getPackName(),
                    location,
                    this->getInputStream(location, pack),
                    metaStream,
                    this->frmMetadataSerializer
            );
        }
    }

    throw std::ios_base::failure("File not found: " + location.toString());
}

std::istream* FallbackResourceManager::getInputStream(const ResourceLocation& location, IResourcePack* resourcePack) {
    return resourcePack->getInputStream(location).release(); // transfer ownership out of unique_ptr
}

std::vector<IResource*> FallbackResourceManager::getAllResources(const ResourceLocation& location) {
    std::vector<IResource*> list;
    ResourceLocation metaLocation = getLocationMcmeta(location);

    for (IResourcePack* pack : this->resourcePacks) {
        if (pack->resourceExists(location)) {
            std::istream* metaStream = pack->resourceExists(metaLocation)
                                       ? this->getInputStream(metaLocation, pack)
                                       : nullptr;

            list.push_back(new SimpleResource(
                    pack->getPackName(),
                    location,
                    this->getInputStream(location, pack),
                    metaStream,
                    this->frmMetadataSerializer
            ));
        }
    }

    if (list.empty()) {
        throw std::ios_base::failure("File not found: " + location.toString());
    }

    return list;
}

ResourceLocation FallbackResourceManager::getLocationMcmeta(const ResourceLocation& location) {
    return {
        location.getResourceDomain(),
        location.getResourcePath() + ".mcmeta"
    };
}

const std::unordered_set<std::string>& FallbackResourceManager::getResourceDomains() const {
    static const std::unordered_set<std::string> empty{};
    return empty;
}
