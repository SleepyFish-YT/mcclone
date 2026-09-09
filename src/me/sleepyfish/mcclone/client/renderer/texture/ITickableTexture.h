//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ITICKABLE_H
#define MCCLONE_ITICKABLE_H

/**
 * @author SleepyFish
 * @brief Tickable interface
 */
class ITickableTexture {

public:

    virtual ~ITickableTexture() = default;

    virtual void tick() = 0;

};

#endif //MCCLONE_ITICKABLE_H
