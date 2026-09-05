//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_NBTTAGEND_H
#define MCCLONE_NBTTAGEND_H

#include "NBTBase.h"
#include "NBTSizeTracker.h"

#include <string>
#include <istream>
#include <ostream>

/**
 * @author SleepyFish
 * @brief NBT tag for end
 */
class NBTTagEnd : public NBTBase {

public:

    NBTTagEnd() = default;

    void write(std::ostream& output) const override {}

    void read(std::istream& input, int depth, NBTSizeTracker& sizeTracker) override {
        sizeTracker.read(64ll);
    }

    int8_t getId() const override {
        return 0;
    }

    std::string toString() const override {
        return "END";
    }

    std::unique_ptr<NBTBase> copy() const override {
        return std::make_unique<NBTTagEnd>();
    }

};


#endif //MCCLONE_NBTTAGEND_H
