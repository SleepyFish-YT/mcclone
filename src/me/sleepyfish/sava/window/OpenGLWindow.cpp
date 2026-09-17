//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#include "OpenGLWindow.h"

#include "../window/RenderInformation.h"

#include "../../mcclone/debug/Logger.h"
#include "../../mcclone/config/Config.h"
#include "../../mcclone/client/renderer/GlStateManager.h"
#include "../../mcclone/client/Minecraft.h"
#include "../../mcclone/client/settings/GameSettings.h"
#include "../../mcclone/client/settings/KeyBinding.h"
#include "../../mcclone/client/audio/SoundEngine.h"

#include <utility>

#ifdef _WIN32
#include <windows.h>
#pragma comment(lib, "winmm.lib")
#include <psapi.h>
#pragma comment(lib, "psapi.lib")
#endif //_WIN32

OpenGLWindow::OpenGLWindow(GameConfiguration::Display displayInfo, std::string title, std::unique_ptr<Minecraft> minecraft) noexcept {
    this->displayInfo = displayInfo;
    this->title = std::move(title);
    this->minecraft = std::move(minecraft);

    this->window = nullptr;
    this->fullscreen = false;
    this->mouseCaptured = false;
    this->renderContext = std::make_unique<RenderInformation>();

    this->renderFps = 0;
    this->lastSecond = std::chrono::steady_clock::now();

    this->savedWindowPosX = 0;
    this->savedWindowPosY = 0;
    this->savedWindowWidth = 0;
    this->savedWindowHeight = 0;
}

OpenGLWindow::~OpenGLWindow() {
    if (this->window)
        ::glfwDestroyWindow(this->window);
}

bool OpenGLWindow::init() {
    if (!::glfwInit()) {
        Logger::error("Failed to initialize GLFW (glfwInit)");
        return false;
    }

    // set glfw hints - version and opengl profile
    ::glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    ::glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    ::glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // for backwards compatibility

    ::GLFWmonitor *monitor = nullptr;
    int width  = this->displayInfo.width;
    int height = this->displayInfo.height;

    if (this->displayInfo.fullscreen) {
        monitor = ::glfwGetPrimaryMonitor();
        const ::GLFWvidmode *mode = ::glfwGetVideoMode(monitor);

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

        ::glfwSetWindowUserPointer(this->window, this);

        ::glfwSetKeyCallback(this->window, [](::GLFWwindow *window, int key, int scancode, int action, int mods) {
            auto* self = static_cast<OpenGLWindow*>(::glfwGetWindowUserPointer(window));
            if (self)
                self->handleKeypress(window, key, scancode, action, mods);
        });
        ::glfwSetMouseButtonCallback(this->window, [](::GLFWwindow *window, int button, int action, int mods) {
            auto* self = static_cast<OpenGLWindow*>(::glfwGetWindowUserPointer(window));
            if (self)
                self->handleMouseButton(window, button, action, mods);
        });
        ::glfwSetCursorPosCallback(this->window, [](::GLFWwindow *window, double x, double y) {
            auto* self = static_cast<OpenGLWindow*>(::glfwGetWindowUserPointer(window));
            if (self)
                self->handleMouseMove(window, x, y);
        });
        ::glfwSetScrollCallback(this->window, [](::GLFWwindow *window, double xOffset, double yOffset) {
            auto* self = static_cast<OpenGLWindow*>(::glfwGetWindowUserPointer(window));
            if (self)
                self->handleMouseScroll(window, xOffset, yOffset);
        });
        ::glfwSetFramebufferSizeCallback(this->window, [](::GLFWwindow *window, int width, int height) {
            auto* self = static_cast<OpenGLWindow*>(::glfwGetWindowUserPointer(window));
            if (self)
                self->handleFramebufferResize(window, width, height);
        });
        ::glfwSetWindowFocusCallback(this->window, [](::GLFWwindow *window, int focused) {
            auto* self = static_cast<OpenGLWindow*>(::glfwGetWindowUserPointer(window));
            if (self)
                self->handleWindowFocus(window, focused);
        });
    }
    ::glfwMakeContextCurrent(nullptr);
    // release context so render thread can claim it

    return true;
}

// this is the render thread (called by runnable(this->start))
void OpenGLWindow::run(std::stop_token st) {
#ifdef _WIN32
    ::timeBeginPeriod(1);
#endif //_WIN32

    ::glfwMakeContextCurrent(this->window);
    {
        if (this->minecraft->debuggerEnabled) {
            Logger::log("Render thread started");
        }

        this->minecraft->initializeFramebuffer();

        while (!st.stop_requested() && !::glfwWindowShouldClose(this->window)) {

#ifdef MCCLONE_DEBUG_MEMORY
#ifdef _WIN32
            static int memFrame = 0;
            if (++memFrame % 300 == 0) {
                ::PROCESS_MEMORY_COUNTERS pmc;
                ::K32GetProcessMemoryInfo(::GetCurrentProcess(), &pmc, sizeof(pmc));
                Logger::log("WorkingSet: {:.1f} MiB", pmc.WorkingSetSize / 1048576.0);
            }
#endif //_WIN32
#endif //MCCLONE_DEBUG_MEMORY

            auto frameStart = std::chrono::steady_clock::now();

            int targetFps = this->minecraft->getLimitFramerate();
            if (!this->focused) {
                targetFps /= 2;
            }

            const auto frameTime = std::chrono::duration<double>(1.0 / targetFps);

            this->renderFps++;

            if (std::chrono::duration_cast<std::chrono::milliseconds>(frameStart - this->lastSecond).count() >= 1000) {
                this->lastSecond = frameStart;
#ifdef MCCLONE_DEBUG
               Logger::log("fps: {}", this->renderFps);
#endif //MCCLONE_DEBUG
                this->renderFps = 0;
            }

            this->renderGameLoop();

            if (this->displayInfo.showGlErrors) {
                ::GLenum error;
                while ((error = GlStateManager::glGetError_()) != GL_NO_ERROR) {
                    Logger::error("OpenGL error: {}", Config::getGlErrorString(error));
                }
            }

            ::glfwSwapBuffers(this->window);

            auto sleepUntil = frameStart + frameTime;
            auto spinStart = sleepUntil - std::chrono::milliseconds(2);
            std::this_thread::sleep_until(spinStart);
            while (std::chrono::steady_clock::now() < sleepUntil) {} // spin
        }

        if (this->minecraft->debuggerEnabled) {
            Logger::log("Render thread stopped");
        }
    }
    ::glfwMakeContextCurrent(nullptr);

#ifdef _WIN32
    ::timeEndPeriod(1);
#endif //_WIN32
}

void OpenGLWindow::renderGameLoop() {
    this->minecraft->renderGameLoop(this->focused);
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
    this->stop();
}

void OpenGLWindow::setTitle(const std::string &wndTitle) {
    this->title = wndTitle;
    ::glfwSetWindowTitle(this->window, this->title.c_str());
}

void OpenGLWindow::onStop() {
    this->minecraft->stop();
}

void OpenGLWindow::onJoin() {
    ::glfwTerminate();
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
        const GLFWvidmode *mode = ::glfwGetVideoMode(::glfwGetPrimaryMonitor());
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

MCCLONE_GLFW_CALLBACK OpenGLWindow::handleKeypress(::GLFWwindow *window, int key, int scancode, int action, int mods) {
    this->minecraft->handleKeypress(key, scancode, action, mods);

    if (action == GLFW_PRESS) {
        if (key == this->minecraft->gameSettings->keyBindFullscreen->getKeyCode()) {
            this->toggleFullscreen();
        }

        if (key == this->minecraft->gameSettings->keyBindExitGame->getKeyCode()) {
            ::glfwSetWindowShouldClose(this->window, true);
        }
    }
}

MCCLONE_GLFW_CALLBACK OpenGLWindow::handleMouseButton(::GLFWwindow *window, int button, int action, int mods) {
    this->minecraft->handleMouseButton(button, action, mods);
}

MCCLONE_GLFW_CALLBACK OpenGLWindow::handleMouseMove(::GLFWwindow *window, double xpos, double ypos) {
    this->minecraft->handleMouseMove(xpos, ypos);
}

MCCLONE_GLFW_CALLBACK OpenGLWindow::handleMouseScroll(::GLFWwindow *window, double xoffset, double yoffset) {
    this->minecraft->handleMouseScroll(xoffset, yoffset);
}

MCCLONE_GLFW_CALLBACK OpenGLWindow::handleFramebufferResize(::GLFWwindow *window, int width, int height) {
    this->minecraft->resizeWindow(width, height);
}

MCCLONE_GLFW_CALLBACK OpenGLWindow::handleWindowFocus(::GLFWwindow *window, int focused) {
    this->focused = (focused == GLFW_TRUE);
}

::GLFWwindow *OpenGLWindow::getWindow() noexcept {
    return this->window;
}

RenderInformation *OpenGLWindow::getRenderContext() noexcept {
    return this->renderContext.get();
}
