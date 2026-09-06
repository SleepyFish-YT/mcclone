//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_NBTTAGBYTE_H
#define MCCLONE_NBTTAGBYTE_H

#include "NBTPrimitive.h"
#include "NBTSizeTracker.h"

#include <string>
#include <format>
#include <istream>
#include <ostream>

/**
 * @author SleepyFish
 * @brief NBT tag for a byte
 */
class NBTTagByte : public NBTPrimitive {

private:

    int8_t data = 0;

public:

    NBTTagByte() = default;

    explicit NBTTagByte(int8_t data) :
        data(data)
    {}

    void write(std::ostream &output) const override {
        output.put(this->data);
    }

    void read(std::istream &input, int depth, NBTSizeTracker &sizeTracker) override {
        sizeTracker.read(72ll);

        this->data = static_cast<int8_t>(input.get());
    }

    int8_t getId() const override {
        return 1;
    }

    std::string toString() const override {
        return std::format("{}b", this->data);
    }

    std::unique_ptr<NBTBase> copy() const override {
        return std::make_unique<NBTTagByte>(this->data);
    }

    bool operator==(const NBTBase &other) const override {
        if (!NBTBase::operator==(other)) return false;
        return this->data == static_cast<const NBTTagByte &>(other).data;
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
        return this->data;
    }

    int8_t getByte() const override {
        return this->data;
    }

    double getDouble() const override {
        return this->data;
    }

    float getFloat() const override {
        return this->data;
    }

};


#endif //MCCLONE_NBTTAGBYTE_H
