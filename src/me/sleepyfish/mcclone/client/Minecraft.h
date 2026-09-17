//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#ifndef MCCLONE_MINECRAFT_H
#define MCCLONE_MINECRAFT_H

#include "../../sava/Runnable.h"
#include "../../sava/ThreadSave.h"
#include "../profiler/IPlayerUsage.h"

#include <atomic>
#include <thread>
#include <chrono>
#include <filesystem>

class Timer;
class MovingObjectPosition;
class Profiler;
class GameSettings;
class ResourceLocation;
class GameConfiguration;
class Framebuffer;
class FrameTimer;
class CrashReport;
template<typename T>
class FutureTaskQueue;
class TextureMap;
class SoundHandler;
class TextureManager;
class IResourceManager;
class IReloadableResourceManager;
class IMetadataSerializer;
class SoundEngine;
class IResourcePack;
class DefaultResourcePack;
class ResourcePackRepository;

/**
 * @author SleepyFish
 * @brief Minecraft main class
 */
class Minecraft : public Runnable, public IPlayerUsage {

protected:

    // minecraft tick thread .run();
    void run(std::stop_token st) override;

    void onStop() override;

    SoundEngine *_soundEngine{};

private:

    uint8_t leftClickCounter{};

    uint8_t rightClickDelayTimer{};

    void runGameLoop(); // throws IOException

    uint16_t tpsCounter{};

    uint16_t tickCounter{};

    long long prevFrameTime{};

    int tempDisplayWidth{};

    int tempDisplayHeight{};

    const bool isDemo_{};

    bool fullscreen{};

    bool enableGLErrorChecking{};

    bool hasCrashed{};

    bool connectedToRealms{};

    bool isGamePaused_{};

    std::filesystem::path fileResourcepacks{};

    std::filesystem::path fileAssets{};

    std::string launchedVersion{};

    Timer *theTimer{};

    void updateFramebufferSize();

    static inline uint16_t debugFPS{};

    void startGame(); // throws LWJGLException

    void shutdownMinecraftApplet();

    Framebuffer *framebufferMc{};

    FutureTaskQueue<void> *scheduledTasks{};

    ThreadSave<bool> _pendingResize{false};
    ThreadSave<int> _pendingResizeW{0};
    ThreadSave<int> _pendingResizeH{0};

    static Minecraft *instance;

    void leftClickMouse();

    void rightClickMouse();

    CrashReport *crashReporter{};

    void middleClickMouse();

    IReloadableResourceManager *mcResourceManager{};

    const IMetadataSerializer *metadataSerializer_{};

    TextureManager *renderEngine{};

    std::string serverName{};

    int serverPort{};

    void checkGLError(const std::string &message);

    void displayDebugInfo(long long elapsedTicksTime);

    std::vector<std::shared_ptr<IResourcePack>> defaultResourcePacks;

    std::shared_ptr<DefaultResourcePack> mcDefaultResourcePack;

    std::shared_ptr<ResourcePackRepository> mcResourcePackRepository;

public:

    static ResourceLocation *locationSleepyPng;

    std::filesystem::path mcDataDir{};

    Profiler *mcProfiler{};

    GameSettings *gameSettings{};

    MovingObjectPosition *objectMouseOver{};

    FrameTimer *frameTimer{};

    Framebuffer *getFramebuffer() noexcept { return this->framebufferMc; }

    TextureMap *textureMapBlocks{};

    SoundHandler *mcSoundHandler{};

    int displayWidth{};

    int displayHeight{};

    bool skipRenderWorld{};

    bool debuggerEnabled{};

    explicit Minecraft(GameConfiguration *gameConfig);

    static Minecraft *getMinecraft() noexcept;

    void initializeFramebuffer(); // called before the game render loop

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

    void renderGameLoop(bool hasFocus); // throws IOException

    bool isUnicode() const noexcept;

    bool _windowHasFocus{};

    bool inGameHasFocus{};

    void setIngameFocus();

    void setIngameNotInFocus();

    void displayInGameMenu();

    std::string getLaunchedVersion() const noexcept { return this->launchedVersion; }

    void crashed(CrashReport *crash);

    bool isDemo() const noexcept { return this->isDemo_; }

    static bool isGuiEnabled();

    static bool isFancyGraphicsEnabled();

    static bool isAmbientOcclusionEnabled();

    void addServerStatsToSnooper(PlayerUsageSnooper *playerSnooper) override;

    void addServerTypeToSnooper(PlayerUsageSnooper *playerSnooper) override;

    bool isSnooperEnabled() override;

    bool isFullScreen() const noexcept { return this->fullscreen; }

    IResourceManager *getResourceManager() noexcept;

    TextureManager *getTextureManager() noexcept {
        return this->renderEngine;
    }

    void runTick(); // throws IOException

};


#endif //MCCLONE_MINECRAFT_H
