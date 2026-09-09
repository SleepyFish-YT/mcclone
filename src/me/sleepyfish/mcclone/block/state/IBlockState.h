//
// Created by SleepyFish on 07.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_IBLOCKSTATE_H
#define MCCLONE_IBLOCKSTATE_H

#include <string>
#include <unordered_map>

// not FULLY implemented yet.

#include "../../block/properties/IProperty.h"
//#include "../../block/Block.h"

#include <vector>
#include <unordered_map>
#include <memory>

/**
 * @author SleepyFish
 * @brief Block state interface
 */
class IBlockState {

public:

    virtual ~IBlockState() = default;

    virtual std::vector<IPropertyBase*> getPropertyNames() const = 0;

    // <T extends Comparable<T>> T getValue(IProperty<T> property)
    // since c++ can not express this generically in a virtual method,
    // we use a type erased approach via void* - cast at call site
    virtual const void* getValue(const IPropertyBase* property) const = 0;

    virtual IBlockState* withProperty(const IPropertyBase* property, const void* value) = 0;

    virtual IBlockState* cycleProperty(const IPropertyBase* property) = 0;

    virtual std::unordered_map<const IPropertyBase*, const void*> getProperties() const = 0;

    // virtual Block* getBlock() const = 0;

};


#endif //MCCLONE_IBLOCKSTATE_H
