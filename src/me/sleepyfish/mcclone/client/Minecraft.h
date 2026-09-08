//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#ifndef MCCLONE_MINECRAFT_H
#define MCCLONE_MINECRAFT_H

#include "../../sava/Runnable.h"
#include "../../sava/FutureTaskQueue.h"

#include <atomic>
#include <thread>
#include <chrono>
#include <filesystem>

class Timer;
class MovingObjectPosition;
class Profiler;
class GameSettings;
class SoundEngine;
class ResourceLocation;
class GameConfiguration;
class Framebuffer;
class FrameTimer;

/**
 * @author SleepyFish
 * @brief Minecraft main class
 */
class Minecraft : public Runnable {

protected:

    // minecraft tick thread .run();
    void run() override;

    void onStop() override;

    uint16_t fpsCounter;

private:

    uint8_t leftClickCounter;

    uint8_t rightClickDelayTimer;

    void runGameLoop(); // throws IOException

    uint16_t tpsCounter;

    uint16_t tickCounter;

    std::chrono::steady_clock::time_point prevFrameTime;

    int tempDisplayWidth;

    int tempDisplayHeight;

    const bool isDemo;

    bool fullscreen;

    bool enableGLErrorChecking;

    bool hasCrashed;

    bool connectedToRealms;

    bool isGamePaused_;

    std::filesystem::path fileResourcepacks;

    std::filesystem::path fileAssets;

    std::string launchedVersion;

    Timer* theTimer;

    void updateFramebufferSize();

    static inline uint16_t debugFPS = 0;

    void startGame(); // throws LWJGLException

    void shutdownMinecraftApplet();

    Framebuffer* framebufferMc;

    FutureTaskQueue<void> scheduledTasks{};

public:

    static ResourceLocation* locationMojangPng;

    std::filesystem::path mcDataDir;

    Profiler *mcProfiler;

    GameSettings *gameSettings;

    SoundEngine *soundEngine;

    MovingObjectPosition *objectMouseOver;

    FrameTimer *frameTimer;

    int displayWidth;

    int displayHeight;

    bool skipRenderWorld;

    explicit Minecraft(GameConfiguration* gameConfig);

    void initializeFramebuffer();

    static long long getSystemTime() noexcept;

    static long long getHighResTime() noexcept;

    bool isGamePaused() const noexcept;

    bool isFramerateLimitBelowMax() const noexcept;

    uint16_t getLimitFramerate() const noexcept;

    void handleKeypress(int key, int scancode, int action, int mods);

    void handleMouseButton(int button, int action, int mods);

    void handleMouseScroll(double xOffset, double yOffset);

    void handleMouseMove(double x, double y);

    void onFullscreenChange(bool fullscreen_, int width, int height);

    void resizeWindow(int width, int height);

};


#endif //MCCLONE_MINECRAFT_H
