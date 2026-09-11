//
// Created by SleepyFish on 11.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_TEXTUREATLASSPRITE_H
#define MCCLONE_TEXTUREATLASSPRITE_H

#include <string>
#include <vector>
#include <optional>

class ResourceLocation;
class AnimationMetadataSection;
class BufferedImage;

using MipmapLevel = std::vector<int>;
using FrameData = std::vector<MipmapLevel>;
using FrameList = std::vector<FrameData>;

/**
 * @author SleepyFish
 * @brief Texture atlas sprite
 */
class TextureAtlasSprite {

protected:

    std::string iconName;

    FrameList framesTextureData;
    FrameData interpolatedFrameData;
    bool interpolatedFrameDataValid = false;

    AnimationMetadataSection* animationMetadata = nullptr;

    bool rotated = false;
    int originX = 0;
    int originY = 0;
    int width = 0;
    int height = 0;

    float minU = 0.0f;
    float maxU = 0.0f;
    float minV = 0.0f;
    float maxV = 0.0f;

    int frameCounter = 0;
    int tickCounter = 0;

    static std::string locationNameClock;
    static std::string locationNameCompass;

    int indexInMap = -1;

public:

    explicit TextureAtlasSprite(const std::string& spriteName);
    virtual ~TextureAtlasSprite() = default;

    static TextureAtlasSprite* makeAtlasSprite(const ResourceLocation& loc);

    static void setLocationNameClock(const std::string& clockName);
    static void setLocationNameCompass(const std::string& compassName);

    void initSprite(int inX, int inY, int originInX, int originInY, bool rotatedIn);

    void copyFrom(const TextureAtlasSprite& atlasSpirit);

    int getOriginX() const { return originX; }
    int getOriginY() const { return originY; }
    int getIconWidth()  const { return width; }
    int getIconHeight() const { return height; }

    float getMinU() const { return minU; }
    float getMaxU() const { return maxU; }
    float getInterpolatedU(double u) const {
        float f = maxU - minU;
        return minU + f * static_cast<float>(u) / 16.0f;
    }

    float getMinV() const { return minV; }
    float getMaxV() const { return maxV; }
    float getInterpolatedV(double v) const {
        float f = maxV - minV;
        return minV + f * static_cast<float>(v) / 16.0f;
    }

    std::string getIconName() const { return iconName; }

    virtual void updateAnimation();
    void updateAnimationInterpolated();

    const FrameData& getFrameTextureData(int index) const {
        return framesTextureData.at(index);
    }
    FrameData& getFrameTextureData(int index) {
        return framesTextureData.at(index);
    }

    int getFrameCount() const {
        return static_cast<int>(framesTextureData.size());
    }

    void setIconWidth(int newWidth) { width  = newWidth; }
    void setIconHeight(int newHeight) { height = newHeight; }

    void loadSprite(BufferedImage* const* images, int imageCount, const AnimationMetadataSection* meta);

    void generateMipmaps(int level);

    void clearFramesTextureData() { framesTextureData.clear(); }

    bool hasAnimationMetadata() const { return animationMetadata != nullptr; }

    void setFramesTextureData(const FrameList& newFramesTextureData) {
        framesTextureData = newFramesTextureData;
    }

    std::string toString() const;

    int glSpriteTextureId = -1;

    int getIndexInMap() const { return indexInMap; }

protected:

    void allocateFrameTextureData(int index);

    static FrameData getFrameTextureData(const FrameData& data, int rows, int columns, int frameIndex);

private:

    void resetSprite();

};


#endif //MCCLONE_TEXTUREATLASSPRITE_H
