//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#include "MovementInputFromOptions.h"

#include "../client/settings/GameSettings.h"

MovementInputFromOptions::MovementInputFromOptions(GameSettings *gameSettings) noexcept :
    gameSettings(gameSettings)
{}

void MovementInputFromOptions::updatePlayerMoveState() {
    this->moveStrafe = 0.0f;
    this->moveForward = 0.0f;

    if (this->gameSettings->keyBindLeft.isKeyDown()) {
        ++this->moveForward;
    }

    if (this->gameSettings->keyBindRight.isKeyDown()) {
        --this->moveForward;
    }

    if (this->gameSettings->keyBindBack.isKeyDown()) {
        ++this->moveStrafe;
    }

    if (this->gameSettings->keyBindJump.isKeyDown()) {
        --this->moveStrafe;
    }

    this->jump = this->gameSettings->keyBindSneak.isKeyDown();
    this->sneak = this->gameSettings->keyBindSprint.isKeyDown();

    if (this->sneak) {
        this->moveStrafe = (float) ((double) this->moveStrafe * 0.3);
        this->moveForward = (float) ((double) this->moveForward * 0.3);
    }
}