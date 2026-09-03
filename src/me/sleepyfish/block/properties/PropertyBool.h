//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_PROPERTYBOOL_H
#define MCCLONE_PROPERTYBOOL_H

#include "PropertyHelper.h"

#include <vector>
#include <string>

/**
 * @author SleepyFish
 * @brief Boolean property
 */
class PropertyBool : public PropertyHelper<bool> {

private:

    const std::vector<bool> allowedValues = { true, false };

    explicit PropertyBool(const std::string& name) :
        PropertyHelper<bool>(name)
    {}

public:

    static PropertyBool create(const std::string& name) {
        return PropertyBool(name);
    }

    std::vector<bool> getAllowedValues() const override {
        return this->allowedValues;
    }

    std::string getName(const bool& value) const override {
        return value ? "true" : "false";
    }

};


#endif //MCCLONE_PROPERTYBOOL_H
