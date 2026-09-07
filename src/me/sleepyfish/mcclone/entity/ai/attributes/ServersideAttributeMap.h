//
// Created by SleepyFish on 07.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_SERVERSIDEATTRIBUTEMAP_H
#define MCCLONE_SERVERSIDEATTRIBUTEMAP_H

#include "BaseAttributeMap.h"
#include "ModifiableAttributeInstance.h"
#include "RangedAttribute.h"

#include <unordered_set>
#include <vector>

/**
 * @author SleepyFish
 */
class ServersideAttributeMap : public BaseAttributeMap {

public:

    ModifiableAttributeInstance* getAttributeInstance(IAttribute* attribute) override {
        return static_cast<ModifiableAttributeInstance*>(BaseAttributeMap::getAttributeInstance(attribute));
    }

    ModifiableAttributeInstance* getAttributeInstanceByName(const std::string& attributeName) override {
        IAttributeInstance* instance = BaseAttributeMap::getAttributeInstanceByName(attributeName);

        if (!instance) {
            auto it = m_descriptionToInstance.find(toLowerCase(attributeName));
            if (it != m_descriptionToInstance.end())
                instance = it->second;
        }

        return static_cast<ModifiableAttributeInstance*>(instance);
    }

    IAttributeInstance* registerAttribute(IAttribute* attribute) override {
        IAttributeInstance* instance = BaseAttributeMap::registerAttribute(attribute);

        RangedAttribute* ranged = dynamic_cast<RangedAttribute*>(attribute);
        if (ranged && !ranged->getDescription().empty())
            m_descriptionToInstance[toLowerCase(ranged->getDescription())] = instance;

        return instance;
    }

    void onAttributeModified(IAttributeInstance* instance) override {
        if (instance->getAttribute().getShouldWatch())
            m_dirtyInstances.insert(instance);

        for (auto& [parent, child] : m_parentToChildren) {
            if (parent == &instance->getAttribute()) {
                ModifiableAttributeInstance* modifiable = getAttributeInstance(child);
                if (modifiable)
                    modifiable->flagForUpdate();
            }
        }
    }

    std::unordered_set<IAttributeInstance*> getDirtyAttributes() const {
        return m_dirtyInstances;
    }

    void clearDirtyAttributes() {
        m_dirtyInstances.clear();
    }

    std::vector<IAttributeInstance*> getWatchedAttributes() const {
        std::vector<IAttributeInstance*> result;
        for (auto* instance : getAllAttributes()) {
            if (instance->getAttribute().getShouldWatch())
                result.push_back(instance);
        }
        return result;
    }

protected:

    IAttributeInstance* createInstance(IAttribute* attribute) override {
        return new ModifiableAttributeInstance(this, attribute);
    }

private:

    std::unordered_set<IAttributeInstance*> m_dirtyInstances;
    std::unordered_map<std::string, IAttributeInstance*> m_descriptionToInstance;

    static std::string toLowerCase(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }

};


#endif //MCCLONE_SERVERSIDEATTRIBUTEMAP_H
