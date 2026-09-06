//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_PROPERTYENUM_H
#define MCCLONE_PROPERTYENUM_H

#include "PropertyHelper.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <functional>

/**
 * @author SleepyFish
 * @brief Enum property
 */
template<typename T>
class PropertyEnum : public PropertyHelper<T> {

private:

    const std::vector<T> allowedValues;
    std::unordered_map<std::string, T> nameToValue;

protected:

    PropertyEnum(const std::string& name, const std::vector<T>& values) :
        PropertyHelper<T>(name),
        allowedValues(values)
    {
        for (const T& t : values) {
            const std::string s = t.getName();

            if (this->nameToValue.count(s))
                throw std::invalid_argument("Multiple values have the same name '" + s + "'");

            this->nameToValue.emplace(s, t);
        }
    }

public:

    std::vector<T> getAllowedValues() const override {
        return this->allowedValues;
    }

    std::string getName(const T& value) const override {
        return value.getName();
    }

    static PropertyEnum<T> create(const std::string& name, const std::vector<T>& values) {
        return PropertyEnum<T>(name, values);
    }

    static PropertyEnum<T> create(const std::string& name, const std::vector<T>& values, std::function<bool(const T&)> filter) {
        std::vector<T> filtered;
        for (const T& v : values)
            if (filter(v))
                filtered.push_back(v);
        return PropertyEnum<T>(name, filtered);
    }

    const T* getValueByName(const std::string& name) const {
        auto it = this->nameToValue.find(name);
        return it != this->nameToValue.end() ? &it->second : nullptr;
    }

};


#endif //MCCLONE_PROPERTYENUM_H
