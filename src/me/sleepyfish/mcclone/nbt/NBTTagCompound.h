//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_NBTTAGCOMPOUND_H
#define MCCLONE_NBTTAGCOMPOUND_H

#include "NBTBase.h"
#include "NBTPrimitive.h"
#include "NBTSizeTracker.h"
#include "NBTTagByte.h"
#include "NBTTagShort.h"
#include "NBTTagInt.h"
#include "NBTTagLong.h"
#include "NBTTagFloat.h"
#include "NBTTagDouble.h"
#include "NBTTagString.h"
#include "NBTTagByteArray.h"
#include "NBTTagIntArray.h"
#include "../debug/Logger.h"

#include <string>
#include <format>
#include <unordered_map>
#include <memory>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <set>

class NBTTagList;

/**
 * @author SleepyFish
 * @brief NBT tag for a compound (key-value map)
 */
class NBTTagCompound : public NBTBase {

private:

    std::unordered_map<std::string, std::unique_ptr<NBTBase>> tagMap;

    static void writeEntry(const std::string& name, const NBTBase& data, std::ostream& output) {
        output.put(data.getId());
        if (data.getId() != 0) {
            const uint16_t len = static_cast<uint16_t>(name.size());
            output.write(reinterpret_cast<const char*>(&len), sizeof(uint16_t));
            output.write(name.data(), name.size());
            data.write(output);
        }
    }

    static int8_t readType(std::istream& input, NBTSizeTracker& sizeTracker) {
        return static_cast<int8_t>(input.get());
    }

    static std::string readKey(std::istream& input, NBTSizeTracker& sizeTracker) {
        uint16_t len = 0;
        input.read(reinterpret_cast<char*>(&len), sizeof(uint16_t));
        std::string key(len, '\0');
        input.read(key.data(), len);
        return key;
    }

    static std::unique_ptr<NBTBase> readNBT(int8_t id, const std::string& key, std::istream& input, int depth, NBTSizeTracker& sizeTracker) {
        auto tag = NBTBase::createNewByType(id);
        try {
            tag->read(input, depth, sizeTracker);
            return tag;
        } catch (const std::exception& e) {
            throw std::runtime_error(std::format("Loading NBT data, tag name: {}, tag type: {}, error: {}", key, id, e.what()));
        }
    }

    std::runtime_error createCrashReport(const std::string& key, int expectedType, const std::exception& ex) const {
        return std::runtime_error(std::format(
                "Reading NBT data - Corrupt NBT tag: tag type found: {}, tag type expected: {}, tag name: {}, error: {}",
                NBT_TYPES[this->tagMap.at(key)->getId()],
                NBT_TYPES[expectedType],
                key,
                ex.what()
        ));
    }

public:

    NBTTagCompound() = default;

    void write(std::ostream& output) const override {
        for (const auto& [key, tag] : this->tagMap) {
            writeEntry(key, *tag, output);
        }
        output.put(0);
    }

    void read(std::istream& input, int depth, NBTSizeTracker& sizeTracker) override {
        sizeTracker.read(384LL);

        if (depth > 512) {
            throw std::runtime_error("Tried to read NBT tag with too high complexity, depth > 512");
        }

        this->tagMap.clear();
        int8_t type;

        while ((type = readType(input, sizeTracker)) != 0) {
            const std::string key = readKey(input, sizeTracker);
            sizeTracker.read(224 + 16LL * key.size());
            auto tag = readNBT(type, key, input, depth + 1, sizeTracker);

            if (this->tagMap.count(key)) {
                sizeTracker.read(288LL);
            }

            this->tagMap[key] = std::move(tag);
        }
    }

    int8_t getId() const override {
        return 10;
    }

    std::set<std::string> getKeySet() const {
        std::set<std::string> keys;
        for (const auto& [key, _] : this->tagMap) {
            keys.insert(key);
        }
        return keys;
    }

    void setTag(const std::string& key, std::unique_ptr<NBTBase> value) {
        this->tagMap[key] = std::move(value);
    }

    void setByte(const std::string& key, int8_t value) {
        this->tagMap[key] = std::make_unique<NBTTagByte>(value);
    }

    void setShort(const std::string& key, int16_t value) {
        this->tagMap[key] = std::make_unique<NBTTagShort>(value);
    }

    void setInteger(const std::string& key, int32_t value) {
        this->tagMap[key] = std::make_unique<NBTTagInt>(value);
    }

    void setLong(const std::string& key, int64_t value) {
        this->tagMap[key] = std::make_unique<NBTTagLong>(value);
    }

    void setFloat(const std::string& key, float value) {
        this->tagMap[key] = std::make_unique<NBTTagFloat>(value);
    }

    void setDouble(const std::string& key, double value) {
        this->tagMap[key] = std::make_unique<NBTTagDouble>(value);
    }

    void setString(const std::string& key, const std::string& value) {
        this->tagMap[key] = std::make_unique<NBTTagString>(value);
    }

    void setByteArray(const std::string& key, const std::vector<int8_t>& value) {
        this->tagMap[key] = std::make_unique<NBTTagByteArray>(value);
    }

    void setIntArray(const std::string& key, const std::vector<int32_t>& value) {
        this->tagMap[key] = std::make_unique<NBTTagIntArray>(value);
    }

    void setBoolean(const std::string& key, bool value) {
        setByte(key, value ? 1 : 0);
    }

    const NBTBase* getTag(const std::string& key) const {
        auto it = this->tagMap.find(key);
        return it != this->tagMap.end() ? it->second.get() : nullptr;
    }

    int8_t getTagId(const std::string& key) const {
        auto it = this->tagMap.find(key);
        return it != this->tagMap.end() ? it->second->getId() : 0;
    }

    bool hasKey(const std::string& key) const {
        return this->tagMap.count(key) > 0;
    }

    bool hasKey(const std::string& key, int type) const {
        const int8_t i = getTagId(key);
        if (i == type) return true;
        if (type == 99) return i == 1 || i == 2 || i == 3 || i == 4 || i == 5 || i == 6;
        return false;
    }

    int8_t getByte(const std::string& key) const {
        try {
            if (!hasKey(key, 99)) return 0;
            return dynamic_cast<const NBTPrimitive&>(*this->tagMap.at(key)).getByte();
        } catch (...) { return 0; }
    }

    int16_t getShort(const std::string& key) const {
        try {
            if (!hasKey(key, 99)) return 0;
            return dynamic_cast<const NBTPrimitive&>(*this->tagMap.at(key)).getShort();
        } catch (...) { return 0; }
    }

    int32_t getInteger(const std::string& key) const {
        try {
            if (!hasKey(key, 99)) return 0;
            return dynamic_cast<const NBTPrimitive&>(*this->tagMap.at(key)).getInt();
        } catch (...) { return 0; }
    }

    int64_t getLong(const std::string& key) const {
        try {
            if (!hasKey(key, 99)) return 0LL;
            return dynamic_cast<const NBTPrimitive&>(*this->tagMap.at(key)).getLong();
        } catch (...) { return 0LL; }
    }

    float getFloat(const std::string& key) const {
        try {
            if (!hasKey(key, 99)) return 0.0f;
            return dynamic_cast<const NBTPrimitive&>(*this->tagMap.at(key)).getFloat();
        } catch (...) { return 0.0f; }
    }

    double getDouble(const std::string& key) const {
        try {
            if (!hasKey(key, 99)) return 0.0;
            return dynamic_cast<const NBTPrimitive&>(*this->tagMap.at(key)).getDouble();
        } catch (...) { return 0.0; }
    }

    std::string getString(const std::string& key) const {
        try {
            if (!hasKey(key, 8)) return "";
            return this->tagMap.at(key)->getString();
        } catch (...) { return ""; }
    }

    const std::vector<int8_t>& getByteArray(const std::string& key) const {
        try {
            if (!hasKey(key, 7)) { static const std::vector<int8_t> empty; return empty; }
            return dynamic_cast<const NBTTagByteArray&>(*this->tagMap.at(key)).getByteArray();
        } catch (const std::exception& e) {
            throw createCrashReport(key, 7, e);
        }
    }

    const std::vector<int32_t>& getIntArray(const std::string& key) const; // idk ??

    NBTTagCompound getCompoundTag(const std::string& key) const; // idk ??

    NBTTagList getTagList(const std::string& key, int type) const; // idk ??

    bool getBoolean(const std::string& key) const {
        return getByte(key) != 0;
    }

    void removeTag(const std::string& key) {
        this->tagMap.erase(key);
    }

    std::string toString() const override {
        std::string result = "{";

        for (const auto& [key, tag] : this->tagMap) {
            if (result.size() != 1) result += ',';
            result += key + ':' + tag->toString();
        }

        return result + "}";
    }

    bool hasNoTags() const override {
        return this->tagMap.empty();
    }

    std::unique_ptr<NBTBase> copy() const override {
        auto compound = std::make_unique<NBTTagCompound>();
        for (const auto& [key, tag] : this->tagMap) {
            compound->tagMap[key] = tag->copy();
        }
        return compound;
    }

    bool operator==(const NBTBase& other) const override {
        if (!NBTBase::operator==(other)) return false;
        const auto& o = static_cast<const NBTTagCompound&>(other);
        if (this->tagMap.size() != o.tagMap.size()) return false;

        for (const auto& [key, tag] : this->tagMap) {
            auto it = o.tagMap.find(key);
            if (it == o.tagMap.end()) return false;
            if (!(*tag == *it->second)) return false;
        }

        return true;
    }

    int hashCode() const override {
        int hash = NBTBase::hashCode();
        for (const auto& [key, tag] : this->tagMap) {
            hash ^= std::hash<std::string>{}(key) ^ tag->hashCode();
        }
        return hash;
    }

    void merge(const NBTTagCompound& other); // idk ??

};


#endif //MCCLONE_NBTTAGCOMPOUND_H
