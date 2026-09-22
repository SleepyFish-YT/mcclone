#pragma once

#include "IMetadataSectionSerializer.h"
#include "PackMetadataSection.h"

#include <nlohmann/json.hpp>

class PackMetadataSectionSerializer : public IMetadataSectionSerializer<PackMetadataSection> {
public:
    std::string getSectionName() const override {
        return "pack";
    }

    PackMetadataSection deserialize(const ::nlohmann::json &json) const override {
        std::string description = json.value("description", "");
        int pack_format = json.value("pack_format", 1);
        return PackMetadataSection(description, pack_format);
    }
};