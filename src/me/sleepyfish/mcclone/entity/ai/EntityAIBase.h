//
// Created by SleepyFish on 14.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ENTITYAIBASE_H
#define MCCLONE_ENTITYAIBASE_H

#include <cstdint>

/**
 * @author SleepyFish
 * @brief Base class for entity AI
 */
class EntityAIBase {

private:

    uint8_t mutexBits;

public:

    virtual ~EntityAIBase() = default;

    virtual bool shouldExecute() = 0;

    virtual bool continueExecuting() { return this->shouldExecute(); }

    virtual bool isInterruptible() { return true; }

    virtual void startExecuting() {}

    virtual void resetTask() {}

    virtual void updateTask() {}

    void setMutexBits(uint8_t bits) {
        this->mutexBits = bits;
    }

    uint8_t getMutexBits() {
        return this->mutexBits;
    }

};


#endif //MCCLONE_ENTITYAIBASE_H
