//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ICOMMANDSENDER_H
#define MCCLONE_ICOMMANDSENDER_H

#include "../util/Vec3.h"
#include "../util/BlockPos.h"
// IChatComponent
// World
// Entity

#include <string>

// not FULLY implemented yet.

/**
 * @author SleepyFish
 * @brief Command sender interface
 */
class ICommandSender {

public:

    virtual std::string getName() const = 0;

    // virtual IChatComponent getDisplayName() = 0;

    // virtual void addChatMessage(IChatComponent component) = 0;

    virtual bool canCommandSenderUseCommand(int permLevel, std::string commandName) = 0;

    virtual BlockPos getPosition() = 0;

    virtual Vec3 getPositionVector() = 0;

    // virtual World getEntityWorld() = 0;

    // virtual Entity getCommandSenderEntity() = 0;

    virtual bool sendCommandFeedback() = 0;

    // virtual void setCommandStat(CommandResultStats::Type type, int amount) = 0;

};

#endif //MCCLONE_ICOMMANDSENDER_H
