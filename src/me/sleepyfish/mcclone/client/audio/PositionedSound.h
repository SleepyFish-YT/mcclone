//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_POSITIONEDSOUND_H
#define MCCLONE_POSITIONEDSOUND_H

#include <utility>

#include "ISound.h"
#include "../../util/ResourceLocation.h"

/**
 * @author SleepyFish
 */
class PositionedSound : public ISound {

protected:

    const ResourceLocation positionedSoundLocation;

    float volume = 1.0f;
    float pitch = 1.0f;
    float xPosF = 0.0f;
    float yPosF = 0.0f;
    float zPosF = 0.0f;
    bool repeat = false;

    int repeatDelay = 0;
    ISound::AttenuationType attenuationType = ISound::AttenuationType::LINEAR;

    explicit PositionedSound(ResourceLocation soundResource) :
        positionedSoundLocation(std::move(soundResource))
    {}

public:

    ResourceLocation getResourceLocation() override {
        return this->positionedSoundLocation;
    }

    bool canRepeat() const override {
        return this->repeat;
    }

    int getRepeatDelay() const override {
        return this->repeatDelay;
    }

    float getVolume() const override {
        return this->volume;
    }

    float getPitch() const override {
        return this->pitch;
    }

    float getXPosF() const override {
        return this->xPosF;
    }

    float getYPosF() const override {
        return this->yPosF;
    }

    float getZPosF() const override {
        return this->zPosF;
    }

    ISound::AttenuationType getAttenuationType() const override {
        return this->attenuationType;
    }

};


#endif //MCCLONE_POSITIONEDSOUND_H
