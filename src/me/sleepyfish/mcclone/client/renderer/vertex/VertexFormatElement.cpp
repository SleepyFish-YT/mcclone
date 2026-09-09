//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#include "VertexFormatElement.h"

#include "../../../debug/Logger.h"

VertexFormatElement::VertexFormatElement(int indexIn, EnumType typeIn, EnumUsage usageIn, int count) :
    type(typeIn),
    index(indexIn),
    elementCount(count)
{
    if (!this->isValidIndex_(indexIn, usageIn)) {
        Logger::warn("Multiple vertex elements of the same type other than UVs are not supported. Forcing type to UV.");
        this->usage = EnumUsage::UV;
    } else {
        this->usage = usageIn;
    }
}
