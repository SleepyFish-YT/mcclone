//
// Created by SleepyFish on 12.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ITICKABLETEXTUREOBJECT_H
#define MCCLONE_ITICKABLETEXTUREOBJECT_H

#include "ITextureObject.h"
#include "ITickableTexture.h"

/**
 * @author SleepyFish
 * @brief Tickable texture object interface
 */
class ITickableTextureObject : public ITextureObject, public ITickableTexture {

public:

    virtual ~ITickableTextureObject() = default;

};


#endif //MCCLONE_ITICKABLETEXTUREOBJECT_H
