//
// Created by SleepyFish on 06.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_IATTRIBUTE_H
#define MCCLONE_IATTRIBUTE_H

#include <string>

/**
 * @author SleepyFish
 * @brief Interface for attributes
 */
class IAttribute {

public:

    virtual ~IAttribute() = default;

    virtual std::string getAttributeUnlocalizedName() const noexcept = 0;

    virtual double clampValue(double value) = 0;

    virtual double getDefaultValue() const noexcept = 0;

    virtual bool getShouldWatch() const noexcept = 0;

    virtual IAttribute* func_180372_d() const noexcept = 0;

};


#endif //MCCLONE_IATTRIBUTE_H
