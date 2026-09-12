//
// Created by SleepyFish on 10.09.2026.
// Project: mcclone
//

#include "ModelSprite.h"

#include "ModelRenderer.h"
#include "../renderer/GlStateManager.h"
#include "../renderer/OpenGlHelper.h"
#include "../renderer/vertex/DefaultVertexFormats.h"
#include "../renderer/Tessellator.h"
#include "../renderer/WorldRenderer.h"

#include <cmath>
#include <glad/glad.h>

namespace {

    constexpr float MIN_ITEM_WIDTH = 6.25e-4f;
    constexpr float TEXELS_PER_BLOCK = 16.0f;

}

ModelSprite::ModelSprite(const ModelRenderer &modelRenderer, int textureOffsetX, int textureOffsetY,
                         float posX, float posY, float posZ, int sizeX, int sizeY, int sizeZ, float sizeAdd) :
         modelRenderer_(&modelRenderer),
         textureOffsetX_(textureOffsetX),
         textureOffsetY_(textureOffsetY),
         posX_(posX),
         posY_(posY),
         posZ_(posZ),
         sizeX_(sizeX),
         sizeY_(sizeY),
         sizeZ_(sizeZ),
         sizeAdd_(sizeAdd)
{
    this->minU_ = static_cast<float>(textureOffsetX) / modelRenderer.textureWidth;
    this->minV_ = static_cast<float>(textureOffsetY) / modelRenderer.textureHeight;
    this->maxU_ = static_cast<float>(textureOffsetX + sizeX) / modelRenderer.textureWidth;
    this->maxV_ = static_cast<float>(textureOffsetY + sizeY) / modelRenderer.textureHeight;
}

void ModelSprite::render(Tessellator &tessellator, float scale) const {
    // Push our offset in model space.
    GlStateManager::translate_(posX_ * scale, posY_ * scale, posZ_ * scale);

    float u0 = this->minU_;
    float u1 = this->maxU_;
    float v0 = this->minV_;
    float v1 = this->maxV_;

    if (this->modelRenderer_->mirror)  std::swap(u0, u1);
    if (this->modelRenderer_->mirrorV) std::swap(v0, v1);

    this->renderItemIn2D(tessellator,
        u0, v0, u1, v1,
        this->sizeX_, this->sizeY_,
        scale * static_cast<float>(this->sizeZ_),
        this->modelRenderer_->textureWidth,
        this->modelRenderer_->textureHeight
    );

    GlStateManager::translate_(-posX_ * scale, -posY_ * scale, -posZ_ * scale);
}

void ModelSprite::renderItemIn2D(Tessellator &tess, float minU, float minV, float maxU, float maxV,
                                 int sizeX, int sizeY, float width, float texWidth, float texHeight)
{
    width = std::max(width, MIN_ITEM_WIDTH);

    const float du = maxU - minU;
    const float dv = maxV - minV;
    const double d0 = std::abs(du) * (texWidth  / TEXELS_PER_BLOCK);
    const double d1 = std::abs(dv) * (texHeight / TEXELS_PER_BLOCK);

    WorldRenderer &wr = tess.getWorldRenderer();

    // --- front face (-Z) ---
    glNormal3f(0.0f, 0.0f, -1.0f);
    wr.begin(OpenGlHelper::GL_QUADS_, DefaultVertexFormats::POSITION_TEX);
    wr.pos(0.0,  d1, 0.0).tex(minU, maxV).endVertex();
    wr.pos( d0,  d1, 0.0).tex(maxU, maxV).endVertex();
    wr.pos( d0, 0.0, 0.0).tex(maxU, minV).endVertex();
    wr.pos(0.0, 0.0, 0.0).tex(minU, minV).endVertex();
    tess.draw();

    // --- back face (+Z) ---
    glNormal3f(0.0f, 0.0f, 1.0f);
    wr.begin(OpenGlHelper::GL_QUADS_, DefaultVertexFormats::POSITION_TEX);
    wr.pos(0.0, 0.0, width).tex(minU, minV).endVertex();
    wr.pos( d0, 0.0, width).tex(maxU, minV).endVertex();
    wr.pos( d0,  d1, width).tex(maxU, maxV).endVertex();
    wr.pos(0.0,  d1, width).tex(minU, maxV).endVertex();
    tess.draw();

    const float uStepHalf = 0.5f * du / static_cast<float>(sizeX);
    const float vStepHalf = 0.5f * dv / static_cast<float>(sizeY);

    // --- left edge (-X) ---
    glNormal3f(-1.0f, 0.0f, 0.0f);
    wr.begin(OpenGlHelper::GL_QUADS_, DefaultVertexFormats::POSITION_TEX);
    for (int i = 0; i < sizeX; ++i) {
        const float t  = static_cast<float>(i) / static_cast<float>(sizeX);
        const float u  = minU + du * t + uStepHalf;
        const double px = static_cast<double>(t) * d0;
        wr.pos(px,  d1, width).tex(u, maxV).endVertex();
        wr.pos(px,  d1, 0.0  ).tex(u, maxV).endVertex();
        wr.pos(px, 0.0, 0.0  ).tex(u, minV).endVertex();
        wr.pos(px, 0.0, width).tex(u, minV).endVertex();
    }
    tess.draw();

    // --- right edge (+X) ---
    glNormal3f(1.0f, 0.0f, 0.0f);
    wr.begin(OpenGlHelper::GL_QUADS_, DefaultVertexFormats::POSITION_TEX);
    for (int j = 0; j < sizeX; ++j) {
        const float t   = static_cast<float>(j) / static_cast<float>(sizeX);
        const float u   = minU + du * t + uStepHalf;
        const float tNext = t + 1.0f / static_cast<float>(sizeX);
        const double px = static_cast<double>(tNext) * d0;
        wr.pos(px, 0.0, width).tex(u, minV).endVertex();
        wr.pos(px, 0.0, 0.0  ).tex(u, minV).endVertex();
        wr.pos(px,  d1, 0.0  ).tex(u, maxV).endVertex();
        wr.pos(px,  d1, width).tex(u, maxV).endVertex();
    }
    tess.draw();

    // --- top edge (+Y) ---
    glNormal3f(0.0f, 1.0f, 0.0f);
    wr.begin(OpenGlHelper::GL_QUADS_, DefaultVertexFormats::POSITION_TEX);
    for (int k = 0; k < sizeY; ++k) {
        const float t     = static_cast<float>(k) / static_cast<float>(sizeY);
        const float v     = minV + dv * t + vStepHalf;
        const float tNext = t + 1.0f / static_cast<float>(sizeY);
        const double py   = static_cast<double>(tNext) * d1;
        wr.pos(0.0, py, width).tex(minU, v).endVertex();
        wr.pos( d0, py, width).tex(maxU, v).endVertex();
        wr.pos( d0, py, 0.0  ).tex(maxU, v).endVertex();
        wr.pos(0.0, py, 0.0  ).tex(minU, v).endVertex();
    }
    tess.draw();

    // --- bottom edge (-Y) ---
    glNormal3f(0.0f, -1.0f, 0.0f);
    wr.begin(OpenGlHelper::GL_QUADS_, DefaultVertexFormats::POSITION_TEX);
    for (int l = 0; l < sizeY; ++l) {
        const float t   = static_cast<float>(l) / static_cast<float>(sizeY);
        const float v   = minV + dv * t + vStepHalf;
        const double py = static_cast<double>(t) * d1;
        wr.pos( d0, py, width).tex(maxU, v).endVertex();
        wr.pos(0.0, py, width).tex(minU, v).endVertex();
        wr.pos(0.0, py, 0.0  ).tex(minU, v).endVertex();
        wr.pos( d0, py, 0.0  ).tex(maxU, v).endVertex();
    }
    tess.draw();
}