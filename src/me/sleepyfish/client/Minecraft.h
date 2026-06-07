//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#ifndef MCCLONE_MINECRAFT_H
#define MCCLONE_MINECRAFT_H

#include "../util/Runnable.h"
#include "../profiler/Profiler.h"
#include "settings/GameSettings.h"
#include "main/GameConfiguration.h"
#include "audio/SoundEngine.h"

#include <atomic>
#include <thread>
#include <chrono>

/**
 * @author SleepyFish
 * @brief Minecraft main class
 */
class Minecraft : public Runnable {

protected:

    void run() override;

private:

    uint8_t leftClickCounter;

    uint8_t rightClickDelayTimer;

    bool gamePaused;

    void runGameLoop();

    uint16_t tpsCounter;

    uint16_t tickCounter;

    std::chrono::steady_clock::time_point prevFrameTime;

public:

    std::filesystem::path mcDataDir;

    Profiler mcProfiler;

    GameSettings* gameSettings;

    SoundEngine soundEngine;

    Minecraft(const GameConfiguration& gameConfig);

    bool isGamePaused() const;

    bool isFramerateLimitBelowMax() const;

    uint16_t getLimitFramerate() const;

};


#endif //MCCLONE_MINECRAFT_H
