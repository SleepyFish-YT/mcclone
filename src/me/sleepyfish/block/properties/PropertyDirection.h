//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_PROPERTYDIRECTION_H
#define MCCLONE_PROPERTYDIRECTION_H

#include "PropertyEnum.h"
#include "../../util/EnumFacing.h"

#include <string>
#include <vector>
#include <functional>

/**
 * @author SleepyFish
 * @brief Direction property
 */
class PropertyDirection : public PropertyEnum<EnumFacing> {

protected:

    PropertyDirection(const std::string& name, const std::vector<EnumFacing>& values) :
        PropertyEnum<EnumFacing>(name, values)
    {}

public:

    static PropertyDirection create(const std::string& name) {
        return create(name, [](const EnumFacing&) { return true; });
    }

    static PropertyDirection create(const std::string& name, std::function<bool(const EnumFacing&)> filter) {
        std::vector<EnumFacing> values;
        for (int i = 0; i < 6; ++i) {
            EnumFacing f((EnumFacing::Value)i);
            if (filter(f))
                values.push_back(f);
        }
        return create(name, values);
    }

    static PropertyDirection create(const std::string& name, const std::vector<EnumFacing>& values) {
        return PropertyDirection(name, values);
    }

};


#endif //MCCLONE_PROPERTYDIRECTION_H
