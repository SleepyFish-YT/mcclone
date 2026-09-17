//
// Created by SleepyFish on 16.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ABSTRACTRESOURCEPACK_H
#define MCCLONE_ABSTRACTRESOURCEPACK_H

#include "IResourcePack.h"

#include <filesystem>
#include <fstream>
#include <memory>
#include <any>

#include <nlohmann/json.hpp>

class IMetadataSerializer;
class ResourceLocation;
class BufferedImage;

/**
 * @author SleepyFish
 * @brief Abstract resource pack
 */
class AbstractResourcePack : public IResourcePack {

public:

    explicit AbstractResourcePack(std::filesystem::path resourcePackFile);
    virtual ~AbstractResourcePack() = default;

    std::unique_ptr<std::istream> getInputStream(const ResourceLocation &location) override;
    bool resourceExists(const ResourceLocation &location) override;
    std::any getPackMetadata(IMetadataSerializer &metadataSerializer, const std::string &metadataSectionName) override;
    BufferedImage &getPackImage() override;
    std::string getPackName() const override;

    static std::any readMetadata(IMetadataSerializer &metadataSerializer, std::istream &stream, const std::string &sectionName);

protected:

    std::filesystem::path resourcePackFile;

    virtual std::unique_ptr<std::istream> getInputStreamByName(const std::string &name) = 0;
    virtual bool hasResourceName(const std::string &name) = 0;

    void logNameNotLowercase(const std::string &name) const;

private:

    static std::string locationToName(const ResourceLocation &location);

};


#endif //MCCLONE_ABSTRACTRESOURCEPACK_H
