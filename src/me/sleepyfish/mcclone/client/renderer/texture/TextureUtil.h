//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_TEXTUREUTIL_H
#define MCCLONE_TEXTUREUTIL_H

// not FULLY implemented yet.
// #include "DynamicTexture.h"

#include <string>
#include <vector>

class TextureAtlasSprite;

/**
 * @author SleepyFish
 * @brief Texture utility - upload, allocate, mipmap generation
 */
class TextureUtil {

private:

    static std::vector<int> dataBuffer_;
    static std::vector<int> dataArray_;
    static int              mipmapBuffer_[4];

    static void uploadTextureSub_(
            int level,
            const int* pixels,
            int width,
            int height,
            int xOffset,
            int yOffset,
            bool blur,
            bool clamp,
            bool mipmap
    );

    static void uploadTextureImageSubImpl_(
            const int* pixels,
            int imgWidth,
            int imgHeight,
            int xOffset,
            int yOffset,
            bool blur,
            bool clamp
    );

    static void setTextureBlurred_(bool blur);
    static void copyToBuffer_(const int* pixels, int count);
    static void copyToBufferPos_(const int* pixels, int offset, int count);

public:

    // static DynamicTexture missingTexture;
    static int* missingTextureData;

    static void init();

    // -------------------------------------------------------------------------
    // Texture gen / delete
    // -------------------------------------------------------------------------
    static int  glGenTextures_();
    static void deleteTexture_(int textureId);
    static void bindTexture_(int textureId);

    // -------------------------------------------------------------------------
    // Upload
    // -------------------------------------------------------------------------
    static int  uploadTextureImage_(int textureId, const int* pixels, int width, int height);
    static void uploadTexture_(int textureId, const int* pixels, int width, int height);
    static int  uploadTextureImageAllocate_(int textureId, const int* pixels, int width, int height, bool blur, bool clamp);
    static int  uploadTextureImageSub_(int textureId, const int* pixels, int imgWidth, int imgHeight, int xOffset, int yOffset, bool blur, bool clamp);
    static void uploadTextureMipmap_(const std::vector<std::vector<int>>& mipmaps, int width, int height, int xOffset, int yOffset, bool blur, bool clamp);

    // -------------------------------------------------------------------------
    // Allocate
    // -------------------------------------------------------------------------
    static void allocateTexture_(int textureId, int width, int height);
    static void allocateTextureImpl_(int textureId, int mipmapLevels, int width, int height);

    // -------------------------------------------------------------------------
    // Mipmap
    // -------------------------------------------------------------------------
    static std::vector<std::vector<int>> generateMipmapData_(int levels, int width, const std::vector<std::vector<int>>& data);

    // -------------------------------------------------------------------------
    // Filter / wrap
    // -------------------------------------------------------------------------
    static void setTextureClamped_(bool clamp);
    static void setTextureBlurMipmap_(bool blur, bool mipmap);

    // -------------------------------------------------------------------------
    // Image loading (stb_image)
    // -------------------------------------------------------------------------
    static std::vector<int> readImageData_(const std::string& path, int& widthOut, int& heightOut);

    // -------------------------------------------------------------------------
    // Pixel ops
    // -------------------------------------------------------------------------
    static std::vector<int> updateAnaglyph_(const std::vector<int>& pixels);
    static int              anaglyphColor_(int color);
    static void             processPixelValues_(int* pixels, int width, int height);

    // -------------------------------------------------------------------------
    // Color blend (mipmap generation)
    // -------------------------------------------------------------------------
    static int blendColors_(int a, int b, int c, int d, bool hasAlpha);
    static int blendColorComponent_(int a, int b, int c, int d, int shift);

};


#endif //MCCLONE_TEXTUREUTIL_H
