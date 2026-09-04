//
// Created by SleepyFish on 04.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_IADMINCOMMAND_H
#define MCCLONE_IADMINCOMMAND_H

#include "ICommand.h"

#include <string>
#include <any>
#include <vector>

class ICommandSender;

class IAdminCommand {

public:

    virtual ~IAdminCommand() = default;

    virtual void notifyOperators(
            const ICommandSender& sender,
            const ICommand& command,
            int flags,
            const std::string& msgFormat,
            const std::vector<std::string>& msgParams  // std::any -> std::string
    ) = 0;

};

#endif //MCCLONE_IADMINCOMMAND_H
