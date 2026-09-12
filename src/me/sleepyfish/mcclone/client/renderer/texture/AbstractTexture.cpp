//
// Created by SleepyFish on 12.09.2026.
// Project: mcclone
//

#include "AbstractTexture.h"

#include "TextureUtil.h"
#include "../GlStateManager.h"

#include <glad/glad.h>

void AbstractTexture::setBlurMipmapDirect(bool blur_, bool mipmap_) {
    this->blur   = blur_;
    this->mipmap = mipmap_;

    GLint minFilter;
    GLint magFilter;

    if (blur_) {
        minFilter = mipmap_ ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
        magFilter = GL_LINEAR;
    } else {
        minFilter = mipmap_ ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST;
        magFilter = GL_NEAREST;
    }

    GlStateManager::glTexParameteri_(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    GlStateManager::glTexParameteri_(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}

void AbstractTexture::setBlurMipmap(bool blur_, bool mipmap_) {
    this->blurLast   = this->blur;
    this->mipmapLast = this->mipmap;
    this->setBlurMipmapDirect(blur_, mipmap_);
}

void AbstractTexture::restoreLastBlurMipmap() {
    this->setBlurMipmapDirect(this->blurLast, this->mipmapLast);
}

int AbstractTexture::getGlTextureId() const {
    if (this->glTextureId == -1) {
        this->glTextureId = TextureUtil::glGenTextures_();
    }

    return this->glTextureId;
}

void AbstractTexture::deleteGlTexture() {
    if (this->glTextureId != -1) {
        TextureUtil::deleteTexture_(this->glTextureId);
        this->glTextureId = -1;
    }
}