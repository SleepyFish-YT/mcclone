//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#include "Minecraft.h"
#include "../../sava/FutureTaskQueue.h"
#include "../../sava/BufferedImage.h"

#include "main/GameConfiguration.h"
#include "settings/GameSettings.h"
#include "settings/KeyBinding.h"
#include "audio/SoundHandler.h"
#include "audio/SoundEngine.h"
#include "renderer/GlStateManager.h"
#include "renderer/OpenGlHelper.h"
#include "renderer/texture/TextureUtil.h"
#include "renderer/texture/TextureMap.h"
#include "renderer/texture/TextureManager.h"
#include "renderer/vertex/DefaultVertexFormats.h"
#include "renderer/Tessellator.h"
#include "renderer/WorldRenderer.h"
#include "resources/SimpleReloadableResourceManager.h"
#include "resources/ResourcePackRepository.h"
#include "resources/IResourcePack.h"
#include "resources/DefaultResourcePack.h"
#include "resources/ResourceIndex.h"
#include "resources/data/IMetadataSerializer.h"
#include "resources/data/PackMetadataSectionSerializer.h"
#include "shader/Framebuffer.h"
#include "gui/ScaledResolution.h"
#include "../debug/Logger.h"
#include "../profiler/Profiler.h"
#include "../config/Config.h"
#include "../util/ResourceLocation.h"
#include "../util/Timer.h"
#include "../util/FrameTimer.h"
#include "../util/MovingObjectPosition.h"
#include "../util/McCloneError.h"
#include "../crash/CrashReport.h"
#include "../client/resources/AbstractResourcePack.h"
#include "../client/gui/FontRenderer.h"
#include "renderer/texture/DynamicTexture.h"

#ifdef _WIN32
#include <windows.h>
#endif //_WIN32

#include <timeapi.h>

#include <glfw/glfw3.h>

Minecraft*  Minecraft::instance = nullptr;
std::unique_ptr<ResourceLocation> Minecraft::locationSleepyPng = std::make_unique<ResourceLocation>("textures/gui/title/sleepy.png");

Minecraft::Minecraft(GameConfiguration *gameConfig) :
    Runnable(),
    isDemo_(gameConfig->gameInformation.isDemo),
    metadataSerializer_(std::make_unique<IMetadataSerializer>()),
    _soundEngine(std::make_unique<SoundEngine>(std::filesystem::path(gameConfig->folderInformation.mcDataDir / "sounds")))
{
    this->mcDataDir = gameConfig->folderInformation.mcDataDir;
    this->fileAssets = gameConfig->folderInformation.assetsDir;
    this->fileResourcepacks = gameConfig->folderInformation.resourcePacksDir;

    {
    this->mcDefaultResourcePack = nullptr;
}

    this->launchedVersion = gameConfig->gameInformation.version;
    // this->profileProperties = gameConfig->userInformation.profileProperties;
    this->displayWidth = gameConfig->displayInformation.width > 0 ? gameConfig->displayInformation.width : 1;
    this->displayHeight = gameConfig->displayInformation.height > 0 ? gameConfig->displayInformation.height : 1;
    this->tempDisplayWidth = gameConfig->displayInformation.width;
    this->tempDisplayHeight = gameConfig->displayInformation.height;
    this->fullscreen = gameConfig->displayInformation.fullscreen;
    this->enableGLErrorChecking = gameConfig->displayInformation.showGlErrors;
    this->debuggerEnabled = gameConfig->debugMode;

    this->scheduledTasks = std::make_unique<FutureTaskQueue<void>>();

    this->hasCrashed = false;
    this->connectedToRealms = false;
    this->isGamePaused_ = false;
    this->skipRenderWorld = false;

    this->leftClickCounter = 0;
    this->rightClickDelayTimer = 0;
    this->tickCounter = 0;
    this->tpsCounter = 0;
    this->prevFrameTime = Minecraft::getHighResTime();

    this->mcProfiler = std::make_unique<Profiler>();
    this->mcProfiler->profilingEnabled = true;

    this->gameSettings = std::make_unique<GameSettings>(*this, this->mcDataDir);

    this->objectMouseOver = std::make_unique<MovingObjectPosition>();

    this->theTimer = std::make_unique<Timer>(20.0f);
    this->frameTimer = std::make_unique<FrameTimer>();

    this->textureMapBlocks = std::make_unique<TextureMap>("textures");
    this->textureMapBlocks->setMipmapLevels(this->gameSettings->mipmapLevels);

    Minecraft::debugFPS = 0;

    if (!gameConfig->serverInformation.serverName.empty()) {
        this->serverName = gameConfig->serverInformation.serverName;
        this->serverPort = gameConfig->serverInformation.serverPort;
    }

    Minecraft::instance = this;
}

Minecraft::~Minecraft() = default;

Minecraft *Minecraft::getMinecraft() noexcept {
    return Minecraft::instance;
}

void Minecraft::startGame() {
    // this->gameSettings = new GameSettings(this->mcDataDir);

}

void Minecraft::shutdownMinecraftApplet() {
    try {
        Logger::log("Stopping!");
    } catch (const std::exception &e) {
        Logger::error("Exception in Minecraft::shutdownMinecraftApplet: {}", e.what());
    }
}

void Minecraft::run(std::stop_token st) {
    if (this->debuggerEnabled) {
        Logger::log("Update thread started");
    }

    try {
        DefaultVertexFormats::staticInit();

        this->startGame();
    } catch (const std::exception &e) {
        Logger::fatal("Exception in Minecraft::run::startGame: {}", e.what());
        // CrashReport *crashreport = CrashReport::makeCrashReport(e, "Initializing game");
        // crashreport->makeCategory("Initialization");
        // this->displayCrashReport(this->addGraphicsAndWorldToCrashReport(crashreport));
        return;
    }

    try {
        while (!st.stop_requested()) {
            if (!this->hasCrashed || this->crashReporter != nullptr) {
                try {
                    this->runGameLoop();
                } catch (const std::exception &e) {
                    Logger::fatal("Exception in Minecraft::run::runGameLoop: {}", e.what());
                    throw;
                }
            }
        }
    } catch (const std::exception &e) {
        // CrashReport crashreport1 = this->addGraphicsAndWorldToCrashReport(CrashReport("Unexpected error", e));
        // this->freeMemory();
        Logger::fatal("Exception in Minecraft::run: {}", e.what());
        // this->displayCrashReport(crashreport1);
        throw;
    }

    this->shutdownMinecraftApplet();

    if (this->debuggerEnabled) {
        Logger::log("Update thread stopped");
    }
}

void Minecraft::onStop() {
    if (this->mcSoundHandler)
        this->mcSoundHandler->unloadSounds();
}

void Minecraft::initializeFramebuffer() {
    OpenGlHelper::initializeTextures();
    TextureUtil::init();

    this->framebufferMc = std::make_unique<Framebuffer>(this->displayWidth, this->displayHeight, true);
    this->framebufferMc->setFramebufferColor_(0.53f, 0.41f, 0.72f, 1.0f);

    this->registerMetadataSerializers();

    this->mcResourcePackRepository = std::make_unique<ResourcePackRepository>(
            this->fileResourcepacks,
            (this->mcDataDir / "server-resource-packs"),
            this->mcDefaultResourcePack.get(),
            this->metadataSerializer_.get(),
            this->gameSettings.get()
    );
    this->mcResourceManager = std::make_unique<SimpleReloadableResourceManager>(this->metadataSerializer_.get());
    std::vector<std::shared_ptr<IResourcePack>> packs;
    for (const auto &entry : this->mcResourcePackRepository->getRepositoryEntries()) {
        packs.push_back(std::shared_ptr<IResourcePack>(entry.getResourcePack(), [](IResourcePack*) {}));
    }
    this->mcResourceManager->reloadResources(packs);
    this->renderEngine = std::make_unique<TextureManager>(this->mcResourceManager.get());

    this->mcSoundHandler = std::make_unique<SoundHandler>(this->mcResourceManager.get(), this->gameSettings.get(), this->_soundEngine.get());
    this->mcResourceManager->registerReloadListener(*this->mcSoundHandler.get());

    if (this->textureMapBlocks == nullptr) {
        Logger::fatal("Minecraft::initializeFramebuffer: TextureMapBlocks is null!");
        return;
    }

    this->renderEngine->loadTickableTexture(TextureMap::LOCATION_BLOCKS_TEXTURE, this->textureMapBlocks.get());
    this->renderEngine->bindTexture(TextureMap::LOCATION_BLOCKS_TEXTURE);
    this->textureMapBlocks->setBlurMipmapDirect(false, this->gameSettings->mipmapLevels > 0);

    /*
    this->modelManager = new ModelManager(this->textureMapBlocks);
    this->mcResourceManager->registerReloadListener(this->modelManager);
    this->renderItem = new RenderItem(this.renderEngine, this.modelManager);
    this->renderManager = new RenderManager(this.renderEngine, this.renderItem);
    this->itemRenderer = new ItemRenderer(this);
    this->mcResourceManager->registerReloadListener(this->renderItem);
    this->entityRenderer = new EntityRenderer(this, this->mcResourceManager);
    this->mcResourceManager->registerReloadListener(this->entityRenderer);
    this->blockRenderDispatcher = new BlockRendererDispatcher(this->modelManager->getBlockModelShapes(), this->gameSettings);
    this->mcResourceManager->registerReloadListener(this->blockRenderDispatcher);
    this->renderGlobal = new RenderGlobal(this);
    this->mcResourceManager->registerReloadListener(this->renderGlobal);
    this->guiAchievement = new GuiAchievement(this);
    GlStateManager::viewport_(0, 0, this->displayWidth, this->displayHeight);
    this->effectRenderer = new EffectRenderer(this->theWorld, this->renderEngine);
    this->checkGLError("Post startup");
    this->ingameGUI = new GuiIngame(this);

    if (this->serverName != nullptr) {
        this->displayGuiScreen(new GuiConnecting(new GuiMainMenu(), this, this.serverName, this.serverPort));
    } else {
        this->displayGuiScreen(new GuiMainMenu());
    }

    this->renderEngine->deleteTexture(this->sleepyLogo);
    this->sleepyLogo = nullptr;
    this->loadingScreen = new LoadingScreenRenderer(this);

    if (this->gameSettings->fullScreen && !this->fullscreen) {
        this->toggleFullscreen();
    }

    this->renderGlobal->makeEntityOutlineShader();
    */

    ResourceLocation resLoc("textures/font/default.png");
    Logger::log("Loading font {}", resLoc.toString());
    this->fontRendererObj = std::make_unique<FontRenderer>(this->gameSettings.get(), resLoc, this->renderEngine.get(), true);
}

void Minecraft::updateFramebufferSize() {
    this->framebufferMc->createBindFramebuffer_(this->displayWidth, this->displayHeight);
    // if (this->entityRenderer != nullptr) {
    //     this->entityRenderer.updateShaderGroupSize(this->displayWidth, this->displayHeight);
    // }
}

void Minecraft::resizeWindow(int width, int height) {
    this->_pendingResizeW.set(width, std::memory_order_relaxed);
    this->_pendingResizeH.set(height,std::memory_order_relaxed);
    this->_pendingResize.set(true, std::memory_order_release);

    /*
    if (this->currentScreen != nullptr) {
        ScaledResolution reso(*this);
        this->currentScreen->onResize(this, reso.getScaledWidth(), reso.getScaledHeight());
    }

    this->loadingScreen = LoadingScreenRenderer(*this);
    */
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
    const long long i = Minecraft::getHighResTime();

    this->mcProfiler->startSection("root");
    {
        if (this->isGamePaused_ /* && this->theWorld != nullptr */) {
            const float delta_ticks = this->theTimer->renderPartialTicks;
            this->theTimer->updateTimer();
            this->theTimer->renderPartialTicks = delta_ticks;
        } else {
            this->theTimer->updateTimer();
        }

        if (!this->scheduledTasks->empty()) {
            this->mcProfiler->startSection("scheduledExecutables");
            {
                this->scheduledTasks->runAll();
            }
            this->mcProfiler->endSection();
        }

        const long long l = Minecraft::getHighResTime();
        this->mcProfiler->startSection("tick");
        {
            for (int j = 0; j < this->theTimer->elapsedTicks; ++j) {
                this->runTick();
            }
        }
        this->mcProfiler->endSection();

        this->mcProfiler->startSection("sound");
        {
            // this->mcSoundHandler->setListener(this->thePlayer, this->theTimer->renderPartialTicks); // not implemented
        }
        this->mcProfiler->endSection();
    }
    this->mcProfiler->endSection();

    this->theTimer->sleepToNextTick();
}

void Minecraft::renderGameLoop(bool hasFocus) {
    if (this->_pendingResize.get()) {
        this->displayWidth  = (int) MathHelper::abs_max(1, this->_pendingResizeW.get());
        this->displayHeight = (int) MathHelper::abs_max(1, this->_pendingResizeH.get());
        this->updateFramebufferSize();

        this->_pendingResize.set(false, std::memory_order_relaxed);
    }

    this->_windowHasFocus = hasFocus;

    const long long time_beforeFramebuffer = Minecraft::getHighResTime();

    ScaledResolution scaledRes(*this);

    this->mcProfiler->startSection("render");

    this->framebufferMc->bindFramebuffer_(true);
    {
        GlStateManager::clearColor_(0.53f, 0.41f, 0.72f, 1.0f);
        GlStateManager::clear_(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->mcProfiler->startSection("preRenderErrors");
            const long long time_preRenderErrors = Minecraft::getHighResTime() - time_beforeFramebuffer;
            this->checkGLError("Pre render");
        this->mcProfiler->endSection();

        // actual render here...
        {
            this->mcProfiler->startSection("display");
            {
                // GlStateManager::enableTexture2D_();
                // if (this->thePlayer != nullptr) {
                //     if (this->thePlayer->isEntityInsideOpaqueBlock()) {
                //         this->gameSettings->thirdPersonView = 0;
                //     }
                // }
            }
            this->mcProfiler->endSection();

            if (!this->skipRenderWorld) {
                this->mcProfiler->startSection("gameRenderer");
                {
                    constexpr double x0 = 64.0,  y0 = 64.0;
                    constexpr double x1 = 128.0, y1 = 128.0;

                    static BufferedImage img = this->mcResourcePackRepository->getRepositoryEntries()[0].getResourcePack()->getPackImage();
                    static DynamicTexture texture = DynamicTexture(img);

                    static Tessellator &tess = Tessellator::getInstance();
                    WorldRenderer &renderer = tess.getWorldRenderer();

                    GlStateManager::enableBlend_();
                    GlStateManager::blendFunc_(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    GlStateManager::disableDepth_();
                    GlStateManager::bindTexture_(texture.getGlTextureId());

                    renderer.begin(7, DefaultVertexFormats::POSITION_TEX_NORMAL);
                    renderer.pos(x0, y1, 0).tex(0, 1).endVertex();
                    renderer.pos(x1, y1, 0).tex(1, 1).endVertex();
                    renderer.pos(x1, y0, 0).tex(1, 0).endVertex();
                    renderer.pos(x0, y0, 0).tex(0, 0).endVertex();
                    tess.draw();

                    GlStateManager::bindTexture_(0);
                    GlStateManager::enableDepth_();
                    GlStateManager::disableBlend_();

                    GlStateManager::pushMatrix_();
                    double scale = scaledRes.getScaleFactor();
                    GlStateManager::scale_(scale, scale, scale);
                    this->fontRendererObj->drawStringWithShadow("https://github.com/SleepyFish-YT/mcclone", 2, 2, -1);
                    GlStateManager::popMatrix_();
                }
                this->mcProfiler->endSection();
            }

            if (this->gameSettings->showDebugInfo && this->gameSettings->showDebugProfilerChart && !this->gameSettings->hideGUI) {
                if (!this->mcProfiler->profilingEnabled) {
                    this->mcProfiler->clearProfiling();
                }

                this->mcProfiler->profilingEnabled = true;
                this->displayDebugInfo(time_preRenderErrors);
            } else {
                this->mcProfiler->profilingEnabled = false;
                this->prevFrameTime = Minecraft::getHighResTime();
            }
        }
    }
    this->framebufferMc->framebufferRender_(this->displayWidth, this->displayHeight);

    this->mcProfiler->endSection();
}

bool Minecraft::isUnicode() const noexcept {
    return /* this->mcLanguageManager->isCurrentLocaleUnicode() || */ this->gameSettings->forceUnicodeFont;
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
            if (key == settings.keyBindScreenshot->getKeyCode()) {
                // ScreenshotHelper::saveScreenshot(...)

                if (this->debuggerEnabled) {
                    Logger::log("base: {}", (void*) this);
                    Logger::log("loc: {}", (void*) Minecraft::locationSleepyPng.get());
                }
            }

            if (key == settings.keyBindPerspective->getKeyCode()) {
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

            // if (this->currentScreen == nullptr)
            {
                if (key == GLFW_KEY_ESCAPE) {
                    this->displayInGameMenu();
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
                        // this->renderGlobal->loadRenderers();
                    }

                    // F3+T - refresh resources
                    if (key == GLFW_KEY_T) {
                        this->refreshResources();
                    }

                    if (key == GLFW_KEY_R) {
                        this->refreshResources();
                    }

                    if (key == GLFW_KEY_G) {
                        settings.advancedItemTooltips = !settings.advancedItemTooltips;
                        settings.saveSettings();
                    }

                    if (key == GLFW_KEY_B) {
                        // this->renderManager->setDebugBoundingBox(!this->renderManager->isDebugBoundingBox());
                    }

                    if (key == GLFW_KEY_P) {
                        settings.pauseOnLostFocus = !settings.pauseOnLostFocus;
                        settings.saveSettings();
                    }

                    // F3+H - clear chat
                    if (key == GLFW_KEY_H) {
                        // this->ingameGUI->getChatGUI().clearChatMessages();
                    }
                }

                if (key == settings.keyBindHideGui->getKeyCode()) {
                    settings.hideGUI = !settings.hideGUI;
                }

                if (key == settings.keyBindToggleDebugOverlay->getKeyCode()) {
                    settings.showDebugInfo = !settings.showDebugInfo;
                }
            }

            for (int i = 0; i < 9; ++i) {
                KeyBinding* keybinding = settings.keyBindHotbar[i];
                if (keybinding != nullptr) {
                    // if (this->thePlayer->isSpectator()) {
                    //     this->ingameGUI->getSpectatorGui()->func_175260_a(l);
                    // } else {
                    //     this->thePlayer->inventory->currentItem = l;
                    // }
                }
            }

            // Inventory
            if (key == settings.keyBindInventory->getKeyCode()) {
                // this->displayGuiScreen(new GuiInventory(this->thePlayer));
            }

            // Drop item
            if (key == settings.keyBindDrop->getKeyCode()) {
                // if (!this->thePlayer.isSpectator()) {
                //     this->thePlayer.dropOneItem((mods & GLFW_MOD_CONTROL) != 0);
                // }
            }

            // Chat
            if (key == settings.keyBindChat->getKeyCode()) {
                // this->displayGuiScreen(new GuiChat());
            }

            // Command
            if (key == settings.keyBindCommand->getKeyCode()) {
                // this->displayGuiScreen(new GuiChat("/"));
            }

            // if (this->thePlayer->isUsingItem()) {
            //     if (!settings.keyBindUseItem->isKeyDown()) {
            //         this->playerController->onStoppedUsingItem(this->thePlayer);
            //     }
            // } else {
            //     if (key = settings->keyBindAttack->getKeyCode()) {
            //         this->leftClickMouse();
            //     }
            //     if (key = settings->keyBindUseItem->getKeyCode()) {
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

            if (keyCode == this->gameSettings->keyBindPickItem->getKeyCode()) {
                // if (this->thePlayer->isSpectator()) {
                //     this->ingameGUI->getSpectatorGui()->func_175261_b();
                // } else {
                //     this->displayGuiScreen(new GuiInventory(this->thePlayer));
                // }

                if (this->debuggerEnabled) {
                    Logger::log("Middle click mouse");
                }
            }

            if (keyCode == this->gameSettings->keyBindAttack->getKeyCode()) {
                this->leftClickMouse();
            }

            if (keyCode == this->gameSettings->keyBindUseItem->getKeyCode()) {
                this->rightClickMouse();
            }
        }

        if (action == GLFW_RELEASE) {
            // if player is using item and key released, stop using
            if (!this->gameSettings->keyBindUseItem->isKeyDown()) {
                // this->playerController->onStoppedUsingItem(this->thePlayer);
            }
        }
    }
    this->mcProfiler->endSection();
}

void Minecraft::handleMouseScroll(double xOffset, double yOffset) {
    if (yOffset != 0) {
        int delta = yOffset > 0 ? 1 : -1;

        // if (this->thePlayer.isSpectator()) {
        //     if (this->ingameGUI->getSpectatorGui()->func_175262_a()) { // func_175262_a = this.SpectatorMenu != nullptr
        //         this->ingameGUI->getSpectatorGui()->func_175259_b(-delta);
        //     } else {
        //         float speed = std::clamp(thePlayer->capabilities->getFlySpeed() + delta * 0.005f, 0.0f, 0.2f);
        //         thePlayer->capabilities->setFlySpeed(speed);
        //     }
        // } else {
        // hotbar scroll
        //     this->thePlayer->inventory->changeCurrentItem(delta);
        // }
    }

    if (xOffset != 0) {
        // touchpad horizontal scroll - treat same as vertical
        int delta = xOffset > 0 ? 1 : -1;
        // this->thePlayer->inventory->changeCurrentItem(delta);
    }
}

void Minecraft::handleMouseMove(double x, double y) {
    if (!this->inGameHasFocus) {
        return;
    }

    // this->entityRenderer->updateCameraAndRender(...)
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

void Minecraft::leftClickMouse() {
    if (this->leftClickCounter >= 0) {
        return;
    }

    // this->thePlayer->swingItem();

    if (this->objectMouseOver == nullptr) {
        // if (this->playerController->isNotCreative()) {
        //     this->leftClickCounter = 10;
        // }
        return;
    }

    switch (this->objectMouseOver->typeOfHit) {
        case MovingObjectPosition::MovingObjectType::ENTITY: {
            // this->playerController->attackEntity(this->thePlayer, this->objectMouseOver->entityHit);
            break;
        }

        case MovingObjectPosition::MovingObjectType::BLOCK: {
            auto blockPos = this->objectMouseOver->getBlockPos();
            // if (this->theWorld->getBlockState(blockpos).getBlock().getMaterial() != Materials::air) {
            //     this->playerController->clickBlock(blockpos, this->objectMouseOver->sideHit);
            //     break;
            // }
            break;
        }

        case MovingObjectPosition::MovingObjectType::MISS: {
            // if (this->playerController->isNotCreative()) {
            //     this->leftClickCounter = 10;
            // }
            break;
        }
    }
}

void Minecraft::rightClickMouse() {

}

void Minecraft::setIngameFocus() {
    if (this->_windowHasFocus) {
        if (!this->inGameHasFocus) {
            this->inGameHasFocus = true;
            // this->mouseHelper->grabMouseCursor();
            // this->displayGuiScreen(nullptr);
            this->leftClickCounter = 255; // usually 1000
        }
    }
}

void Minecraft::setIngameNotInFocus() {
    if (this->inGameHasFocus) {
        KeyBinding::unPressAllKeys();
        this->inGameHasFocus = false;
        // this->mousehelper->ungrabMouseCursor();
    }
}

void Minecraft::displayInGameMenu() {
    /*
    if (this->currentScreen == nullptr) {
        this->displayGuiScreen(new GuiIngameMenu());

        if (this->isSingleplayer() && !this->theIntegratedServer->getPublic()) {
            this->mcSoundHandler->pauseSounds();
        }
    }
    */
}

void Minecraft::crashed(CrashReport *crash) {
    this->hasCrashed = true;
    this->crashReporter.reset(crash);
}

bool Minecraft::isGuiEnabled() {
    return Minecraft::instance == nullptr || !Minecraft::instance->gameSettings->hideGUI;
}

bool Minecraft::isFancyGraphicsEnabled() {
    if (Minecraft::instance != nullptr) {
        return Minecraft::instance->gameSettings->fancyGraphics;
    }

    return false;
}

bool Minecraft::isAmbientOcclusionEnabled() {
    if (Minecraft::instance != nullptr) {
        return Minecraft::instance->gameSettings->ambientOcclusion != 0;
    }

    return false;
}

void Minecraft::middleClickMouse() {
    if (this->objectMouseOver != nullptr) {

    }
}

void Minecraft::addServerStatsToSnooper(PlayerUsageSnooper *playerSnooper) {

}

void Minecraft::addServerTypeToSnooper(PlayerUsageSnooper *playerSnooper) {

}

bool Minecraft::isSnooperEnabled() {
    return this->gameSettings->snooperEnabled;
}

IResourceManager *Minecraft::getResourceManager() noexcept {
    return this->mcResourceManager.get();
}

TextureManager *Minecraft::getTextureManager() noexcept {
    return this->renderEngine.get();
}

void Minecraft::runTick() {
    if (this->rightClickDelayTimer > 0) {
        --this->rightClickDelayTimer;
    }

    this->mcProfiler->startSection("gui");
    {
        if (!this->isGamePaused_) {
            // this->ingameGUI->updateTick();
        }
    }
    this->mcProfiler->endSection();

    // this->entityRenderer->getMouseOver(1.0f);
    this->mcProfiler->startSection("gameMode");

    if (!this->isGamePaused_ /*&& this->theWorld != nullptr*/) {
        // this->playerController->updateController();
    }

    this->mcProfiler->endStartSection("textures");

    if (!this->isGamePaused_) {
        if (this->renderEngine != nullptr) {
            this->renderEngine->update();
        }
    }

    /*
    if (this->currentScreen == null && this->thePlayer != nullptr) {
        if (this->thePlayer.getHealth() <= 0.0f) {
            this->displayGuiScreen(nullptr);
        } else if (this->thePlayer->isPlayerSleeping() && this->theWorld != nullptr) {
            this->displayGuiScreen(new GuiSleepMP());
        }
    } else if (this->currentScreen != nullptr && this->currentScreen instanceof GuiSleepMP && !this->thePlayer->isPlayerSleeping()) {
        this->displayGuiScreen(nullptr);
    }

    if (this->currentScreen != null) {
        this->leftClickCounter = 255;
    }

    if (this->currentScreen != nullptr) {
        try {
            this->currentScreen->handleInput();
        } catch (std::exception exception) {
            CrashReport *crashreport = CrashReport::makeCrashReport(exception, "Updating screen events");
            CrashReportCategory *crashreportcategory = crashreport->makeCategory("Affected screen");
            crashreportcategory->addCrashSectionCallable("Screen name", new Callable<String>() {
                public String call() throws Exception {
                    return Minecraft.this.currentScreen.getClass().getCanonicalName();
                }
            });
            throw;
        }

        if (this->currentScreen != nullptr) {
            try {
                this->currentScreen->updateScreen();
            } catch (std::exception throwable) {
                CrashReport *crashreport1 = CrashReport::makeCrashReport(throwable, "Ticking screen");
                CrashReportCategory *crashreportcategory1 = crashreport1->makeCategory("Affected screen");
                crashreportcategory1->addCrashSectionCallable("Screen name", new Callable<String>() {
                    public String call() throws Exception {
                        return Minecraft.this.currentScreen.getClass().getCanonicalName();
                    }
                });
                throw;
            }
        }
    }

    if (this->currentScreen == nullptr || this->currentScreen->allowUserInput) {

    }

    if (this->theWorld != nullptr) {

    } else if (this->entityRenderer->isShaderActive()) {

    }

    if (!this->isGamePaused_) {

    }

    if (this->theWorld != nullptr) {

    } else if (this->myNetworkManager != nullptr) {

    }
    */

    this->mcProfiler->endSection();
}

void Minecraft::checkGLError(const std::string &message) {
    if (this->enableGLErrorChecking) {
        unsigned int i = GlStateManager::glGetError_();
        if (i != 0) {
            std::string s = Config::getGlErrorString(i);
            Logger::error("########## GL ERROR ##########");
            Logger::error("@ " + message);
            Logger::error(std::to_string(i) + ": " + s);
        }
    }
}

void Minecraft::displayDebugInfo(long long int elapsedTicksTime) {
    if (this->mcProfiler->profilingEnabled) {
        // later...
    }
}

Framebuffer *Minecraft::getFramebuffer() noexcept {
    return this->framebufferMc.get();
}

void Minecraft::registerMetadataSerializers() {
    this->metadataSerializer_->registerMetadataSectionType(new PackMetadataSectionSerializer());
}

std::vector<BufferedImage> Minecraft::getIcons() const {
    const auto &entries = this->mcResourcePackRepository->getRepositoryEntries();
    if (entries.empty()) return {};
    const BufferedImage base = entries[0].getResourcePack()->getPackImage();

    std::vector<BufferedImage> icons;
    icons.reserve(3);
    for (const int s : { 64, 32, 16 })
        icons.push_back(base.sized(s, s));

    return icons;
}

void Minecraft::refreshResources() {

}
