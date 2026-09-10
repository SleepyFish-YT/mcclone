//
// Created by SleepyFish on 10.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_POSITIONTEXTUREVERTEX_H
#define MCCLONE_POSITIONTEXTUREVERTEX_H

#include "../../util/Vec3.h"

/**
 * @author SleepyFish
 * @brief A vertex with a 3D position and a 2D texture coordinate
 */
class PositionTextureVertex {

public:

    Vec3 vector3D{};
    float texturePositionX{};
    float texturePositionY{};

    PositionTextureVertex() = default;

    PositionTextureVertex(float x, float y, float z, float u, float v) :
        vector3D(x, y, z),
        texturePositionX(u),
        texturePositionY(v)
    {}

    PositionTextureVertex(const Vec3& vec, float u, float v) :
        vector3D(vec),
        texturePositionX(u),
        texturePositionY(v)
    {}

    PositionTextureVertex(const PositionTextureVertex& other, float u, float v) :
        vector3D(other.vector3D),
        texturePositionX(u),
        texturePositionY(v)
    {}

    PositionTextureVertex setTexturePosition(float u, float v) const {
        return {
            *this,
            u,
            v
        };
    }

};


#endif //MCCLONE_POSITIONTEXTUREVERTEX_H
