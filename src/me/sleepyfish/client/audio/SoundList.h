//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_SOUNDLIST_H
#define MCCLONE_SOUNDLIST_H

#include "SoundCategory.h"

#include <vector>
#include <string>

/**
 * @author SleepyFish
 */
class SoundList {

    class SoundEntry {

    public:

        enum class Type {
            FILE = 0,
            SOUND_EVENT
        };

        static const char* getTypeName(SoundEntry::Type type) {
            switch (type) {
                case SoundEntry::Type::FILE: return "file";
                case SoundEntry::Type::SOUND_EVENT: return "event";
                default: return nullptr;
            }
        }

        static SoundEntry::Type* getType(const std::string& name) {
            static SoundEntry::Type FILE_TYPE = Type::FILE;
            static SoundEntry::Type SOUND_EVENT_TYPE = Type::SOUND_EVENT;

            if (name == "file") return &FILE_TYPE;
            if (name == "event") return &SOUND_EVENT_TYPE;
            return nullptr;
        }

    private:

        std::string name;

        float volume = 1.0f;

        float pitch = 1.0f;

        int weight = 1;

        SoundEntry::Type type = SoundEntry::Type::FILE;

        bool streaming = false;

    public:

        std::string getSoundEntryName() const {
            return this->name;
        }

        void setSoundEntryName(const std::string& n) {
            this->name = n;
        }

        float getSoundEntryVolume() const {
            return this->volume;
        }

        void setSoundEntryVolume(float v) {
            this->volume = v;
        }

        float getSoundEntryPitch() const {
            return this->pitch;
        }

        void setSoundEntryPitch(float p) {
            this->pitch = p;
        }

        int getSoundEntryWeight() const {
            return this->weight;
        }

        void setSoundEntryWeight(int w) {
            this->weight = w;
        }

        SoundEntry::Type getSoundEntryType() const {
            return this->type;
        }

        void setSoundEntryType(SoundEntry::Type t) {
            this->type = t;
        }

        bool isStreaming() const {
            return this->streaming;
        }

        void setStreaming(bool s) {
            this->streaming = s;
        }

    };

private:

    std::vector<SoundList::SoundEntry> soundList;

    bool replaceExisting = false;

    SoundCategory category = SoundCategory::MASTER;

public:

    std::vector<SoundList::SoundEntry>& getSoundList() {
        return this->soundList;
    }

    const std::vector<SoundList::SoundEntry>& getSoundList() const {
        return this->soundList;
    }

    bool canReplaceExisting() const {
        return this->replaceExisting;
    }

    void setReplaceExisting(bool replace) {
        this->replaceExisting = replace;
    }

    SoundCategory getSoundCategory() const {
        return this->category;
    }

    void setSoundCategory(SoundCategory cat) {
        this->category = cat;
    }

};


#endif //MCCLONE_SOUNDLIST_H
