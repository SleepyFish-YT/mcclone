//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ITICKABLETEXTURE_H
#define MCCLONE_ITICKABLETEXTURE_H

/**
 * @author SleepyFish
 * @brief Tickable texture interface
 */
class ITickableTexture {

public:

    virtual ~ITickableTexture() = default;

    virtual void tick() = 0;

};


#endif //MCCLONE_ITICKABLETEXTURE_H
