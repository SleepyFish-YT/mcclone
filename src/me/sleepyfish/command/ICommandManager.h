//
// Created by SleepyFish on 04.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ICOMMANDMANAGER_H
#define MCCLONE_ICOMMANDMANAGER_H

#include "ICommandSender.h"
#include "ICommand.h"

#include <string>
#include <vector>
#include <map>

/**
 * @author SleepyFish
 */
class ICommandManager {

public:

    virtual ~ICommandManager() = default;

    virtual int executeCommand(ICommandSender& sender, const std::string& rawCommand) = 0;

    virtual std::vector<std::string> getTabCompletionOptions(
            ICommandSender& sender,
            const std::string& input,
            const BlockPos& pos
    ) = 0;

    virtual std::vector<ICommand*> getPossibleCommands(ICommandSender& sender) = 0;

    virtual std::map<std::string, ICommand*> getCommands() = 0;

};

#endif //MCCLONE_ICOMMANDMANAGER_H
