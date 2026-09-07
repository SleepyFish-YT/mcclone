//
// Created by SleepyFish on 06.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_BASEATTRIBUTEMAP_H
#define MCCLONE_BASEATTRIBUTEMAP_H

#include "IAttribute.h"
#include "IAttributeInstance.h"
#include "AttributeModifier.h"

#include "../../../../sava/SavaUtil.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>

/**
 * @author SleepyFish
 */
class BaseAttributeMap {

protected:

    virtual IAttributeInstance* createInstance(IAttribute* attribute) = 0;

    // key: parent attribute, value: child attributes that depend on it
    std::unordered_multimap<IAttribute*, IAttribute*> m_parentToChildren;

    std::unordered_map<IAttribute*, IAttributeInstance*> m_attributes;
    std::unordered_map<std::string, IAttributeInstance*> m_attributesByName;

public:

    virtual ~BaseAttributeMap() = default;

    virtual IAttributeInstance* getAttributeInstance(IAttribute* attribute) {
        auto it = this->m_attributes.find(attribute);
        return it != this->m_attributes.end() ? it->second : nullptr;
    }

    virtual IAttributeInstance* getAttributeInstanceByName(const std::string& attributeName) {
        auto it = this->m_attributesByName.find(SavaUtil::StringUtil::ToLowerCase(attributeName));
        return it != this->m_attributesByName.end() ? it->second : nullptr;
    }

    virtual IAttributeInstance* registerAttribute(IAttribute* attribute) {
        const std::string key = SavaUtil::StringUtil::ToLowerCase(attribute->getAttributeUnlocalizedName());

        if (this->m_attributesByName.count(key)) {
            throw std::invalid_argument("Attribute is already registered!");
        }

        IAttributeInstance* instance = this->createInstance(attribute);
        this->m_attributesByName[key] = instance;
        this->m_attributes[attribute] = instance;

        for (IAttribute* parent = attribute->func_180372_d(); parent != nullptr; parent = parent->func_180372_d()) {
            this->m_parentToChildren.emplace(parent, attribute);
        }

        return instance;
    }

    std::vector<IAttributeInstance*> getAllAttributes() const {
        std::vector<IAttributeInstance*> result;
        result.reserve(this->m_attributesByName.size());
        for (auto& [key, val] : this->m_attributesByName) {
            result.push_back(val);
        }
        return result;
    }

    virtual void onAttributeModified(IAttributeInstance* instance) {}

    void removeAttributeModifiers(const std::vector<std::pair<std::string, AttributeModifier>>& modifiers) {
        for (auto& [name, modifier] : modifiers) {
            IAttributeInstance* instance = this->getAttributeInstanceByName(name);
            if (instance) {
                instance->removeModifier(modifier);
            }
        }
    }

    void applyAttributeModifiers(const std::vector<std::pair<std::string, AttributeModifier>>& modifiers) {
        for (auto& [name, modifier] : modifiers) {
            IAttributeInstance* instance = this->getAttributeInstanceByName(name);
            if (instance) {
                instance->removeModifier(modifier);
                instance->applyModifier(modifier);
            }
        }
    }

};


#endif //MCCLONE_BASEATTRIBUTEMAP_H
