//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ISOUNDEVENTACCESSOR_H
#define MCCLONE_ISOUNDEVENTACCESSOR_H

/**
 * @author SleepyFish
 * @brief Sound event accessor interface
 */
template<typename T>
class ISoundEventAccessor {

public:

    virtual ~ISoundEventAccessor() = default;

    virtual int getWeight() = 0;

    virtual T cloneEntry() = 0;

};


#endif //MCCLONE_ISOUNDEVENTACCESSOR_H
