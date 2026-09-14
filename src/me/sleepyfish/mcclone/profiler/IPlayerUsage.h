//
// Created by SleepyFish on 15.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_IPLAYERUSAGE_H
#define MCCLONE_IPLAYERUSAGE_H

class PlayerUsageSnooper;

/**
 * @author SleepyFish
 * @brief Player usage interface
 */
class IPlayerUsage {

public:

    virtual ~IPlayerUsage() {}

    virtual void addServerStatsToSnooper(PlayerUsageSnooper *playerSnooper) = 0;

    virtual void addServerTypeToSnooper(PlayerUsageSnooper *playerSnooper) = 0;

    virtual bool isSnooperEnabled() = 0;

};


#endif //MCCLONE_IPLAYERUSAGE_H
