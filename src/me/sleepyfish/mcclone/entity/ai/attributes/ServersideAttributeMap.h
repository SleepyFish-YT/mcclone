//
// Created by SleepyFish on 07.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_SERVERSIDEATTRIBUTEMAP_H
#define MCCLONE_SERVERSIDEATTRIBUTEMAP_H

#include "BaseAttributeMap.h"
#include "ModifiableAttributeInstance.h"
#include "RangedAttribute.h"
#include "../../../../sava/SavaUtil.h"

#include <unordered_set>
#include <vector>

/**
 * @author SleepyFish
 */
class ServersideAttributeMap : public BaseAttributeMap {

public:

    virtual ModifiableAttributeInstance* getAttributeInstance(IAttribute* attribute) override {
        return dynamic_cast<ModifiableAttributeInstance*>(BaseAttributeMap::getAttributeInstance(attribute));
    }

    virtual ModifiableAttributeInstance* getAttributeInstanceByName(const std::string& attributeName) override {
        IAttributeInstance* instance = BaseAttributeMap::getAttributeInstanceByName(attributeName);

        if (!instance) {
            auto it = this->m_descriptionToInstance.find(SavaUtil::StringUtil::ToLowerCase(attributeName));
            if (it != this->m_descriptionToInstance.end()) {
                instance = it->second;
            }
        }

        return dynamic_cast<ModifiableAttributeInstance*>(instance);
    }

    virtual IAttributeInstance* registerAttribute(IAttribute* attribute) override {
        IAttributeInstance* instance = BaseAttributeMap::registerAttribute(attribute);

        auto* ranged = dynamic_cast<RangedAttribute*>(attribute);
        if (ranged && !ranged->getDescription().empty()) {
            this->m_descriptionToInstance[SavaUtil::StringUtil::ToLowerCase(ranged->getDescription())] = instance;
        }

        return instance;
    }

    virtual void onAttributeModified(IAttributeInstance* instance) override {
        if (instance->getAttribute().getShouldWatch()) {
            this->m_dirtyInstances.insert(instance);
        }

        for (auto& [parent, child] : this->m_parentToChildren) {
            if (parent == &instance->getAttribute()) {
                ModifiableAttributeInstance* modifiable = getAttributeInstance(child);
                if (modifiable) {
                    modifiable->flagForUpdate();
                }
            }
        }
    }

    std::unordered_set<IAttributeInstance*> getDirtyAttributes() const {
        return this->m_dirtyInstances;
    }

    void clearDirtyAttributes() {
        this->m_dirtyInstances.clear();
    }

    std::vector<IAttributeInstance*> getWatchedAttributes() const {
        std::vector<IAttributeInstance*> result;
        for (auto* instance : getAllAttributes()) {
            if (instance->getAttribute().getShouldWatch()) {
                result.push_back(instance);
            }
        }

        return result;
    }

protected:

    virtual IAttributeInstance* createInstance(IAttribute* attribute) override {
        return new ModifiableAttributeInstance(this, attribute);
    }

private:

    std::unordered_set<IAttributeInstance*> m_dirtyInstances;
    std::unordered_map<std::string, IAttributeInstance*> m_descriptionToInstance;

};


#endif //MCCLONE_SERVERSIDEATTRIBUTEMAP_H
