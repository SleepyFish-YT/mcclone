//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#include "TextureUtil.h"

#include "../GlStateManager.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <stdexcept>

// -------------------------------------------------------------------------
// Static member definitions
// -------------------------------------------------------------------------

std::vector<int> TextureUtil::dataBuffer_        = std::vector<int>(4194304);
std::vector<int> TextureUtil::dataArray_         = std::vector<int>(4194304);
int              TextureUtil::mipmapBuffer_[4]   = {};
// DynamicTexture   TextureUtil::missingTexture     = DynamicTexture(16, 16);
int*             TextureUtil::missingTextureData = nullptr;

void TextureUtil::init() {
    // missingTextureData = missingTexture.getTextureData();

    constexpr int MAGENTA = -524040;    // 0xFF7F007F
    constexpr int BLACK   = -16777216;  // 0xFF000000
    constexpr int HALF    = 8;

    int magentaRow[8] = { MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA, MAGENTA };
    int blackRow[8]   = { BLACK,   BLACK,   BLACK,   BLACK,   BLACK,   BLACK,   BLACK,   BLACK   };

    for (int l = 0; l < 16; ++l) {
        const int* first  = l < HALF ? magentaRow : blackRow;
        const int* second = l < HALF ? blackRow   : magentaRow;
        std::memcpy(missingTextureData + 16 * l,         first,  HALF * sizeof(int));
        std::memcpy(missingTextureData + 16 * l + HALF,  second, HALF * sizeof(int));
    }

    // missingTexture.updateDynamicTexture();
}

// -------------------------------------------------------------------------
// Texture gen / delete
// -------------------------------------------------------------------------

int  TextureUtil::glGenTextures_()         { return GlStateManager::generateTexture_(); }
void TextureUtil::deleteTexture_(int id)   { GlStateManager::deleteTexture_(id);        }
void TextureUtil::bindTexture_(int id)     { GlStateManager::bindTexture_(id);          }

// -------------------------------------------------------------------------
// Upload
// -------------------------------------------------------------------------

int TextureUtil::uploadTextureImage_(int textureId, const int* pixels, int width, int height) {
    return uploadTextureImageAllocate_(textureId, pixels, width, height, false, false);
}

void TextureUtil::uploadTexture_(int textureId, const int* pixels, int width, int height) {
    bindTexture_(textureId);
    uploadTextureSub_(0, pixels, width, height, 0, 0, false, false, false);
}

int TextureUtil::uploadTextureImageAllocate_(int textureId, const int* pixels, int width, int height, bool blur, bool clamp) {
    allocateTexture_(textureId, width, height);
    return uploadTextureImageSub_(textureId, pixels, width, height, 0, 0, blur, clamp);
}

int TextureUtil::uploadTextureImageSub_(int textureId, const int* pixels, int imgWidth, int imgHeight, int xOffset, int yOffset, bool blur, bool clamp) {
    bindTexture_(textureId);
    uploadTextureImageSubImpl_(pixels, imgWidth, imgHeight, xOffset, yOffset, blur, clamp);
    return textureId;
}

void TextureUtil::uploadTextureImageSubImpl_(const int* pixels, int imgWidth, int imgHeight, int xOffset, int yOffset, bool blur, bool clamp) {
    const int rowsPerBatch = 4194304 / imgWidth;
    setTextureBlurred_(blur);
    setTextureClamped_(clamp);

    for (int offset = 0; offset < imgWidth * imgHeight; offset += imgWidth * rowsPerBatch) {
        int row        = offset / imgWidth;
        int rowCount   = std::min(rowsPerBatch, imgHeight - row);
        int pixelCount = imgWidth * rowCount;
        copyToBuffer_(pixels + offset, pixelCount);
        glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset + row, imgWidth, rowCount, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, dataBuffer_.data());
    }
}

void TextureUtil::uploadTextureSub_(int level, const int* pixels, int width, int height, int xOffset, int yOffset, bool blur, bool clamp, bool mipmap) {
    const int rowsPerBatch = 4194304 / width;
    setTextureBlurMipmap_(blur, mipmap);
    setTextureClamped_(clamp);

    for (int offset = 0; offset < width * height; offset += width * rowsPerBatch) {
        int row      = offset / width;
        int rowCount = std::min(rowsPerBatch, height - row);
        int count    = width * rowCount;
        copyToBufferPos_(pixels, offset, count);
        glTexSubImage2D(GL_TEXTURE_2D, level, xOffset, yOffset + row, width, rowCount, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, dataBuffer_.data());
    }
}

void TextureUtil::uploadTextureMipmap_(const std::vector<std::vector<int>>& mipmaps, int width, int height, int xOffset, int yOffset, bool blur, bool clamp) {
    for (int i = 0; i < static_cast<int>(mipmaps.size()); ++i) {
        uploadTextureSub_(
                i,
                mipmaps[i].data(),
                width  >> i,
                height >> i,
                xOffset >> i,
                yOffset >> i,
                blur,
                clamp,
                mipmaps.size() > 1
        );
    }
}

// -------------------------------------------------------------------------
// Allocate
// -------------------------------------------------------------------------

void TextureUtil::allocateTexture_(int textureId, int width, int height) {
    allocateTextureImpl_(textureId, 0, width, height);
}

void TextureUtil::allocateTextureImpl_(int textureId, int mipmapLevels, int width, int height) {
    deleteTexture_(textureId);
    bindTexture_(textureId);

    if (mipmapLevels >= 0) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipmapLevels);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD,  0.0f);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD,  static_cast<float>(mipmapLevels));
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0.0f);
    }

    for (int i = 0; i <= mipmapLevels; ++i) {
        glTexImage2D(GL_TEXTURE_2D, i, GL_RGBA, width >> i, height >> i, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, nullptr);
    }
}

// -------------------------------------------------------------------------
// Mipmap generation
// -------------------------------------------------------------------------

std::vector<std::vector<int>> TextureUtil::generateMipmapData_(int levels, int width, const std::vector<std::vector<int>>& data) {
    std::vector<std::vector<int>> result(levels + 1);
    result[0] = data[0];

    if (levels <= 0) return result;

    bool hasAlpha = false;
    for (int pixel : data[0]) {
        if ((pixel >> 24) == 0) {
            hasAlpha = true;
            break;
        }
    }

    for (int l = 1; l <= levels; ++l) {
        if (!data[l].empty()) {
            result[l] = data[l];
        } else {
            const std::vector<int>& prev = result[l - 1];
            int w = width >> l;
            int h = static_cast<int>(prev.size()) / w / 2; // half height
            int wPrev = w * 2;
            std::vector<int> mip(prev.size() / 4);

            for (int x = 0; x < w; ++x) {
                for (int y = 0; y < h; ++y) {
                    int k = 2 * (x + y * wPrev);
                    mip[x + y * w] = blendColors_(
                            prev[k],
                            prev[k + 1],
                            prev[k + wPrev],
                            prev[k + wPrev + 1],
                            hasAlpha
                    );
                }
            }

            result[l] = std::move(mip);
        }
    }

    return result;
}

int TextureUtil::blendColors_(int a, int b, int c, int d, bool hasAlpha) {
    // Gamma-correct average per channel
    int alpha = ((a >> 24 & 0xFF) + (b >> 24 & 0xFF) + (c >> 24 & 0xFF) + (d >> 24 & 0xFF)) / 4;
    int red   = blendColorComponent_(a, b, c, d, 16);
    int green = blendColorComponent_(a, b, c, d, 8);
    int blue  = blendColorComponent_(a, b, c, d, 0);
    return (alpha << 24) | (red << 16) | (green << 8) | blue;
}

int TextureUtil::blendColorComponent_(int a, int b, int c, int d, int shift) {
    float fa = std::pow((a >> shift & 0xFF) / 255.0f, 2.2f);
    float fb = std::pow((b >> shift & 0xFF) / 255.0f, 2.2f);
    float fc = std::pow((c >> shift & 0xFF) / 255.0f, 2.2f);
    float fd = std::pow((d >> shift & 0xFF) / 255.0f, 2.2f);
    float avg = std::pow((fa + fb + fc + fd) * 0.25f, 1.0f / 2.2f);
    return static_cast<int>(avg * 255.0f);
}

// -------------------------------------------------------------------------
// Filter / wrap
// -------------------------------------------------------------------------

void TextureUtil::setTextureClamped_(bool clamp) {
    if (clamp) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
}

void TextureUtil::setTextureBlurred_(bool blur) {
    setTextureBlurMipmap_(blur, false);
}

void TextureUtil::setTextureBlurMipmap_(bool blur, bool mipmap) {
    if (blur) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
}

// -------------------------------------------------------------------------
// Buffer copy
// -------------------------------------------------------------------------

void TextureUtil::copyToBuffer_(const int* pixels, int count) {
    copyToBufferPos_(pixels, 0, count);
}

void TextureUtil::copyToBufferPos_(const int* pixels, int offset, int count) {
    std::memcpy(dataBuffer_.data(), pixels + offset, count * sizeof(int));
}

// -------------------------------------------------------------------------
// Image loading (stb_image replaces ImageIO)
// -------------------------------------------------------------------------

std::vector<int> TextureUtil::readImageData_(const std::string& path, int& widthOut, int& heightOut) {
    int channels;
    // force RGBA
    uint8_t* data = stbi_load(path.c_str(), &widthOut, &heightOut, &channels, 4);
    if (!data)
        throw std::runtime_error("Failed to load image: " + path);

    int pixelCount = widthOut * heightOut;
    std::vector<int> result(pixelCount);

    // stb_image gives RGBA, pack into ARGB int
    for (int i = 0; i < pixelCount; ++i) {
        uint8_t r = data[i * 4 + 0];
        uint8_t g = data[i * 4 + 1];
        uint8_t b = data[i * 4 + 2];
        uint8_t a = data[i * 4 + 3];
        result[i] = (a << 24) | (r << 16) | (g << 8) | b;
    }

    stbi_image_free(data);
    return result;
}

// -------------------------------------------------------------------------
// Pixel ops
// -------------------------------------------------------------------------

std::vector<int> TextureUtil::updateAnaglyph_(const std::vector<int>& pixels) {
    std::vector<int> result(pixels.size());
    for (size_t i = 0; i < pixels.size(); ++i)
        result[i] = anaglyphColor_(pixels[i]);
    return result;
}

int TextureUtil::anaglyphColor_(int color) {
    int a = (color >> 24) & 0xFF;
    int r = (color >> 16) & 0xFF;
    int g = (color >>  8) & 0xFF;
    int b =  color        & 0xFF;
    int i1 = (r * 30 + g * 59 + b * 11) / 100;
    int j1 = (r * 30 + g * 70)          / 100;
    int k1 = (r * 30 + b * 70)          / 100;
    return (a << 24) | (i1 << 16) | (j1 << 8) | k1;
}

void TextureUtil::processPixelValues_(int* pixels, int width, int height) {
    std::vector<int> row(width);
    int half = height / 2;
    for (int j = 0; j < half; ++j) {
        std::memcpy(row.data(),                       pixels + j * width,                width * sizeof(int));
        std::memcpy(pixels + j * width,               pixels + (height - 1 - j) * width, width * sizeof(int));
        std::memcpy(pixels + (height - 1 - j) * width, row.data(),                       width * sizeof(int));
    }
}
