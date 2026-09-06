//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_IPROPERTY_H
#define MCCLONE_IPROPERTY_H

#include <string>
#include <vector>

/**
 * @author SleepyFish
 * @brief Block property interface
 */
template<typename T>
class IProperty {

public:

    virtual ~IProperty() = default;

    virtual std::string getName() const = 0;

    virtual std::vector<T> getAllowedValues() const = 0;

    virtual std::string getName(const T& value) const = 0;

};


#endif //MCCLONE_IPROPERTY_H
