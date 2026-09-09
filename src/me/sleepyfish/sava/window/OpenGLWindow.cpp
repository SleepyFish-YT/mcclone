//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#include "OpenGLWindow.h"

#include "../window/RenderInformation.h"

#include "../../mcclone/debug/Logger.h"
#include "../../mcclone/client/renderer/GlStateManager.h"

#include "../../mcclone/client/Minecraft.h"
#include "../../mcclone/client/settings/GameSettings.h"
#include "../../mcclone/client/audio/SoundEngine.h"

#include <utility>

OpenGLWindow::OpenGLWindow(GameConfiguration::DisplayInformation displayInfo, std::string title, Minecraft* minecraft) noexcept {
    this->minecraft = minecraft;
    this->window = nullptr;
    this->title = std::move(title);
    this->fullscreen = false;
    this->mouseCaptured = false;
    this->displayInfo = displayInfo;
    this->renderContext = new RenderInformation();
    this->setRunning(false);
    this->frameCount = 0;
    this->savedWindowPosX = 0;
    this->savedWindowPosY = 0;
    this->savedWindowWidth = 0;
    this->savedWindowHeight = 0;
}

bool OpenGLWindow::init() {
    if (!::glfwInit()) {
        Logger::error("Failed to initialize GLFW (glfwInit)");
        return false;
    }

    // set glfw hints - version and opengl profile
    ::glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    ::glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    ::glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // could use GLFW_OPENGL_COMPAT_PROFILE for backwards compatibility

    ::GLFWmonitor* monitor = nullptr;
    int width  = this->displayInfo.width;
    int height = this->displayInfo.height;

    if (this->displayInfo.fullscreen) {
        monitor = ::glfwGetPrimaryMonitor();
        const ::GLFWvidmode* mode = ::glfwGetVideoMode(monitor);

        ::glfwWindowHint(GLFW_RED_BITS,     mode->redBits);
        ::glfwWindowHint(GLFW_GREEN_BITS,   mode->greenBits);
        ::glfwWindowHint(GLFW_BLUE_BITS,    mode->blueBits);
        ::glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        width = mode->width;
        height = mode->height;

        this->fullscreen = true;

        this->savedWindowWidth  = 900;
        this->savedWindowHeight = 600;
        this->savedWindowPosX = (mode->width  - this->savedWindowWidth)  / 2;
        this->savedWindowPosY = (mode->height - this->savedWindowHeight) / 2;
    }

    this->window = ::glfwCreateWindow(width, height, this->title.c_str(), monitor, nullptr);
    if (!this->window) {
        Logger::error("Failed to create window (glfwCreateWindow)");
        ::glfwTerminate();
        return false;
    }

    this->displayInfo.width  = width;
    this->displayInfo.height = height;

    // set window size limits (440x260 - 8192x4320)
    ::glfwSetWindowSizeLimits(this->window, 440, 260, 8192, 4320);

    // make context current here just long enough to init GLAD
    ::glfwMakeContextCurrent(this->window);
    {
        if (!::gladLoadGLLoader((GLADloadproc) ::glfwGetProcAddress)) {
            Logger::error("Failed to initialize GLAD (gladLoadGLLoader)");
            ::glfwTerminate();
            return false;
        }

        this->renderContext->init();
        this->renderContext->print();

        GlStateManager::viewport_(0, 0, this->displayInfo.width, this->displayInfo.height);
        GlStateManager::clearColor_(0.53f, 0.41f, 0.72f, 1.0f);

        ::glfwSetWindowUserPointer(this->window, this);

        ::glfwSetKeyCallback(this->window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            auto* self = static_cast<OpenGLWindow*>(::glfwGetWindowUserPointer(window));
            self->handleKeypress(window, key, scancode, action, mods);
        });
        ::glfwSetMouseButtonCallback(this->window, [](GLFWwindow* window, int button, int action, int mods) {
            auto* self = static_cast<OpenGLWindow*>(::glfwGetWindowUserPointer(window));
            self->handleMouseButton(window, button, action, mods);
        });
        ::glfwSetCursorPosCallback(this->window, [](GLFWwindow* window, double x, double y) {
            auto* self = static_cast<OpenGLWindow*>(::glfwGetWindowUserPointer(window));
            self->handleMouseMove(window, x, y);
        });
        ::glfwSetScrollCallback(this->window, [](GLFWwindow* window, double xOffset, double yOffset) {
            auto* self = static_cast<OpenGLWindow*>(::glfwGetWindowUserPointer(window));
            self->handleMouseScroll(window, xOffset, yOffset);
        });
    }
    ::glfwMakeContextCurrent(nullptr);
    // release context so render thread can claim it

    return true;
}

// this is the render thread (called by runnable(this->start))
void OpenGLWindow::run() {
    ::glfwMakeContextCurrent(this->window);
    Logger::log("Render thread started");

    this->minecraft->initializeFramebuffer();

    while (this->isRunning() && !glfwWindowShouldClose(this->window)) {
        this->frameCount++;

        GlStateManager::clear_(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->renderGameLoop();

        if (this->displayInfo.showGlErrors) {
            GLenum error = GlStateManager::glGetError_();
            if (error != GL_NO_ERROR) {
                Logger::error("OpenGL error: " + std::to_string(error));
            }
        }

        ::glfwSwapBuffers(this->window);
    }

    Logger::log("Render thread stopped");
    ::glfwMakeContextCurrent(nullptr);
}

void OpenGLWindow::renderGameLoop() {
    this->minecraft->renderGameLoop();
}

void OpenGLWindow::thread_start() {
    this->start();
    this->minecraft->start();
}

void OpenGLWindow::thread_run() {
    while (!::glfwWindowShouldClose(this->window) && this->isRunning()) {
        ::glfwPollEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void OpenGLWindow::onStop() {
    this->minecraft->stop();
    ::glfwTerminate();
}

void OpenGLWindow::onJoin() {
    this->minecraft->join();
}

void OpenGLWindow::toggleFullscreen() {
    int width = 0, height = 0;

    if (this->fullscreen) {
        // restore from saved position and size
        ::glfwSetWindowMonitor(
                this->window,
                nullptr, // no monitor = windowed mode
                this->savedWindowPosX,
                this->savedWindowPosY,
                this->savedWindowWidth,
                this->savedWindowHeight,
                GLFW_DONT_CARE
        );

        width = this->savedWindowWidth;
        height = this->savedWindowHeight;

        this->fullscreen = false;
    } else {
        // save current window state BEFORE going fullscreen
        ::glfwGetWindowPos(this->window, &this->savedWindowPosX, &this->savedWindowPosY);
        ::glfwGetWindowSize(this->window, &this->savedWindowWidth, &this->savedWindowHeight);

        // go fullscreen
        const GLFWvidmode* mode = ::glfwGetVideoMode(::glfwGetPrimaryMonitor());
        ::glfwSetWindowMonitor(
                this->window,
                ::glfwGetPrimaryMonitor(),
                0, 0,  // position does not matter in fullscreen
                mode->width,
                mode->height,
                mode->refreshRate
        );

        width = mode->width;
        height = mode->height;

        this->fullscreen = true;
    }

    this->minecraft->onFullscreenChange(this->fullscreen, width, height);
}

void OpenGLWindow::toggleCaptureMouse() {
    this->mouseCaptured = !this->mouseCaptured;
    ::glfwSetInputMode(this->window, GLFW_CURSOR, this->mouseCaptured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

GLFWkeyfun OpenGLWindow::handleKeypress(GLFWwindow* window, int key, int scancode, int action, int mods) {
    this->minecraft->handleKeypress(key, scancode, action, mods);

    if (action == GLFW_PRESS) {
        if (key == this->minecraft->gameSettings->keyBindFullscreen.getKeyCode()) {
            this->toggleFullscreen();
        }

        if (key == this->minecraft->gameSettings->keyBindExitGame.getKeyCode()) {
            ::glfwSetWindowShouldClose(this->window, true);
        }
    }

    return nullptr;
}

GLFWmousebuttonfun OpenGLWindow::handleMouseButton(GLFWwindow* window, int button, int action, int mods) {
    this->minecraft->handleMouseButton(button, action, mods);

    return nullptr;
}

GLFWcursorposfun OpenGLWindow::handleMouseMove(GLFWwindow* window, double xpos, double ypos) {
    this->minecraft->handleMouseMove(xpos, ypos);

    return nullptr;
}

GLFWscrollfun OpenGLWindow::handleMouseScroll(GLFWwindow* window, double xoffset, double yoffset) {
    this->minecraft->handleMouseScroll(xoffset, yoffset);

    return nullptr;
}

::GLFWwindow* OpenGLWindow::getWindow() noexcept {
    return this->window;
}

RenderInformation* OpenGLWindow::getRenderContext() noexcept {
    return this->renderContext;
}
