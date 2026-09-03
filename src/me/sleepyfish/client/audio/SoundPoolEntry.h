//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_SOUNDPOOLENTRY_H
#define MCCLONE_SOUNDPOOLENTRY_H

#include "../../util/ResourceLocation.h"

/**
 * @author SleepyFish
 * @brief Sound pool entry
 */
class SoundPoolEntry {

private:

    double pitch;
    double volume;

public:

    const ResourceLocation location;

    const bool streamingSound;

    SoundPoolEntry(const ResourceLocation& location, const double pitch, const double volume, const bool streamingSound) noexcept :
        location(location),
        pitch(pitch),
        volume(volume),
        streamingSound(streamingSound)
    {}

    SoundPoolEntry(const SoundPoolEntry& other) noexcept :
        location(other.location),
        pitch(other.pitch),
        volume(other.volume),
        streamingSound(other.streamingSound)
    {}

    double getPitch() const noexcept {
        return this->pitch;
    }

    void setPitch(const double pitch_) noexcept {
        this->pitch = pitch_;
    }

    double getVolume() const noexcept {
        return this->volume;
    }

    void setVolume(const double volume_) noexcept {
        this->volume = volume_;
    }

};


#endif //MCCLONE_SOUNDPOOLENTRY_H
