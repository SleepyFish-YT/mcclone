//
// Created by SleepyFish on 07.09.2026.
// Project: mcclone
//

#include "Framebuffer.h"

#include "../../client/renderer/GlStateManager.h"
#include "../../client/renderer/OpenGlHelper.h"
#include "../../client/renderer/texture/TextureUtil.h"

#include <glad/glad.h>

void Framebuffer::createBindFramebuffer_(int width, int height) {
    if (!OpenGlHelper::isFramebufferEnabled_()) {
        this->m_framebufferWidth  = width;
        this->m_framebufferHeight = height;
    } else {
        GlStateManager::enableDepth_();
        if (this->m_framebufferObject >= 0) {
            this->deleteFramebuffer_();
        }
        this->createFramebuffer_(width, height);
        this->checkFramebufferComplete_();
        OpenGlHelper::glBindFramebuffer_(OpenGlHelper::GL_FRAMEBUFFER_, 0);
    }
}

void Framebuffer::createFramebuffer_(int width, int height) {
    this->m_framebufferWidth = width;
    this->m_framebufferHeight = height;
    this->m_framebufferTextureWidth = width;
    this->m_framebufferTextureHeight = height;

    if (!OpenGlHelper::isFramebufferEnabled_()) {
        this->framebufferClear_();
        return;
    }

    this->m_framebufferObject  = OpenGlHelper::glGenFramebuffers_();
    this->m_framebufferTexture = TextureUtil::glGenTextures_();

    if (this->m_useDepth) {
        this->m_depthBuffer = OpenGlHelper::glGenRenderbuffers_();
    }

    this->setFramebufferFilter_(GL_NEAREST);
    GlStateManager::bindTexture_(this->m_framebufferTexture);
    GlStateManager::glTexImage2D_(GL_TEXTURE_2D, 0, GL_RGBA8, this->m_framebufferTextureWidth, this->m_framebufferTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    OpenGlHelper::glBindFramebuffer_(OpenGlHelper::GL_FRAMEBUFFER_, this->m_framebufferObject);
    OpenGlHelper::glFramebufferTexture2D_(OpenGlHelper::GL_FRAMEBUFFER_, OpenGlHelper::GL_COLOR_ATTACHMENT0_, GL_TEXTURE_2D, this->m_framebufferTexture, 0);

    if (this->m_useDepth) {
        OpenGlHelper::glBindRenderbuffer_(OpenGlHelper::GL_RENDERBUFFER_, this->m_depthBuffer);
        OpenGlHelper::glRenderbufferStorage_(OpenGlHelper::GL_RENDERBUFFER_, GL_DEPTH_COMPONENT24, this->m_framebufferTextureWidth, this->m_framebufferTextureHeight);
        OpenGlHelper::glFramebufferRenderbuffer_(OpenGlHelper::GL_FRAMEBUFFER_, OpenGlHelper::GL_DEPTH_ATTACHMENT_, OpenGlHelper::GL_RENDERBUFFER_, this->m_depthBuffer);
    }

    this->framebufferClear_();
    this->unbindFramebufferTexture_();
}

void Framebuffer::setFramebufferFilter_(int filter) {
    if (!OpenGlHelper::isFramebufferEnabled_()) {
        return;
    }

    this->m_framebufferFilter = filter;
    GlStateManager::bindTexture_(this->m_framebufferTexture);

    // allow anisotropic filtering
    GlStateManager::glTexParameteri_(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    GlStateManager::glTexParameteri_(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    GlStateManager::glTexParameteri_(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    GlStateManager::glTexParameteri_(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    GlStateManager::bindTexture_(0);
}

void Framebuffer::deleteFramebuffer_() {
    if (!OpenGlHelper::isFramebufferEnabled_()) {
        return;
    }

    this->unbindFramebufferTexture_();
    this->unbindFramebuffer_();

    if (this->m_depthBuffer > -1) {
        OpenGlHelper::glDeleteRenderbuffers_(this->m_depthBuffer);
        this->m_depthBuffer = -1;
    }
    if (this->m_framebufferTexture > -1) {
        TextureUtil::deleteTexture_(this->m_framebufferTexture);
        this->m_framebufferTexture = -1;
    }
    if (this->m_framebufferObject > -1) {
        OpenGlHelper::glBindFramebuffer_(OpenGlHelper::GL_FRAMEBUFFER_, 0);
        OpenGlHelper::glDeleteFramebuffers_(this->m_framebufferObject);
        this->m_framebufferObject = -1;
    }
}

void Framebuffer::checkFramebufferComplete_() {
    int status = OpenGlHelper::glCheckFramebufferStatus_(OpenGlHelper::GL_FRAMEBUFFER_);
    if (status == OpenGlHelper::GL_FRAMEBUFFER_COMPLETE_) return;
    if (status == OpenGlHelper::GL_FB_INCOMPLETE_ATTACHMENT_)
        throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
    if (status == OpenGlHelper::GL_FB_INCOMPLETE_MISS_ATTACH_)
        throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
    if (status == OpenGlHelper::GL_FB_INCOMPLETE_DRAW_BUFFER_)
        throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
    if (status == OpenGlHelper::GL_FB_INCOMPLETE_READ_BUFFER_)
        throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
    throw std::runtime_error("glCheckFramebufferStatus returned unknown status: " + std::to_string(status));
}

void Framebuffer::bindFramebufferTexture_() {
    if (OpenGlHelper::isFramebufferEnabled_()) {
        GlStateManager::bindTexture_(this->m_framebufferTexture);
    }
}

void Framebuffer::unbindFramebufferTexture_() {
    if (OpenGlHelper::isFramebufferEnabled_()) {
        GlStateManager::bindTexture_(0);
    }
}

void Framebuffer::bindFramebuffer_(bool setViewport) {
    if (!OpenGlHelper::isFramebufferEnabled_()) {
        return;
    }

    OpenGlHelper::glBindFramebuffer_(OpenGlHelper::GL_FRAMEBUFFER_, this->m_framebufferObject);
    if (setViewport) {
        GlStateManager::viewport_(0, 0, this->m_framebufferWidth, this->m_framebufferHeight);
    }
}

void Framebuffer::unbindFramebuffer_() {
    if (OpenGlHelper::isFramebufferEnabled_()) {
        OpenGlHelper::glBindFramebuffer_(OpenGlHelper::GL_FRAMEBUFFER_, 0);
    }
}

void Framebuffer::framebufferRenderExt_(int width, int height, bool disableBlend) {
    if (!OpenGlHelper::isFramebufferEnabled_()) {
        return;
    }

    GlStateManager::colorMask_(true, true, true, false);
    GlStateManager::disableDepth_();
    GlStateManager::depthMask_(false);
    GlStateManager::matrixMode_(GL_PROJECTION);
    GlStateManager::loadIdentity_();
    GlStateManager::ortho_(0.0, width, height, 0.0, 1000.0, 3000.0);
    GlStateManager::matrixMode_(GL_MODELVIEW);
    GlStateManager::loadIdentity_();
    GlStateManager::translate_(0.0f, 0.0f, -2000.0f);
    GlStateManager::viewport_(0, 0, width, height);
    GlStateManager::enableTexture2D_();
    GlStateManager::disableLighting_();
    GlStateManager::disableAlpha_();
    if (disableBlend) {
        GlStateManager::disableBlend_();
        GlStateManager::enableColorMaterial_();
    }
    GlStateManager::color_(1.0f, 1.0f, 1.0f, 1.0f);
    this->bindFramebufferTexture_();
    float f  = (float) width;
    float f1 = (float) height;
    float f2 = (float) this->m_framebufferWidth  / (float) this->m_framebufferTextureWidth;
    float f3 = (float) this->m_framebufferHeight / (float) this->m_framebufferTextureHeight;
    // Tessellator& tessellator = Tessellator::getInstance();
    // WorldRenderer& worldrenderer = tessellator.getWorldRenderer();
    // worldrenderer.begin(7, DefaultVertexFormats::POSITION_TEX_COLOR);
    // worldrenderer.pos(0.0, f1, 0.0).tex(0.0, 0.0).color(255, 255, 255, 255).endVertex();
    // worldrenderer.pos(f,  f1, 0.0).tex(f2,  0.0).color(255, 255, 255, 255).endVertex();
    // worldrenderer.pos(f,  0.0, 0.0).tex(f2,  f3).color(255, 255, 255, 255).endVertex();
    // worldrenderer.pos(0.0, 0.0, 0.0).tex(0.0, f3).color(255, 255, 255, 255).endVertex();
    // tessellator.draw();
    this->unbindFramebufferTexture_();
    GlStateManager::depthMask_(true);
    GlStateManager::colorMask_(true, true, true, true);
}

void Framebuffer::framebufferClear_() {
    this->bindFramebuffer_(true);
    GlStateManager::clearColor_(this->m_framebufferColor[0], this->m_framebufferColor[1], this->m_framebufferColor[2], this->m_framebufferColor[3]);

    int mask = GL_COLOR_BUFFER_BIT;
    if (this->m_useDepth) {
        GlStateManager::clearDepth_(1.0);
        mask |= GL_DEPTH_BUFFER_BIT;
    }

    GlStateManager::clear_(mask);
    this->unbindFramebuffer_();
}