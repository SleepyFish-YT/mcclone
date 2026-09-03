//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_NBTTAGLIST_H
#define MCCLONE_NBTTAGLIST_H

#include "NBTBase.h"
#include "NBTSizeTracker.h"
#include "NBTTagEnd.h"
#include "NBTTagCompound.h"
#include "NBTTagIntArray.h"
#include "NBTTagDouble.h"
#include "NBTTagFloat.h"
#include "../debug/Logger.h"

#include <string>
#include <format>
#include <vector>
#include <memory>
#include <istream>
#include <ostream>
#include <stdexcept>

/**
 * @author SleepyFish
 * @brief NBT tag for a list of tags
 */
class NBTTagList : public NBTBase {

private:

    std::vector<std::unique_ptr<NBTBase>> tagList;
    int8_t tagType = 0;

public:

    NBTTagList() = default;

    void write(std::ostream& output) const override {
        const int8_t type = !this->tagList.empty() ? this->tagList[0]->getId() : 0;
        output.put(type);

        const int32_t size = static_cast<int32_t>(this->tagList.size());
        output.write(reinterpret_cast<const char*>(&size), sizeof(int32_t));

        for (const auto& tag : this->tagList) {
            tag->write(output);
        }
    }

    void read(std::istream& input, int depth, NBTSizeTracker& sizeTracker) override {
        sizeTracker.read(296LL);

        if (depth > 512) {
            throw std::runtime_error("Tried to read NBT tag with too high complexity, depth > 512");
        }

        this->tagType = static_cast<int8_t>(input.get());

        int32_t size = 0;
        input.read(reinterpret_cast<char*>(&size), sizeof(int32_t));

        if (this->tagType == 0 && size > 0) {
            throw std::runtime_error("Missing type on ListTag");
        }

        sizeTracker.read(32LL * static_cast<int64_t>(size));
        this->tagList.clear();
        this->tagList.reserve(size);

        for (int j = 0; j < size; ++j) {
            auto tag = NBTBase::createNewByType(this->tagType);
            tag->read(input, depth + 1, sizeTracker);
            this->tagList.push_back(std::move(tag));
        }
    }

    int8_t getId() const override {
        return 9;
    }

    std::string toString() const override {
        std::string result = "[";

        for (int i = 0; i < static_cast<int>(this->tagList.size()); ++i) {
            if (i != 0) result += ',';
            result += std::format("{}:{}", i, this->tagList[i]->toString());
        }

        return result + "]";
    }

    void appendTag(std::unique_ptr<NBTBase> nbt) {
        if (nbt->getId() == 0) {
            Logger::warn("Invalid TagEnd added to ListTag");
            return;
        }

        if (this->tagType == 0) {
            this->tagType = nbt->getId();
        } else if (this->tagType != nbt->getId()) {
            Logger::warn("Adding mismatching tag types to tag list");
            return;
        }

        this->tagList.push_back(std::move(nbt));
    }

    void set(int idx, std::unique_ptr<NBTBase> nbt) {
        if (nbt->getId() == 0) {
            Logger::warn("Invalid TagEnd added to ListTag");
            return;
        }

        if (idx < 0 || idx >= static_cast<int>(this->tagList.size())) {
            Logger::warn("index out of bounds to set tag in tag list");
            return;
        }

        if (this->tagType == 0) {
            this->tagType = nbt->getId();
        } else if (this->tagType != nbt->getId()) {
            Logger::warn("Adding mismatching tag types to tag list");
            return;
        }

        this->tagList[idx] = std::move(nbt);
    }

    std::unique_ptr<NBTBase> removeTag(int i) {
        auto tag = std::move(this->tagList[i]);
        this->tagList.erase(this->tagList.begin() + i);
        return tag;
    }

    bool hasNoTags() const override {
        return this->tagList.empty();
    }

    NBTTagCompound getCompoundTagAt(int i) const;

    std::vector<int32_t> getIntArrayAt(int i) const;

    double getDoubleAt(int i) const {
        if (i >= 0 && i < static_cast<int>(this->tagList.size())) {
            if (this->tagList[i]->getId() == 6)
                return static_cast<const NBTTagDouble&>(*this->tagList[i]).getDouble();
        }
        return 0.0;
    }

    float getFloatAt(int i) const {
        if (i >= 0 && i < static_cast<int>(this->tagList.size())) {
            if (this->tagList[i]->getId() == 5)
                return static_cast<const NBTTagFloat&>(*this->tagList[i]).getFloat();
        }
        return 0.0f;
    }

    std::string getStringTagAt(int i) const {
        if (i >= 0 && i < static_cast<int>(this->tagList.size())) {
            return this->tagList[i]->getId() == 8 ? this->tagList[i]->getString() : this->tagList[i]->toString();
        }
        return "";
    }

    const NBTBase& get(int idx) const {
        if (idx >= 0 && idx < static_cast<int>(this->tagList.size()))
            return *this->tagList[idx];
        static const NBTTagEnd empty;
        return empty;
    }

    int tagCount() const {
        return static_cast<int>(this->tagList.size());
    }

    std::unique_ptr<NBTBase> copy() const override {
        auto copy = std::make_unique<NBTTagList>();
        copy->tagType = this->tagType;

        for (const auto& tag : this->tagList) {
            copy->tagList.push_back(tag->copy());
        }

        return copy;
    }

    bool operator==(const NBTBase& other) const override {
        if (!NBTBase::operator==(other)) return false;
        const auto& o = static_cast<const NBTTagList&>(other);
        if (this->tagType != o.tagType) return false;
        if (this->tagList.size() != o.tagList.size()) return false;

        for (size_t i = 0; i < this->tagList.size(); ++i) {
            if (!(*this->tagList[i] == *o.tagList[i])) return false;
        }

        return true;
    }

    int hashCode() const override {
        int hash = NBTBase::hashCode();
        for (const auto& tag : this->tagList) {
            hash ^= tag->hashCode();
        }
        return hash;
    }

    int getTagType() const {
        return this->tagType;
    }

};

#endif //MCCLONE_NBTTAGLIST_H