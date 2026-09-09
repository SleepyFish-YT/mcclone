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
#include <GLFW/glfw3.h>

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

    GameConfiguration::DisplayInformation displayInfo{};

    RenderInformation* renderContext{};

    // render thread .run();
    void run() override;

    void onStop() override;

    void onJoin() override;

    void renderGameLoop();

public:

    uint64_t frameCount;

    int savedWindowPosX;

    int savedWindowPosY;

    int savedWindowWidth;

    int savedWindowHeight;

    OpenGLWindow(GameConfiguration::DisplayInformation displayInfo, std::string title, Minecraft* minecraft) noexcept;

    bool init();

    void thread_start();

    void thread_run();

    void toggleFullscreen();

    void toggleCaptureMouse();

    GLFWkeyfun handleKeypress(GLFWwindow* window, int key, int scancode, int action, int mods);

    GLFWmousebuttonfun handleMouseButton(GLFWwindow* window, int button, int action, int mods);

    GLFWcursorposfun handleMouseMove(GLFWwindow* window, double xpos, double ypos);

    GLFWscrollfun handleMouseScroll(GLFWwindow* window, double xoffset, double yoffset);

    RenderInformation* getRenderContext() noexcept;

    ::GLFWwindow* getWindow() noexcept;

};


#endif //MCCLONE_OPENGLWINDOW_H
