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
 * @version 1.0
 * @brief Buffered image
 */
struct BufferedImage {

public:

    std::vector<uint8_t> data;
    int width;
    int height;
    int channels;

};


#endif //MCCLONE_BUFFEREDIMAGE_H
