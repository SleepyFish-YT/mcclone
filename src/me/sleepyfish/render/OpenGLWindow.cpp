//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#include "OpenGLWindow.h"
#include "../debug/Logger.h"

OpenGLWindow::OpenGLWindow(GameConfiguration::DisplayInformation displayInfo, const std::string& title, Minecraft* minecraft) {
    this->minecraft = minecraft;
    this->window = nullptr;
    this->title = title;
    this->fullscreen = false;
    this->mouseCaptured = false;
    this->displayInfo = displayInfo;
    this->renderContext = {};
    this->running = false;
    this->debugFps = 0;
    this->frameCount = 0;
    this->mouseX = 0;
    this->mouseY = 0;
    this->lastFpsTime = std::chrono::steady_clock::now();
}

bool OpenGLWindow::init() {
    if (!glfwInit()) {
        Logger::error("Failed to initialize GLFW (glfwInit)");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->window = glfwCreateWindow(this->displayInfo.width, this->displayInfo.height, this->title.c_str(), nullptr, nullptr);
    if (!this->window) {
        Logger::error("Failed to create window (glfwCreateWindow)");
        glfwTerminate();
        return false;
    }

    // Make context current here just long enough to init GLAD
    glfwMakeContextCurrent(this->window);
    {
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            Logger::error("Failed to initialize GLAD (gladLoadGLLoader)");
            glfwTerminate();
            return false;
        }

        this->renderContext.init();
        this->renderContext.print();

        glViewport(0, 0, this->displayInfo.width, this->displayInfo.height);
        glClearColor(0.53f, 0.81f, 0.92f, 1.0f);

        glfwSetWindowUserPointer(this->window, this);

        glfwSetKeyCallback(this->window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            auto* self = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(window));
            self->handleKeypress(key, scancode, action, mods);
        });
        glfwSetMouseButtonCallback(this->window, [](GLFWwindow* window, int button, int action, int mods) {
            auto* self = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(window));
            self->handleMouseButton(button, action, mods);
        });
        glfwSetCursorPosCallback(this->window, [](GLFWwindow* window, double x, double y) {
            auto* self = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(window));
            self->handleMouseMove(x, y);
        });
        glfwSetScrollCallback(this->window, [](GLFWwindow* window, double xOffset, double yOffset) {
            auto* self = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(window));
            self->handleMouseScroll(xOffset, yOffset);
        });

        if (this->displayInfo.fullscreen) {
            this->toggleFullscreen();
        }
    }
    glfwMakeContextCurrent(nullptr);
    // Release context so render thread can claim it

    return true;
}

void OpenGLWindow::run() {
    // This is the render thread (called by Runnable::start())
    glfwMakeContextCurrent(this->window);
    Logger::log("Render thread started");

    while (this->running && !glfwWindowShouldClose(this->window)) {
        this->frameCount++;
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->lastFpsTime).count();

        if (elapsed >= 1000) {
            this->debugFps = this->frameCount;
            this->frameCount = 0;
            this->lastFpsTime = now;

            std::string fpsTitle = this->title + "  -  " + std::to_string(this->debugFps) + " FPS";
            glfwSetWindowTitle(this->window, fpsTitle.c_str());
        }

        glClear(GL_COLOR_BUFFER_BIT);

        if (this->displayInfo.showGlErrors) {
            GLenum error = glGetError();
            if (error != GL_NO_ERROR) {
                Logger::error("OpenGL error: " + std::to_string(error));
            }
        }

        glfwSwapBuffers(this->window);
    }

    Logger::log("Render thread stopped");
    glfwMakeContextCurrent(nullptr);
}

void OpenGLWindow::execute() {
    this->running = true;

    // Release context so render thread can claim it
    glfwMakeContextCurrent(nullptr);

    // Start minecraft update thread
    this->minecraft->start();

    // Start render thread via Runnable::start()
    this->start();

    // Main thread pumps events (must stay on main thread)
    while (this->running && !glfwWindowShouldClose(this->window)) {
        glfwPollEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    this->running = false;
    this->minecraft->stop();

    this->join();
    this->minecraft->join();

    this->shutdown();
}

void OpenGLWindow::shutdown() {
    Logger::log("Shutting down...\n");
    Logger::close();
    glfwTerminate();
}

void OpenGLWindow::toggleFullscreen() {
    if (this->fullscreen) {
        glfwSetWindowMonitor(this->window, nullptr, 100, 100, this->displayInfo.width, this->displayInfo.height, GLFW_DONT_CARE);
        this->fullscreen = false;
    } else {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowMonitor(this->window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
        this->fullscreen = true;
    }
}

void OpenGLWindow::toggleCaptureMouse() {
    this->mouseCaptured = !this->mouseCaptured;
    glfwSetInputMode(this->window, GLFW_CURSOR, this->mouseCaptured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void OpenGLWindow::handleKeypress(int key, int scancode, int action, int mods) {
    this->minecraft->mcProfiler.startSection("keyboard");
    {
        GameSettings& settings = this->minecraft->gameSettings;

        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            KeyBinding::setKeyBindState(key, true);

            if (action == GLFW_PRESS) {
                KeyBinding::onTick(key);
            }
        }

        if (action == GLFW_RELEASE) {
            KeyBinding::setKeyBindState(key, false);
        }

        if (action == GLFW_PRESS) {
            if (key == settings.keyBindFullscreen.getKeyCode()) {
                this->toggleFullscreen();
            }

            if (key == settings.keyBindScreenshot.getKeyCode()) {
                // ScreenshotHelper::saveScreenshot(...)
            }

            if (key == settings.keyBindPerspective.getKeyCode()) {
                ++settings.thirdPersonView;

                if (settings.thirdPersonView > 2) {
                    settings.thirdPersonView = 0;
                }

                if (settings.thirdPersonView == 0) {
                    // this->minecraft->entityRenderer.loadEntityShader(this->minecraft->getRenderViewEntity());
                } else if (settings.thirdPersonView == 1) {
                    // this->minecraft->entityRenderer.loadEntityShader(nullptr);
                }

                settings.saveSettings();
            }

            // if (this->minecraft->currentScreen == nullptr) {
            {
                // ESC - pause menu
                if (key == GLFW_KEY_ESCAPE) {
                    // this->minecraft->displayInGameMenu();
                }

                // F3 + key shortcuts
                if (mods & GLFW_MOD_CONTROL) {

                    // F3+F - render distance
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
                        // this->minecraft->renderGlobal.loadRenderers();
                    }

                    // F3+T - refresh resources
                    if (key == GLFW_KEY_T) {
                        // this->minecraft->refreshResources();
                    }

                    // F3+R - refresh resources (alt)
                    if (key == GLFW_KEY_R) {
                        // this->minecraft->refreshResources();
                    }

                    // F3+G - advanced tooltips
                    if (key == GLFW_KEY_G) {
                        settings.advancedItemTooltips = !settings.advancedItemTooltips;
                        settings.saveSettings();
                    }

                    // F3+B - debug bounding boxes
                    if (key == GLFW_KEY_B) {
                        // this->minecraft->renderManager.setDebugBoundingBox(!this->minecraft->renderManager.isDebugBoundingBox());
                    }

                    // F3+P - pause on lost focus
                    if (key == GLFW_KEY_P) {
                        settings.pauseOnLostFocus = !settings.pauseOnLostFocus;
                        settings.saveSettings();
                    }

                    // F3+H - clear chat
                    if (key == GLFW_KEY_H) {
                        // this->minecraft->ingameGUI.getChatGUI().clearChatMessages();
                    }
                }

                // F1 - hide HUD
                if (key == settings.keyBindHideGui.getKeyCode()) {
                    // settings.hideGUI = !settings.hideGUI;
                }

                // F3 - debug overlay
                if (key == settings.keyBindToggleDebugOverlay.getKeyCode()) {
                    settings.showDebugInfo = !settings.showDebugInfo;
                }
            }

            return;

            // Hotbar 1-9
            for (int i = 0; i < 9; i++) {
                if (settings.keyBindHotbar[i]->isPressed()) {
                    // if (this->minecraft->thePlayer.isSpectator()) {
                    //     this->minecraft->ingameGUI.getSpectatorGui().func_175260_a(i);
                    // } else {
                    //     this->minecraft->thePlayer.inventory.currentItem = i;
                    // }
                }
            }

            // Inventory
            while (settings.keyBindInventory.isPressed()) {
                // this->minecraft->displayGuiScreen(new GuiInventory(this->minecraft->thePlayer));
            }

            // Drop item
            while (settings.keyBindDrop.isPressed()) {
                // if (!this->minecraft->thePlayer.isSpectator()) {
                //     this->minecraft->thePlayer.dropOneItem((mods & GLFW_MOD_CONTROL) != 0);
                // }
            }

            // Chat
            while (settings.keyBindChat.isPressed()) {
                // this->minecraft->displayGuiScreen(new GuiChat());
            }

            // Command
            while (settings.keyBindCommand.isPressed()) {
                // this->minecraft->displayGuiScreen(new GuiChat("/"));
            }

            // Attack / use item / pick block
            // if (this->minecraft->thePlayer.isUsingItem()) {
            //     if (!settings.keyBindUseItem.isKeyDown()) {
            //         this->minecraft->playerController.onStoppedUsingItem(this->minecraft->thePlayer);
            //     }
            // } else {
            //     while (settings.keyBindAttack.isPressed())    { this->minecraft->leftClickMouse(); }
            //     while (settings.keyBindUseItem.isPressed())   { this->minecraft->rightClickMouse(); }
            //     while (settings.keyBindPickBlock.isPressed()) { this->minecraft->middleClickMouse(); }
            // }

            // F12 - close window
            if (key == GLFW_KEY_F12) {
                glfwSetWindowShouldClose(this->window, true);
            }
        }
    }
    this->minecraft->mcProfiler.endSection();
}

void OpenGLWindow::handleMouseButton(int button, int action, int mods) {
    this->minecraft->mcProfiler.startSection("mouse");
    {
        int keyCode = button;

        KeyBinding::setKeyBindState(keyCode, action == GLFW_PRESS);

        if (action == GLFW_PRESS) {
            // Middle click - spectator GUI
            if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
                // if (this->minecraft->thePlayer.isSpectator()) {
                //     this->minecraft->ingameGUI.getSpectatorGui().func_175261_b();
                // } else {
                KeyBinding::onTick(keyCode);
                // }
            } else {
                KeyBinding::onTick(keyCode);
            }

            if (button == GLFW_MOUSE_BUTTON_LEFT) {
                // this->minecraft->leftClickMouse();
            }

            if (button == GLFW_MOUSE_BUTTON_RIGHT) {
                // this->minecraft->rightClickMouse();
            }
        }

        if (action == GLFW_RELEASE) {
            // if player is using item and key released, stop using
            // if (!this->minecraft->gameSettings.keyBindUseItem.isKeyDown()) {
            //     this->minecraft->playerController.onStoppedUsingItem(this->minecraft->thePlayer);
            // }
        }
    }
    this->minecraft->mcProfiler.endSection();
}

void OpenGLWindow::handleMouseMove(double x, double y) {
    this->mouseX = x;
    this->mouseY = y;

    // if (!this->minecraft->inGameHasFocus) return;
    // this->minecraft->entityRenderer.updateCameraAndRender(...)
}

void OpenGLWindow::handleMouseScroll(double xOffset, double yOffset) {
    GameSettings& settings = this->minecraft->gameSettings;

    if (yOffset != 0) {
        int delta = yOffset > 0 ? 1 : -1;

        // if (this->minecraft->thePlayer.isSpectator()) {
        //     if (this->minecraft->ingameGUI.getSpectatorGui().func_175262_a()) {
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

GLFWwindow* OpenGLWindow::getWindow() {
    return this->window;
}

RenderInformation& OpenGLWindow::getRenderContext() {
    return this->renderContext;
}