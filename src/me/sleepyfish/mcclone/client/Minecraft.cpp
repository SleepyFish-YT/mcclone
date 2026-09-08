//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#include "Minecraft.h"

#include "main/GameConfiguration.h"
#include "settings/GameSettings.h"
#include "audio/SoundEngine.h"

#include "../debug/Logger.h"
#include "../profiler/Profiler.h"
#include "../util/ResourceLocation.h"
#include "../util/Timer.h"
#include "../util/FrameTimer.h"
#include "../util/MovingObjectPosition.h"
#include "../util/McCloneError.h"
#include "shader/Framebuffer.h"

#ifdef _WIN32
#include <windows.h>
#endif //_WIN32

#include <timeapi.h>
#include <glfw/glfw3.h>

ResourceLocation* Minecraft::locationMojangPng = new ResourceLocation("textures/gui/title/mojang.png");

Minecraft::Minecraft(GameConfiguration* gameConfig) :
    Runnable(),
    soundEngine(new SoundEngine(std::filesystem::path(gameConfig->folderInformation.mcDataDir / "sounds"))),
    isDemo(gameConfig->gameInformation.isDemo)
{
    this->mcDataDir = gameConfig->folderInformation.mcDataDir;
    this->fileAssets = gameConfig->folderInformation.assetsDir;
    this->fileResourcepacks = gameConfig->folderInformation.resourcePacksDir;
    this->launchedVersion = gameConfig->gameInformation.version;
    // this->profileProperties = gameConfig.userInformation.profileProperties;
    // this->mcDefaultResourcePack = new DefaultResourcePack((new ResourceIndex(gameConfig.folderInformation.assetsDir, gameConfig.folderInformation.assetIndex)).getResourceMap());

    this->displayWidth = gameConfig->displayInformation.width > 0 ? gameConfig->displayInformation.width : 1;
    this->displayHeight = gameConfig->displayInformation.height > 0 ? gameConfig->displayInformation.height : 1;
    this->tempDisplayWidth = gameConfig->displayInformation.width;
    this->tempDisplayHeight = gameConfig->displayInformation.height;
    this->fullscreen = gameConfig->displayInformation.fullscreen;
    this->enableGLErrorChecking = gameConfig->displayInformation.showGlErrors;

    this->hasCrashed = false;
    this->connectedToRealms = false;
    this->isGamePaused_ = false;
    this->skipRenderWorld = false;

    this->leftClickCounter = 0;
    this->rightClickDelayTimer = 0;
    this->tickCounter = 0;
    this->tpsCounter = 0;
    this->prevFrameTime = std::chrono::steady_clock::now();

    this->mcProfiler = new Profiler();
    this->mcProfiler->profilingEnabled = true;

    this->gameSettings = new GameSettings(this->mcDataDir);

    this->objectMouseOver = new MovingObjectPosition();

    this->theTimer = new Timer(20.0f);
    this->frameTimer = new FrameTimer();

    this->fpsCounter = 0;
    Minecraft::debugFPS = 0;
}

void Minecraft::startGame() {

}

void Minecraft::shutdownMinecraftApplet() {
    try {
        Logger::log("Stopping!");
    } catch (const std::exception& e) {

    }
}

void Minecraft::run() {
    Logger::log("Update thread started");

    try {
        this->startGame();
    } catch (const std::exception& e) {
        Logger::fatal("startGame failed: {}", e.what());
        this->hasCrashed = true;
        return;
    }

    try {
        while (this->isRunning()) {
            if (!this->hasCrashed /* || this->crashReporter != nullptr */) {
                try {
                    this->runGameLoop();
                } catch (const std::exception& e) {
                    Logger::fatal("Exception: {}", e.what());
                    throw;
                }
            }
        }
    } catch (const McCloneError& e) {
        Logger::fatal("McCloneError: {}", e.what());
        throw;
    } catch (const std::exception& e) {
        Logger::fatal("Exception: {}", e.what());
        throw;
    } catch (...) {
        this->shutdownMinecraftApplet();
        throw;
    }

    Logger::log("Update thread stopped");
}

void Minecraft::onStop() {
    this->soundEngine->destory();
}

void Minecraft::initializeFramebuffer() {
    this->framebufferMc = new Framebuffer(this->displayWidth, this->displayHeight, true);
    this->framebufferMc->setFramebufferColor(0.0f, 0.0f, 0.0f, 0.0f);
}

long long Minecraft::getSystemTime() noexcept {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

long long Minecraft::getHighResTime() noexcept {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch()
    ).count();
}

void Minecraft::runGameLoop() {
    auto now = std::chrono::steady_clock::now();

    this->mcProfiler->startSection("root");
    {
        if (this->isGamePaused_ /* && this->theWorld != nullptr */) {
            const float delta_ticks = this->theTimer->renderPartialTicks;
            this->theTimer->updateTimer();
            this->theTimer->renderPartialTicks = delta_ticks;
        } else {
            this->theTimer->updateTimer();
        }

        this->mcProfiler->startSection("scheduledExecutables");
        {
            if (!this->scheduledTasks.empty()) {
                this->scheduledTasks.runAll();
            }
        }
        this->mcProfiler->endSection();

        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - this->prevFrameTime).count() >= 1000) {
            Minecraft::debugFPS = this->fpsCounter;

            Logger::log("TPS: {}", this->tpsCounter);

            this->prevFrameTime = now;
            this->tpsCounter = this->tickCounter;
            this->tickCounter = 0;
            this->fpsCounter = 0;
        }

        this->soundEngine->cleanup();
    }
    this->mcProfiler->endSection();
}

bool Minecraft::isGamePaused() const noexcept {
    return this->isGamePaused_;
}

bool Minecraft::isFramerateLimitBelowMax() const noexcept {
    return this->getLimitFramerate() < this->gameSettings->limitFramerate;
}

uint16_t Minecraft::getLimitFramerate() const noexcept {
    const uint16_t framerate_limit = this->gameSettings->limitFramerate;
    return this->isGamePaused() ? framerate_limit / 2 : framerate_limit;
}

void Minecraft::handleKeypress(int key, int scancode, int action, int mods) {
    this->mcProfiler->startSection("keyboard");
    {
        GameSettings &settings = *this->gameSettings;

        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            KeyBinding::setKeyBindState(key, true);
            KeyBinding::onTick(key);
        }

        if (action == GLFW_RELEASE) {
            KeyBinding::setKeyBindState(key, false);
        }

        if (action == GLFW_PRESS) {
            if (key == settings.keyBindScreenshot.getKeyCode()) {
                // ScreenshotHelper::saveScreenshot(...)
            }

            if (key == settings.keyBindPerspective.getKeyCode()) {
                ++settings.thirdPersonView;

                if (settings.thirdPersonView > 2) {
                    settings.thirdPersonView = 0;
                }

                if (settings.thirdPersonView == 0) {
                    // this->entityRenderer.loadEntityShader(this->getRenderViewEntity());
                } else if (settings.thirdPersonView == 1) {
                    // this->entityRenderer.loadEntityShader(nullptr);
                }

                settings.saveSettings();
            }

            // if (this->currentScreen == nullptr) {
            {
                // ESC - pause menu
                if (key == GLFW_KEY_ESCAPE) {
                    // this->displayInGameMenu();
                }

                // F3 + key shortcuts
                if (mods & GLFW_MOD_CONTROL) {
                    if (key == GLFW_KEY_F) {
                        if (mods & GLFW_MOD_SHIFT) {
                            settings.renderDistanceChunks--;
                        } else {
                            settings.renderDistanceChunks++;
                        }

                        settings.renderDistanceChunks = std::clamp(settings.renderDistanceChunks, 2, 32);
                        settings.saveSettings();
                    }

                    // F3+A - reload renderers
                    if (key == GLFW_KEY_A) {
                        // this->renderGlobal.loadRenderers();
                    }

                    // F3+T - refresh resources
                    if (key == GLFW_KEY_T) {
                        // this->refreshResources();
                    }

                    if (key == GLFW_KEY_R) {
                        // this->refreshResources();
                    }

                    if (key == GLFW_KEY_G) {
                        settings.advancedItemTooltips = !settings.advancedItemTooltips;
                        settings.saveSettings();
                    }

                    if (key == GLFW_KEY_B) {
                        // this->renderManager.setDebugBoundingBox(!this->renderManager.isDebugBoundingBox());
                    }

                    if (key == GLFW_KEY_P) {
                        settings.pauseOnLostFocus = !settings.pauseOnLostFocus;
                        settings.saveSettings();
                    }

                    // F3+H - clear chat
                    if (key == GLFW_KEY_H) {
                        // this->ingameGUI.getChatGUI().clearChatMessages();
                    }
                }

                // F1 - hide HUD
                if (key == settings.keyBindHideGui.getKeyCode()) {
                    // settings.hideGUI = !settings.hideGUI;
                }

                if (key == settings.keyBindToggleDebugOverlay.getKeyCode()) {
                    settings.showDebugInfo = !settings.showDebugInfo;
                }
            }

            for (int i = 0; i < 9; ++i) {
                KeyBinding* keybinding = settings.keyBindHotbar[i];
                if (keybinding != nullptr) {
                    // if (this->thePlayer.isSpectator()) {
                    //     this->ingameGUI.getSpectatorGui().func_175260_a(l);
                    // } else {
                    //     this->thePlayer.inventory.currentItem = l;
                    // }
                }
            }

            // Inventory
            if (key == settings.keyBindInventory.getKeyCode()) {
                // this->displayGuiScreen(GuiInventory(this->thePlayer));
            }

            // Drop item
            if (key == settings.keyBindDrop.getKeyCode()) {
                // if (!this->thePlayer.isSpectator()) {
                //     this->thePlayer.dropOneItem((mods & GLFW_MOD_CONTROL) != 0);
                // }
            }

            // Chat
            if (key == settings.keyBindChat.getKeyCode()) {
                // this->displayGuiScreen(GuiChat());
            }

            // Command
            if (key == settings.keyBindCommand.getKeyCode()) {
                // this->displayGuiScreen(GuiChat("/"));
            }

            // if (this->thePlayer.isUsingItem()) {
            //     if (!settings.keyBindUseItem.isKeyDown()) {
            //         this->playerController.onStoppedUsingItem(this->thePlayer);
            //     }
            // } else {
            //     if (key = settings.keyBindAttack.getKeyCode()) {
            //         this->leftClickMouse();
            //     }
            //     if (key = settings.keyBindUseItem.getKeyCode()) {
            //         this->rightClickMouse();
            //     }
            //     if (key = settings.keyBindPickBlock.getKeyCode()) {
            //         this->middleClickMouse();
            //     }
            // }
        }
    }
    this->mcProfiler->endSection();
}

void Minecraft::handleMouseButton(int button, int action, int mods) {
    this->mcProfiler->startSection("mouse");
    {
        const int keyCode = 1000 + button;

        KeyBinding::setKeyBindState(keyCode, action == GLFW_PRESS);

        if (action == GLFW_PRESS) {
            KeyBinding::onTick(keyCode);

            if (keyCode == this->gameSettings->keyBindPickItem.getKeyCode()) {
                // if (this->thePlayer.isSpectator()) {
                //     this->ingameGUI.getSpectatorGui().func_175261_b();
                // } else {
                //     this->displayGuiScreen(GuiInventory(this->thePlayer));
                // }
                Logger::log("Middle click mouse");
            }

            if (keyCode == this->gameSettings->keyBindAttack.getKeyCode()) {
                // this->leftClickMouse();
                Logger::log("Left click mouse");
            }

            if (keyCode == this->gameSettings->keyBindUseItem.getKeyCode()) {
                // this->rightClickMouse();
                Logger::log("Right click mouse");
            }
        }

        if (action == GLFW_RELEASE) {
            // if player is using item and key released, stop using
            // if (!this->gameSettings.keyBindUseItem.isKeyDown()) {
            //     this->playerController.onStoppedUsingItem(this->thePlayer);
            // }
        }
    }
    this->mcProfiler->endSection();
}

void Minecraft::handleMouseScroll(double xOffset, double yOffset) {
    if (yOffset != 0) {
        int delta = yOffset > 0 ? 1 : -1;

        // if (this->minecraft->thePlayer.isSpectator()) {
        //     if (this->minecraft->ingameGUI.getSpectatorGui().func_175262_a()) { // func_175262_a = this.SpectatorMenu != null
        //         this->minecraft->ingameGUI.getSpectatorGui().func_175259_b(-delta);
        //     } else {
        //         float speed = std::clamp(thePlayer.capabilities.getFlySpeed() + delta * 0.005f, 0.0f, 0.2f);
        //         thePlayer.capabilities.setFlySpeed(speed);
        //     }
        // } else {
        // hotbar scroll
        //     this->minecraft->thePlayer.inventory.changeCurrentItem(delta);
        // }
    }

    if (xOffset != 0) {
        // touchpad horizontal scroll - treat same as vertical
        int delta = xOffset > 0 ? 1 : -1;
        // this->minecraft->thePlayer.inventory.changeCurrentItem(delta);
    }
}

void Minecraft::handleMouseMove(double x, double y) {
    // if (!this->minecraft->inGameHasFocus) return;
    // this->minecraft->entityRenderer.updateCameraAndRender(...)
}

void Minecraft::onFullscreenChange(bool fullscreen_, int width, int height) {
    if (fullscreen_) {
        this->displayWidth = width;
        this->displayHeight = height;
    } else {
        this->displayWidth = this->tempDisplayWidth;
        this->displayHeight = this->tempDisplayHeight;
    }

    // useless? im not sure yet.
    {
        if (this->displayWidth <= 0) {
            this->displayWidth = 1;
        }
        if (this->displayHeight <= 0) {
            this->displayHeight = 1;
        }
    }

    this->fullscreen = fullscreen_;
}

void Minecraft::updateFramebufferSize() {

}

void Minecraft::resizeWindow(int width, int height) {
    this->displayWidth = MathHelper::abs_max(1, width);
    this->displayHeight = MathHelper::abs_max(1, height);

    /*
    if (this->currentScreen != nullptr) {
        ScaledResolution scaledresolution(this);
        this->currentScreen->onResize(this, scaledresolution.getScaledWidth(), scaledresolution.getScaledHeight());
    }

    this->loadingScreen = LoadingScreenRenderer(this);
    this->updateFramebufferSize();
    */
}


