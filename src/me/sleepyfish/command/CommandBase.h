//
// Created by SleepyFish on 04.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_COMMANDBASE_H
#define MCCLONE_COMMANDBASE_H

#include "ICommand.h"

#include <string>
#include <vector>
#include <list>
#include <optional>
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <limits>

// not FULLY implemented yet.

/**
 * @author SleepyFish
 */
class CommandBase : public ICommand {

public:

    class CoordinateArg {

    private:

        double m_absolute;
        double m_offset;
        bool m_relative;

    public:

        CoordinateArg(double absolute, double offset, bool relative) :
            m_absolute(absolute),
            m_offset(offset),
            m_relative(relative)
        {}

        double getAbsolute() const {
            return m_absolute;
        }

        double getOffset() const {
            return m_offset;
        }

        bool isRelative() const {
            return m_relative;
        }

    };

    ~CommandBase() override = default;

    virtual int getRequiredPermissionLevel() const {
        return 4;
    }

    std::string getCommandName() override {
        return "CommandBase";
    }

    std::string getCommandUsage(const ICommandSender &sender) override {
        return "base";
    }

    std::vector<std::string> getCommandAliases() override {
        return {};
    }

    void processCommand(ICommandSender &sender, const std::vector<std::string> &args) override {
    }

    bool canCommandSenderUseCommand(ICommandSender& sender) override {
        return sender.canCommandSenderUseCommand(getRequiredPermissionLevel(), getCommandName());
    }

    std::vector<std::string> addTabCompletionOptions(ICommandSender& sender, const std::vector<std::string>& args, const BlockPos& pos) override {
        return {};
    }

    bool isUsernameIndex(const std::vector<std::string>& args, int index) override {
        return false;
    }

    bool operator<(ICommand& other) override {
        return getCommandName() < other.getCommandName();
    }

    int compareTo(ICommand& other) {
        return getCommandName().compare(other.getCommandName());
    }

};

#endif //MCCLONE_COMMANDBASE_H
