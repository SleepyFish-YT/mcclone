//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_SOUNDEVENTACCESSOR_H
#define MCCLONE_SOUNDEVENTACCESSOR_H

#include "ISoundEventAccessor.h"
#include "SoundPoolEntry.h"

/**
 * @author SleepyFish
 * @brief Sound event accessor
 */
class SoundEventAccessor : public ISoundEventAccessor<SoundPoolEntry> {

private:

    const SoundPoolEntry entry;
    const int weight;

public:

    SoundEventAccessor(const SoundPoolEntry &entry, const int weight) :
        entry(entry),
        weight(weight)
    {}

    int getWeight() override {
        return this->weight;
    }

    SoundPoolEntry cloneEntry() override {
        return SoundPoolEntry(this->entry);
    }

};


#endif //MCCLONE_SOUNDEVENTACCESSOR_H
