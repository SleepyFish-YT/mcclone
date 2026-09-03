//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#ifndef MCCLONE_OPENGLWINDOW_H
#define MCCLONE_OPENGLWINDOW_H

#include "../render/RenderInformation.h"
#include "../client/main/GameConfiguration.h"
#include "../client/Minecraft.h"
#include "../util/Runnable.h"

#include <string>
#include <chrono>
#include <glm/vec2.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

/**
 * @author SleepyFish
 * @brief OpenGL window class
 */
class OpenGLWindow : public Runnable {

private:

    Minecraft* minecraft;
    ::GLFWwindow* window;
    std::string title;
    bool fullscreen;
    bool mouseCaptured;
    GameConfiguration::DisplayInformation displayInfo;
    RenderInformation renderContext;


    void run() override;

public:

    uint16_t debugFps;
    uint64_t frameCount;

    int savedWindowPosX;
    int savedWindowPosY;
    int savedWindowWidth;
    int savedWindowHeight;

    double mouseX;
    double mouseY;
    std::chrono::steady_clock::time_point lastFpsTime;

    OpenGLWindow(GameConfiguration::DisplayInformation displayInfo, std::string title, Minecraft* minecraft);

    bool init();
    void execute();
    void shutdown();
    void toggleFullscreen();
    void toggleCaptureMouse();
    void handleKeypress(int key, int scancode, int action, int mods);
    void handleMouseButton(int button, int action, int mods);
    void handleMouseMove(double x, double y);
    void handleMouseScroll(double xOffset, double yOffset);
    RenderInformation& getRenderContext();
    ::GLFWwindow* getWindow();

};

#endif //MCCLONE_OPENGLWINDOW_H