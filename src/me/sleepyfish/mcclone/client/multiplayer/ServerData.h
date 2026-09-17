//
// Created by SleepyFish on 15.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_SERVERDATA_H
#define MCCLONE_SERVERDATA_H

#include <string>
#include <optional>

class NBTTagCompound;

/**
 * @author SleepyFish
 * @brief Server data base
 */
class ServerData {

public:

    enum class ServerResourceMode {
        ENABLED = 0,
        DISABLED,
        PROMPT
    };

    std::string serverName;
    std::string serverIP;
    std::string populationInfo;
    std::string serverMOTD;
    long        pingToServer = 0;
    int         version      = 47;
    std::string gameVersion  = "1.8.9";
    bool        field_78841_f = false;
    std::string playerList;

    ServerData(const std::string &name, const std::string &ip, bool isLan);

    ServerResourceMode getResourceMode() const;
    void setResourceMode(ServerResourceMode mode);

    std::string getBase64EncodedIconData() const;
    void        setBase64EncodedIconData(const std::string &icon);

    bool isOnLAN() const;
    void copyFrom(const ServerData& other);

    // NBT
    NBTTagCompound *getNBTCompound() const;
    static ServerData getServerDataFromNBTCompound(const NBTTagCompound& nbt);

private:

    ServerResourceMode        resourceMode = ServerResourceMode::PROMPT;
    std::optional<std::string> serverIcon;
    bool                      lanServer    = false;

};

#endif //MCCLONE_SERVERDATA_H
