//
// Created by SleepyFish on 12.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_IRESOURCEPACK_H
#define MCCLONE_IRESOURCEPACK_H

#include <string>
#include <set>
#include <istream>
#include <memory>
#include <any>

class ResourceLocation;
class IMetadataSerializer;
class BufferedImage;

/**
 * @author SleepyFish
 * @brief Resource pack interface
 */
class IResourcePack {

public:

    virtual ~IResourcePack() = default;

    // throws std::ios_base::failure
    virtual std::unique_ptr<std::istream> getInputStream(const ResourceLocation &location) = 0;

    virtual bool resourceExists(const ResourceLocation &location) const = 0;

    virtual std::set<std::string> getResourceDomains() const = 0;

    virtual std::any getPackMetadata(IMetadataSerializer &metadataSerializer, const std::string &metadataSectionName) = 0;

    // throws std::ios_base::failure
    virtual BufferedImage &getPackImage() = 0;

    virtual std::string getPackName() const = 0;

};


#endif //MCCLONE_IRESOURCEPACK_H
