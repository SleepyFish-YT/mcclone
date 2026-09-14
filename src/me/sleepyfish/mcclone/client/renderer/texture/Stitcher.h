//
// Created by SleepyFish on 14.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_STITCHER_H
#define MCCLONE_STITCHER_H

#include <vector>
#include <unordered_set>
#include <string>
#include <algorithm>

class TextureAtlasSprite;
class StitcherException;

/**
 * @author SleepyFish
 * @brief Texture stitcher
 */
class Stitcher {

public:

    class Holder;
    class Slot;

    Stitcher(int maxWidth, int maxHeight, bool forcePowerOf2, int maxTileDimension, int mipmapLevel) :
        mipmapLevelStitcher(mipmapLevel),
        maxWidth(maxWidth),
        maxHeight(maxHeight),
        forcePowerOf2(forcePowerOf2),
        maxTileDimension(maxTileDimension),
        currentWidth(0),
        currentHeight(0)
    {}

    int getCurrentWidth() const { return this->currentWidth; }

    int getCurrentHeight() const { return this->currentHeight; }

    void addSprite(TextureAtlasSprite *sprite);

    void doStitch();

    std::vector<TextureAtlasSprite *> getStitchSlots();

    class Holder {

    public:

        Holder(TextureAtlasSprite *sprite, int mipmapLevel);

        TextureAtlasSprite *getAtlasSprite() const { return this->theTexture; }

        int getWidth() const;
        int getHeight() const;

        void rotate() { this->rotated = !this->rotated; }
        bool isRotated() const { return this->rotated; }

        void setNewDimension(int maxDim);

        bool operator<(const Holder &other) const;

    private:

        TextureAtlasSprite *theTexture;
        int width;
        int height;
        int mipmapLevelHolder;
        bool rotated;
        float scaleFactor = 1.0f;

    };

    class Slot {

    public:

        Slot(int originX, int originY, int width, int height) :
            originX(originX),
            originY(originY),
            width(width),
            height(height),
            holder(nullptr)
        {}

        Holder *getStitchHolder() const { return this->holder; }

        int getOriginX() const { return this->originX; }
        int getOriginY() const { return this->originY; }

        bool addSlot(Holder *holderIn);
        void getAllStitchSlots(std::vector<Slot *> &result);

    private:

        int originX;
        int originY;
        int width;
        int height;
        std::vector<Slot> subSlots;
        Holder *holder;

    };

private:

    int mipmapLevelStitcher;
    int currentWidth;
    int currentHeight;
    int maxWidth;
    int maxHeight;
    bool forcePowerOf2;
    int maxTileDimension;

    std::vector<Holder> setStitchHolders;
    std::vector<Slot> stitchSlots;

    static int getMipmapDimension(int value, int mipmapLevel);

    bool allocateSlot(Holder &holder);

    bool expandAndAllocateSlot(Holder &holder);

};


#endif //MCCLONE_STITCHER_H
