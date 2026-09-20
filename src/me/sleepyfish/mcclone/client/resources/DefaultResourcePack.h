//
// Created by SleepyFish on 16.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_DEFAULTRESOURCEPACK_H
#define MCCLONE_DEFAULTRESOURCEPACK_H

#include "AbstractResourcePack.h"

#include <filesystem>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <string>

class ResourceLocation;

/**
 * @author SleepyFish
 * @brief Default resource pack
 */
class DefaultResourcePack : public AbstractResourcePack {

public:

    explicit DefaultResourcePack(std::filesystem::path root, std::unordered_map<std::string, std::filesystem::path> mapAssets);

    ~DefaultResourcePack() override = default;

    static const std::unordered_set<std::string> defaultResourceDomains;

    std::unique_ptr<std::istream> getInputStream(const ResourceLocation& location) override;
    bool resourceExists(const ResourceLocation &location) override;
    std::unordered_set<std::string> getResourceDomains() const override;
    std::any getPackMetadata(IMetadataSerializer& metadataSerializer, const std::string& metadataSectionName) override;
    BufferedImage getPackImage() override;
    std::string getPackName() const override;

protected:

    std::unique_ptr<std::istream> getInputStreamByName(const std::string& name) override;
    bool hasResourceName(const std::string &name) override;

private:

    std::unique_ptr<std::istream> getInputStreamAssets(const ResourceLocation& location) const;
    std::unique_ptr<std::istream> getResourceStream(const ResourceLocation& location) const;

    std::unordered_map<std::string, std::filesystem::path> mapAssets;

    std::unique_ptr<std::istream> openRootFile(const std::string &name) const;
    std::filesystem::path resourcesRoot;

};


#endif //MCCLONE_DEFAULTRESOURCEPACK_H
