//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_BUFFEREDIMAGE_H
#define MCCLONE_BUFFEREDIMAGE_H

#include <cstdint>
#include <vector>

/**
 * @author SleepyFish - SleepyAVA
 * @version 1.3
 * @brief Buffered image
 */
struct BufferedImage {

public:

    std::vector<uint8_t> data; // raw channel bytes (R, G, B, A per pixel)
    std::vector<int>   pixels; // packed ARGB ints, kept in sync via syncPixels()
    int width    = 0;
    int height   = 0;
    int channels = 0;

    BufferedImage(const std::vector<int>& packedPixels, int w, int h) {
        width    = w;
        height   = h;
        channels = 4;
        pixels   = packedPixels;
        data.resize(static_cast<size_t>(w) * h * 4);
        for (int i = 0; i < w * h; ++i) {
            int argb    = packedPixels[i];
            data[i*4+0] = (argb >> 16) & 0xFF; // R
            data[i*4+1] = (argb >>  8) & 0xFF; // G
            data[i*4+2] = (argb      ) & 0xFF; // B
            data[i*4+3] = (argb >> 24) & 0xFF; // A
        }
    }

    void syncPixels() {
        pixels.resize(static_cast<size_t>(width) * height);
        getRGB(0, 0, width, height, pixels.data(), 0, width);
    }

    void getRGB(int x, int y, int w, int h, int* dest, int offset, int scanSize) const {
        for (int row = 0; row < h; ++row) {
            for (int col = 0; col < w; ++col) {
                const int sx  = x + col;
                const int sy  = y + row;
                const size_t idx = (static_cast<size_t>(sy) * width + sx) * channels;

                uint8_t r = 0, g = 0, b = 0, a = 255;
                if (channels >= 1) r = data[idx + 0];
                if (channels >= 2) g = data[idx + 1];
                if (channels >= 3) b = data[idx + 2];
                if (channels >= 4) a = data[idx + 3];

                dest[offset + row * scanSize + col] =
                        (static_cast<int>(a) << 24) |
                        (static_cast<int>(r) << 16) |
                        (static_cast<int>(g) <<  8) |
                        (static_cast<int>(b));
            }
        }
    }

    // Write a packed ARGB int back into data[] and pixels[]
    void setRGB(int x, int y, int argb) {
        const size_t idx = (static_cast<size_t>(y) * width + x) * channels;

        const uint8_t a = (argb >> 24) & 0xFF;
        const uint8_t r = (argb >> 16) & 0xFF;
        const uint8_t g = (argb >>  8) & 0xFF;
        const uint8_t b = (argb      ) & 0xFF;

        if (channels >= 1) data[idx + 0] = r;
        if (channels >= 2) data[idx + 1] = g;
        if (channels >= 3) data[idx + 2] = b;
        if (channels >= 4) data[idx + 3] = a;

        if (!pixels.empty())
            pixels[static_cast<size_t>(y) * width + x] = argb;
    }

};


#endif //MCCLONE_BUFFEREDIMAGE_H
