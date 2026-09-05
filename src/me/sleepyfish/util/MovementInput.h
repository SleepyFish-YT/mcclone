//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_MOVEMENTINPUT_H
#define MCCLONE_MOVEMENTINPUT_H

/**
 * @author SleepyFish
 */
struct MovementInput {

    float moveStrafe{};
    float moveForward{};

    bool jump{};
    bool sneak{};

    virtual void updatePlayerMoveState() = 0;

};


#endif //MCCLONE_MOVEMENTINPUT_H
