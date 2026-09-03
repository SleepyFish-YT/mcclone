//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_MOVEMENTINPUTFROMOPTIONS_H
#define MCCLONE_MOVEMENTINPUTFROMOPTIONS_H

#include "MovementInput.h"

class GameSettings;

/**
 * @author SleepyFish
 * @brief This class is used to handle movement input from options
 */
class MovementInputFromOptions : public MovementInput {

private:

    const GameSettings *gameSettings;

public:

    MovementInputFromOptions(GameSettings *gameSettings) noexcept;

    void updatePlayerMoveState() override;

};


#endif //MCCLONE_MOVEMENTINPUTFROMOPTIONS_H
