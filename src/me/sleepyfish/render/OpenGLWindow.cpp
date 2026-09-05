//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#include "OpenGLWindow.h"

#include "../debug/Logger.h"

#include "../client/settings/GameSettings.h"
#include "../client/audio/SoundEngine.h"

OpenGLWindow::OpenGLWindow(GameConfiguration::DisplayInformation displayInfo, std::string title, Minecraft* minecraft) {
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
    this->savedWindowPosX = 0;
    this->savedWindowPosY = 0;
    this->savedWindowWidth = 0;
    this->savedWindowHeight = 0;
    this->mouseX = 0;
    this->mouseY = 0;
    this->lastFpsTime = std::chrono::steady_clock::now();
}

bool OpenGLWindow::init() {
    if (!::glfwInit()) {
        Logger::error("Failed to initialize GLFW (glfwInit)");
        return false;
    }

    // set glfw hints - version and opengl profile
    ::glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    ::glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    ::glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->window = ::glfwCreateWindow(this->displayInfo.width, this->displayInfo.height, this->title.c_str(), nullptr, nullptr);
    if (!this->window) {
        Logger::error("Failed to create window (glfwCreateWindow)");
        ::glfwTerminate();
        return false;
    }

    // set window size limits
    ::glfwSetWindowSizeLimits(this->window, 440, 260, 8192, 4320);

    // Make context current here just long enough to init GLAD
    ::glfwMakeContextCurrent(this->window);
    {
        if (!::gladLoadGLLoader((GLADloadproc) ::glfwGetProcAddress)) {
            Logger::error("Failed to initialize GLAD (gladLoadGLLoader)");
            ::glfwTerminate();
            return false;
        }

        this->renderContext.init();
        this->renderContext.print();

        ::glViewport(0, 0, this->displayInfo.width, this->displayInfo.height);
        ::glClearColor(0.53f, 0.41f, 0.72f, 1.0f);

        ::glfwSetWindowUserPointer(this->window, this);

        ::glfwSetKeyCallback(this->window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            auto* self = static_cast<OpenGLWindow*>(::glfwGetWindowUserPointer(window));
            self->handleKeypress(key, scancode, action, mods);
        });
        ::glfwSetMouseButtonCallback(this->window, [](GLFWwindow* window, int button, int action, int mods) {
            auto* self = static_cast<OpenGLWindow*>(::glfwGetWindowUserPointer(window));
            self->handleMouseButton(button, action, mods);
        });
        ::glfwSetCursorPosCallback(this->window, [](GLFWwindow* window, double x, double y) {
            auto* self = static_cast<OpenGLWindow*>(::glfwGetWindowUserPointer(window));
            self->handleMouseMove(x, y);
        });
        ::glfwSetScrollCallback(this->window, [](GLFWwindow* window, double xOffset, double yOffset) {
            auto* self = static_cast<OpenGLWindow*>(::glfwGetWindowUserPointer(window));
            self->handleMouseScroll(xOffset, yOffset);
        });

        if (this->displayInfo.fullscreen) {
            this->toggleFullscreen();
        }
    }
    ::glfwMakeContextCurrent(nullptr);
    // Release context so render thread can claim it

    return true;
}

void OpenGLWindow::run() {
    // this is the render thread (called by runnable::start())
    ::glfwMakeContextCurrent(this->window);
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
            ::glfwSetWindowTitle(this->window, fpsTitle.c_str());
        }

        ::glClear(GL_COLOR_BUFFER_BIT);

        if (this->displayInfo.showGlErrors) {
            GLenum error = ::glGetError();
            if (error != GL_NO_ERROR) {
                Logger::error("OpenGL error: " + std::to_string(error));
            }
        }

        ::glfwSwapBuffers(this->window);
    }

    Logger::log("Render thread stopped");
    ::glfwMakeContextCurrent(nullptr);
}

void OpenGLWindow::execute() {
    this->running = true;

    this->minecraft->start();
    this->start();

    while (this->running && !::glfwWindowShouldClose(this->window)) {
        ::glfwPollEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    this->running = false;
    this->minecraft->stop();

    this->join();
    this->minecraft->join();

    this->shutdown();
}

void OpenGLWindow::shutdown() {
    this->minecraft->soundEngine->destory();
    Logger::log("Shutting down...\n");
    ::glfwTerminate();
}

void OpenGLWindow::toggleFullscreen() {
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
        this->fullscreen = true;
    }

    // inverted since the code above does toggle it
    this->minecraft->onFullscreenChange(!this->fullscreen);
}

void OpenGLWindow::toggleCaptureMouse() {
    this->mouseCaptured = !this->mouseCaptured;
    ::glfwSetInputMode(this->window, GLFW_CURSOR, this->mouseCaptured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void OpenGLWindow::handleKeypress(int key, int scancode, int action, int mods) {
    this->minecraft->handleKeypress(key, scancode, action, mods);

    if (action == GLFW_PRESS) {
        if (key == this->minecraft->gameSettings->keyBindFullscreen.getKeyCode()) {
            this->toggleFullscreen();
        }

        if (key == this->minecraft->gameSettings->keyBindExitGame.getKeyCode()) {
            ::glfwSetWindowShouldClose(this->window, true);
        }
    }
}

void OpenGLWindow::handleMouseButton(int button, int action, int mods) {
    this->minecraft->handleMouseButton(button, action, mods);
}

void OpenGLWindow::handleMouseMove(double x, double y) {
    this->mouseX = x;
    this->mouseY = y;

    this->minecraft->handleMouseMove(x, y);
}

void OpenGLWindow::handleMouseScroll(double xOffset, double yOffset) {
    this->minecraft->handleMouseScroll(xOffset, yOffset);
}

::GLFWwindow* OpenGLWindow::getWindow() {
    return this->window;
}

RenderInformation& OpenGLWindow::getRenderContext() {
    return this->renderContext;
}
