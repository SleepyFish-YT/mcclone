//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_NBTTAGDOUBLE_H
#define MCCLONE_NBTTAGDOUBLE_H

#include "NBTPrimitive.h"
#include "NBTSizeTracker.h"
#include "../util/MathHelper.h"

#include <string>
#include <format>
#include <istream>
#include <ostream>
#include <cmath>
#include <bit>

/**
 * @author SleepyFish
 * @brief NBT tag for a double
 */
class NBTTagDouble : public NBTPrimitive {

private:

    double data = 0.0;

public:

    NBTTagDouble() = default;

    explicit NBTTagDouble(double data) :
            data(data)
    {}

    void write(std::ostream& output) const override {
        output.write(reinterpret_cast<const char*>(&this->data), sizeof(double));
    }

    void read(std::istream& input, int depth, NBTSizeTracker& sizeTracker) override {
        sizeTracker.read(128ll);
        input.read(reinterpret_cast<char*>(&this->data), sizeof(double));
    }

    int8_t getId() const override {
        return 6;
    }

    std::string toString() const override {
        return std::format("{}d", this->data);
    }

    std::unique_ptr<NBTBase> copy() const override {
        return std::make_unique<NBTTagDouble>(this->data);
    }

    bool operator==(const NBTBase& other) const override {
        if (!NBTBase::operator==(other)) return false;
        return this->data == static_cast<const NBTTagDouble&>(other).data;
    }

    int hashCode() const override {
        const int64_t i = std::bit_cast<int64_t>(this->data);
        return NBTBase::hashCode() ^ static_cast<int>(i ^ (i >> 32));
    }

    int64_t getLong() const override {
        return static_cast<int64_t>(std::floor(this->data));
    }

    int32_t getInt() const override {
        return MathHelper::floor_double(this->data);
    }

    int16_t getShort() const override {
        return static_cast<int16_t>(MathHelper::floor_double(this->data) & 65535);
    }

    int8_t getByte() const override {
        return static_cast<int8_t>(MathHelper::floor_double(this->data) & 255);
    }

    double getDouble() const override {
        return this->data;
    }

    float getFloat() const override {
        return static_cast<float>(this->data);
    }

};


#endif //MCCLONE_NBTTAGDOUBLE_H
