//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_SOUNDEVENTACCESSORCOMPOSITE_H
#define MCCLONE_SOUNDEVENTACCESSORCOMPOSITE_H

#include "ISoundEventAccessor.h"
#include "SoundPoolEntry.h"
#include "SoundCategory.h"
#include "ResourceLocation.h"
#include "SoundHandler.h"

#include <vector>
#include <random>

/**
 * @author SleepyFish
 */
class SoundEventAccessorComposite : public ISoundEventAccessor<SoundPoolEntry> {

private:

    std::vector<ISoundEventAccessor<SoundPoolEntry>*> soundPool;
    std::mt19937 rnd;
    ResourceLocation soundLocation;
    SoundCategory category;
    double eventPitch;
    double eventVolume;

public:

    SoundEventAccessorComposite(const ResourceLocation& soundLocation, double pitch, double volume, SoundCategory category) :
        soundLocation(soundLocation),
        eventVolume(volume),
        eventPitch(pitch),
        category(category),
        rnd(std::random_device{}())
    {}

    int getWeight() override {
        int total = 0;
        for (const auto& accessor : this->soundPool)
            total += accessor->getWeight();
        return total;
    }

    SoundPoolEntry cloneEntry() override {
        const int total = getWeight();

        if (!this->soundPool.empty() && total != 0) {
            std::uniform_int_distribution<int> dist(0, total - 1);
            int j = dist(this->rnd);

            for (const auto& accessor : this->soundPool) {
                j -= accessor->getWeight();

                if (j < 0) {
                    SoundPoolEntry entry = accessor->cloneEntry();
                    entry.setPitch(entry.getPitch() * this->eventPitch);
                    entry.setVolume(entry.getVolume() * this->eventVolume);
                    return entry;
                }
            }
        }

        return SoundHandler::missing_sound;
    }

    void addSoundToEventPool(ISoundEventAccessor<SoundPoolEntry>* sound) {
        this->soundPool.push_back(sound);
    }

    const ResourceLocation& getSoundEventLocation() const {
        return this->soundLocation;
    }

    SoundCategory getSoundCategory() const {
        return this->category;
    }

};


#endif //MCCLONE_SOUNDEVENTACCESSORCOMPOSITE_H
