//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_PROPERTYINTEGER_H
#define MCCLONE_PROPERTYINTEGER_H

#include "PropertyHelper.h"

#include <vector>
#include <string>
#include <stdexcept>

/**
 * @author SleepyFish
 * @brief Integer property
 */
class PropertyInteger : public PropertyHelper<int> {

private:

    const std::vector<int> allowedValues;

    static std::vector<int> buildAllowedValues(const std::string& name, int min, int max) {
        if (min < 0)
            throw std::invalid_argument("Min value of " + name + " must be 0 or greater");
        if (max <= min)
            throw std::invalid_argument("Max value of " + name + " must be greater than min (" + std::to_string(min) + ")");

        std::vector<int> values;
        for (int i = min; i <= max; ++i)
            values.push_back(i);
        return values;
    }

    PropertyInteger(const std::string& name, int min, int max) :
        PropertyHelper<int>(name),
        allowedValues(buildAllowedValues(name, min, max))
    {}

public:

    static PropertyInteger create(const std::string& name, int min, int max) {
        return PropertyInteger(name, min, max);
    }

    std::vector<int> getAllowedValues() const override {
        return this->allowedValues;
    }

    std::string getName(const int& value) const override {
        return std::to_string(value);
    }

    bool operator==(const PropertyInteger& other) const {
        return PropertyHelper<int>::operator==(other) && this->allowedValues == other.allowedValues;
    }

    bool operator!=(const PropertyInteger& other) const {
        return !(*this == other);
    }

    std::size_t hashCode() const override {
        std::size_t h = PropertyHelper<int>::hashCode();
        for (int v : this->allowedValues)
            h = 31 * h + std::hash<int>{}(v);
        return h;
    }

};


#endif //MCCLONE_PROPERTYINTEGER_H
