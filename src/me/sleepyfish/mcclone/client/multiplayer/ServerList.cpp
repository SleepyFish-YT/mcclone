//
// Created by SleepyFish on 15.09.2026.
// Project: mcclone
//

// not FULLY implemented yet.

#include "ServerList.h"

#include "../Minecraft.h"
#include "../../nbt/CompressedStreamTools.h"
#include "../../nbt/NBTTagCompound.h"
#include "../../nbt/NBTTagList.h"
#include "../../debug/Logger.h"

#include <filesystem>

//TODO: CHECK IMPLEMENTATION THIS MIGHT CAUSE CRASHES

ServerList::ServerList(Minecraft *mc) :
    mc(mc)
{
    this->loadServerList();
}

void ServerList::loadServerList() {
    try {
        this->servers.clear();

        std::filesystem::path path = std::filesystem::path(this->mc->mcDataDir) / "servers.dat";
        NBTTagCompound *nbt = CompressedStreamTools::read(path);

        if (nbt == nullptr) return;

        NBTTagList list = nbt->getTagList("servers", 10);

        for (int i = 0; i < list.tagCount(); ++i)
            this->servers.push_back(ServerData::getServerDataFromNBTCompound(list.getCompoundTagAt(i)));

        delete nbt;
    } catch (std::exception& e) {
        Logger::error("Couldn't load server list ({})", e.what());
    }
}

void ServerList::saveServerList() {
    try {
        NBTTagList *list = new NBTTagList();

         for (const ServerData& server : this->servers)
             list->appendTag(std::unique_ptr<NBTBase>(server.getNBTCompound()));

        NBTTagCompound *nbt = new NBTTagCompound();
        nbt->setTag("servers", std::unique_ptr<NBTBase>(list));

        std::filesystem::path path = std::filesystem::path(this->mc->mcDataDir) / "servers.dat";
        CompressedStreamTools::safeWrite(nbt, path);
    } catch (std::exception& e) {
        Logger::error("Couldn't save server list ({})", e.what());
    }
}

ServerData& ServerList::getServerData(int index) {
    return this->servers[index];
}

void ServerList::removeServerData(int index) {
    this->servers.erase(this->servers.begin() + index);
}

void ServerList::addServerData(const ServerData& server) {
    this->servers.push_back(server);
}

int ServerList::countServers() const {
    return static_cast<int>(this->servers.size());
}

void ServerList::swapServers(int a, int b) {
    std::swap(this->servers[a], this->servers[b]);
    this->saveServerList();
}

void ServerList::setServerData(int index, const ServerData& server) {
    this->servers[index] = server;
}

void ServerList::updateServerData(const ServerData& server) {
    ServerList list(Minecraft::getMinecraft());
    list.loadServerList();

    for (int i = 0; i < list.countServers(); ++i) {
        ServerData& existing = list.getServerData(i);
        if (existing.serverName == server.serverName &&
            existing.serverIP   == server.serverIP) {
            list.setServerData(i, server);
            break;
        }
    }

    list.saveServerList();
}
