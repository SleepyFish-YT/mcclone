//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_NBTTAGSTRING_H
#define MCCLONE_NBTTAGSTRING_H

#include "NBTBase.h"
#include "NBTSizeTracker.h"

#include <string>
#include <format>
#include <istream>
#include <ostream>

/**
 * @author SleepyFish
 * @brief NBT tag for a string
 */
class NBTTagString : public NBTBase {

private:

    std::string data;

public:

    NBTTagString() : data("") {}

    explicit NBTTagString(const std::string &data) : data(data) {}

    void write(std::ostream &output) const override {
        uint16_t length = static_cast<uint16_t>(data.length());
        output.write(reinterpret_cast<const char*>(&length), sizeof(length));
        output.write(data.c_str(), data.length());
    }

    void read(std::istream &input, int depth, NBTSizeTracker &sizeTracker) override {
        sizeTracker.read(288ll);

        uint16_t length;
        input.read(reinterpret_cast<char*>(&length), sizeof(length));

        std::string str;
        str.resize(length);
        input.read(&str[0], length);

        this->data = str;
        sizeTracker.read(16ll * this->data.length());
    }

    int8_t getId() const override {
        return 8;
    }

    std::string toString() const override {
        std::string escaped = this->data;
        size_t pos = 0;
        while ((pos = escaped.find('"', pos)) != std::string::npos) {
            escaped.replace(pos, 1, "\\\"");
            pos += 2;
        }
        return std::format("\"{}\"", escaped);
    }

    std::unique_ptr<NBTBase> copy() const override {
        return std::make_unique<NBTTagString>(this->data);
    }

    bool hasNoTags() const override {
        return this->data.empty();
    }

    bool operator==(const NBTBase &other) const override {
        if (!NBTBase::operator==(other)) return false;
        return this->data == static_cast<const NBTTagString &>(other).data;
    }

    int hashCode() const override {
        return NBTBase::hashCode() ^ std::hash<std::string>{}(this->data);
    }

    std::string getString() const override {
        return this->data;
    }

};


#endif //MCCLONE_NBTTAGSTRING_H
