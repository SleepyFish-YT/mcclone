//
// Created by SleepyFish on 06.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_BASEATTRIBUTE_H
#define MCCLONE_BASEATTRIBUTE_H

#include "IAttribute.h"

#include <string>
#include <stdexcept>
#include <algorithm>

/**
 * @author SleepyFish
 * @brief Base class for attributes
 */
class BaseAttribute : public IAttribute {

private:

    IAttribute *field_180373_a;

    const std::string unlocalizedName;

    const double defaultValue;

    bool shouldWatch{};

protected:

    BaseAttribute(IAttribute *attribute, std::string unlocNameIn, double defaultValIn) :
        field_180373_a(attribute),
        unlocalizedName(std::move(unlocNameIn)),
        defaultValue(defaultValIn)
    {
        if (unlocNameIn.empty()) {
            throw std::invalid_argument("Name cannot be null!");
        }
    }

public:

    virtual std::string getAttributeUnlocalizedName() const noexcept override {
        return this->unlocalizedName;
    }

    virtual double clampValue(double value) override {
        return std::clamp(value, -128.0, 128.0);
    }

    virtual double getDefaultValue() const noexcept override {
        return this->defaultValue;
    }

    virtual bool getShouldWatch() const noexcept override {
        return this->shouldWatch;
    }

    BaseAttribute& setShouldWatch(bool shouldWatchIn) noexcept {
        this->shouldWatch = shouldWatchIn;
        return *this;
    }

    virtual IAttribute* func_180372_d() const noexcept override {
        return this->field_180373_a;
    }

    int hashCode() const noexcept {
        return static_cast<int>(std::hash<std::string>{}(this->unlocalizedName));
    }

};


#endif //MCCLONE_BASEATTRIBUTE_H
