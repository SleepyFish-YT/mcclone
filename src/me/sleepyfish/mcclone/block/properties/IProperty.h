//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_IPROPERTY_H
#define MCCLONE_IPROPERTY_H

#include <string>
#include <typeindex>
#include <vector>

/**
 * @author SleepyFish
 * @brief Raw Block property interface
 */
class IPropertyBase {

public:

    virtual ~IPropertyBase() = default;

    virtual std::string getName() const = 0;

    virtual std::type_index getType() const = 0;

};

/**
 * @author SleepyFish
 * @brief Block property interface
 */
template<typename T>
class IProperty : public IPropertyBase {

public:

    virtual std::vector<T> getAllowedValues() const = 0;

    virtual std::string getName(const T& value) const = 0;

};


#endif //MCCLONE_IPROPERTY_H
