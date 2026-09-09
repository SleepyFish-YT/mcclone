//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_IIMAGEBUFFER_H
#define MCCLONE_IIMAGEBUFFER_H

#include "../../../sava/BufferedImage.h"

/**
 * @author SleepyFish
 * @brief Image buffer interface
 */
class IImageBuffer {

public:

    virtual ~IImageBuffer() = default;

    virtual BufferedImage parseUserSkin(const BufferedImage& image) = 0;

    virtual void skinAvailable() = 0;

};


#endif //MCCLONE_IIMAGEBUFFER_H
