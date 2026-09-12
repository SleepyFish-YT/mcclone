//
// Created by SleepyFish on 10.09.2026.
// Project: mcclone
//

// not FULLY implemented yet.

#include "WorldRenderer.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>
#include <numeric>
#include <stdexcept>
#include <vector>

#include "vertex/VertexFormat.h"
#include "vertex/VertexFormatElement.h"
#include "texture/TextureAtlasSprite.h"
// #include "RenderEnv.h"
#include "../../block/state/IBlockState.h"
#include "../../util/BlockPos.h"
#include "GlStateManager.h"
#include <glad/glad.h>

namespace {

    inline bool isLittleEndian() {
        uint32_t v = 1;
        return *reinterpret_cast<uint8_t*>(&v) == 1;
    }

    inline int clampInt(int v, int lo, int hi) {
        return MathHelper::clamp_int(v, lo, hi);
    }

    inline float intBitsToFloat(int bits) {
        float f;
        std::memcpy(&f, &bits, 4);
        return f;
    }

    inline int floatToRawIntBits(float f) {
        int bits;
        std::memcpy(&bits, &f, 4);
        return bits;
    }

}

WorldRenderer::State::State(std::vector<int> buffer, VertexFormat format, std::vector<TextureAtlasSprite*> quadSprites) :
    stateRawBuffer(std::move(buffer)),
    stateVertexFormat(std::move(format)),
    stateQuadSprites(std::move(quadSprites))
{}

WorldRenderer::State::State(std::vector<int> buffer, VertexFormat format) :
    stateRawBuffer(std::move(buffer)),
    stateVertexFormat(std::move(format))
{}

int WorldRenderer::State::getVertexCount() const {
    return static_cast<int>(stateRawBuffer.size()) / stateVertexFormat.getIntegerSize();
}

WorldRenderer::WorldRenderer(int bufferSizeIn) {
    m_byteBuffer.resize(static_cast<size_t>(bufferSizeIn) * 4, 0);
    rawIntBuffer.resize(bufferSizeIn, 0);
    m_drawnIcons.resize(256, false);
}

void WorldRenderer::growBuffer(int needed) {
    int usedInts = getBufferSize();
    int remaining = static_cast<int>(rawIntBuffer.size()) - usedInts;

    if (needed <= remaining)
        return;

    constexpr int CHUNK = 2097152;
    int oldBytes = static_cast<int>(m_byteBuffer.size());
    int neededBytes = (usedInts + needed) * 4;
    int base = oldBytes % CHUNK;
    int newBytes = base + ((neededBytes - base) / CHUNK + 1) * CHUNK;

    m_byteBuffer.resize(static_cast<size_t>(newBytes), 0);
    rawIntBuffer.resize(static_cast<size_t>(newBytes / 4), 0);

    if (m_quadSprites) {
        int newQuadSize = getBufferQuadSize();
        std::vector<TextureAtlasSprite*> newSprites(newQuadSize, nullptr);
        int copyLen = std::min(static_cast<int>(m_quadSpritesStorage.size()), newQuadSize);
        std::copy(m_quadSpritesStorage.begin(),
                  m_quadSpritesStorage.begin() + copyLen,
                  newSprites.begin());
        m_quadSpritesStorage = std::move(newSprites);
        m_quadSprites = &m_quadSpritesStorage;
        m_quadSpritesPrev = nullptr;
    }
}

void WorldRenderer::reset() {
    vertexCount           = 0;
    m_vertexFormatElement = nullptr;
    m_vertexFormatIndex   = 0;
    m_quadSprite          = nullptr;
    m_modeTriangles       = false;
    animatedSprites.clear();
}

void WorldRenderer::begin(int glMode, const VertexFormat& format) {
    if (m_isDrawing)
        throw std::runtime_error("Already building!");

    m_isDrawing           = true;
    reset();
    drawMode              = glMode;
    m_vertexFormat        = format;
    m_vertexFormatElement = &m_vertexFormat.getElement(m_vertexFormatIndex);
    m_noColor             = false;

    if (m_hasBlockLayer) {
        if (!m_quadSprites)
            m_quadSprites = m_quadSpritesPrev;
        int needed = getBufferQuadSize();
        if (!m_quadSprites || static_cast<int>(m_quadSpritesStorage.size()) < needed) {
            m_quadSpritesStorage.assign(static_cast<size_t>(needed), nullptr);
            m_quadSprites = &m_quadSpritesStorage;
        }
    } else {
        if (m_quadSprites)
            m_quadSpritesPrev = m_quadSprites;
        m_quadSprites = nullptr;
    }
}

void WorldRenderer::finishDrawing() {
    if (!m_isDrawing)
        throw std::runtime_error("Not building!");

    m_isDrawing = false;
    m_byteBuffer.resize(static_cast<size_t>(getBufferSize()) * 4);
}

WorldRenderer& WorldRenderer::pos(double x, double y, double z) {
    int i = vertexCount * m_vertexFormat.getNextOffset() + m_vertexFormat.getOffset(m_vertexFormatIndex);

    auto type = m_vertexFormatElement->getType();
    using T = VertexFormatElement::EnumType;

    auto fx = static_cast<float>(x + m_xOffset);
    auto fy = static_cast<float>(y + m_yOffset);
    auto fz = static_cast<float>(z + m_zOffset);

    switch (type) {
        case T::FLOAT:
            std::memcpy(&m_byteBuffer[i],     &fx, 4);
            std::memcpy(&m_byteBuffer[i + 4], &fy, 4);
            std::memcpy(&m_byteBuffer[i + 8], &fz, 4);
            break;

        case T::UINT: case T::INT: {
            int ix = floatToRawIntBits(fx);
            int iy = floatToRawIntBits(fy);
            int iz = floatToRawIntBits(fz);
            std::memcpy(&m_byteBuffer[i],     &ix, 4);
            std::memcpy(&m_byteBuffer[i + 4], &iy, 4);
            std::memcpy(&m_byteBuffer[i + 8], &iz, 4);
            break;
        }

        case T::USHORT: case T::SHORT: {
            auto sx = static_cast<int16_t>(x + m_xOffset);
            auto sy = static_cast<int16_t>(y + m_yOffset);
            auto sz = static_cast<int16_t>(z + m_zOffset);
            std::memcpy(&m_byteBuffer[i],     &sx, 2);
            std::memcpy(&m_byteBuffer[i + 2], &sy, 2);
            std::memcpy(&m_byteBuffer[i + 4], &sz, 2);
            break;
        }

        case T::UBYTE: case T::BYTE:
            m_byteBuffer[i]     = static_cast<uint8_t>(x + m_xOffset);
            m_byteBuffer[i + 1] = static_cast<uint8_t>(y + m_yOffset);
            m_byteBuffer[i + 2] = static_cast<uint8_t>(z + m_zOffset);
            break;
    }

    nextVertexFormatIndex();
    return *this;
}

WorldRenderer& WorldRenderer::color(float red, float green, float blue, float alpha) {
    return color(static_cast<int>(red   * 255.0f),
                 static_cast<int>(green * 255.0f),
                 static_cast<int>(blue  * 255.0f),
                 static_cast<int>(alpha * 255.0f));
}

WorldRenderer& WorldRenderer::color(int red, int green, int blue, int alpha) {
    if (m_noColor) return *this;

    int i = vertexCount * m_vertexFormat.getNextOffset() + m_vertexFormat.getOffset(m_vertexFormatIndex);

    auto type = m_vertexFormatElement->getType();
    using T = VertexFormatElement::EnumType;

    switch (type) {
        case T::FLOAT: {
            const float fr = red / 255.0f, fg = green / 255.0f, fb = blue / 255.0f, fa = alpha / 255.0f;
            std::memcpy(&m_byteBuffer[i],      &fr, 4);
            std::memcpy(&m_byteBuffer[i + 4],  &fg, 4);
            std::memcpy(&m_byteBuffer[i + 8],  &fb, 4);
            std::memcpy(&m_byteBuffer[i + 12], &fa, 4);
            break;
        }

        case T::UINT: case T::INT: {
            auto fr = static_cast<float>(red),   fg = static_cast<float>(green);
            auto fb = static_cast<float>(blue),  fa = static_cast<float>(alpha);
            std::memcpy(&m_byteBuffer[i],      &fr, 4);
            std::memcpy(&m_byteBuffer[i + 4],  &fg, 4);
            std::memcpy(&m_byteBuffer[i + 8],  &fb, 4);
            std::memcpy(&m_byteBuffer[i + 12], &fa, 4);
            break;
        }

        case T::USHORT: case T::SHORT: {
            auto sr = static_cast<int16_t>(red),   sg = static_cast<int16_t>(green);
            auto sb = static_cast<int16_t>(blue),  sa = static_cast<int16_t>(alpha);
            std::memcpy(&m_byteBuffer[i],     &sr, 2);
            std::memcpy(&m_byteBuffer[i + 2], &sg, 2);
            std::memcpy(&m_byteBuffer[i + 4], &sb, 2);
            std::memcpy(&m_byteBuffer[i + 6], &sa, 2);
            break;
        }

        case T::UBYTE: case T::BYTE:
            if (isLittleEndian()) {
                m_byteBuffer[i]     = static_cast<uint8_t>(red);
                m_byteBuffer[i + 1] = static_cast<uint8_t>(green);
                m_byteBuffer[i + 2] = static_cast<uint8_t>(blue);
                m_byteBuffer[i + 3] = static_cast<uint8_t>(alpha);
            } else {
                m_byteBuffer[i]     = static_cast<uint8_t>(alpha);
                m_byteBuffer[i + 1] = static_cast<uint8_t>(blue);
                m_byteBuffer[i + 2] = static_cast<uint8_t>(green);
                m_byteBuffer[i + 3] = static_cast<uint8_t>(red);
            }
            break;
    }

    nextVertexFormatIndex();
    return *this;
}

WorldRenderer& WorldRenderer::tex(double u, double v) {
    int i = vertexCount * m_vertexFormat.getNextOffset() + m_vertexFormat.getOffset(m_vertexFormatIndex);

    auto type = m_vertexFormatElement->getType();
    using T = VertexFormatElement::EnumType;

    switch (type) {
        case T::FLOAT: {
            auto fu = static_cast<float>(u), fv = static_cast<float>(v);
            std::memcpy(&m_byteBuffer[i],     &fu, 4);
            std::memcpy(&m_byteBuffer[i + 4], &fv, 4);
            break;
        }

        case T::UINT: case T::INT: {
            auto iu = static_cast<int>(u), iv = static_cast<int>(v);
            std::memcpy(&m_byteBuffer[i],     &iu, 4);
            std::memcpy(&m_byteBuffer[i + 4], &iv, 4);
            break;
        }

        case T::USHORT: case T::SHORT: {
            auto sv = static_cast<int16_t>(v), su = static_cast<int16_t>(u);
            std::memcpy(&m_byteBuffer[i],     &sv, 2);
            std::memcpy(&m_byteBuffer[i + 2], &su, 2);
            break;
        }

        case T::UBYTE: case T::BYTE:
            m_byteBuffer[i]     = static_cast<uint8_t>(v);
            m_byteBuffer[i + 1] = static_cast<uint8_t>(u);
            break;
    }

    nextVertexFormatIndex();
    return *this;
}

WorldRenderer& WorldRenderer::lightmap(int s, int t) {
    int i = vertexCount * m_vertexFormat.getNextOffset() + m_vertexFormat.getOffset(m_vertexFormatIndex);

    auto type = m_vertexFormatElement->getType();
    using T = VertexFormatElement::EnumType;

    switch (type) {
        case T::FLOAT: {
            auto fs = static_cast<float>(s), ft = static_cast<float>(t);
            std::memcpy(&m_byteBuffer[i],     &fs, 4);
            std::memcpy(&m_byteBuffer[i + 4], &ft, 4);
            break;
        }

        case T::UINT: case T::INT:
            std::memcpy(&m_byteBuffer[i],     &s, 4);
            std::memcpy(&m_byteBuffer[i + 4], &t, 4);
            break;

        case T::USHORT: case T::SHORT: {
            auto st = static_cast<int16_t>(t), ss = static_cast<int16_t>(s);
            std::memcpy(&m_byteBuffer[i],     &st, 2);
            std::memcpy(&m_byteBuffer[i + 2], &ss, 2);
            break;
        }

        case T::UBYTE: case T::BYTE:
            m_byteBuffer[i]     = static_cast<uint8_t>(t);
            m_byteBuffer[i + 1] = static_cast<uint8_t>(s);
            break;
    }

    nextVertexFormatIndex();
    return *this;
}

WorldRenderer& WorldRenderer::normal(float x, float y, float z) {
    int i = vertexCount * m_vertexFormat.getNextOffset() + m_vertexFormat.getOffset(m_vertexFormatIndex);

    auto type = m_vertexFormatElement->getType();
    using T = VertexFormatElement::EnumType;

    switch (type) {
        case T::FLOAT:
            std::memcpy(&m_byteBuffer[i],     &x, 4);
            std::memcpy(&m_byteBuffer[i + 4], &y, 4);
            std::memcpy(&m_byteBuffer[i + 8], &z, 4);
            break;

        case T::UINT: case T::INT: {
            int ix = static_cast<int>(x), iy = static_cast<int>(y), iz = static_cast<int>(z);
            std::memcpy(&m_byteBuffer[i],     &ix, 4);
            std::memcpy(&m_byteBuffer[i + 4], &iy, 4);
            std::memcpy(&m_byteBuffer[i + 8], &iz, 4);
            break;
        }

        case T::USHORT: case T::SHORT: {
            auto sx = static_cast<int16_t>(static_cast<int>(x * 32767.0f) & 0xFFFF);
            auto sy = static_cast<int16_t>(static_cast<int>(y * 32767.0f) & 0xFFFF);
            auto sz = static_cast<int16_t>(static_cast<int>(z * 32767.0f) & 0xFFFF);
            std::memcpy(&m_byteBuffer[i],     &sx, 2);
            std::memcpy(&m_byteBuffer[i + 2], &sy, 2);
            std::memcpy(&m_byteBuffer[i + 4], &sz, 2);
            break;
        }

        case T::UBYTE: case T::BYTE:
            m_byteBuffer[i]     = static_cast<uint8_t>(static_cast<int>(x * 127.0f) & 0xFF);
            m_byteBuffer[i + 1] = static_cast<uint8_t>(static_cast<int>(y * 127.0f) & 0xFF);
            m_byteBuffer[i + 2] = static_cast<uint8_t>(static_cast<int>(z * 127.0f) & 0xFF);
            break;
    }

    nextVertexFormatIndex();
    return *this;
}

void WorldRenderer::endVertex() {
    ++vertexCount;
    growBuffer(m_vertexFormat.getIntegerSize());
    m_vertexFormatIndex   = 0;
    m_vertexFormatElement = &m_vertexFormat.getElement(m_vertexFormatIndex);
}

void WorldRenderer::addVertexData(const std::vector<int>& vertexData) {
    growBuffer(static_cast<int>(vertexData.size()));
    int pos = getBufferSize();
    std::copy(vertexData.begin(), vertexData.end(), rawIntBuffer.begin() + pos);
    vertexCount += static_cast<int>(vertexData.size()) / m_vertexFormat.getIntegerSize();
}

void WorldRenderer::putBrightness4(int v0, int v1, int v2, int v3) {
    int i = (vertexCount - 4) * m_vertexFormat.getIntegerSize() + m_vertexFormat.getUvOffsetById(1) / 4;
    int stride = m_vertexFormat.getNextOffset() >> 2;
    rawIntBuffer[i]              = v0;
    rawIntBuffer[i + stride]     = v1;
    rawIntBuffer[i + stride * 2] = v2;
    rawIntBuffer[i + stride * 3] = v3;
}

void WorldRenderer::putPosition(double x, double y, double z) {
    int intSize = m_vertexFormat.getIntegerSize();
    int base    = (vertexCount - 4) * intSize;

    for (int k = 0; k < 4; ++k) {
        int l  = base + k * intSize;
        int i1 = l + 1;
        int j1 = l + 2;
        rawIntBuffer[l]  = floatToRawIntBits(static_cast<float>(x + m_xOffset) + intBitsToFloat(rawIntBuffer[l]));
        rawIntBuffer[i1] = floatToRawIntBits(static_cast<float>(y + m_yOffset) + intBitsToFloat(rawIntBuffer[i1]));
        rawIntBuffer[j1] = floatToRawIntBits(static_cast<float>(z + m_zOffset) + intBitsToFloat(rawIntBuffer[j1]));
    }
}

void WorldRenderer::putColor4(int argb) {
    for (int i = 0; i < 4; ++i)
        putColor(argb, i + 1);
}

void WorldRenderer::putColorRGB_F4(float red, float green, float blue) {
    for (int i = 0; i < 4; ++i)
        putColorRGB_F(red, green, blue, i + 1);
}

void WorldRenderer::putNormal(float x, float y, float z) {
    int xi = static_cast<uint8_t>(static_cast<int>(x * 127.0f));
    int yi = static_cast<uint8_t>(static_cast<int>(y * 127.0f));
    int zi = static_cast<uint8_t>(static_cast<int>(z * 127.0f));
    int packed = xi | (yi << 8) | (zi << 16);

    int stride = m_vertexFormat.getNextOffset() >> 2;
    int base   = (vertexCount - 4) * stride + m_vertexFormat.getNormalOffset() / 4;
    rawIntBuffer[base]              = packed;
    rawIntBuffer[base + stride]     = packed;
    rawIntBuffer[base + stride * 2] = packed;
    rawIntBuffer[base + stride * 3] = packed;
}

int WorldRenderer::getColorIndex(int vertexIndex) const {
    return ((vertexCount - vertexIndex) * m_vertexFormat.getNextOffset() + m_vertexFormat.getColorOffset()) / 4;
}

void WorldRenderer::putColor(int argb, int vertexIndex) {
    int i = getColorIndex(vertexIndex);
    int r = (argb >> 16) & 0xFF;
    int g = (argb >> 8)  & 0xFF;
    int b =  argb        & 0xFF;
    int a = (argb >> 24) & 0xFF;
    putColorRGBA(i, r, g, b, a);
}

void WorldRenderer::putColorRGB_F(float red, float green, float blue, int vertexIndex) {
    int i = getColorIndex(vertexIndex);
    int r = clampInt(static_cast<int>(red   * 255.0f), 0, 255);
    int g = clampInt(static_cast<int>(green * 255.0f), 0, 255);
    int b = clampInt(static_cast<int>(blue  * 255.0f), 0, 255);
    putColorRGBA(i, r, g, b, 255);
}

void WorldRenderer::putColorRGBA(int index, int red, int green, int blue, int alpha) {
    if (isLittleEndian())
        rawIntBuffer[index] = (alpha << 24) | (blue << 16) | (green << 8) | red;
    else
        rawIntBuffer[index] = (red << 24) | (green << 16) | (blue << 8) | alpha;
}

void WorldRenderer::putColorMultiplier(float red, float green, float blue, int vertexIndex) {
    if (m_noColor) return;

    int i = getColorIndex(vertexIndex);
    int j = rawIntBuffer[i];

    if (isLittleEndian()) {
        auto r = static_cast<int>((j        & 0xFF) * red);
        auto g = static_cast<int>((j >>  8  & 0xFF) * green);
        auto b = static_cast<int>((j >> 16  & 0xFF) * blue);
        j = (j & 0xFF000000) | (b << 16) | (g << 8) | r;
    } else {
        auto r = static_cast<int>((j >> 24 & 0xFF) * red);
        auto g = static_cast<int>((j >> 16 & 0xFF) * green);
        auto b = static_cast<int>((j >>  8 & 0xFF) * blue);
        j = (j & 0xFF) | (r << 24) | (g << 16) | (b << 8);
    }

    rawIntBuffer[i] = j;
}

void WorldRenderer::putColorMultiplierRgba(float r, float g, float b, float a, int vertexIndex) {
    if (m_noColor) return;

    int i = getColorIndex(vertexIndex);
    int j = rawIntBuffer[i];

    if (isLittleEndian()) {
        auto ri = static_cast<int>((j        & 0xFF) * r);
        auto gi = static_cast<int>((j >>  8  & 0xFF) * g);
        auto bi = static_cast<int>((j >> 16  & 0xFF) * b);
        auto ai = static_cast<int>((j >> 24  & 0xFF) * a);
        j = (ai << 24) | (bi << 16) | (gi << 8) | ri;
    } else {
        auto ri = static_cast<int>((j >> 24 & 0xFF) * r);
        auto gi = static_cast<int>((j >> 16 & 0xFF) * g);
        auto bi = static_cast<int>((j >>  8 & 0xFF) * b);
        auto ai = static_cast<int>((j       & 0xFF) * a);
        j = (ri << 24) | (gi << 16) | (bi << 8) | ai;
    }

    rawIntBuffer[i] = j;
}

void WorldRenderer::noColor() {
    m_noColor = true;
}

void WorldRenderer::setTranslation(double x, double y, double z) {
    m_xOffset = x;
    m_yOffset = y;
    m_zOffset = z;
}

float WorldRenderer::getDistanceSq(const std::vector<float>& fb,
                                   float cx, float cy, float cz,
                                   int intSize, int offset) {
    float x0 = fb[offset],              y0 = fb[offset + 1],             z0 = fb[offset + 2];
    float x1 = fb[offset + intSize],    y1 = fb[offset + intSize + 1],   z1 = fb[offset + intSize + 2];
    float x2 = fb[offset + intSize*2],  y2 = fb[offset + intSize*2 + 1], z2 = fb[offset + intSize*2 + 2];
    float x3 = fb[offset + intSize*3],  y3 = fb[offset + intSize*3 + 1], z3 = fb[offset + intSize*3 + 2];

    float dx = (x0 + x1 + x2 + x3) * 0.25f - cx;
    float dy = (y0 + y1 + y2 + y3) * 0.25f - cy;
    float dz = (z0 + z1 + z2 + z3) * 0.25f - cz;
    return dx*dx + dy*dy + dz*dz;
}

void WorldRenderer::sortVertexData(float camX, float camY, float camZ) {
    int quadCount = vertexCount / 4;

    std::vector<float> floatView(rawIntBuffer.size());
    for (size_t n = 0; n < rawIntBuffer.size(); ++n)
        floatView[n] = intBitsToFloat(rawIntBuffer[n]);

    std::vector<float> distances(quadCount);
    int intSize  = m_vertexFormat.getIntegerSize();
    int nextOffF = m_vertexFormat.getNextOffset() / 4;

    auto ox = static_cast<float>(camX + m_xOffset);
    auto oy = static_cast<float>(camY + m_yOffset);
    auto oz = static_cast<float>(camZ + m_zOffset);

    for (int q = 0; q < quadCount; ++q)
        distances[q] = getDistanceSq(floatView, ox, oy, oz, intSize, q * nextOffF);

    std::vector<int> order(quadCount);
    std::iota(order.begin(), order.end(), 0);
    std::sort(order.begin(), order.end(), [&](int a, int b) {
        return distances[a] > distances[b];
    });

    std::vector<bool> done(quadCount, false);
    std::vector<int>  tmp(intSize);

    for (int i = 0; i < quadCount; ++i) {
        if (done[i] || order[i] == i) { done[i] = true; continue; }

        std::copy(rawIntBuffer.begin() + i * intSize,
                  rawIntBuffer.begin() + i * intSize + intSize,
                  tmp.begin());

        int cur = i;
        while (true) {
            int next = order[cur];
            done[cur] = true;
            if (next == i) {
                std::copy(tmp.begin(), tmp.end(), rawIntBuffer.begin() + cur * intSize);
                break;
            }
            std::copy(rawIntBuffer.begin() + next * intSize,
                      rawIntBuffer.begin() + next * intSize + intSize,
                      rawIntBuffer.begin() + cur * intSize);
            cur = next;
        }
    }

    if (m_quadSprites) {
        auto& qs = *m_quadSprites;
        std::vector<TextureAtlasSprite*> sorted(quadCount);
        for (int q = 0; q < quadCount; ++q)
            sorted[q] = qs[order[q]];
        std::copy(sorted.begin(), sorted.end(), qs.begin());
    }
}

WorldRenderer::State WorldRenderer::getVertexState() const {
    int sz = getBufferSize();
    std::vector<int> buf(rawIntBuffer.begin(), rawIntBuffer.begin() + sz);

    std::vector<TextureAtlasSprite*> sprites;
    if (m_quadSprites) {
        int quadCount = vertexCount / 4;
        sprites.assign((*m_quadSprites).begin(), (*m_quadSprites).begin() + quadCount);
    }

    return {
        std::move(buf),
        VertexFormat(m_vertexFormat),
        std::move(sprites)
    };
}

void WorldRenderer::setVertexState(const State& state) {
    int needed = static_cast<int>(state.getRawBuffer().size());
    growBuffer(needed);

    std::copy(state.getRawBuffer().begin(), state.getRawBuffer().end(), rawIntBuffer.begin());
    vertexCount    = state.getVertexCount();
    m_vertexFormat = VertexFormat(state.getVertexFormat());

    if (!state.stateQuadSprites.empty()) {
        if (!m_quadSprites)
            m_quadSprites = m_quadSpritesPrev;

        int needed2 = getBufferQuadSize();
        if (!m_quadSprites || static_cast<int>(m_quadSpritesStorage.size()) < needed2) {
            m_quadSpritesStorage.assign(static_cast<size_t>(needed2), nullptr);
            m_quadSprites = &m_quadSpritesStorage;
        }
        std::copy(state.stateQuadSprites.begin(), state.stateQuadSprites.end(),
                  m_quadSpritesStorage.begin());
    } else {
        if (m_quadSprites)
            m_quadSpritesPrev = m_quadSprites;
        m_quadSprites = nullptr;
    }
}

/*
void WorldRenderer::setSprite(TextureAtlasSprite* sprite) {
    if (!animatedSprites.empty() && sprite && sprite->getAnimationIndex() >= 0)
        animatedSprites[sprite->getAnimationIndex()] = true;

    if (m_quadSprites)
        m_quadSprite = sprite;
}

void WorldRenderer::putSprite(TextureAtlasSprite* sprite) {
    if (!animatedSprites.empty() && sprite && sprite->getAnimationIndex() >= 0)
        animatedSprites[sprite->getAnimationIndex()] = true;

    if (m_quadSprites) {
        int idx = vertexCount / 4;
        (*m_quadSprites)[idx - 1] = sprite;
    }
}
*/

void WorldRenderer::setBlockLayer(EnumWorldBlockLayer layer) {
    m_blockLayer    = layer;
    m_hasBlockLayer = true;

    if (m_quadSprites)
        m_quadSpritesPrev = m_quadSprites;
    m_quadSprites = nullptr;
    m_quadSprite  = nullptr;
}

void WorldRenderer::drawMultiTexture() {
    if (!m_quadSprites) return;

    int quadCount = vertexCount / 4;
    m_drawnIcons.assign(m_drawnIcons.size(), false);

    int drawnCount        = 0;
    int grassOverlayFirst = -1;

    for (int i = 0; i < quadCount; ++i) {
        TextureAtlasSprite* sprite = (*m_quadSprites)[i];
        if (!sprite) continue;

        int idx = sprite->getIndexInMap();
        if (static_cast<int>(m_drawnIcons.size()) <= idx)
            m_drawnIcons.resize(static_cast<size_t>(idx + 1), false);

        if (m_drawnIcons[idx]) continue;

        /* if (sprite == TextureAtlasSprite::getGrassSideOverlay()) {
            if (grassOverlayFirst < 0) grassOverlayFirst = i;
        } else */ {
            i = drawForIcon(sprite, i) - 1;
            ++drawnCount;
            if (m_blockLayer != EnumWorldBlockLayer::TRANSLUCENT)
                m_drawnIcons[idx] = true;
        }
    }

    if (grassOverlayFirst >= 0) {
        // drawForIcon(TextureAtlasSprite::getGrassSideOverlay(), grassOverlayFirst);
        ++drawnCount;
    }
}

int WorldRenderer::drawForIcon(TextureAtlasSprite* target, int startQuad) {
    ::glBindTexture(GL_TEXTURE_2D, target->glSpriteTextureId);

    int firstNotDrawn = -1;
    int runStart      = -1;
    int quadCount     = vertexCount / 4;

    for (int q = startQuad; q < quadCount; ++q) {
        TextureAtlasSprite* sprite = (*m_quadSprites)[q];

        if (sprite == target) {
            if (runStart < 0) runStart = q;
        } else if (runStart >= 0) {
            draw(runStart, q);

            if (m_blockLayer == EnumWorldBlockLayer::TRANSLUCENT)
                return q;

            runStart = -1;
            if (firstNotDrawn < 0) firstNotDrawn = q;
        }
    }

    if (runStart >= 0)
        draw(runStart, quadCount);

    return firstNotDrawn >= 0 ? firstNotDrawn : quadCount;
}

void WorldRenderer::draw(int firstQuad, int lastQuad) {
    int count = lastQuad - firstQuad;
    if (count > 0)
        GlStateManager::glDrawArrays_(drawMode, firstQuad * 4, count * 4);
}

void WorldRenderer::quadsToTriangles() {
    if (drawMode != 7) return;

    int stride = m_vertexFormat.getNextOffset();
    int needed = static_cast<int>(m_byteBuffer.size()) * 2;

    if (static_cast<int>(m_byteBufferTriangles.size()) < needed)
        m_byteBufferTriangles.resize(static_cast<size_t>(needed), 0);

    m_byteBufferTriangles.clear();

    for (int k = 0; k < vertexCount; k += 4) {
        auto base0 = m_byteBuffer.begin() + k * stride;
        m_byteBufferTriangles.insert(m_byteBufferTriangles.end(), base0, base0 + 3 * stride);
        m_byteBufferTriangles.insert(m_byteBufferTriangles.end(), base0, base0 + stride);
        auto base2 = m_byteBuffer.begin() + (k + 2) * stride;
        m_byteBufferTriangles.insert(m_byteBufferTriangles.end(), base2, base2 + 2 * stride);
    }

    m_modeTriangles = true;
}

const std::vector<uint8_t>& WorldRenderer::getByteBuffer() const {
    return m_modeTriangles ? m_byteBufferTriangles : m_byteBuffer;
}

int WorldRenderer::getVertexCount() const {
    return m_modeTriangles ? (vertexCount / 4 * 6) : vertexCount;
}

int WorldRenderer::getDrawMode() const {
    return m_modeTriangles ? GL_TRIANGLES : drawMode;
}

int WorldRenderer::getBufferSize() const {
    return vertexCount * m_vertexFormat.getIntegerSize();
}

int WorldRenderer::getBufferQuadSize() const {
    return static_cast<int>(rawIntBuffer.size()) / (m_vertexFormat.getIntegerSize() * 4);
}

void WorldRenderer::nextVertexFormatIndex() {
    ++m_vertexFormatIndex;
    m_vertexFormatIndex %= m_vertexFormat.getElementCount();
    m_vertexFormatElement = &m_vertexFormat.getElement(m_vertexFormatIndex);

    if (m_vertexFormatElement->getUsage() == VertexFormatElement::EnumUsage::PADDING) {
        nextVertexFormatIndex();
    }
}

RenderEnv* WorldRenderer::getRenderEnv(const IBlockState& blockState, const BlockPos& pos) {
    /*if (!renderEnv) {
        renderEnv = new RenderEnv(blockState, pos);
    } else {
        renderEnv->reset(blockState, pos);
    }*/

    return renderEnv;
}
