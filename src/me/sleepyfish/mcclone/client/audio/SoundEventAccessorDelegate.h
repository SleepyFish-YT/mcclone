//
// Created by SleepyFish on 16.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_SOUNDEVENTACCESSORDELEGATE_H
#define MCCLONE_SOUNDEVENTACCESSORDELEGATE_H

#include "ISoundEventAccessor.h"
#include "SoundPoolEntry.h"
#include "../../util/ResourceLocation.h"

class SoundRegistry;

/**
 * @author SleepyFish
 * @brief Sound event accessor delegate
 */
class SoundEventAccessorDelegate : public ISoundEventAccessor<SoundPoolEntry> {

private:

    SoundRegistry*   registry;
    ResourceLocation target;
    SoundPoolEntry*  missingSound;

public:

    SoundEventAccessorDelegate(SoundRegistry* registry, const ResourceLocation& target, SoundPoolEntry *missingSound) :
            registry(registry),
            target(target),
            missingSound(missingSound)
    {}

    int getWeight() override {
        auto *composite = this->registry->getObject(this->target);
        return composite == nullptr ? 0 : composite->getWeight();
    }

    SoundPoolEntry cloneEntry() override {
        auto *composite = this->registry->getObject(this->target);
        return composite == nullptr ? *this->missingSound : composite->cloneEntry();
    }

};

#endif //MCCLONE_SOUNDEVENTACCESSORDELEGATE_H
