//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_NBTBASE_H
#define MCCLONE_NBTBASE_H

#include <string>
#include <memory>
#include <array>
#include <cstdint>
#include <stdexcept>
#include <istream>
#include <iostream>

class NBTSizeTracker;

/**
 * @author SleepyFish
 * @brief Base class for all NBT tags
 */
class NBTBase {

public:

    static constexpr std::array<const char*, 12> NBT_TYPES = {
            "END", "BYTE", "SHORT", "INT", "LONG", "FLOAT",
            "DOUBLE", "BYTE[]", "STRING", "LIST", "COMPOUND", "INT[]"
    };

    virtual ~NBTBase() = default;

    virtual void write(std::ostream& output) const = 0;

    virtual void read(std::istream& input, int depth, NBTSizeTracker& sizeTracker) = 0;

    virtual std::string toString() const = 0;

    virtual int8_t getId() const = 0;

    virtual std::unique_ptr<NBTBase> copy() const = 0;

    virtual bool hasNoTags() const { return false; }

    virtual bool operator==(const NBTBase& other) const {
        return this->getId() == other.getId();
    }

    virtual int hashCode() const {
        return this->getId();
    }

    static std::unique_ptr<NBTBase> createNewByType(int8_t id);

    virtual std::string getString() const {
        return this->toString();
    }

};

#endif //MCCLONE_NBTBASE_H
