//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#include "RenderInformation.h"

#include "../../mcclone/debug/Logger.h"

void RenderInformation::init() {
    this->vendor   = reinterpret_cast<const char*>(::glGetString(GL_VENDOR));
    this->renderer = reinterpret_cast<const char*>(::glGetString(GL_RENDERER));
    this->version  = reinterpret_cast<const char*>(::glGetString(GL_VERSION));

    ::glGetIntegerv(GL_MAJOR_VERSION, &this->majorVersion);
    ::glGetIntegerv(GL_MINOR_VERSION, &this->minorVersion);
}

void RenderInformation::print() const {
    Logger::log("OpenGL Vendor: {}", this->vendor);
    Logger::log("OpenGL Renderer: {}", this->renderer);
    Logger::log("OpenGL Version: {}", this->version);
}

const std::string &RenderInformation::getVendor() const noexcept {
    return this->vendor;
}

const std::string &RenderInformation::getRenderer() const noexcept {
    return this->renderer;
}

const std::string &RenderInformation::getVersion() const noexcept {
    return this->version;
}

int RenderInformation::getMajorVersion() const noexcept {
    return this->majorVersion;
}

int RenderInformation::getMinorVersion() const noexcept {
    return this->minorVersion;
}

bool RenderInformation::isVersionAtLeast(int major, int minor) const noexcept {
    return this->majorVersion > major || (this->majorVersion == major && this->minorVersion >= minor);
}
