//
// Created by SleepyFish on 04.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_COMMANDBASE_H
#define MCCLONE_COMMANDBASE_H

#include "ICommand.h"
#include "NumberInvalidException.h"
#include "CommandException.h"
#include "IAdminCommand.h"
#include "ICommandSender.h"

// #include "BlockPos.h"
// #include "PlayerNotFoundException.h"
// #include "EntityNotFoundException.h"
// #include "PlayerSelector.h"
// #include "IChatComponent.h"
// #include "ChatComponentText.h"
// #include "MinecraftServer.h"
// #include "Entity.h"
// #include "EntityPlayerMP.h"
// #include "Item.h"
// #include "Block.h"
// #include "ResourceLocation.h"

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
        return {"none", "null"};
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

    static int parseInt(const std::string& input) {
        try {
            size_t pos;
            int result = std::stoi(input, &pos);
            if (pos != input.size()) throw std::invalid_argument("");
            return result;
        } catch (...) {
            throw NumberInvalidException("commands.generic.num.invalid", { input });
        }
    }

    static int parseInt(const std::string& input, int min) {
        return parseInt(input, min, std::numeric_limits<int>::max());
    }

    static int parseInt(const std::string& input, int min, int max) {
        int i = parseInt(input);
        if (i < min)
            throw NumberInvalidException("commands.generic.num.tooSmall", { std::to_string(i), std::to_string(min) });
        if (i > max)
            throw NumberInvalidException("commands.generic.num.tooBig",   { std::to_string(i), std::to_string(max) });
        return i;
    }

    static long parseLong(const std::string& input) {
        try {
            size_t pos;
            long result = std::stol(input, &pos);
            if (pos != input.size()) throw std::invalid_argument("");
            return result;
        } catch (...) {
            throw NumberInvalidException("commands.generic.num.invalid", { input });
        }
    }

    static long parseLong(const std::string& input, long min, long max) {
        long i = parseLong(input);
        if (i < min)
            throw NumberInvalidException("commands.generic.num.tooSmall", { std::to_string(i), std::to_string(min) });
        if (i > max)
            throw NumberInvalidException("commands.generic.num.tooBig",   { std::to_string(i), std::to_string(max) });
        return i;
    }

    static double parseDouble(const std::string& input) {
        try {
            size_t pos;
            double d = std::stod(input, &pos);
            if (pos != input.size()) throw std::invalid_argument("");
            if (!std::isfinite(d))
                throw NumberInvalidException("commands.generic.num.invalid", { input });
            return d;
        } catch (const NumberInvalidException&) {
            throw;
        } catch (...) {
            throw NumberInvalidException("commands.generic.num.invalid", { input });
        }
    }

    static double parseDouble(const std::string& input, double min) {
        return parseDouble(input, min, std::numeric_limits<double>::max());
    }

    static double parseDouble(const std::string& input, double min, double max) {
        double d = parseDouble(input);
        if (d < min)
            throw NumberInvalidException("commands.generic.double.tooSmall", { std::to_string(d), std::to_string(min) });
        if (d > max)
            throw NumberInvalidException("commands.generic.double.tooBig",   { std::to_string(d), std::to_string(max) });
        return d;
    }

    // Relative-coordinate overloads ("~" prefix)
    static double parseDouble(double base, const std::string& input, bool centerBlock) {
        return parseDouble(base, input, -30000000, 30000000, centerBlock);
    }

    static double parseDouble(double base, std::string input, int min, int max, bool centerBlock) {
        bool relative = !input.empty() && input[0] == '~';

        if (relative && std::isnan(base))
            throw NumberInvalidException("commands.generic.num.invalid", { std::to_string(base) });

        double d = relative ? base : 0.0;

        if (!relative || input.size() > 1) {
            bool hasDecimal = input.find('.') != std::string::npos;
            if (relative) input = input.substr(1);

            d += parseDouble(input);

            if (!hasDecimal && !relative && centerBlock)
                d += 0.5;
        }

        if (min != 0 || max != 0) {
            if (d < min) throw NumberInvalidException("commands.generic.double.tooSmall", { std::to_string(d), std::to_string(min) });
            if (d > max) throw NumberInvalidException("commands.generic.double.tooBig",   { std::to_string(d), std::to_string(max) });
        }

        return d;
    }

    static CoordinateArg parseCoordinate(double base, const std::string& input, bool centerBlock) {
        return parseCoordinate(base, input, -30000000, 30000000, centerBlock);
    }

    static CoordinateArg parseCoordinate(double base, std::string input, int min, int max, bool centerBlock) {
        bool relative = !input.empty() && input[0] == '~';

        if (relative && std::isnan(base))
            throw NumberInvalidException("commands.generic.num.invalid", { std::to_string(base) });

        double offset = 0.0;

        if (!relative || input.size() > 1) {
            bool hasDecimal = input.find('.') != std::string::npos;
            if (relative) input = input.substr(1);

            offset += parseDouble(input);

            if (!hasDecimal && !relative && centerBlock)
                offset += 0.5;
        }

        if (min != 0 || max != 0) {
            if (offset < min) throw NumberInvalidException("commands.generic.double.tooSmall", { std::to_string(offset), std::to_string(min) });
            if (offset > max) throw NumberInvalidException("commands.generic.double.tooBig",   { std::to_string(offset), std::to_string(max) });
        }

        return CoordinateArg(offset + (relative ? base : 0.0), offset, relative);
    }

    /*
    static BlockPos parseBlockPos(
            const ICommandSender& sender,
            const std::vector<std::string>& args,
            int startIndex,
            bool centerBlock)
    {
        BlockPos origin = sender.getPosition();
        return BlockPos(
                parseDouble(origin.getX(), args[startIndex],     -30000000, 30000000, centerBlock),
                parseDouble(origin.getY(), args[startIndex + 1],         0,      256, false),
                parseDouble(origin.getZ(), args[startIndex + 2], -30000000, 30000000, centerBlock)
        );
    }
    */
    static bool parseBoolean(const std::string& input) {
        if (input == "true" || input == "1") return true;
        if (input == "false" || input == "0") return false;
        throw CommandException("commands.generic.boolean.invalid", { input });
    }
    /*
    static EntityPlayerMP& getCommandSenderAsPlayer(ICommandSender& sender) {
        auto* player = dynamic_cast<EntityPlayerMP*>(&sender);
        if (!player)
            throw PlayerNotFoundException("You must specify which player you wish to perform this action on.");
        return *player;
    }

    static EntityPlayerMP& getPlayer(ICommandSender& sender, const std::string& username) {
        EntityPlayerMP* player = PlayerSelector::matchOnePlayer(sender, username);

        if (!player) {
            try {
                UUID uuid = UUID::fromString(username);
                player = MinecraftServer::getServer().getConfigurationManager().getPlayerByUUID(uuid);
            } catch (const std::invalid_argument&) {}
        }

        if (!player)
            player = MinecraftServer::getServer().getConfigurationManager().getPlayerByUsername(username);

        if (!player) throw PlayerNotFoundException();
        return *player;
    }

    static Entity& getEntity(ICommandSender& sender, const std::string& name) {
        return getEntity<Entity>(sender, name);
    }

    template<typename T>
    static T& getEntity(ICommandSender& sender, const std::string& name) {
        static_assert(std::is_base_of<Entity, T>::value, "T must derive from Entity");

        Entity* entity = PlayerSelector::matchOneEntity(sender, name);
        auto& server   = MinecraftServer::getServer();

        if (!entity)
            entity = server.getConfigurationManager().getPlayerByUsername(name);

        if (!entity) {
            try {
                UUID uuid = UUID::fromString(name);
                entity = server.getEntityFromUuid(uuid);
                if (!entity)
                    entity = server.getConfigurationManager().getPlayerByUUID(uuid);
            } catch (const std::invalid_argument&) {
                throw EntityNotFoundException("commands.generic.entity.invalidUuid");
            }
        }

        T* typed = dynamic_cast<T*>(entity);
        if (!typed) throw EntityNotFoundException();
        return *typed;
    }

    static std::vector<Entity*> getEntities(ICommandSender& sender, const std::string& name) {
        if (PlayerSelector::hasArguments(name))
            return PlayerSelector::matchEntities(sender, name);
        return { &getEntity(sender, name) };
    }

    static std::string getPlayerName(ICommandSender& sender, const std::string& query) {
        try {
            return getPlayer(sender, query).getName();
        } catch (const PlayerNotFoundException&) {
            if (PlayerSelector::hasArguments(query)) throw;
            return query;
        }
    }

    static std::string getEntityName(ICommandSender& sender, const std::string& query) {
        try {
            return getPlayer(sender, query).getName();
        } catch (const PlayerNotFoundException&) {
            try {
                return getEntity(sender, query).getUniqueID().toString();
            } catch (const EntityNotFoundException&) {
                if (PlayerSelector::hasArguments(query)) throw;
                return query;
            }
        }
    }

    static IChatComponent getChatComponentFromNthArg(
            ICommandSender& sender,
            const std::vector<std::string>& args,
            int index)
    {
        return getChatComponentFromNthArg(sender, args, index, false);
    }

    static IChatComponent getChatComponentFromNthArg(
            ICommandSender& sender,
            const std::vector<std::string>& args,
            int index,
            bool resolveSelectors)
    {
        ChatComponentText result("");

        for (int i = index; i < static_cast<int>(args.size()); ++i) {
            if (i > index) result.appendText(" ");

            IChatComponent part = ChatComponentText(args[i]);

            if (resolveSelectors) {
                auto resolved = PlayerSelector::matchEntitiesToChatComponent(sender, args[i]);
                if (resolved) {
                    part = *resolved;
                } else if (PlayerSelector::hasArguments(args[i])) {
                    throw PlayerNotFoundException();
                }
            }

            result.appendSibling(part);
        }

        return result;
    }
    */

    static std::string buildString(const std::vector<std::string>& args, int startPos) {
        std::ostringstream oss;
        for (int i = startPos; i < static_cast<int>(args.size()); ++i) {
            if (i > startPos) oss << ' ';
            oss << args[i];
        }
        return oss.str();
    }

    /*

    static Item& getItemByText(ICommandSender& sender, const std::string& id) {
        ResourceLocation loc(id);
        Item* item = Item::itemRegistry.getObject(loc);
        if (!item)
            throw NumberInvalidException("commands.give.item.notFound", { id });
        return *item;
    }

    static Block& getBlockByText(ICommandSender& sender, const std::string& id) {
        ResourceLocation loc(id);
        if (!Block::blockRegistry.containsKey(loc))
            throw NumberInvalidException("commands.give.block.notFound", { id });
        Block* block = Block::blockRegistry.getObject(loc);
        if (!block)
            throw NumberInvalidException("commands.give.block.notFound", { id });
        return *block;
    }

    */

    static std::string joinNiceString(const std::vector<std::string>& elements) {
        std::ostringstream oss;
        for (size_t i = 0; i < elements.size(); ++i) {
            if (i > 0) {
                oss << (i == elements.size() - 1 ? " and " : ", ");
            }
            oss << elements[i];
        }
        return oss.str();
    }

    static std::string joinNiceStringFromCollection(const std::vector<std::string>& strings) {
        return joinNiceString(strings);
    }

    /*
    static IChatComponent join(const std::vector<IChatComponent>& components) {
        ChatComponentText result("");
        for (size_t i = 0; i < components.size(); ++i) {
            if (i > 0) {
                result.appendText(i == components.size() - 1 ? " and " : ", ");
            }
            result.appendSibling(components[i]);
        }
        return result;
    }
    */

    static bool doesStringStartWith(const std::string& original, const std::string& candidate) {
        if (original.size() > candidate.size()) return false;
        return std::equal(
                original.begin(), original.end(),
                candidate.begin(),
                [](char a, char b) { return std::tolower(a) == std::tolower(b); }
        );
    }

    static std::vector<std::string> getListOfStringsMatchingLastWord(
            const std::vector<std::string>& args,
            const std::vector<std::string>& possibilities)
    {
        const std::string& last = args.back();
        std::vector<std::string> result;
        for (const auto& s : possibilities) {
            if (doesStringStartWith(last, s))
                result.push_back(s);
        }
        return result;
    }

    // Tab-complete XYZ coords from a BlockPos
    static std::vector<std::string> getTabCompletionsXYZ(
            const std::vector<std::string>& args,
            int startIndex,
            const BlockPos* pos)
    {
        if (!pos) return {};
        int i = static_cast<int>(args.size()) - 1;
        if      (i == startIndex)     return { std::to_string(pos->x) };
        else if (i == startIndex + 1) return { std::to_string(pos->y) };
        else if (i == startIndex + 2) return { std::to_string(pos->z) };
        return {};
    }

    // Tab-complete XZ coords from a BlockPos
    static std::vector<std::string> getTabCompletionsXZ(
            const std::vector<std::string>& args,
            int startIndex,
            const BlockPos* pos)
    {
        if (!pos) return {};
        int i = static_cast<int>(args.size()) - 1;
        if      (i == startIndex)     return { std::to_string(pos->x) };
        else if (i == startIndex + 1) return { std::to_string(pos->z) };
        return {};
    }

    // -------------------------------------------------------------------------
    // Static: operator notification
    // -------------------------------------------------------------------------

    static void notifyOperators(
            ICommandSender& sender,
            const ICommand& command,
            const std::string& msgFormat,
            const std::vector<std::string>& msgParams = {})
    {
        notifyOperators(sender, command, 0, msgFormat, msgParams);
    }

    static void notifyOperators(
            const ICommandSender& sender,
            const ICommand& command,
            const int flags,
            const std::string& msgFormat,
            const std::vector<std::string>& msgParams = {})
    {
        if (s_admin)
            s_admin->notifyOperators(sender, command, flags, msgFormat, msgParams);
    }

    static void setAdminCommander(IAdminCommand* command) {
        s_admin = command;
    }

private:

    static inline IAdminCommand* s_admin = nullptr;

};

#endif //MCCLONE_COMMANDBASE_H
