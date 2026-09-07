//
// Created by SleepyFish on 07.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_RANGEDATTRIBUTE_H
#define MCCLONE_RANGEDATTRIBUTE_H

#include "BaseAttribute.h"

#include <stdexcept>
#include <string>
#include <algorithm>

/**
 * @author SleepyFish
 */
class RangedAttribute : public BaseAttribute {

private:

    double minimumValue;

    double maximumValue;

    std::string description;

public:

    RangedAttribute(IAttribute* parent, const std::string& unlocName, double defaultVal, double minVal, double maxVal) :
        BaseAttribute(parent, unlocName, defaultVal),
        minimumValue(minVal),
        maximumValue(maxVal)
    {
        if (minVal > maxVal) {
            throw std::invalid_argument("Minimum value cannot be bigger than maximum value!");
        }

        if (defaultVal < minVal) {
            throw std::invalid_argument("Default value cannot be lower than minimum value!");
        }

        if (defaultVal > maxVal) {
            throw std::invalid_argument("Default value cannot be bigger than maximum value!");
        }
    }

    RangedAttribute& setDescription(const std::string& descriptionIn) {
        this->description = descriptionIn;
        return *this;
    }

    std::string getDescription() const {
        return this->description;
    }

    virtual double clampValue(double value) override {
        return std::clamp(value, this->minimumValue, this->maximumValue);
    }

};


#endif //MCCLONE_RANGEDATTRIBUTE_H
