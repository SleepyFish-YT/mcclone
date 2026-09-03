//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#ifndef MCCLONE_MINECRAFT_H
#define MCCLONE_MINECRAFT_H

#include "../util/Runnable.h"

#include "main/GameConfiguration.h"

#include <atomic>
#include <thread>
#include <chrono>
#include <filesystem>

class Profiler;
class GameSettings;
class SoundEngine;

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

    Profiler *mcProfiler;

    GameSettings *gameSettings;

    SoundEngine *soundEngine;

    explicit Minecraft(const GameConfiguration& gameConfig);

    static long long getSystemTime();

    static long long getHighResTime();

    bool isGamePaused() const;

    bool isFramerateLimitBelowMax() const;

    uint16_t getLimitFramerate() const;

    void handleKeypress(int key, int scancode, int action, int mods);

    void handleMouseButton(int button, int action, int mods);

    void handleMouseScroll(double xOffset, double yOffset);

    void handleMouseMove(double x, double y);

    void onFullscreenChange(bool fullscreen);

};


#endif //MCCLONE_MINECRAFT_H
