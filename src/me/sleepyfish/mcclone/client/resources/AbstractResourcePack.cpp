//
// Created by SleepyFish on 16.09.2026.
// Project: mcclone
//

#include "AbstractResourcePack.h"

#include "data/IMetadataSerializer.h"
#include "../../debug/Logger.h"
#include "../../util/ResourceLocation.h"

#include <stdexcept>

AbstractResourcePack::AbstractResourcePack(std::filesystem::path resourcePackFile) :
        resourcePackFile(std::move(resourcePackFile))
{}

std::string AbstractResourcePack::locationToName(const ResourceLocation &location) {
    return "assets/" + location.getResourceDomain() + "/" + location.getResourcePath();
}

std::unique_ptr<std::istream> AbstractResourcePack::getInputStream(const ResourceLocation &location) {
    return this->getInputStreamByName(locationToName(location));
}

bool AbstractResourcePack::resourceExists(const ResourceLocation &location) {
    return this->hasResourceName(locationToName(location));
}

std::any AbstractResourcePack::getPackMetadata(IMetadataSerializer &metadataSerializer, const std::string &metadataSectionName) {
    auto stream = this->getInputStreamByName("pack.mcmeta");
    if (!stream) return std::any{};
    return AbstractResourcePack::readMetadata(metadataSerializer, *stream, metadataSectionName);
}

std::any AbstractResourcePack::readMetadata(IMetadataSerializer &metadataSerializer, std::istream &stream, const std::string &sectionName) {
    try {
        ::nlohmann::json j = ::nlohmann::json::parse(stream);
        return metadataSerializer.parseMetadataSection(sectionName, j);
    } catch (const std::exception &e) {
        throw std::runtime_error(std::string("Failed to parse pack metadata: ") + e.what());
    }
}

BufferedImage &AbstractResourcePack::getPackImage() {
    throw std::runtime_error("getPackImage not implemented");
}

std::string AbstractResourcePack::getPackName() const {
    return this->resourcePackFile.filename().string();
}

void AbstractResourcePack::logNameNotLowercase(const std::string &name) const {
    Logger::warn("ResourcePack: ignored non-lowercase namespace: {} in {}", name, this->resourcePackFile.string());
}
