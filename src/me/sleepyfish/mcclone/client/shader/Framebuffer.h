//
// Created by SleepyFish on 07.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_FRAMEBUFFER_H
#define MCCLONE_FRAMEBUFFER_H

#include <stdexcept>
#include <string>

// not FULLY implemented yet.

/**
 * @author SleepyFish
 */
class Framebuffer {

public:

    int framebufferTextureWidth;
    int framebufferTextureHeight;
    int framebufferWidth;
    int framebufferHeight;
    bool useDepth;
    int framebufferObject;
    int framebufferTexture;
    int depthBuffer;
    float framebufferColor[4];
    int framebufferFilter;

    Framebuffer(int widthIn, int heightIn, bool useDepthIn) {
        this->useDepth = useDepthIn;
        this->framebufferObject = -1;
        this->framebufferTexture = -1;
        this->depthBuffer = -1;
        this->framebufferColor[0] = 1.0f;
        this->framebufferColor[1] = 1.0f;
        this->framebufferColor[2] = 1.0f;
        this->framebufferColor[3] = 0.0f;

        this->createBindFramebuffer(widthIn, heightIn);
    }

    void createBindFramebuffer(int width, int height) {
        // TODO: if (!OpenGlHelper::isFramebufferEnabled()) {
        //     framebufferWidth  = width;
        //     framebufferHeight = height;
        // } else {
        //     GlStateManager::enableDepth();
        //     if (framebufferObject >= 0)
        //         deleteFramebuffer();
        //     createFramebuffer(width, height);
        //     checkFramebufferComplete();
        //     OpenGlHelper::glBindFramebuffer(OpenGlHelper::GL_FRAMEBUFFER, 0);
        // }
    }

    void deleteFramebuffer() {
        // TODO: if (!OpenGlHelper::isFramebufferEnabled()) return;
        // unbindFramebufferTexture();
        // unbindFramebuffer();
        // if (depthBuffer > -1) {
        //     OpenGlHelper::glDeleteRenderbuffers(depthBuffer);
        //     depthBuffer = -1;
        // }
        // if (framebufferTexture > -1) {
        //     TextureUtil::deleteTexture(framebufferTexture);
        //     framebufferTexture = -1;
        // }
        // if (framebufferObject > -1) {
        //     OpenGlHelper::glBindFramebuffer(OpenGlHelper::GL_FRAMEBUFFER, 0);
        //     OpenGlHelper::glDeleteFramebuffers(framebufferObject);
        //     framebufferObject = -1;
        // }
    }

    void createFramebuffer(int width, int height) {
        this->framebufferWidth = width;
        this->framebufferHeight = height;
        this->framebufferTextureWidth = width;
        this->framebufferTextureHeight = height;

        // TODO: if (!OpenGlHelper::isFramebufferEnabled()) {
        //     framebufferClear();
        //     return;
        // }
        // framebufferObject  = OpenGlHelper::glGenFramebuffers();
        // framebufferTexture = TextureUtil::glGenTextures();
        // if (useDepth)
        //     depthBuffer = OpenGlHelper::glGenRenderbuffers();
        // setFramebufferFilter(9728);
        // GlStateManager::bindTexture(framebufferTexture);
        // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, framebufferTextureWidth, framebufferTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        // OpenGlHelper::glBindFramebuffer(OpenGlHelper::GL_FRAMEBUFFER, framebufferObject);
        // OpenGlHelper::glFramebufferTexture2D(OpenGlHelper::GL_FRAMEBUFFER, OpenGlHelper::GL_COLOR_ATTACHMENT0, 3553, framebufferTexture, 0);
        // if (useDepth) {
        //     OpenGlHelper::glBindRenderbuffer(OpenGlHelper::GL_RENDERBUFFER, depthBuffer);
        //     OpenGlHelper::glRenderbufferStorage(OpenGlHelper::GL_RENDERBUFFER, 33190, framebufferTextureWidth, framebufferTextureHeight);
        //     OpenGlHelper::glFramebufferRenderbuffer(OpenGlHelper::GL_FRAMEBUFFER, OpenGlHelper::GL_DEPTH_ATTACHMENT, OpenGlHelper::GL_RENDERBUFFER, depthBuffer);
        // }
        // framebufferClear();
        // unbindFramebufferTexture();
    }

    void setFramebufferFilter(int filter) {
        // TODO: if (!OpenGlHelper::isFramebufferEnabled()) return;
        // framebufferFilter = filter;
        // GlStateManager::bindTexture(framebufferTexture);
        // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (float) filter);
        // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (float) filter);
        // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 10496.0f);
        // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 10496.0f);
        // GlStateManager::bindTexture(0);
    }

    void checkFramebufferComplete() {
        // TODO: int status = OpenGlHelper::glCheckFramebufferStatus(OpenGlHelper::GL_FRAMEBUFFER);
        // if (status == OpenGlHelper::GL_FRAMEBUFFER_COMPLETE) return;
        // if (status == OpenGlHelper::GL_FB_INCOMPLETE_ATTACHMENT)
        //     throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
        // if (status == OpenGlHelper::GL_FB_INCOMPLETE_MISS_ATTACH)
        //     throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
        // if (status == OpenGlHelper::GL_FB_INCOMPLETE_DRAW_BUFFER)
        //     throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
        // if (status == OpenGlHelper::GL_FB_INCOMPLETE_READ_BUFFER)
        //     throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
        // throw std::runtime_error("glCheckFramebufferStatus returned unknown status: " + std::to_string(status));
    }

    void bindFramebufferTexture() {
        // TODO: if (OpenGlHelper::isFramebufferEnabled())
        //     GlStateManager::bindTexture(framebufferTexture);
    }

    void unbindFramebufferTexture() {
        // TODO: if (OpenGlHelper::isFramebufferEnabled())
        //     GlStateManager::bindTexture(0);
    }

    void bindFramebuffer(bool setViewport) {
        // TODO: if (!OpenGlHelper::isFramebufferEnabled()) return;
        // OpenGlHelper::glBindFramebuffer(OpenGlHelper::GL_FRAMEBUFFER, framebufferObject);
        // if (setViewport)
        //     GlStateManager::viewport(0, 0, framebufferWidth, framebufferHeight);
    }

    void unbindFramebuffer() {
        // TODO: if (OpenGlHelper::isFramebufferEnabled())
        //     OpenGlHelper::glBindFramebuffer(OpenGlHelper::GL_FRAMEBUFFER, 0);
    }

    void setFramebufferColor(float r, float g, float b, float a) {
        this->framebufferColor[0] = r;
        this->framebufferColor[1] = g;
        this->framebufferColor[2] = b;
        this->framebufferColor[3] = a;
    }

    void framebufferRender(int width, int height) {
        this->framebufferRenderExt(width, height, true);
    }

    void framebufferRenderExt(int width, int height, bool disableBlend) {
        // TODO: if (!OpenGlHelper::isFramebufferEnabled()) return;
        // GlStateManager::colorMask(true, true, true, false);
        // GlStateManager::disableDepth();
        // GlStateManager::depthMask(false);
        // GlStateManager::matrixMode(5889);
        // GlStateManager::loadIdentity();
        // GlStateManager::ortho(0.0, width, height, 0.0, 1000.0, 3000.0);
        // GlStateManager::matrixMode(5888);
        // GlStateManager::loadIdentity();
        // GlStateManager::translate(0.0f, 0.0f, -2000.0f);
        // GlStateManager::viewport(0, 0, width, height);
        // GlStateManager::enableTexture2D();
        // GlStateManager::disableLighting();
        // GlStateManager::disableAlpha();
        // if (disableBlend) {
        //     GlStateManager::disableBlend();
        //     GlStateManager::enableColorMaterial();
        // }
        // GlStateManager::color(1.0f, 1.0f, 1.0f, 1.0f);
        // bindFramebufferTexture();
        // float f  = (float) width;
        // float f1 = (float) height;
        // float f2 = (float) framebufferWidth  / (float) framebufferTextureWidth;
        // float f3 = (float) framebufferHeight / (float) framebufferTextureHeight;
        // Tessellator& tessellator    = Tessellator::getInstance();
        // WorldRenderer& worldrenderer = tessellator.getWorldRenderer();
        // worldrenderer.begin(7, DefaultVertexFormats::POSITION_TEX_COLOR);
        // worldrenderer.pos(0.0, f1, 0.0).tex(0.0, 0.0).color(255, 255, 255, 255).endVertex();
        // worldrenderer.pos(f,  f1, 0.0).tex(f2,  0.0).color(255, 255, 255, 255).endVertex();
        // worldrenderer.pos(f,  0.0, 0.0).tex(f2,  f3).color(255, 255, 255, 255).endVertex();
        // worldrenderer.pos(0.0, 0.0, 0.0).tex(0.0, f3).color(255, 255, 255, 255).endVertex();
        // tessellator.draw();
        // unbindFramebufferTexture();
        // GlStateManager::depthMask(true);
        // GlStateManager::colorMask(true, true, true, true);
    }

    void framebufferClear() {
        // TODO: bindFramebuffer(true);
        // GlStateManager::clearColor(framebufferColor[0], framebufferColor[1], framebufferColor[2], framebufferColor[3]);
        // int mask = GL_COLOR_BUFFER_BIT;
        // if (useDepth) {
        //     GlStateManager::clearDepth(1.0);
        //     mask |= GL_DEPTH_BUFFER_BIT;
        // }
        // GlStateManager::clear(mask);
        // unbindFramebuffer();
    }

};


#endif //MCCLONE_FRAMEBUFFER_H
