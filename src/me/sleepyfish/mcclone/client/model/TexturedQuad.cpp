//
// Created by SleepyFish on 10.09.2026.
// Project: mcclone
//

#include "TexturedQuad.h"

#include "../renderer/WorldRenderer.h"
#include "../renderer/Tessellator.h"
#include "../renderer/vertex/DefaultVertexFormats.h"

// not shader implemented yet.

void TexturedQuad::draw(WorldRenderer& renderer, float scale) const {
    const Vec3 vec30 = this->vertexPositions[1].vector3D.subtractReverse(this->vertexPositions[0].vector3D);
    const Vec3 vec31 = this->vertexPositions[1].vector3D.subtractReverse(this->vertexPositions[2].vector3D);
    const Vec3 vec32 = vec31.crossProduct(vec30).normalize();

    auto f0 = static_cast<float>(vec32.xCoord);
    auto f1 = static_cast<float>(vec32.yCoord);
    auto f2 = static_cast<float>(vec32.zCoord);

    if (this->invertNormal) {
        f0 = -f0;
        f1 = -f1;
        f2 = -f2;
    }
/*
    if (Config::isShaders()) {
        renderer.begin(7, SVertexFormat::defVertexFormatTextured);
    } else */{
        renderer.begin(7, DefaultVertexFormats::OLDMODEL_POSITION_TEX_NORMAL);
    }

    for (int i = 0; i < 4; ++i) {
        const PositionTextureVertex& v = this->vertexPositions[i];

        renderer.pos(
                v.vector3D.xCoord * static_cast<double>(scale),
                v.vector3D.yCoord * static_cast<double>(scale),
                v.vector3D.zCoord * static_cast<double>(scale)
        )
        .tex(v.texturePositionX, v.texturePositionY)
        .normal(f0, f1, f2)
        .endVertex();
    }

    Tessellator::getInstance().draw();
}
