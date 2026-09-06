//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ITICKABLE_H
#define MCCLONE_ITICKABLE_H

/**
 * @author SleepyFish
 */
class ITickable {

public:

    virtual ~ITickable() = default;

    virtual void update() = 0;

};


#endif //MCCLONE_ITICKABLE_H
