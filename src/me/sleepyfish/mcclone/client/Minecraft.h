//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#ifndef MCCLONE_MINECRAFT_H
#define MCCLONE_MINECRAFT_H

#include "../../sava/Runnable.h"

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
class CrashReport;
template<typename T>
class FutureTaskQueue;

/**
 * @author SleepyFish
 * @brief Minecraft main class
 */
class Minecraft : public Runnable {

protected:

    // minecraft tick thread .run();
    void run() override;

    void onStop() override;

private:

    uint8_t leftClickCounter{};

    uint8_t rightClickDelayTimer{};

    void runGameLoop(); // throws IOException

    uint16_t tpsCounter{};

    uint16_t tickCounter{};

    std::chrono::steady_clock::time_point prevFrameTime{};

    int tempDisplayWidth{};

    int tempDisplayHeight{};

    const bool isDemo{};

    bool fullscreen{};

    bool enableGLErrorChecking{};

    bool hasCrashed{};

    bool connectedToRealms{};

    bool isGamePaused_{};

    std::filesystem::path fileResourcepacks{};

    std::filesystem::path fileAssets{};

    std::string launchedVersion{};

    Timer* theTimer{};

    void updateFramebufferSize();

    static inline uint16_t debugFPS{};

    void startGame(); // throws LWJGLException

    void shutdownMinecraftApplet();

    Framebuffer* framebufferMc{};

    FutureTaskQueue<void>* scheduledTasks{};

    std::atomic<bool> pendingResize{false};
    std::atomic<int> pendingResizeW{0};
    std::atomic<int> pendingResizeH{0};

    static Minecraft* instance;

    void leftClickMouse();

    void rightClickMouse();

    CrashReport* crashReporter{};

public:

    static ResourceLocation* locationMojangPng;

    std::filesystem::path mcDataDir{};

    Profiler *mcProfiler{};

    GameSettings *gameSettings{};

    SoundEngine *soundEngine{};

    MovingObjectPosition *objectMouseOver{};

    FrameTimer *frameTimer{};

    Framebuffer* getFramebuffer() noexcept { return this->framebufferMc; }

    int displayWidth{};

    int displayHeight{};

    bool skipRenderWorld{};

    bool debuggerEnabled{};

    explicit Minecraft(GameConfiguration* gameConfig);

    static Minecraft *getMinecraft() noexcept;

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

    void renderGameLoop(bool hasFocus);

    bool isUnicode() const noexcept;

    bool _windowHasFocus{};

    bool inGameHasFocus{};

    void setIngameFocus();

    void setIngameNotInFocus();

    void displayInGameMenu();

    std::string getLaunchedVersion() const noexcept { return this->launchedVersion; }

    void crashed(CrashReport* crash);

};


#endif //MCCLONE_MINECRAFT_H
