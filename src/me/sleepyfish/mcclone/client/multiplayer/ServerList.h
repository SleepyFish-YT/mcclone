//
// Created by SleepyFish on 15.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_SERVERLIST_H
#define MCCLONE_SERVERLIST_H

#include "ServerData.h"

#include <vector>

class Minecraft;

/**
 * @author SleepyFish
 * @brief Server list
 */
class ServerList {

public:

    explicit ServerList(Minecraft* mc);

    void loadServerList();
    void saveServerList();

    ServerData& getServerData(int index);
    void removeServerData(int index);
    void addServerData(const ServerData& server);
    int  countServers() const;
    void swapServers(int a, int b);
    void setServerData(int index, const ServerData& server);

    static void updateServerData(const ServerData& server);

private:

    Minecraft* mc;
    std::vector<ServerData> servers;

};


#endif //MCCLONE_SERVERLIST_H
