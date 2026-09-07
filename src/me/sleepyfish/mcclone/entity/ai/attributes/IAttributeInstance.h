//
// Created by SleepyFish on 06.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_IATTRIBUTEINSTANCE_H
#define MCCLONE_IATTRIBUTEINSTANCE_H

#include "IAttribute.h"
#include "../../../../sava/UUID.h"
#include "AttributeModifier.h"

#include <vector>

/**
 * @author SleepyFish
 */
class IAttributeInstance {

public:

    virtual ~IAttributeInstance() = default;

    virtual IAttribute& getAttribute() = 0;

    virtual double getBaseValue() const = 0;

    virtual void setBaseValue(double baseValue) = 0;

    virtual std::vector<AttributeModifier> getModifiersByOperation(int operation) const = 0;

    virtual std::vector<AttributeModifier> getAllModifiers() const = 0;

    virtual bool hasModifier(const AttributeModifier &modifier) const = 0;

    virtual AttributeModifier* getModifier(const UUID &uuid) = 0;

    virtual void applyModifier(const AttributeModifier &modifier) = 0;

    virtual void removeModifier(const AttributeModifier &modifier) = 0;

    virtual void removeAllModifiers() = 0;

    virtual double getAttributeValue() const = 0;

};


#endif //MCCLONE_IATTRIBUTEINSTANCE_H
