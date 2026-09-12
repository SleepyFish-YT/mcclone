//
// Created by SleepyFish on 12.09.2026.
// Project: mcclone
//

#include "ResourceLocation.h"

#include "../../sava/SavaUtil.h"

ResourceLocation::ResourceLocation(const std::string& resourceName) {
    auto [domain, path] = ResourceLocation::SplitObjectName(resourceName);
    this->resourceDomain = domain.empty() ? std::string(MCCLONE_NAME) : SavaUtil::StringUtil::ToLowerCase(domain);
    this->resourcePath = path;

    if (this->resourcePath.empty()) {
        throw std::invalid_argument("resourcePath must not be null/empty");
    }
}

ResourceLocation::ResourceLocation(const std::string& resourceDomainIn, const std::string& resourcePathIn) {
    this->resourceDomain = resourceDomainIn.empty() ? std::string(MCCLONE_NAME) : SavaUtil::StringUtil::ToLowerCase(resourceDomainIn);
    this->resourcePath = resourcePathIn;

    if (this->resourcePath.empty()) {
        throw std::invalid_argument("resourcePath must not be null/empty");
    }
}
