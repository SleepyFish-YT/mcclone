//
// Created by SleepyFish on 12.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_IRESOURCE_H
#define MCCLONE_IRESOURCE_H

#include <string>
#include <memory>
#include <vector>
#include <memory>
#include <any>

class ResourceLocation;
class IMetadataSection;

/**
 * @author SleepyFish
 * @brief A single resource (file) in a resource pack.
 */
class IResource {

public:

    virtual ~IResource() = default;

    virtual const ResourceLocation& getResourceLocation() const = 0;

    virtual std::unique_ptr<std::istream> getInputStream() = 0;

    virtual bool hasMetadata() const = 0;

    virtual std::any getMetadata(const std::string& key) = 0;

    virtual const std::string& getResourcePackName() const = 0;

};


#endif //MCCLONE_IRESOURCE_H
