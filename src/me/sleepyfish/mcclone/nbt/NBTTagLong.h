//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_NBTTAGLONG_H
#define MCCLONE_NBTTAGLONG_H

#include "NBTPrimitive.h"
#include "NBTSizeTracker.h"

#include <string>
#include <format>
#include <istream>
#include <ostream>

/**
 * @author SleepyFish
 * @brief NBT tag for a long
 */
class NBTTagLong : public NBTPrimitive {

private:

    int64_t data = 0;

public:

    NBTTagLong() = default;

    explicit NBTTagLong(int64_t data) : data(data) {}

    void write(std::ostream &output) const override {
        output.write(reinterpret_cast<const char*>(&this->data), sizeof(this->data));
    }

    void read(std::istream &input, int depth, NBTSizeTracker &sizeTracker) override {
        sizeTracker.read(128ll);
        input.read(reinterpret_cast<char*>(&this->data), sizeof(this->data));
    }

    int8_t getId() const override {
        return 4;
    }

    std::string toString() const override {
        return std::format("{}L", this->data);
    }

    std::unique_ptr<NBTBase> copy() const override {
        return std::make_unique<NBTTagLong>(this->data);
    }

    bool operator==(const NBTBase &other) const override {
        if (!NBTBase::operator==(other)) return false;
        return this->data == static_cast<const NBTTagLong &>(other).data;
    }

    int hashCode() const override {
        return NBTBase::hashCode() ^ static_cast<int>(this->data ^ (this->data >> 32));
    }

    int64_t getLong() const override {
        return this->data;
    }

    int32_t getInt() const override {
        return static_cast<int32_t>(this->data & -1LL);
    }

    int16_t getShort() const override {
        return static_cast<int16_t>(this->data & 65535LL);
    }

    int8_t getByte() const override {
        return static_cast<int8_t>(this->data & 255LL);
    }

    double getDouble() const override {
        return static_cast<double>(this->data);
    }

    float getFloat() const override {
        return static_cast<float>(this->data);
    }

};


#endif //MCCLONE_NBTTAGLONG_H
