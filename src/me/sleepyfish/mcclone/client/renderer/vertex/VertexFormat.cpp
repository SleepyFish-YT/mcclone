//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#include "VertexFormat.h"

#include "../../../debug/Logger.h"

VertexFormat& VertexFormat::addElement(const VertexFormatElement& element) {
    if (element.isPositionElement() && hasPosition()) {
        Logger::warn("VertexFormat error: Trying to add a position VertexFormatElement when one already exists, ignoring.");
        return *this;
    }

    this->elements.push_back(element);
    this->offsets.push_back(this->nextOffset);

    switch (element.getUsage()) {
        case VertexFormatElement::EnumUsage::NORMAL:
            this->normalElementOffset = this->nextOffset;
            break;
        case VertexFormatElement::EnumUsage::COLOR:
            this->colorElementOffset = this->nextOffset;
            break;
        case VertexFormatElement::EnumUsage::UV: {
            int idx = element.getIndex();
            if (idx >= static_cast<int>(this->uvOffsetsById.size()))
                this->uvOffsetsById.resize(idx + 1, -1);
            this->uvOffsetsById[idx] = this->nextOffset;
            break;
        }
        default:
            break;
    }

    this->nextOffset += element.getSize();
    return *this;
}
