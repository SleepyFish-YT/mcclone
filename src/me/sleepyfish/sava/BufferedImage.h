//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_BUFFEREDIMAGE_H
#define MCCLONE_BUFFEREDIMAGE_H

#include <algorithm>   // std::copy_n, std::min, std::clamp
#include <cstdint>
#include <cstring>     // std::memcpy
#include <stdexcept>   // std::invalid_argument, std::logic_error
#include <vector>

/**
 * @author SleepyFish - SleepyAVA
 * @version 1.4
 * @brief Buffered image
 */
class BufferedImage {

private:

    BufferedImage(int w, int h, int ch) :
        data(static_cast<size_t>(w) * h * ch),
        width(w),
        height(h),
        channels(ch)
    {}

public:

    /**
     * @author SleepyFish
     * @brief .sized() filter
     */
    enum class Filter { Nearest, Bilinear };

    std::vector<uint8_t> data; // raw channel bytes (r, g, b, a per pixel)
    std::vector<int> pixels; // packed ARGB ints, kept in sync via syncPixels()
    int width    = 0;
    int height   = 0;
    int channels = 0;

    BufferedImage(const std::vector<int> &packedPixels, int w, int h) {
        width    = w;
        height   = h;
        channels = 4;
        pixels   = packedPixels;
        data.resize(static_cast<size_t>(w) * h * 4);
        for (int i = 0; i < w * h; ++i) {
            int argb    = packedPixels[i];
            data[i*4+0] = (argb >> 16) & 0xFF; // r
            data[i*4+1] = (argb >>  8) & 0xFF; // g
            data[i*4+2] = (argb      ) & 0xFF; // b
            data[i*4+3] = (argb >> 24) & 0xFF; // a
        }
    }

    void syncPixels() {
        pixels.resize(static_cast<size_t>(width) * height);
        getRGB(0, 0, width, height, pixels.data(), 0, width);
    }

    void getRGB(int x, int y, int w, int h, int *dest, int offset, int scanSize) const {
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

    // write a packed ARGB int back into data[] and pixels[]
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

    BufferedImage sized(int w, int h, Filter filter = Filter::Bilinear) const {
        if (w <= 0 || h <= 0)
            throw std::invalid_argument("BufferedImage::sized: target size must be positive");

        if (width <= 0 || height <= 0 || channels <= 0 || data.size() < static_cast<size_t>(width) * height * channels)
            throw std::logic_error("BufferedImage::sized: source image is empty or inconsistent");

        BufferedImage out(w, h, channels);
        const auto ch = static_cast<size_t>(channels);

        if (w == width && h == height) {
            std::copy_n(data.begin(), out.data.size(), out.data.begin());
        } else if (filter == Filter::Nearest) {
            std::vector<size_t> xmap(static_cast<size_t>(w));
            for (int dx = 0; dx < w; ++dx) {
                const uint64_t sx = (2ull * dx + 1) * width / (2ull * w);
                xmap[dx] = std::min<uint64_t>(sx, width - 1) * ch;
            }

            for (int dy = 0; dy < h; ++dy) {
                const uint64_t sy = std::min<uint64_t>((2ull * dy + 1) * height / (2ull * h), height - 1);
                const uint8_t *srcRow = data.data() + sy * width * ch;
                uint8_t *dstRow = out.data.data() + static_cast<size_t>(dy) * w * ch;

                for (int dx = 0; dx < w; ++dx)
                    std::memcpy(dstRow + static_cast<size_t>(dx) * ch, srcRow + xmap[dx], ch);
            }
        } else {
            struct Tap { size_t i0, i1; float f; };
            auto makeTaps = [](int srcN, int dstN) {
                std::vector<Tap> t(static_cast<size_t>(dstN));
                const float scale = static_cast<float>(srcN) / dstN;
                for (int d = 0; d < dstN; ++d) {
                    float s = (d + 0.5f) * scale - 0.5f;
                    s = std::clamp(s, 0.0f, static_cast<float>(srcN - 1));
                    const auto i0 = static_cast<size_t>(s);
                    t[d] = { i0, std::min<size_t>(i0 + 1, srcN - 1), s - static_cast<float>(i0) };
                }
                return t;
            };

            const std::vector<Tap> xt = makeTaps(width, w);
            const std::vector<Tap> yt = makeTaps(height, h);

            for (int dy = 0; dy < h; ++dy) {
                const Tap &ty = yt[dy];
                const uint8_t *r0 = data.data() + ty.i0 * width * ch;
                const uint8_t *r1 = data.data() + ty.i1 * width * ch;
                uint8_t *dstRow = out.data.data() + static_cast<size_t>(dy) * w * ch;

                for (int dx = 0; dx < w; ++dx) {
                    const Tap &tx = xt[dx];
                    for (size_t c = 0; c < ch; ++c) {
                        const float top = r0[tx.i0 * ch + c] + (r0[tx.i1 * ch + c] - r0[tx.i0 * ch + c]) * tx.f;
                        const float bot = r1[tx.i0 * ch + c] + (r1[tx.i1 * ch + c] - r1[tx.i0 * ch + c]) * tx.f;

                        dstRow[static_cast<size_t>(dx) * ch + c] = static_cast<uint8_t>(top + (bot - top) * ty.f + 0.5f);
                    }
                }
            }
        }

        if (!pixels.empty())
            out.syncPixels();

        return out;
    }

};


#endif //MCCLONE_BUFFEREDIMAGE_H
