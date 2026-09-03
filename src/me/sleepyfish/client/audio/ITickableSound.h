//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ITICKABLESOUND_H
#define MCCLONE_ITICKABLESOUND_H

#include "ISound.h"
#include "../../util/ITickable.h"

/**
 * @author SleepyFish
 * @brief Tickable sound interface
 */
class ITickableSound : public ISound, public ITickable {

public:

    virtual ~ITickableSound() override = default;

    virtual bool isDonePlaying() = 0;

};


#endif //MCCLONE_ITICKABLESOUND_H
