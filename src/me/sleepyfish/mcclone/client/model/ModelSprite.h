//
// Created by SleepyFish on 10.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_MODELSPRITE_H
#define MCCLONE_MODELSPRITE_H

#include <array>
#include <cstdint>

class ModelRenderer;
class Tessellator;

/**
 * A textured 3D sprite drawn against a ModelRenderer's texture atlas.
 */
class ModelSprite {

private:

    const ModelRenderer *modelRenderer_ = nullptr;

    int textureOffsetX_ = 0;
    int textureOffsetY_ = 0;
    float posX_ = 0.0f, posY_ = 0.0f, posZ_ = 0.0f;
    int sizeX_ = 0, sizeY_ = 0, sizeZ_ = 0;
    float sizeAdd_ = 0.0f;

    float minU_ = 0.0f, minV_ = 0.0f;
    float maxU_ = 0.0f, maxV_ = 0.0f;

public:

    ModelSprite() = default;

    ModelSprite(const ModelRenderer &modelRenderer, int textureOffsetX, int textureOffsetY,
                float posX, float posY, float posZ, int sizeX, int sizeY, int sizeZ, float sizeAdd);

    void render(Tessellator &tessellator, float scale) const;

    static void renderItemIn2D(Tessellator &tess, float minU, float minV, float maxU, float maxV,
                               int sizeX, int sizeY, float width, float texWidth, float texHeight);


};


#endif //MCCLONE_MODELSPRITE_H
