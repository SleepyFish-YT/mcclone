//
// Created by SleepyFish on 10.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_WORLDRENDERER_H
#define MCCLONE_WORLDRENDERER_H

#include "vertex/VertexFormat.h"
#include "vertex/VertexFormatElement.h"
#include "../../util/EnumWorldBlockLayer.h"

#include <cstdint>
#include <vector>
#include <bitset>
#include <memory>
#include <stdexcept>

class TextureAtlasSprite;
class RenderEnv;
class IBlockState;
class BlockPos;

/**
 * @author SleepyFish
 * @brief World renderer
 */
class WorldRenderer {

public:

    class State {

    public:

        State(std::vector<int> buffer, VertexFormat format, std::vector<TextureAtlasSprite *> quadSprites);

        State(std::vector<int> buffer, VertexFormat format);

        const std::vector<int> &getRawBuffer() const { return stateRawBuffer; }

        const VertexFormat &getVertexFormat() const { return stateVertexFormat; }

        int getVertexCount() const;

        std::vector<TextureAtlasSprite *> stateQuadSprites;

    private:

        std::vector<int> stateRawBuffer;
        VertexFormat stateVertexFormat;

    };

    explicit WorldRenderer(int bufferSizeIn);

    ~WorldRenderer() = default;

    void begin(int glMode, const VertexFormat &format);

    void finishDrawing();

    void reset();

    WorldRenderer &pos(double x, double y, double z);

    WorldRenderer &color(float red, float green, float blue, float alpha);

    WorldRenderer &color(int red, int green, int blue, int alpha);

    WorldRenderer &tex(double u, double v);

    WorldRenderer &lightmap(int s, int t);

    WorldRenderer &normal(float x, float y, float z);

    void endVertex();

    void addVertexData(const std::vector<int> &vertexData);

    void putBrightness4(int v0, int v1, int v2, int v3);

    void putPosition(double x, double y, double z);

    void putColor4(int argb);

    void putColorRGB_F4(float red, float green, float blue);

    void putColorMultiplier(float red, float green, float blue, int vertexIndex);

    void putColorMultiplierRgba(float r, float g, float b, float a, int vertexIndex);

    void putColorRGB_F(float red, float green, float blue, int vertexIndex);

    void putColorRGBA(int index, int red, int green, int blue, int alpha);

    void putNormal(float x, float y, float z);

    void noColor();

    bool isColorDisabled() const { return m_noColor; }

    void setTranslation(double x, double y, double z);

    void sortVertexData(float cameraX, float cameraY, float cameraZ);

    State getVertexState() const;

    void setVertexState(const State &state);

    // void setSprite(TextureAtlasSprite* sprite);
    // void putSprite(TextureAtlasSprite* sprite);
    bool isMultiTexture() const { return m_quadSprites != nullptr; }

    void drawMultiTexture();

    void setBlockLayer(EnumWorldBlockLayer layer);

    EnumWorldBlockLayer getBlockLayer() const { return m_blockLayer; }

    void quadsToTriangles();

    const std::vector<uint8_t> &getByteBuffer() const;

    const VertexFormat &getVertexFormat() const { return m_vertexFormat; }

    int getVertexCount() const;

    int getDrawMode() const;

    int getBufferSize() const;

    bool isDrawing() const { return m_isDrawing; }

    double getXOffset() const { return m_xOffset; }

    double getYOffset() const { return m_yOffset; }

    double getZOffset() const { return m_zOffset; }

    RenderEnv *getRenderEnv(const IBlockState &blockState, const BlockPos &pos);

    std::vector<int> rawIntBuffer;
    int vertexCount = 0;
    int drawMode = 0;

    RenderEnv *renderEnv = nullptr;
    std::vector<bool> animatedSprites;
    std::vector<bool> animatedSpritesCached;

private:

    void growBuffer(int needed);

    int drawForIcon(TextureAtlasSprite *sprite, int startQuad);

    void draw(int firstQuad, int lastQuad);

    int getColorIndex(int vertexIndex) const;

    void putColor(int argb, int vertexIndex);

    void nextVertexFormatIndex();

    int getBufferQuadSize() const;

    static float
    getDistanceSq(const std::vector<float> &floatBuf, float cx, float cy, float cz, int integerSize, int offset);

    std::vector<uint8_t> m_byteBuffer;
    std::vector<uint8_t> m_byteBufferTriangles;

    VertexFormat m_vertexFormat;
    const VertexFormatElement *m_vertexFormatElement = nullptr;
    int m_vertexFormatIndex = 0;

    bool m_isDrawing = false;
    bool m_noColor = false;
    bool m_modeTriangles = false;

    double m_xOffset = 0.0;
    double m_yOffset = 0.0;
    double m_zOffset = 0.0;

    EnumWorldBlockLayer m_blockLayer = EnumWorldBlockLayer::SOLID;
    bool m_hasBlockLayer = false;

    std::vector<TextureAtlasSprite *> *m_quadSprites = nullptr;
    std::vector<TextureAtlasSprite *> *m_quadSpritesPrev = nullptr;
    TextureAtlasSprite *m_quadSprite = nullptr;

    std::vector<TextureAtlasSprite *> m_quadSpritesStorage;
    std::vector<TextureAtlasSprite *> m_quadSpritesPrevStorage;

    std::vector<bool> m_drawnIcons;

};


#endif //MCCLONE_WORLDRENDERER_H
