//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_NBTTAGFLOAT_H
#define MCCLONE_NBTTAGFLOAT_H

#include "NBTPrimitive.h"
#include "NBTSizeTracker.h"

#include "../util/MathHelper.h"

#include <string>
#include <format>
#include <istream>
#include <ostream>
#include <cmath>

/**
 * @author SleepyFish
 * @brief NBT tag for a float
 */
class NBTTagFloat : public NBTPrimitive {

private:

    float data = 0.0f;

public:

    NBTTagFloat() = default;

    explicit NBTTagFloat(float data) : data(data) {}

    void write(std::ostream &output) const override {
        output.write(reinterpret_cast<const char*>(&this->data), sizeof(this->data));
    }

    void read(std::istream &input, int depth, NBTSizeTracker &sizeTracker) override {
        sizeTracker.read(96ll);
        input.read(reinterpret_cast<char*>(&this->data), sizeof(this->data));
    }

    int8_t getId() const override {
        return 5;
    }

    std::string toString() const override {
        return std::format("{}f", this->data);
    }

    std::unique_ptr<NBTBase> copy() const override {
        return std::make_unique<NBTTagFloat>(this->data);
    }

    bool operator==(const NBTBase &other) const override {
        if (!NBTBase::operator==(other)) return false;
        return this->data == static_cast<const NBTTagFloat &>(other).data;
    }

    int hashCode() const override {
        return NBTBase::hashCode() ^ std::bit_cast<int32_t>(this->data);
    }

    int64_t getLong() const override {
        return static_cast<int64_t>(this->data);
    }

    int32_t getInt() const override {
        return MathHelper::floor_float(this->data);
    }

    int16_t getShort() const override {
        return static_cast<int16_t>(MathHelper::floor_float(this->data) & 65535);
    }

    int8_t getByte() const override {
        return static_cast<int8_t>(MathHelper::floor_float(this->data) & 255);
    }

    double getDouble() const override {
        return this->data;
    }

    float getFloat() const override {
        return this->data;
    }

};


#endif //MCCLONE_NBTTAGFLOAT_H
