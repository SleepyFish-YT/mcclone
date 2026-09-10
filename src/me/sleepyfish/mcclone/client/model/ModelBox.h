//
// Created by SleepyFish on 10.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_MODELBOX_H
#define MCCLONE_MODELBOX_H

#include "PositionTextureVertex.h"
#include "TexturedQuad.h"

#include <array>
#include <optional>
#include <string>
#include <string_view>

class WorldRenderer;
class ModelRenderer;

/**
 * @author SleepyFish
 * @brief A box model
 */
class ModelBox {

private:

    std::array<PositionTextureVertex, 8> vertexPositions;

    std::array<std::optional<TexturedQuad>, 6> quadList;

    void buildVertices(float x1, float y1, float z1, float x2, float y2, float z2) noexcept {
        this->vertexPositions[0] = PositionTextureVertex(x1, y1, z1, 0.0f, 0.0f);
        this->vertexPositions[1] = PositionTextureVertex(x2, y1, z1, 0.0f, 8.0f);
        this->vertexPositions[2] = PositionTextureVertex(x2, y2, z1, 8.0f, 8.0f);
        this->vertexPositions[3] = PositionTextureVertex(x1, y2, z1, 8.0f, 0.0f);
        this->vertexPositions[4] = PositionTextureVertex(x1, y1, z2, 0.0f, 0.0f);
        this->vertexPositions[5] = PositionTextureVertex(x2, y1, z2, 0.0f, 8.0f);
        this->vertexPositions[6] = PositionTextureVertex(x2, y2, z2, 8.0f, 8.0f);
        this->vertexPositions[7] = PositionTextureVertex(x1, y2, z2, 8.0f, 0.0f);
    }

    void buildQuadsFromTable(const std::array<std::array<int, 4>, 6>& faceUV, ModelRenderer& renderer, bool mirror);

    void buildQuadsFromAtlas(ModelRenderer& renderer, int textureX, int textureY, float dx, float dy, float dz, bool mirror);

public:

    float posX1, posY1, posZ1;
    float posX2, posY2, posZ2;

    std::string boxName;

    ModelBox(ModelRenderer& renderer, const std::array<std::array<int, 4>, 6>& faceUV, float x, float y, float z, float dx, float dy, float dz, float inflate, bool mirror);

    ModelBox(ModelRenderer& renderer, int textureX, int textureY, float x, float y, float z, int dx, int dy, int dz, float inflate, bool mirror);

    void render(WorldRenderer& renderer, float scale) const;

    ModelBox& setBoxName(std::string_view name) {
        this->boxName = name;
        return *this;
    }

};


#endif //MCCLONE_MODELBOX_H
