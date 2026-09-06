//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#include "NBTBase.h"

#include "NBTTagByte.h"
#include "NBTTagByteArray.h"
#include "NBTTagCompound.h"
#include "NBTTagDouble.h"
#include "NBTTagEnd.h"
#include "NBTTagInt.h"
#include "NBTTagList.h"
#include "NBTTagLong.h"
#include "NBTTagShort.h"
#include "NBTTagString.h"
#include "NBTTagIntArray.h"

#include <memory>

std::unique_ptr<NBTBase> NBTBase::createNewByType(int8_t id) {
    switch (id) {
        case 0:
            return std::make_unique<NBTTagEnd>();
        case 1:
            return std::make_unique<NBTTagByte>();
        case 2:
            return std::make_unique<NBTTagShort>();
        case 3:
            return std::make_unique<NBTTagInt>();
        case 4:
            return std::make_unique<NBTTagLong>();
        case 5:
            return std::make_unique<NBTTagFloat>();
        case 6:
            return std::make_unique<NBTTagDouble>();
        case 7:
            return std::make_unique<NBTTagByteArray>();
        case 8:
            return std::make_unique<NBTTagString>();
        case 9:
            return std::make_unique<NBTTagList>();
        case 10:
            return std::make_unique<NBTTagCompound>();
        case 11:
            return std::make_unique<NBTTagIntArray>();
    }

    return nullptr;
}