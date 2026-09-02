//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#include "Minecraft.h"
#include "../debug/Logger.h"

Minecraft::Minecraft(const GameConfiguration& gameConfig) : Runnable(), soundEngine(gameConfig.folderInformation.mcDataDir / "sounds") {
    this->mcDataDir = gameConfig.folderInformation.mcDataDir;

    this->leftClickCounter = 0;
    this->rightClickDelayTimer = 0;
    this->tickCounter = 0;
    this->tpsCounter = 0;
    this->prevFrameTime = std::chrono::steady_clock::now();
    this->gamePaused = false;

    this->mcProfiler = Profiler();
    this->mcProfiler.profilingEnabled = true;

    this->gameSettings = new GameSettings(this->mcDataDir);
}

void Minecraft::run() {
    Logger::log("Minecraft update thread started");

    ::timeBeginPeriod(1);
    {
        const auto TICK_DURATION = std::chrono::milliseconds(50);

        while (this->running) {
            auto tickStart = std::chrono::steady_clock::now();

            this->runGameLoop();

            auto elapsed = std::chrono::steady_clock::now() - tickStart;
            if (elapsed < TICK_DURATION) {
                std::this_thread::sleep_for(TICK_DURATION - elapsed - std::chrono::milliseconds(1));
                while (std::chrono::steady_clock::now() - tickStart < TICK_DURATION) {}
            }
        }
    }
    ::timeEndPeriod(1);

    Logger::log("Minecraft update thread stopped");
}

void Minecraft::runGameLoop() {
    ++this->tickCounter;

    auto now = std::chrono::steady_clock::now();

    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - this->prevFrameTime).count() >= 1000) {
        this->prevFrameTime = now;
        this->tpsCounter = this->tickCounter;
        this->tickCounter = 0;

        //Logger::trace("TPS: " + std::to_string(this->tpsCounter));
    }

    if (this->gameSettings->keyBindAttack.isPressed()) {
        //this->soundEngine.playSound("sigma", 0.8f, 0.7f);
    }

    this->mcProfiler.startSection("soundEngine");
    {
        this->soundEngine.cleanup();
    }
    this->mcProfiler.endSection();
}

bool Minecraft::isGamePaused() const {
    return this->gamePaused;
}

bool Minecraft::isFramerateLimitBelowMax() const {
    return this->getLimitFramerate() < 120; // this.getLimitFramerate() < GameSettings.Options.FRAMERATE_LIMIT.getValueMax();
}

uint16_t Minecraft::getLimitFramerate() const {
    const uint16_t framerate_limit = 240;
    return this->isGamePaused() ? framerate_limit / 2 : framerate_limit;
}
