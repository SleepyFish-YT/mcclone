//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_VERTEXFORMATELEMENT_H
#define MCCLONE_VERTEXFORMATELEMENT_H

#include <string>
#include <string_view>
#include <functional>

/**
 * @author SleepyFish
 * @brief Vertex format element descriptor
 */
class VertexFormatElement {

public:

    // -------------------------------------------------------------------------
    // EnumType
    // -------------------------------------------------------------------------

    enum class EnumType {

        FLOAT = 0,
        UBYTE,
        BYTE,
        USHORT,
        SHORT,
        UINT,
        INT

    };

    struct TypeInfo {

        int              size;
        std::string_view displayName;
        int              glConstant;

    };

    static const TypeInfo& getTypeInfo(EnumType type) {
        static const TypeInfo infos[] = {
                { 4, "Float",          5126 },
                { 1, "Unsigned Byte",  5121 },
                { 1, "Byte",           5120 },
                { 2, "Unsigned Short", 5123 },
                { 2, "Short",          5122 },
                { 4, "Unsigned Int",   5125 },
                { 4, "Int",            5124 },
        };

        return infos[static_cast<int>(type)];
    }

    static const int getTypeSize(EnumType type) {
        return getTypeInfo(type).size;
    }

    static const int getTypeGLConstant(EnumType type) {
        return getTypeInfo(type).glConstant;
    }

    // -------------------------------------------------------------------------
    // EnumUsage
    // -------------------------------------------------------------------------

    enum class EnumUsage {

        POSITION = 0,
        NORMAL,
        COLOR,
        UV,
        MATRIX,
        BLEND_WEIGHT,
        PADDING

    };

    static std::string_view getUsageDisplayName(EnumUsage usage) {
        switch (usage) {
            case EnumUsage::POSITION:     return "Position";
            case EnumUsage::NORMAL:       return "Normal";
            case EnumUsage::COLOR:        return "Vertex Color";
            case EnumUsage::UV:           return "UV";
            case EnumUsage::MATRIX:       return "Bone Matrix";
            case EnumUsage::BLEND_WEIGHT: return "Blend Weight";
            case EnumUsage::PADDING:      return "Padding";
            default:                      return "Unknown";
        }
    }

private:

    EnumType  type;
    EnumUsage usage;
    int       index;
    int       elementCount;

    bool isValidIndex_(int idx, EnumUsage u) const {
        return idx == 0 || u == EnumUsage::UV;
    }

public:

    VertexFormatElement(int indexIn, EnumType typeIn, EnumUsage usageIn, int count);

    EnumType  getType()         const { return this->type;         }
    EnumUsage getUsage()        const { return this->usage;        }
    int       getElementCount() const { return this->elementCount; }
    int       getIndex()        const { return this->index;        }

    int getSize() const {
        return getTypeInfo(type).size * this->elementCount;
    }

    bool isPositionElement() const {
        return this->usage == EnumUsage::POSITION;
    }

    std::string toString() const {
        return std::to_string(this->elementCount) + "," +
               std::string(getUsageDisplayName(this->usage)) + "," +
               std::string(getTypeInfo(this->type).displayName);
    }

    bool operator==(const VertexFormatElement& other) const {
        return this->elementCount == other.elementCount &&
               this->index        == other.index        &&
               this->type         == other.type         &&
               this->usage        == other.usage;
    }

    bool operator!=(const VertexFormatElement& other) const {
        return !(*this == other);
    }

    std::size_t hashCode() const {
        std::size_t h = std::hash<int>{}(static_cast<int>(this->type));
        h = 31 * h + std::hash<int>{}(static_cast<int>(this->usage));
        h = 31 * h + std::hash<int>{}(this->index);
        h = 31 * h + std::hash<int>{}(this->elementCount);
        return h;
    }

};

namespace std {

    template<>
    struct hash<VertexFormatElement> {
        size_t operator()(const VertexFormatElement& e) const noexcept {
            return e.hashCode();
        }
    };

};


#endif //MCCLONE_VERTEXFORMATELEMENT_H
