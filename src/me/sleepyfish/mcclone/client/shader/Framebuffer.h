//
// Created by SleepyFish on 07.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_FRAMEBUFFER_H
#define MCCLONE_FRAMEBUFFER_H

#include <stdexcept>
#include <string>

/**
 * @author SleepyFish
 * @note i hate opengl
 * @brief shader framebuffer class
 */
class Framebuffer {

public:

    int m_framebufferTextureWidth;
    int m_framebufferTextureHeight;
    int m_framebufferWidth;
    int m_framebufferHeight;
    bool m_useDepth;
    int m_framebufferObject;
    int m_framebufferTexture;
    int m_depthBuffer;
    float m_framebufferColor[4];
    int m_framebufferFilter;

    Framebuffer(int widthIn, int heightIn, bool useDepthIn) {
        this->m_useDepth = useDepthIn;
        this->m_framebufferObject = -1;
        this->m_framebufferTexture = -1;
        this->m_depthBuffer = -1;
        this->m_framebufferColor[0] = 1.0f;
        this->m_framebufferColor[1] = 1.0f;
        this->m_framebufferColor[2] = 1.0f;
        this->m_framebufferColor[3] = 0.0f;

        this->createBindFramebuffer_(widthIn, heightIn);
    }

    void createBindFramebuffer_(int width, int height);

    void deleteFramebuffer_();

    void createFramebuffer_(int width, int height);

    void setFramebufferFilter_(int filter);

    void checkFramebufferComplete_();

    void bindFramebufferTexture_();

    void unbindFramebufferTexture_();

    void bindFramebuffer_(bool setViewport);

    void unbindFramebuffer_();

    void setFramebufferColor_(float r, float g, float b, float a) {
        this->m_framebufferColor[0] = r;
        this->m_framebufferColor[1] = g;
        this->m_framebufferColor[2] = b;
        this->m_framebufferColor[3] = a;
    }

    void framebufferRender_(int width, int height) {
        this->framebufferRenderExt_(width, height, true);
    }

    void framebufferRenderExt_(int width, int height, bool disableBlend);

    void framebufferClear_();

};


#endif //MCCLONE_FRAMEBUFFER_H
