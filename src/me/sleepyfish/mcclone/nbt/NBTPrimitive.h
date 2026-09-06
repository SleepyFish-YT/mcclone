//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_NBTPRIMITIVE_H
#define MCCLONE_NBTPRIMITIVE_H

#include "NBTBase.h"

/**
 * @author SleepyFish
 * @brief Base class for all primitive NBT tags
 */
class NBTPrimitive : public NBTBase {

public:

    virtual int64_t getLong() const = 0;

    virtual int32_t getInt() const = 0;

    virtual int16_t getShort() const = 0;

    virtual int8_t getByte() const = 0;

    virtual double getDouble() const = 0;

    virtual float getFloat() const = 0;

};


#endif //MCCLONE_NBTPRIMITIVE_H
