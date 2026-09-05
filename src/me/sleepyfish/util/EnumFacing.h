//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ENUMFACING_H
#define MCCLONE_ENUMFACING_H

#include "Vec3i.h"
#include "MathHelper.h"

#include <string>
#include <unordered_map>
#include <stdexcept>
#include <random>
#include <vector>
#include <algorithm>

/**
 * @author SleepyFish
 */
class EnumFacing {

public:

    enum class Plane {
        HORIZONTAL = 0,
        VERTICAL
    };

    enum class Axis {
        X = 0,
        Y,
        Z
    };

    enum class AxisDirection {
        POSITIVE = 0,
        NEGATIVE
    };

    enum Value {
        DOWN = 0,
        UP,
        NORTH,
        SOUTH,
        WEST,
        EAST
    };

private:

    struct FacingData {

        int index;

        int opposite;

        int horizontalIndex;

        std::string name;

        AxisDirection axisDirection;

        Axis axis;

        Vec3i directionVec;

    };



    static const FacingData DATA[6];

    static const int HORIZONTALS[4];

    static std::unordered_map<std::string, Value> NAME_LOOKUP;

    static std::unordered_map<std::string, Axis> AXIS_NAME_LOOKUP;

    static bool initialized;

    static void initialize() {
        if (initialized) return;
        NAME_LOOKUP["down"] = DOWN;
        NAME_LOOKUP["up"] = UP;
        NAME_LOOKUP["north"] = NORTH;
        NAME_LOOKUP["south"] = SOUTH;
        NAME_LOOKUP["west"] = WEST;
        NAME_LOOKUP["east"] = EAST;
        AXIS_NAME_LOOKUP["x"] = Axis::X;
        AXIS_NAME_LOOKUP["y"] = Axis::Y;
        AXIS_NAME_LOOKUP["z"] = Axis::Z;
        initialized = true;
    }

    Value value;

public:

    EnumFacing(Value v) : value(v) { EnumFacing::initialize(); }

    static EnumFacing down() noexcept { return {DOWN}; }

    static EnumFacing up() noexcept { return {UP}; }

    static EnumFacing north() noexcept { return {NORTH}; }

    static EnumFacing south() noexcept { return {SOUTH}; }

    static EnumFacing west() noexcept { return {WEST}; }

    static EnumFacing east() noexcept { return {EAST}; }

    bool operator==(const EnumFacing &other) const noexcept { return value == other.value; }

    bool operator!=(const EnumFacing &other) const noexcept { return value != other.value; }

    int getIndex() const noexcept { return DATA[value].index; }

    int getHorizontalIndex() const noexcept { return DATA[value].horizontalIndex; }

    AxisDirection getAxisDirection() const noexcept { return DATA[value].axisDirection; }

    Axis getAxis() const noexcept { return DATA[value].axis; }

    const Vec3i &getDirectionVec() const noexcept { return DATA[value].directionVec; }

    std::string getName() const noexcept { return DATA[value].name; }

    std::string toString() const noexcept { return DATA[value].name; }

    static int getAxisOffset(AxisDirection dir) noexcept {
        return dir == AxisDirection::POSITIVE ? 1 : -1;
    }

    int getFrontOffsetX() const noexcept {
        return DATA[value].axis == Axis::X ? getAxisOffset(DATA[value].axisDirection) : 0;
    }

    int getFrontOffsetY() const noexcept {
        return DATA[value].axis == Axis::Y ? getAxisOffset(DATA[value].axisDirection) : 0;
    }

    int getFrontOffsetZ() const noexcept {
        return DATA[value].axis == Axis::Z ? getAxisOffset(DATA[value].axisDirection) : 0;
    }

    EnumFacing getOpposite() const noexcept {
        return {(Value) DATA[value].opposite};
    }

    EnumFacing rotateY() const {
        switch (value) {
            case NORTH:
                return {EAST};
            case EAST:
                return {SOUTH};
            case SOUTH:
                return {WEST};
            case WEST:
                return {NORTH};
            default:
                throw std::runtime_error("Unable to get Y-rotated facing of " + getName());
        }
    }

    EnumFacing rotateYCCW() const {
        switch (value) {
            case NORTH:
                return {WEST};
            case EAST:
                return {NORTH};
            case SOUTH:
                return {EAST};
            case WEST:
                return {SOUTH};
            default:
                throw std::runtime_error("Unable to get CCW facing of " + getName());
        }
    }

    EnumFacing rotateX() const {
        switch (value) {
            case NORTH:
                return {DOWN};
            case SOUTH:
                return {UP};
            case UP:
                return {NORTH};
            case DOWN:
                return {SOUTH};
            default:
                throw std::runtime_error("Unable to get X-rotated facing of " + getName());
        }
    }

    EnumFacing rotateZ() const {
        switch (value) {
            case EAST:
                return {DOWN};
            case WEST:
                return {UP};
            case UP:
                return {EAST};
            case DOWN:
                return {WEST};
            default:
                throw std::runtime_error("Unable to get Z-rotated facing of " + getName());
        }
    }

    EnumFacing rotateAround(Axis axis) const {
        switch (axis) {
            case Axis::X:
                if (value != WEST && value != EAST) return rotateX();
                return *this;
            case Axis::Y:
                if (value != UP && value != DOWN) return rotateY();
                return *this;
            case Axis::Z:
                if (value != NORTH && value != SOUTH) return rotateZ();
                return *this;
            default:
                throw std::runtime_error("Unable to get CW facing for axis");
        }
    }

    static EnumFacing byName(const std::string &name) {
        initialize();
        std::string lower = name;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        auto it = NAME_LOOKUP.find(lower);
        if (it == NAME_LOOKUP.end())
            throw std::runtime_error("No facing with name: " + name);
        return {it->second};
    }

    static EnumFacing getFront(int index) noexcept {
        return {(Value) (MathHelper::abs_int(index % 6))};
    }

    static EnumFacing getHorizontal(int index) noexcept {
        static const Value horizontals[4] = {SOUTH, WEST, NORTH, EAST};
        return {horizontals[MathHelper::abs_int(index % 4)]};
    }

    static EnumFacing fromAngle(double angle) noexcept {
        return getHorizontal(MathHelper::floor_double(angle / 90.0 + 0.5) & 3);
    }

    static EnumFacing random(std::mt19937 &rand) {
        std::uniform_int_distribution<int> dist(0, 5);
        return {(Value) dist(rand)};
    }

    static EnumFacing getFacingFromVector(float x, float y, float z) {
        EnumFacing best(NORTH);
        float bestDot = -std::numeric_limits<float>::max();

        for (int i = 0; i < 6; ++i) {
            EnumFacing f((Value) i);
            const Vec3i &dv = f.getDirectionVec();
            float dot = x * (float) dv.x + y * (float) dv.y + z * (float) dv.z;
            if (dot > bestDot) {
                bestDot = dot;
                best = f;
            }
        }

        return best;
    }

    static EnumFacing getFacingFromAxis(AxisDirection dir, Axis axis) {
        for (int i = 0; i < 6; ++i) {
            EnumFacing f((Value) i);
            if (f.getAxisDirection() == dir && f.getAxis() == axis)
                return f;
        }

        throw std::runtime_error("No such direction");
    }

    static bool isHorizontal(Axis axis) noexcept { return axis == Axis::X || axis == Axis::Z; }

    static bool isVertical(Axis axis) noexcept { return axis == Axis::Y; }

    static Axis axisByName(const std::string &name) {
        initialize();
        std::string lower = name;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        auto it = AXIS_NAME_LOOKUP.find(lower);
        if (it == AXIS_NAME_LOOKUP.end())
            throw std::runtime_error("No axis with name: " + name);
        return it->second;
    }

    static std::vector<EnumFacing> facingsForPlane(Plane plane) {
        switch (plane) {
            case Plane::HORIZONTAL:
                return {EnumFacing(NORTH), EnumFacing(EAST), EnumFacing(SOUTH), EnumFacing(WEST)};
            case Plane::VERTICAL:
                return {EnumFacing(UP), EnumFacing(DOWN)};
            default:
                throw std::runtime_error("Someone's been tampering with the universe!");
        }
    }

    static EnumFacing randomFromPlane(Plane plane, std::mt19937 &rand) {
        auto facings = facingsForPlane(plane);
        std::uniform_int_distribution<int> dist(0, (int) facings.size() - 1);
        return facings[dist(rand)];
    }

    Value getValue() const noexcept { return value; }

};


inline const EnumFacing::FacingData EnumFacing::DATA[6] = {
        {0, 1, -1, "down", EnumFacing::AxisDirection::NEGATIVE, EnumFacing::Axis::Y, Vec3i(0, -1, 0)},
        {1, 0, -1, "up", EnumFacing::AxisDirection::POSITIVE, EnumFacing::Axis::Y, Vec3i(0, 1, 0)},
        {2, 3, 2, "north", EnumFacing::AxisDirection::NEGATIVE, EnumFacing::Axis::Z, Vec3i(0, 0, -1)},
        {3, 2, 0, "south", EnumFacing::AxisDirection::POSITIVE, EnumFacing::Axis::Z, Vec3i(0, 0, 1)},
        {4, 5, 1, "west", EnumFacing::AxisDirection::NEGATIVE, EnumFacing::Axis::X, Vec3i(-1, 0, 0)},
        {5, 4, 3, "east", EnumFacing::AxisDirection::POSITIVE, EnumFacing::Axis::X, Vec3i(1, 0, 0)}
};

inline std::unordered_map<std::string, EnumFacing::Value> EnumFacing::NAME_LOOKUP;

inline std::unordered_map<std::string, EnumFacing::Axis> EnumFacing::AXIS_NAME_LOOKUP;

inline bool EnumFacing::initialized = false;


#endif //MCCLONE_ENUMFACING_H
