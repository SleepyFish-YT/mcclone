//
// Created by SleepyFish on 10.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_TEXTUREDQUAD_H
#define MCCLONE_TEXTUREDQUAD_H

#include "PositionTextureVertex.h"

#include <array>
#include <algorithm>

class WorldRenderer;

/**
 * @author SleepyFish
 * @brief A textured quad
 */
class TexturedQuad {

private:

    bool invertNormal{};

public:

    std::array<PositionTextureVertex, 4> vertexPositions{};
    int nVertices{};

    explicit TexturedQuad(std::array<PositionTextureVertex, 4> vertices) noexcept :
        vertexPositions(std::move(vertices)),
        nVertices(4),
        invertNormal(false)
    {}

    TexturedQuad(std::array<PositionTextureVertex, 4> vertices, int texcoordU1, int texcoordV1, int texcoordU2, int texcoordV2, float textureWidth, float textureHeight) noexcept :
        TexturedQuad(std::move(vertices))
    {
        const float uOffset = 0.0f / textureWidth;
        const float vOffset = 0.0f / textureHeight;

        this->vertexPositions[0] = this->vertexPositions[0].setTexturePosition(
                static_cast<float>(texcoordU2) / textureWidth - uOffset,
                static_cast<float>(texcoordV1) / textureHeight + vOffset
        );
        this->vertexPositions[1] = this->vertexPositions[1].setTexturePosition(
                static_cast<float>(texcoordU1) / textureWidth + uOffset,
                static_cast<float>(texcoordV1) / textureHeight + vOffset
        );
        this->vertexPositions[2] = this->vertexPositions[2].setTexturePosition(
                static_cast<float>(texcoordU1) / textureWidth + uOffset,
                static_cast<float>(texcoordV2) / textureHeight - vOffset
        );
        this->vertexPositions[3] = this->vertexPositions[3].setTexturePosition(
                static_cast<float>(texcoordU2) / textureWidth - uOffset,
                static_cast<float>(texcoordV2) / textureHeight - vOffset
        );
    }

    void flipFace() noexcept {
        std::reverse(this->vertexPositions.begin(), this->vertexPositions.end());

        // hardcoded indexes if std::reverse does not work properly
        // std::swap(this->vertexPositions[0], this->vertexPositions[3]);
        // std::swap(this->vertexPositions[1], this->vertexPositions[2]);
    }

    void draw(WorldRenderer& renderer, float scale) const;

};


#endif //MCCLONE_TEXTUREDQUAD_H
