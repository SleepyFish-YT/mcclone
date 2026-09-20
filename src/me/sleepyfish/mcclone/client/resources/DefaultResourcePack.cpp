//
// Created by SleepyFish on 16.09.2026.
// Project: mcclone
//

#include "DefaultResourcePack.h"

#include "../renderer/texture/TextureUtil.h"
#include "../../util/ResourceLocation.h"
#include "../../../sava/BufferedImage.h"
#include "data/IMetadataSerializer.h"

#include <any>
#include <filesystem>
#include <fstream>
#include <ios>
#include <memory>
#include <system_error>
#include <utility>

namespace {

    namespace fs = std::filesystem;

    std::unique_ptr<std::ifstream> openFile(const fs::path &p) {
        std::error_code ec;
        if (!fs::is_regular_file(p, ec)) return nullptr;
        auto s = std::make_unique<std::ifstream>(p, std::ios::binary);
        if (!*s) return nullptr;
        return s;
    }

    // Stat only: no stream allocation, no file handle. Never throws.
    bool isFile(const fs::path &p) noexcept {
        std::error_code ec;
        return fs::is_regular_file(p, ec);
    }

    // assets/mcclone/<domain>/<path>, relative to the working directory.
    fs::path assetPathFor(const ResourceLocation &loc) {
        return fs::path("assets/mcclone") / loc.getResourceDomain() / loc.getResourcePath();
    }

} // namespace

const std::unordered_set<std::string> DefaultResourcePack::defaultResourceDomains = { std::string(MCCLONE_NAME), "realms" };

DefaultResourcePack::DefaultResourcePack(std::filesystem::path root, std::unordered_map<std::string, std::filesystem::path> mapAssets) :
        AbstractResourcePack(""),
        mapAssets(std::move(mapAssets)),
        resourcesRoot(std::move(root))
{
    // No-op if it already exists. Failure is non-fatal: openRootFile() just misses.
    if (!this->resourcesRoot.empty()) {
        std::error_code ec;
        fs::create_directories(this->resourcesRoot, ec);
    }
}

std::unique_ptr<std::istream> DefaultResourcePack::getInputStream(const ResourceLocation& location) {
    if (auto s = this->getResourceStream(location)) return s;
    if (auto s = this->getInputStreamAssets(location)) return s;

    throw std::ios_base::failure("Could not find resource: " + location.toString());
}

std::unique_ptr<std::istream> DefaultResourcePack::getInputStreamAssets(const ResourceLocation& location) const {
    const auto it = this->mapAssets.find(location.toString());
    if (it == this->mapAssets.end()) return nullptr;

    return openFile(it->second);
}

std::unique_ptr<std::istream> DefaultResourcePack::getResourceStream(const ResourceLocation& location) const {
    return openFile(assetPathFor(location));
}

bool DefaultResourcePack::resourceExists(const ResourceLocation& location) {
    if (isFile(assetPathFor(location))) return true;

    const auto it = this->mapAssets.find(location.toString());
    return it != this->mapAssets.end() && isFile(it->second);
}

std::unordered_set<std::string> DefaultResourcePack::getResourceDomains() const {
    return defaultResourceDomains;
}

std::any DefaultResourcePack::getPackMetadata(IMetadataSerializer& metadataSerializer, const std::string& metadataSectionName) {
    auto stream = this->openRootFile("pack.mcmeta");
    if (!stream) return std::any{};

    try {
        return AbstractResourcePack::readMetadata(metadataSerializer, *stream, metadataSectionName);
    } catch (const std::exception&) {
        return std::any{};
    }
}

BufferedImage DefaultResourcePack::getPackImage() {
    auto stream = this->openRootFile("pack.png");
    if (!stream)
        throw std::ios_base::failure("Could not find pack.png (looked in " + this->resourcesRoot.string() + " and the asset index)");

    int width = 0, height = 0;
    auto imageData = TextureUtil::readImageData_(*stream, width, height);

    return {std::move(imageData), width, height};
}

std::string DefaultResourcePack::getPackName() const {
    return "Default";
}

std::unique_ptr<std::istream> DefaultResourcePack::getInputStreamByName(const std::string& name) {
    return openFile(fs::path(name));
}

bool DefaultResourcePack::hasResourceName(const std::string& name) {
    return isFile(fs::path(name));
}

std::unique_ptr<std::istream> DefaultResourcePack::openRootFile(const std::string &name) const {
    if (!this->resourcesRoot.empty()) { // empty root would resolve against the CWD
        if (auto s = openFile(this->resourcesRoot / name))
            return s;
    }

    if (const auto it = this->mapAssets.find(name); it != this->mapAssets.end())
        return openFile(it->second);

    return nullptr;
}
