//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_MOVINGSOUND_H
#define MCCLONE_MOVINGSOUND_H

#include "PositionedSound.h"
#include "ITickableSound.h"

/**
 * @author SleepyFish
 */
class MovingSound : public PositionedSound, public ITickableSound {

protected:

    bool donePlaying = false;

    explicit MovingSound(const ResourceLocation &location)
            : PositionedSound(location)
    {}

public:

    bool isDonePlaying() override {
        return this->donePlaying;
    }

};


#endif //MCCLONE_MOVINGSOUND_H
