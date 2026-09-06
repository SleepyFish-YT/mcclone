//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_NBTTAGBYTEARRAY_H
#define MCCLONE_NBTTAGBYTEARRAY_H

#include "NBTBase.h"
#include "NBTSizeTracker.h"

#include <string>
#include <format>
#include <vector>
#include <istream>
#include <ostream>
#include <numeric>

/**
 * @author SleepyFish
 * @brief NBT tag for a byte array
 */
class NBTTagByteArray : public NBTBase {

private:

    std::vector<int8_t> data;

public:

    NBTTagByteArray() = default;

    explicit NBTTagByteArray(const std::vector<int8_t>& data) :
            data(data)
    {}

    void write(std::ostream& output) const override {
        const int32_t length = static_cast<int32_t>(this->data.size());
        output.write(reinterpret_cast<const char*>(&length), sizeof(int32_t));
        output.write(reinterpret_cast<const char*>(this->data.data()), this->data.size());
    }

    void read(std::istream& input, int depth, NBTSizeTracker& sizeTracker) override {
        sizeTracker.read(192ll);

        int32_t length = 0;
        input.read(reinterpret_cast<char*>(&length), sizeof(int32_t));

        sizeTracker.read(8ll * length);

        this->data.resize(length);
        input.read(reinterpret_cast<char*>(this->data.data()), length);
    }

    int8_t getId() const override {
        return 7;
    }

    std::string toString() const override {
        return std::format("[{} bytes]", this->data.size());
    }

    std::unique_ptr<NBTBase> copy() const override {
        return std::make_unique<NBTTagByteArray>(this->data);
    }

    bool operator==(const NBTBase& other) const override {
        if (!NBTBase::operator==(other)) return false;
        return this->data == static_cast<const NBTTagByteArray&>(other).data;
    }

    int hashCode() const override {
        return static_cast<int>(std::accumulate(
                this->data.begin(), this->data.end(), NBTBase::hashCode(),
                [](int acc, int8_t b) { return acc ^ b; }
        ));
    }

    const std::vector<int8_t>& getByteArray() const {
        return this->data;
    }

};


#endif //MCCLONE_NBTTAGBYTEARRAY_H
