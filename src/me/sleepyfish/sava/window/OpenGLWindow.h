//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#ifndef MCCLONE_OPENGLWINDOW_H
#define MCCLONE_OPENGLWINDOW_H

#include "../Runnable.h"

#include "../../mcclone/client/main/GameConfiguration.h"

#include <string>
#include <chrono>

#include <glm/vec2.hpp>
#include <glad/glad.h>
#include <glfw/glfw3.h>

#define MCCLONE_GLFW_CALLBACK void

class Minecraft;
class RenderInformation;

/**
 * @author SleepyFish - SleepyAVA
 * @version 1.4
 * @brief OpenGL window class
 */
class OpenGLWindow : public Runnable {

private:

    Minecraft* minecraft{};

    ::GLFWwindow* window{};
    std::string title{};
    bool fullscreen{};
    bool mouseCaptured{};
    bool focused{};

    GameConfiguration::DisplayInformation displayInfo{};
    RenderInformation* renderContext{};

    // render thread .run();
    void run() override;

    void onStop() override;

    void onJoin() override;

    void renderGameLoop();

public:

    uint16_t renderFps{};

    std::chrono::steady_clock::time_point lastSecond{};

    int savedWindowPosX{};
    int savedWindowPosY{};
    int savedWindowWidth{};
    int savedWindowHeight{};

    OpenGLWindow(GameConfiguration::DisplayInformation displayInfo, std::string title, Minecraft* minecraft) noexcept;

    bool init();

    void thread_start();

    void thread_run();

    void setTitle(const std::string& wndTitle);

    std::string getTitle() const noexcept { return this->title; }

    void toggleFullscreen();

    void toggleCaptureMouse();

    MCCLONE_GLFW_CALLBACK handleKeypress(::GLFWwindow* window, int key, int scancode, int action, int mods);

    MCCLONE_GLFW_CALLBACK handleMouseButton(::GLFWwindow* window, int button, int action, int mods);

    MCCLONE_GLFW_CALLBACK handleMouseMove(::GLFWwindow* window, double xpos, double ypos);

    MCCLONE_GLFW_CALLBACK handleMouseScroll(::GLFWwindow* window, double xoffset, double yoffset);

    MCCLONE_GLFW_CALLBACK handleFramebufferResize(::GLFWwindow* window, int width, int height);

    MCCLONE_GLFW_CALLBACK handleWindowFocus(::GLFWwindow* window, int focused);

    RenderInformation* getRenderContext() noexcept;

    ::GLFWwindow* getWindow() noexcept;

};

#endif //MCCLONE_OPENGLWINDOW_H
