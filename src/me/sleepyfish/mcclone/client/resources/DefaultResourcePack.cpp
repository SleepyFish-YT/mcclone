//
// Created by SleepyFish on 16.09.2026.
// Project: mcclone
//

#include "DefaultResourcePack.h"

#include "../renderer/texture/TextureUtil.h"
#include "../../util/ResourceLocation.h"
#include "../../../sava/BufferedImage.h"
#include "data/IMetadataSerializer.h"

#include <fstream>
#include <stdexcept>

namespace {

    // one stat, rejects directories, never throws
    std::unique_ptr<std::ifstream> openFile(const std::filesystem::path &p) {
        std::error_code ec;
        if (!std::filesystem::is_regular_file(p, ec)) return nullptr;
        auto s = std::make_unique<std::ifstream>(p, std::ios::binary);
        if (!*s) return nullptr;
        return s;
    }

} // namespace

const std::unordered_set<std::string> DefaultResourcePack::defaultResourceDomains = { std::string(MCCLONE_NAME), "realms" };

DefaultResourcePack::DefaultResourcePack(std::filesystem::path root, std::unordered_map<std::string, std::filesystem::path> mapAssets) :
    AbstractResourcePack(""),
    mapAssets(std::move(mapAssets)),
    resourcesRoot(std::move(root))
{
    if (!std::filesystem::exists(resourcesRoot)) {
        std::filesystem::create_directories(resourcesRoot);
    }
}

std::unique_ptr<std::istream> DefaultResourcePack::getInputStream(const ResourceLocation& location) {
    auto stream = this->getResourceStream(location);
    if (stream) return stream;

    stream = this->getInputStreamAssets(location);
    if (stream) return stream;

    throw std::ios_base::failure("Could not find resource: " + location.getResourcePath());
}

std::unique_ptr<std::istream> DefaultResourcePack::getInputStreamAssets(const ResourceLocation& location) const {
    auto it = this->mapAssets.find(location.toString());
    if (it == this->mapAssets.end()) return nullptr;

    if (!std::filesystem::is_regular_file(it->second)) return nullptr;

    auto stream = std::make_unique<std::ifstream>(it->second, std::ios::binary);
    if (!stream->is_open()) return nullptr;

    return stream;
}

std::unique_ptr<std::istream> DefaultResourcePack::getResourceStream(const ResourceLocation& location) const {
    std::filesystem::path assetPath = std::filesystem::path("assets") / location.getResourceDomain() / location.getResourcePath();

    if (!std::filesystem::exists(assetPath)) return nullptr;

    auto stream = std::make_unique<std::ifstream>(assetPath, std::ios::binary);
    if (!stream->is_open()) return nullptr;

    return stream;
}

bool DefaultResourcePack::resourceExists(const ResourceLocation& location) {
    return this->getResourceStream(location) != nullptr || this->mapAssets.count(location.toString()) > 0;
}

std::unordered_set<std::string> DefaultResourcePack::getResourceDomains() const {
    return defaultResourceDomains;
}

std::any DefaultResourcePack::getPackMetadata(IMetadataSerializer& metadataSerializer, const std::string& metadataSectionName) {
    auto it = this->mapAssets.find("pack.mcmeta");
    if (it == this->mapAssets.end()) return std::any{};

    try {
        std::ifstream stream(it->second, std::ios::binary);
        if (!stream.is_open()) return std::any{};
        return AbstractResourcePack::readMetadata(metadataSerializer, stream, metadataSectionName);
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

    return {imageData, width, height};
}

std::string DefaultResourcePack::getPackName() const {
    return "Default";
}

std::unique_ptr<std::istream> DefaultResourcePack::getInputStreamByName(const std::string& name) {
    std::filesystem::path path(name);
    if (!std::filesystem::exists(path)) return nullptr;

    auto stream = std::make_unique<std::ifstream>(path, std::ios::binary);
    if (!stream->is_open()) return nullptr;

    return stream;
}

bool DefaultResourcePack::hasResourceName(const std::string& name) {
    return std::filesystem::exists(std::filesystem::path(name));
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

