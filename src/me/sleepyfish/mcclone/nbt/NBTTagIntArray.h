//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_NBTTAGINTARRAY_H
#define MCCLONE_NBTTAGINTARRAY_H

#include "NBTBase.h"
#include "NBTSizeTracker.h"

#include <string>
#include <vector>
#include <istream>
#include <ostream>
#include <cstring>

/**
 * @author SleepyFish
 * @brief NBT tag for a int array
 */
class NBTTagIntArray : public NBTBase {

private:

    std::vector<int32_t> intArray;

public:

    NBTTagIntArray() = default;

    explicit NBTTagIntArray(const std::vector<int32_t> &intArray) : intArray(intArray) {}

    explicit NBTTagIntArray(const int32_t *array, size_t size) : intArray(array, array + size) {}

    void write(std::ostream &output) const override {
        int32_t length = static_cast<int32_t>(this->intArray.size());
        output.write(reinterpret_cast<const char*>(&length), sizeof(length));
        output.write(reinterpret_cast<const char*>(this->intArray.data()), length * sizeof(int32_t));
    }

    void read(std::istream &input, int depth, NBTSizeTracker &sizeTracker) override {
        sizeTracker.read(192ll);

        int32_t length;
        input.read(reinterpret_cast<char*>(&length), sizeof(length));
        sizeTracker.read(32ll * length);

        this->intArray.resize(length);
        input.read(reinterpret_cast<char*>(this->intArray.data()), length * sizeof(int32_t));
    }

    int8_t getId() const override {
        return 11;
    }

    std::string toString() const override {
        std::string s = "[";
        for (size_t i = 0; i < this->intArray.size(); ++i) {
            s += std::to_string(this->intArray[i]);
            if (i < this->intArray.size() - 1) {
                s += ",";
            }
        }
        return s + "]";
    }

    std::unique_ptr<NBTBase> copy() const override {
        return std::make_unique<NBTTagIntArray>(this->intArray);
    }

    bool hasNoTags() const override {
        return this->intArray.empty();
    }

    bool operator==(const NBTBase &other) const override {
        if (!NBTBase::operator==(other)) return false;
        return this->intArray == static_cast<const NBTTagIntArray &>(other).intArray;
    }

    int hashCode() const override {
        int hash = NBTBase::hashCode();
        for (int32_t i : this->intArray) {
            hash ^= i;
        }
        return hash;
    }

    const std::vector<int32_t>& getIntArray() const {
        return this->intArray;
    }

};


#endif //MCCLONE_NBTTAGINTARRAY_H
