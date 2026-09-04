//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ICOMMAND_H
#define MCCLONE_ICOMMAND_H

#include "CommandException.h"
#include "ICommandSender.h"

#include <string>
#include <vector>
#include <stdexcept>

class ICommandSender;

struct BlockPos;

/**
 * @author SleepyFish
 * @brief Command interface
 */
class ICommand {

public:

    virtual ~ICommand() = default;

    virtual std::string getCommandName() = 0;

    virtual std::string getCommandUsage(const ICommandSender &sender) = 0;

    virtual std::vector<std::string> getCommandAliases() = 0;

    virtual void processCommand(ICommandSender &sender, const std::vector<std::string> &args) = 0; // : throws CommandException

    virtual bool canCommandSenderUseCommand(ICommandSender &sender) = 0;

    virtual std::vector<std::string> addTabCompletionOptions(ICommandSender &sender, const std::vector<std::string> &args, const BlockPos &pos) = 0;

    virtual bool isUsernameIndex(const std::vector<std::string> &args, int index) = 0;

    virtual bool operator<(ICommand &other) { // comparable<ICommand> operator
        return getCommandName() < other.getCommandName();
    }

};


#endif //MCCLONE_ICOMMAND_H
