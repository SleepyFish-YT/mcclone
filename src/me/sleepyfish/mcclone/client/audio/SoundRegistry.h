//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_SOUNDREGISTRY_H
#define MCCLONE_SOUNDREGISTRY_H

#include "../../util/RegistrySimple.h"
#include "../../util/ResourceLocation.h"
#include "SoundEventAccessorComposite.h"

/**
 * @author SleepyFish
 */
class SoundRegistry : public RegistrySimple<ResourceLocation, SoundEventAccessorComposite, ResourceLocation::Hash> {

public:

    void registerSound(const SoundEventAccessorComposite &sound) {
        this->putObject(sound.getSoundEventLocation(), sound);
    }

    void clearMap() {
        this->registryObjects.clear();
    }

};


#endif //MCCLONE_SOUNDREGISTRY_H
