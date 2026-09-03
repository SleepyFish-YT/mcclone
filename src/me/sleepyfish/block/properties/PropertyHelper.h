//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_PROPERTYHELPER_H
#define MCCLONE_PROPERTYHELPER_H

#include "IProperty.h"

#include <string>
#include <typeindex>
#include <typeinfo>

/**
 * @author SleepyFish
 * @brief Block property helper
 */
template<typename T>
class PropertyHelper : public IProperty<T> {

private:

    const std::string name;
    const std::type_index valueClass;

protected:

    explicit PropertyHelper(const std::string& name) :
        name(name),
        valueClass(typeid(T))
    {}

public:

    std::string getName() const override {
        return this->name;
    }

    std::string toString() const {
        return "PropertyHelper{name=" + this->name + ", clazz=" + this->valueClass.name() + "}";
    }

    bool operator==(const PropertyHelper<T>& other) const {
        return this->valueClass == other.valueClass && this->name == other.name;
    }

    bool operator!=(const PropertyHelper<T>& other) const {
        return *this != other;
    }

    virtual std::size_t hashCode() const {
        return 31 * this->valueClass.hash_code() + std::hash<std::string>{}(this->name);
    }

};


#endif //MCCLONE_PROPERTYHELPER_H
