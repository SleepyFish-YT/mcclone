//
// Created by SleepyFish on 15.09.2026.
// Project: mcclone
//

#include "SimpleReloadableResourceManager.h"

#include "FallbackResourceManager.h"
#include "IResourcePack.h"
#include "IResource.h"
#include "../../util/ResourceLocation.h"
#include "../../debug/Logger.h"

#include <stdexcept>
#include <format>

SimpleReloadableResourceManager::SimpleReloadableResourceManager(const IMetadataSerializer* rmMetadataSerializer) :
    rmMetadataSerializer(rmMetadataSerializer)
{}

SimpleReloadableResourceManager::~SimpleReloadableResourceManager() {
    for (auto& [domain, manager] : this->domainResourceManagers) {
        delete manager;
    }
}

void SimpleReloadableResourceManager::reloadResourcePack(IResourcePack* resourcePack) {
    for (const std::string &domain : resourcePack->getResourceDomains()) {
        this->setResourceDomains.insert(domain);

        auto it = this->domainResourceManagers.find(domain);
        FallbackResourceManager *fallbackManager = nullptr;

        if (it == this->domainResourceManagers.end()) {
            fallbackManager = new FallbackResourceManager(this->rmMetadataSerializer);
            this->domainResourceManagers[domain] = fallbackManager;
        } else {
            fallbackManager = it->second;
        }

        fallbackManager->addResourcePack(resourcePack);
    }
}

const std::unordered_set<std::string>& SimpleReloadableResourceManager::getResourceDomains() const {
    return this->setResourceDomains;
}

IResource* SimpleReloadableResourceManager::getResource(const ResourceLocation& location) {
    auto it = this->domainResourceManagers.find(location.getResourceDomain());

    if (it != this->domainResourceManagers.end()) {
        return it->second->getResource(location);
    }

    throw std::ios_base::failure("File not found: " + location.toString());
}

std::vector<IResource*> SimpleReloadableResourceManager::getAllResources(const ResourceLocation& location) {
    auto it = this->domainResourceManagers.find(location.getResourceDomain());

    if (it != this->domainResourceManagers.end()) {
        return it->second->getAllResources(location);
    }

    throw std::ios_base::failure("File not found: " + location.toString());
}

void SimpleReloadableResourceManager::clearResources() {
    this->domainResourceManagers.clear();
    this->setResourceDomains.clear();
}

void SimpleReloadableResourceManager::reloadResources(const std::vector<std::shared_ptr<IResourcePack>>& resourcesPacksList) {
    this->clearResources();

    std::string packNames;
    for (size_t i = 0; i < resourcesPacksList.size(); ++i) {
        if (i > 0) packNames += ", ";
        packNames += resourcesPacksList[i]->getPackName();
    }

    Logger::log("Reloading ResourceManager: {}", packNames);

    for (const std::shared_ptr<IResourcePack>& resourcePack : resourcesPacksList) {
        this->reloadResourcePack(resourcePack.get());
    }

    this->notifyReloadListeners();
}

void SimpleReloadableResourceManager::registerReloadListener(IResourceManagerReloadListener& reloadListener) {
    this->reloadListeners.push_back(&reloadListener);
    reloadListener.onResourceManagerReload(*this);
}

void SimpleReloadableResourceManager::notifyReloadListeners() {
    for (IResourceManagerReloadListener *listener : this->reloadListeners) {
        listener->onResourceManagerReload(*this);
    }
}