//
// Created by SleepyFish on 15.09.2026.
// Project: mcclone
//

#include "SimpleResource.h"

#include "data/IMetadataSerializer.h"

SimpleResource::SimpleResource(
        std::string resourcePackName,
        ResourceLocation srResourceLocation,
        std::istream* resourceInputStream,
        std::istream* mcmetaInputStream,
        const IMetadataSerializer* srMetadataSerializer
) :
        resourcePackName(std::move(resourcePackName)),
        srResourceLocation(std::move(srResourceLocation)),
        resourceInputStream(resourceInputStream),
        mcmetaInputStream(mcmetaInputStream),
        srMetadataSerializer(srMetadataSerializer)
{}

const ResourceLocation& SimpleResource::getResourceLocation() const {
    return this->srResourceLocation;
}

std::unique_ptr<std::istream> SimpleResource::getInputStream() {
    // wrap without taking ownership - caller must not outlive this SimpleResource
    return std::unique_ptr<std::istream>(this->resourceInputStream);
}

bool SimpleResource::hasMetadata() const {
    return this->mcmetaInputStream != nullptr;
}

std::any SimpleResource::getMetadata(const std::string& sectionName) {
    if (!this->hasMetadata()) {
        return std::any{};
    }

    // parse mcmeta JSON once
    if (this->mcmetaJson.is_null() && !this->mcmetaJsonChecked) {
        this->mcmetaJsonChecked = true;

        try {
            this->mcmetaJson = nlohmann::json::parse(*this->mcmetaInputStream);
        } catch (nlohmann::json::parse_error& e) {
            return std::any{};
        }
    }

    // check cache first
    auto it = this->mapMetadataSections.find(sectionName);
    if (it != this->mapMetadataSections.end()) {
        return it->second;
    }

    // parse and cache the section
    std::any section = this->srMetadataSerializer->parseMetadataSection(sectionName, this->mcmetaJson);
    this->mapMetadataSections[sectionName] = section;
    return section;
}

const std::string& SimpleResource::getResourcePackName() const {
    return this->resourcePackName;
}

bool SimpleResource::operator==(const SimpleResource& other) const {
    return this->srResourceLocation == other.srResourceLocation &&
           this->resourcePackName == other.resourcePackName;
}