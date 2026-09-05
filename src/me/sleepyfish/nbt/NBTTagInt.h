//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_NBTTAGINT_H
#define MCCLONE_NBTTAGINT_H

#include "NBTPrimitive.h"
#include "NBTSizeTracker.h"

#include <string>
#include <format>
#include <istream>
#include <ostream>

/**
 * @author SleepyFish
 * @brief NBT tag for a int
 */
class NBTTagInt : public NBTPrimitive {

private:

    int32_t data = 0;

public:

    NBTTagInt() = default;

    explicit NBTTagInt(int32_t data) : data(data) {}

    void write(std::ostream &output) const override {
        output.write(reinterpret_cast<const char*>(&this->data), sizeof(this->data));
    }

    void read(std::istream &input, int depth, NBTSizeTracker &sizeTracker) override {
        sizeTracker.read(96ll);
        input.read(reinterpret_cast<char*>(&this->data), sizeof(this->data));
    }

    int8_t getId() const override {
        return 3;
    }

    std::string toString() const override {
        return std::to_string(this->data);
    }

    std::unique_ptr<NBTBase> copy() const override {
        return std::make_unique<NBTTagInt>(this->data);
    }

    bool operator==(const NBTBase &other) const override {
        if (!NBTBase::operator==(other)) return false;
        return this->data == static_cast<const NBTTagInt &>(other).data;
    }

    int hashCode() const override {
        return NBTBase::hashCode() ^ this->data;
    }

    int64_t getLong() const override {
        return this->data;
    }

    int32_t getInt() const override {
        return this->data;
    }

    int16_t getShort() const override {
        return static_cast<int16_t>(this->data & 65535);
    }

    int8_t getByte() const override {
        return static_cast<int8_t>(this->data & 255);
    }

    double getDouble() const override {
        return this->data;
    }

    float getFloat() const override {
        return static_cast<float>(this->data);
    }

};


#endif //MCCLONE_NBTTAGINT_H
