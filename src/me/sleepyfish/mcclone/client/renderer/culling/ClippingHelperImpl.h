//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_CLIPPINGHELPERIMPL_H
#define MCCLONE_CLIPPINGHELPERIMPL_H

#include "ClippingHelper.h"
#include "../GLAllocation.h"
#include "../GlStateManager.h"

#include <cmath>
#include <vector>

#include <glad/glad.h>

/**
 * @author SleepyFish
 * @brief Clipping helper
 */
class ClippingHelperImpl : public ClippingHelper {

private:

    static ClippingHelperImpl instance;

    std::vector<float> projectionMatrixBuffer;
    std::vector<float> modelviewMatrixBuffer;
    std::vector<float> clippingMatrixBuffer;

    ClippingHelperImpl() :
        projectionMatrixBuffer(GLAllocation::createDirectFloatBuffer(16)),
        modelviewMatrixBuffer(GLAllocation::createDirectFloatBuffer(16)),
        clippingMatrixBuffer(GLAllocation::createDirectFloatBuffer(16))
    {}

    void normalize(float* plane) noexcept {
        float f = std::sqrt(plane[0] * plane[0] + plane[1] * plane[1] + plane[2] * plane[2]);
        plane[0] /= f;
        plane[1] /= f;
        plane[2] /= f;
        plane[3] /= f;
    }

public:

    static ClippingHelper* getInstance() {
        ClippingHelperImpl::instance.init();
        return &ClippingHelperImpl::instance;
    }

    void init() {
        GlStateManager::getFloat_(GL_PROJECTION_MATRIX, this->projectionMatrixBuffer.data());
        GlStateManager::getFloat_(GL_MODELVIEW_MATRIX,  this->modelviewMatrixBuffer.data());

        const float* p = this->projectionMatrixBuffer.data();
        const float* m = this->modelviewMatrixBuffer.data();
        float* c = this->clippingMatrix;

        c[0]  = m[0]  * p[0]  + m[1]  * p[4]  + m[2]  * p[8]  + m[3]  * p[12];
        c[1]  = m[0]  * p[1]  + m[1]  * p[5]  + m[2]  * p[9]  + m[3]  * p[13];
        c[2]  = m[0]  * p[2]  + m[1]  * p[6]  + m[2]  * p[10] + m[3]  * p[14];
        c[3]  = m[0]  * p[3]  + m[1]  * p[7]  + m[2]  * p[11] + m[3]  * p[15];
        c[4]  = m[4]  * p[0]  + m[5]  * p[4]  + m[6]  * p[8]  + m[7]  * p[12];
        c[5]  = m[4]  * p[1]  + m[5]  * p[5]  + m[6]  * p[9]  + m[7]  * p[13];
        c[6]  = m[4]  * p[2]  + m[5]  * p[6]  + m[6]  * p[10] + m[7]  * p[14];
        c[7]  = m[4]  * p[3]  + m[5]  * p[7]  + m[6]  * p[11] + m[7]  * p[15];
        c[8]  = m[8]  * p[0]  + m[9]  * p[4]  + m[10] * p[8]  + m[11] * p[12];
        c[9]  = m[8]  * p[1]  + m[9]  * p[5]  + m[10] * p[9]  + m[11] * p[13];
        c[10] = m[8]  * p[2]  + m[9]  * p[6]  + m[10] * p[10] + m[11] * p[14];
        c[11] = m[8]  * p[3]  + m[9]  * p[7]  + m[10] * p[11] + m[11] * p[15];
        c[12] = m[12] * p[0]  + m[13] * p[4]  + m[14] * p[8]  + m[15] * p[12];
        c[13] = m[12] * p[1]  + m[13] * p[5]  + m[14] * p[9]  + m[15] * p[13];
        c[14] = m[12] * p[2]  + m[13] * p[6]  + m[14] * p[10] + m[15] * p[14];
        c[15] = m[12] * p[3]  + m[13] * p[7]  + m[14] * p[11] + m[15] * p[15];

        // right
        this->frustum[0][0] = c[3]  - c[0];
        this->frustum[0][1] = c[7]  - c[4];
        this->frustum[0][2] = c[11] - c[8];
        this->frustum[0][3] = c[15] - c[12];
        this->normalize(this->frustum[0]);

        // left
        this->frustum[1][0] = c[3]  + c[0];
        this->frustum[1][1] = c[7]  + c[4];
        this->frustum[1][2] = c[11] + c[8];
        this->frustum[1][3] = c[15] + c[12];
        this->normalize(this->frustum[1]);

        // bottom
        this->frustum[2][0] = c[3]  + c[1];
        this->frustum[2][1] = c[7]  + c[5];
        this->frustum[2][2] = c[11] + c[9];
        this->frustum[2][3] = c[15] + c[13];
        this->normalize(this->frustum[2]);

        // top
        this->frustum[3][0] = c[3]  - c[1];
        this->frustum[3][1] = c[7]  - c[5];
        this->frustum[3][2] = c[11] - c[9];
        this->frustum[3][3] = c[15] - c[13];
        this->normalize(this->frustum[3]);

        // far
        this->frustum[4][0] = c[3]  - c[2];
        this->frustum[4][1] = c[7]  - c[6];
        this->frustum[4][2] = c[11] - c[10];
        this->frustum[4][3] = c[15] - c[14];
        this->normalize(this->frustum[4]);

        // near
        this->frustum[5][0] = c[3]  + c[2];
        this->frustum[5][1] = c[7]  + c[6];
        this->frustum[5][2] = c[11] + c[10];
        this->frustum[5][3] = c[15] + c[14];
        this->normalize(this->frustum[5]);
    }

};

inline ClippingHelperImpl ClippingHelperImpl::instance;


#endif //MCCLONE_CLIPPINGHELPERIMPL_H
