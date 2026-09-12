//
// Created by SleepyFish on 10.09.2026.
// Project: mcclone
//

#include "ModelBox.h"

#include "ModelRenderer.h"

ModelBox::ModelBox(ModelRenderer &renderer, const std::array<std::array<int, 4>, 6> &faceUV, float x, float y, float z, float dx, float dy, float dz, float inflate, bool mirror) {
    this->posX1 = x;
    this->posY1 = y;
    this->posZ1 = z;
    this->posX2 = x + dx;
    this->posY2 = y + dy;
    this->posZ2 = z + dz;

    float x2 = x + dx;
    float y2 = y + dy;
    float z2 = z + dz;

    // apply inflation
    x -= inflate;
    y -= inflate;
    z -= inflate;
    x2 += inflate;
    y2 += inflate;
    z2 += inflate;

    if (mirror) {
        std::swap(x, x2);
    }

    this->buildVertices(x, y, z, x2, y2, z2);
    this->buildQuadsFromTable(faceUV, renderer, mirror);
}

ModelBox::ModelBox(ModelRenderer &renderer, int textureX, int textureY, float x, float y, float z, int dx, int dy, int dz, float inflate, bool mirror) {
    this->posX1 = x;
    this->posY1 = y;
    this->posZ1 = z;
    this->posX2 = x + static_cast<float>(dx);
    this->posY2 = y + static_cast<float>(dy);
    this->posZ2 = z + static_cast<float>(dz);

    float x2 = x + static_cast<float>(dx);
    float y2 = y + static_cast<float>(dy);
    float z2 = z + static_cast<float>(dz);

    x -= inflate;
    y -= inflate;
    z -= inflate;
    x2 += inflate;
    y2 += inflate;
    z2 += inflate;

    if (mirror) {
        std::swap(x, x2);
    }

    this->buildVertices(x, y, z, x2, y2, z2);
    this->buildQuadsFromAtlas(renderer, textureX, textureY, static_cast<float>(dx), static_cast<float>(dy), static_cast<float>(dz), mirror);
}

void ModelBox::buildQuadsFromTable(const std::array<std::array<int, 4>, 6>& faceUV, ModelRenderer& renderer, bool mirror) {
    auto makeQuad = [&](std::array<PositionTextureVertex, 4> verts, const std::array<int, 4>* uv, bool flip) -> std::optional<TexturedQuad> {
        if (uv == nullptr) {
            return std::nullopt;
        }

        const int u1 = (*uv)[0], v1 = (*uv)[1];
        const int u2 = (*uv)[2], v2 = (*uv)[3];

        const int tw = renderer.textureWidth;
        const int th = renderer.textureHeight;

        if (flip) {
            return TexturedQuad(std::move(verts), u2, v2, u1, v1, tw, th);
        }

        return TexturedQuad(std::move(verts), u1, v1, u2, v2, tw, th);
    };

    const auto& v = this->vertexPositions;

    this->quadList[0] = makeQuad({v[5], v[1], v[2], v[6]}, &faceUV[4], false); // east
    this->quadList[1] = makeQuad({v[0], v[4], v[7], v[3]}, &faceUV[5], false); // west
    this->quadList[2] = makeQuad({v[5], v[4], v[0], v[1]}, &faceUV[1], true);  // down
    this->quadList[3] = makeQuad({v[2], v[3], v[7], v[6]}, &faceUV[0], true);  // up
    this->quadList[4] = makeQuad({v[1], v[0], v[3], v[2]}, &faceUV[2], false); // north
    this->quadList[5] = makeQuad({v[4], v[5], v[6], v[7]}, &faceUV[3], false); // south

    if (mirror) {
        for (auto& quad : this->quadList) {
            if (quad.has_value()) {
                quad->flipFace();
            }
        }
    }
}

void ModelBox::buildQuadsFromAtlas(ModelRenderer& renderer, int textureX, int textureY, float dx, float dy, float dz, bool mirror) {
    const int tw = renderer.textureWidth;
    const int th = renderer.textureHeight;

    const int tx = textureX;
    const int ty = textureY;
    const int w = static_cast<int>(dx);
    const int h = static_cast<int>(dy);
    const int d = static_cast<int>(dz);

    const auto& v = this->vertexPositions;

    this->quadList[0] = TexturedQuad({v[5], v[1], v[2], v[6]}, tx + d + w, ty + d, tx + d + w + d, ty + d + h, tw, th); // east
    this->quadList[1] = TexturedQuad({v[0], v[4], v[7], v[3]}, tx, ty + d, tx + d, ty + d + h, tw, th); // west
    this->quadList[2] = TexturedQuad({v[5], v[4], v[0], v[1]}, tx + d, ty, tx + d + w, ty + d, tw, th); // down
    this->quadList[3] = TexturedQuad({v[2], v[3], v[7], v[6]}, tx + d + w, ty + d, tx + d + w + w, ty, tw, th); // up
    this->quadList[4] = TexturedQuad({v[1], v[0], v[3], v[2]}, tx + d, ty + d, tx + d + w, ty + d + h, tw, th); // north
    this->quadList[5] = TexturedQuad({v[4], v[5], v[6], v[7]}, tx + d + w + d, ty + d, tx + d + w + d + w, ty + d + h, tw, th); // south

    if (mirror) {
        for (auto& quad : this->quadList) {
            if (quad.has_value()) {
                quad->flipFace();
            }
        }
    }
}

void ModelBox::render(WorldRenderer &renderer, float scale) const {
    for (const auto &quad: this->quadList) {
        if (quad.has_value()) {
            quad->draw(renderer, scale);
        }
    }
}