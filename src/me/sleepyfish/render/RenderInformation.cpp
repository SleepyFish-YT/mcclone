//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#include "RenderInformation.h"

#include "../debug/Logger.h"

void RenderInformation::init() {
    this->vendor   = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    this->renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    this->version  = reinterpret_cast<const char*>(glGetString(GL_VERSION));

    glGetIntegerv(GL_MAJOR_VERSION, &this->majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &this->minorVersion);
}

void RenderInformation::print() const {
    Logger::log("OpenGL Vendor: {}", this->vendor);
    Logger::log("OpenGL Version: {}", this->version);
    Logger::log("OpenGL Renderer: {}", this->renderer);
}

const std::string &RenderInformation::getVendor() const {
    return this->vendor;
}

const std::string &RenderInformation::getRenderer() const {
    return this->renderer;
}

const std::string &RenderInformation::getVersion() const {
    return this->version;
}

int RenderInformation::getMajorVersion() const {
    return this->majorVersion;
}

int RenderInformation::getMinorVersion() const {
    return this->minorVersion;
}

bool RenderInformation::isVersionAtLeast(int major, int minor) const {
    return this->majorVersion > major || (this->majorVersion == major && this->minorVersion >= minor);
}
