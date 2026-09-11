//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_VERTEXFORMAT_H
#define MCCLONE_VERTEXFORMAT_H

#include "VertexFormatElement.h"

#include <vector>
#include <string>
#include <functional>

/**
 * @author SleepyFish
 * @brief Vertex format descriptor - ordered list of VertexFormatElements
 */
class VertexFormat {

private:

    std::vector<VertexFormatElement> elements;
    std::vector<int> offsets;
    int              nextOffset;
    int              colorElementOffset;
    std::vector<int> uvOffsetsById;
    int              normalElementOffset;

    bool hasPosition() const {
        for (const auto& e : this->elements) {
            if (e.isPositionElement()) {
                return true;
            }
        }

        return false;
    }

public:

    VertexFormat() :
        nextOffset(0),
        colorElementOffset(-1),
        normalElementOffset(-1)
    {}

    VertexFormat(const VertexFormat& other) :
        VertexFormat()
    {
        for (int i = 0; i < other.getElementCount(); ++i) {
            this->addElement(other.getElement(i));
        }

        this->nextOffset = other.nextOffset;
    }

    void clear() {
        this->elements.clear();
        this->offsets.clear();
        this->colorElementOffset  = -1;
        this->normalElementOffset = -1;
        this->uvOffsetsById.clear();
        this->nextOffset = 0;
    }

    VertexFormat& addElement(const VertexFormatElement& element);

    bool hasNormal()       const { return this->normalElementOffset >= 0; }
    int  getNormalOffset() const { return this->normalElementOffset;      }
    bool hasColor()        const { return this->colorElementOffset >= 0;  }
    int  getColorOffset()  const { return this->colorElementOffset;       }

    bool hasUvOffset(int id) const {
        return id < static_cast<int>(this->uvOffsetsById.size()) && this->uvOffsetsById[id] >= 0;
    }

    int getUvOffsetById(int id) const {
        return this->uvOffsetsById[id];
    }

    int getNextOffset()   const { return this->nextOffset;                        }
    int getIntegerSize()  const { return this->nextOffset / 4;                    }
    int getElementCount() const { return static_cast<int>(this->elements.size()); }
    int getOffset(int i)  const { return this->offsets[i];                        }

    const VertexFormatElement& getElement(int index) const {
        return this->elements[index];
    }

    const std::vector<VertexFormatElement>& getElements() const {
        return this->elements;
    }

    std::string toString() const {
        std::string s = "format: " + std::to_string(this->elements.size()) + " elements: ";

        for (int i = 0; i < static_cast<int>(this->elements.size()); ++i) {
            s += this->elements[i].toString();
            if (i != static_cast<int>(this->elements.size()) - 1) {
                s += ' ';
            }
        }

        return s;
    }

    bool operator==(const VertexFormat& other) const {
        return this->nextOffset == other.nextOffset &&
               this->elements   == other.elements   &&
               this->offsets    == other.offsets;
    }

    bool operator!=(const VertexFormat& other) const {
        return !(*this == other);
    }

    std::size_t hashCode() const {
        std::size_t h = 0;

        for (const auto& e : this->elements)
            h = 31 * h + e.hashCode();
        for (int o : this->offsets)
            h = 31 * h + std::hash<int>{}(o);

        h = 31 * h + std::hash<int>{}(this->nextOffset);
        return h;
    }

};

namespace std {

    template<>
    struct hash<VertexFormat> {
        size_t operator()(const VertexFormat& f) const noexcept {
            return f.hashCode();
        }
    };

};


#endif //MCCLONE_VERTEXFORMAT_H
