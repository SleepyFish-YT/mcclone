//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_POSITIONEDSOUNDRECORD_H
#define MCCLONE_POSITIONEDSOUNDRECORD_H

#include "PositionedSound.h"

/**
 * @author SleepyFish
 */
class PositionedSoundRecord : public PositionedSound {

private:

    PositionedSoundRecord(const ResourceLocation &soundResource, float volume, float pitch, bool repeat, int repeatDelay, ISound::AttenuationType attenuationType, float xPosition, float yPosition, float zPosition) :
        PositionedSound(soundResource)
    {
        this->volume = volume;
        this->pitch = pitch;
        this->xPosF = xPosition;
        this->yPosF = yPosition;
        this->zPosF = zPosition;
        this->repeat = repeat;
        this->repeatDelay = repeatDelay;
        this->attenuationType = attenuationType;
    }

public:

    static PositionedSoundRecord create(const ResourceLocation &soundResource, float pitch) {
        return PositionedSoundRecord(soundResource, 0.25F, pitch, false, 0, ISound::AttenuationType::NONE, 0.0F, 0.0F, 0.0F);
    }

    static PositionedSoundRecord create(const ResourceLocation &soundResource) {
        return PositionedSoundRecord(soundResource, 1.0F, 1.0F, false, 0, ISound::AttenuationType::NONE, 0.0F, 0.0F, 0.0F);
    }

    static PositionedSoundRecord create(const ResourceLocation &soundResource, float xPosition, float yPosition, float zPosition) {
        return PositionedSoundRecord(soundResource, 4.0F, 1.0F, false, 0, ISound::AttenuationType::LINEAR, xPosition, yPosition, zPosition);
    }

    PositionedSoundRecord(const ResourceLocation &soundResource, float volume, float pitch, float xPosition, float yPosition, float zPosition) :
        PositionedSoundRecord(soundResource, volume, pitch, false, 0, ISound::AttenuationType::LINEAR, xPosition, yPosition, zPosition)
    {}

};


#endif //MCCLONE_POSITIONEDSOUNDRECORD_H
