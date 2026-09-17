//
// Created by SleepyFish on 15.09.2026.
// Project: mcclone
//

#include "ServerData.h"

#include "../../nbt/NBTTagCompound.h"

ServerData::ServerData(const std::string &name, const std::string &ip, bool isLan) :
    serverName(name),
    serverIP(ip),
    lanServer(isLan)
{}

ServerData::ServerResourceMode ServerData::getResourceMode() const {
    return this->resourceMode;
}

void ServerData::setResourceMode(ServerResourceMode mode) {
    this->resourceMode = mode;
}

std::string ServerData::getBase64EncodedIconData() const {
    return this->serverIcon.value_or("");
}

void ServerData::setBase64EncodedIconData(const std::string &icon) {
    this->serverIcon = icon;
}

bool ServerData::isOnLAN() const {
    return this->lanServer;
}

void ServerData::copyFrom(const ServerData& other) {
    this->serverIP    = other.serverIP;
    this->serverName  = other.serverName;
    this->resourceMode = other.resourceMode;
    this->serverIcon  = other.serverIcon;
    this->lanServer   = other.lanServer;
}

NBTTagCompound* ServerData::getNBTCompound() const {
    NBTTagCompound *nbt = new NBTTagCompound();
    nbt->setString("name", this->serverName);
    nbt->setString("ip", this->serverIP);

    if (this->serverIcon.has_value())
        nbt->setString("icon", this->serverIcon.value());

    if (this->resourceMode == ServerResourceMode::ENABLED)
        nbt->setBoolean("acceptTextures", true);
    else if (this->resourceMode == ServerResourceMode::DISABLED)
        nbt->setBoolean("acceptTextures", false);

    return nbt;
}

ServerData ServerData::getServerDataFromNBTCompound(const NBTTagCompound& nbt) {
    ServerData data(nbt.getString("name"), nbt.getString("ip"), false);

    if (nbt.hasKey("icon", 8))
        data.setBase64EncodedIconData(nbt.getString("icon"));

    if (nbt.hasKey("acceptTextures", 1)) {
        data.setResourceMode(nbt.getBoolean("acceptTextures") ? ServerResourceMode::ENABLED : ServerResourceMode::DISABLED);
    } else {
        data.setResourceMode(ServerResourceMode::PROMPT);
    }

    return data;
}

