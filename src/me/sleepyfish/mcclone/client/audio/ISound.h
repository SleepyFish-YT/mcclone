//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ISOUND_H
#define MCCLONE_ISOUND_H

#include "../../util/ResourceLocation.h"

/**
 * @author SleepyFish
 * @brief Sound interface
 */
class ISound {

public:

    enum class AttenuationType {

        NONE = 0,
        LINEAR = 2 // why is minecraft using 2 instead of 1 here?

    };

    virtual ~ISound() = default;

    virtual ResourceLocation getResourceLocation() = 0;

    virtual bool canRepeat() const = 0;

    virtual int getRepeatDelay() const = 0;

    virtual float getVolume() const = 0;

    virtual float getPitch() const = 0;

    virtual float getXPosF() const = 0;

    virtual float getYPosF() const = 0;

    virtual float getZPosF() const = 0;

    virtual ISound::AttenuationType getAttenuationType() const = 0;

};


#endif //MCCLONE_ISOUND_H
