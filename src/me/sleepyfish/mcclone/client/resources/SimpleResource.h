//
// Created by SleepyFish on 15.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_SIMPLERESOURCE_H
#define MCCLONE_SIMPLERESOURCE_H

#include "IResource.h"
#include "../../util/ResourceLocation.h"

#include <nlohmann/json.hpp>
#include <unordered_map>
#include <string>
#include <istream>
#include <any>

class IMetadataSerializer;

/**
 * @author SleepyFish
 * @brief Simple resource
 */
class SimpleResource : public IResource {

private:

    std::unordered_map<std::string, std::any> mapMetadataSections;
    std::string resourcePackName;
    ResourceLocation srResourceLocation;
    std::istream *resourceInputStream;
    std::istream *mcmetaInputStream;
    const IMetadataSerializer *srMetadataSerializer;
    bool mcmetaJsonChecked = false;
    ::nlohmann::json mcmetaJson;

public:

    SimpleResource(std::string resourcePackName, ResourceLocation srResourceLocation, std::istream *resourceInputStream,
                   std::istream *mcmetaInputStream, const IMetadataSerializer *srMetadataSerializer);

    ~SimpleResource() override = default;

    const ResourceLocation& getResourceLocation() const override;

    std::unique_ptr<std::istream> getInputStream() override;

    bool hasMetadata() const override;

    std::any getMetadata(const std::string &sectionName) override;

    const std::string &getResourcePackName() const override;

    bool operator==(const SimpleResource &other) const;

};


#endif //MCCLONE_SIMPLERESOURCE_H
