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
 * @version 1.1
 * @brief Buffered image
 */
struct BufferedImage {

public:

    std::vector<uint8_t> data;
    int width;
    int height;
    int channels;

    void getRGB(int x, int y, int w, int h, int* dest, int offset, int scansize) const {
        for (int row = 0; row < h; ++row) {
            for (int col = 0; col < w; ++col) {
                const int sx = x + col;
                const int sy = y + row;
                const size_t idx = (static_cast<size_t>(sy) * width + sx) * channels;

                uint8_t r = 0, g = 0, b = 0, a = 255;
                if (channels >= 1) r = data[idx + 0];
                if (channels >= 2) g = data[idx + 1];
                if (channels >= 3) b = data[idx + 2];
                if (channels >= 4) a = data[idx + 3];

                dest[offset + row * scansize + col] =
                        (static_cast<int>(a) << 24) |
                        (static_cast<int>(r) << 16) |
                        (static_cast<int>(g) <<  8) |
                        (static_cast<int>(b));
            }
        }
    }

};


#endif //MCCLONE_BUFFEREDIMAGE_H
