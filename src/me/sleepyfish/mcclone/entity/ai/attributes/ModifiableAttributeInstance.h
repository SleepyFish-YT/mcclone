//
// Created by SleepyFish on 07.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_MODIFIABLEATTRIBUTEINSTANCE_H
#define MCCLONE_MODIFIABLEATTRIBUTEINSTANCE_H

#include "IAttributeInstance.h"
#include "IAttribute.h"
#include "AttributeModifier.h"
#include "BaseAttributeMap.h"
#include "../../../../sava/UUID.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stdexcept>

/**
 * @author SleepyFish
 */
class ModifiableAttributeInstance : public IAttributeInstance {

private:

    BaseAttributeMap* attributeMap;
    IAttribute* genericAttribute;
    double baseValue;
    mutable bool needsUpdate;
    mutable double cachedValue;

    std::unordered_map<int, std::unordered_set<AttributeModifier, std::hash<AttributeModifier>>> mapByOperation;
    std::unordered_map<std::string, std::unordered_set<AttributeModifier, std::hash<AttributeModifier>>> mapByName;
    std::unordered_map<UUID, AttributeModifier, std::hash<UUID>> mapByUUID;

    std::unordered_set<AttributeModifier, std::hash<AttributeModifier>> getModifiersForCompute(int operation) const {
        auto it = this->mapByOperation.find(operation);
        std::unordered_set<AttributeModifier, std::hash<AttributeModifier>> result;

        if (it != this->mapByOperation.end()) {
            result = it->second;
        }

        for (IAttribute* parent = this->genericAttribute->func_180372_d(); parent != nullptr; parent = parent->func_180372_d()) {
            IAttributeInstance* parentInstance = this->attributeMap->getAttributeInstance(parent);

            if (parentInstance) {
                for (auto& mod : parentInstance->getModifiersByOperation(operation)) {
                    result.insert(mod);
                }
            }
        }

        return result;
    }

    double computeValue() const {
        double value = this->baseValue;

        for (auto& mod : this->getModifiersForCompute(0)) {
            value += mod.getAmount();
        }

        double result = value;

        for (auto& mod : this->getModifiersForCompute(1)) {
            result += value * mod.getAmount();
        }

        for (auto& mod : this->getModifiersForCompute(2)) {
            result *= 1.0 + mod.getAmount();
        }

        return this->genericAttribute->clampValue(result);
    }

public:

    ModifiableAttributeInstance(BaseAttributeMap* attributeMap, IAttribute* attribute) :
        attributeMap(attributeMap),
        genericAttribute(attribute),
        baseValue(attribute->getDefaultValue()),
        needsUpdate(true),
        cachedValue(0.0)
    {
        for (int i = 0; i < 3; ++i) {
            this->mapByOperation[i] = {};
        }
    }

    IAttribute& getAttribute() override {
        return *this->genericAttribute;
    }

    double getBaseValue() const override {
        return this->baseValue;
    }

    void setBaseValue(double value) override {
        if (value != this->baseValue) {
            this->baseValue = value;
            this->flagForUpdate();
        }
    }

    std::vector<AttributeModifier> getModifiersByOperation(int operation) const override {
        auto it = this->mapByOperation.find(operation);
        if (it == this->mapByOperation.end()) {
            return {};
        }
        return {it->second.begin(), it->second.end()};
    }

    std::vector<AttributeModifier> getAllModifiers() const override {
        std::unordered_set<UUID, std::hash<UUID>> seen;
        std::vector<AttributeModifier> result;

        for (int i = 0; i < 3; ++i) {
            auto it = this->mapByOperation.find(i);
            if (it == this->mapByOperation.end()) {
                continue;
            }

            for (auto& mod : it->second) {
                if (seen.insert(mod.getID()).second) {
                    result.push_back(mod);
                }
            }
        }

        return result;
    }

    AttributeModifier* getModifier(const UUID& uuid) override {
        auto it = this->mapByUUID.find(uuid);
        return it != this->mapByUUID.end() ? &it->second : nullptr;
    }

    bool hasModifier(const AttributeModifier& modifier) const override {
        return this->mapByUUID.count(modifier.getID()) > 0;
    }

    void applyModifier(const AttributeModifier& modifier) override {
        if (this->mapByUUID.count(modifier.getID())) {
            throw std::invalid_argument("Modifier is already applied on this attribute!");
        }

        this->mapByOperation[modifier.getOperation()].insert(modifier);
        this->mapByName[modifier.getName()].insert(modifier);
        this->mapByUUID.emplace(modifier.getID(), modifier);
        this->flagForUpdate();
    }

    void removeModifier(const AttributeModifier& modifier) override {
        for (int i = 0; i < 3; ++i) {
            this->mapByOperation[i].erase(modifier);
        }

        auto it = this->mapByName.find(modifier.getName());
        if (it != this->mapByName.end()) {
            it->second.erase(modifier);

            if (it->second.empty()) {
                this->mapByName.erase(it);
            }
        }

        this->mapByUUID.erase(modifier.getID());
        this->flagForUpdate();
    }

    void removeAllModifiers() override {
        std::vector<AttributeModifier> all = this->getAllModifiers();
        for (auto& mod : all) {
            this->removeModifier(mod);
        }
    }

    double getAttributeValue() const override {
        if (this->needsUpdate) {
            this->cachedValue = this->computeValue();
            this->needsUpdate = false;
        }

        return this->cachedValue;
    }

    void flagForUpdate() {
        this->needsUpdate = true;
        this->attributeMap->onAttributeModified(this);
    }

};


#endif //MCCLONE_MODIFIABLEATTRIBUTEINSTANCE_H
