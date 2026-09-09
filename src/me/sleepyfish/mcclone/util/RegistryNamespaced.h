//
// Created by SleepyFish on 10.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_REGISTRYNAMESPACED_H
#define MCCLONE_REGISTRYNAMESPACED_H

#include "RegistrySimple.h"
#include "ObjectIntIdentityMap.h"

#include <unordered_map>
#include <stdexcept>

/**
 * @author SleepyFish
 * @brief Namespaced registry - maps keys to values with integer IDs
 */
template<typename K, typename V>
class RegistryNamespaced : public RegistrySimple<K, V> {

protected:

    ObjectIntIdentityMap<V>  underlyingIntegerMap;
    std::unordered_map<V, K> inverseObjectRegistry;

    std::unordered_map<K, V> createUnderlyingMap() override {
        return std::unordered_map<K, V>();
    }

public:

    void register_(int id, const K &key, V *value) {
        this->underlyingIntegerMap.put(value, id);
        this->putObject(key, value);
        this->inverseObjectRegistry[value] = key;
    }

    V *getObject(const K &name) override {
        return RegistrySimple<K, V>::getObject(name);
    }

    const K *getNameForObject(V *value) const {
        auto it = this->inverseObjectRegistry.find(value);
        if (it == this->inverseObjectRegistry.end()) {
            return nullptr;
        }

        return &it->second;
    }

    bool containsKey(const K &key) const override {
        return RegistrySimple<K, V>::containsKey(key);
    }

    int getIDForObject(V *value) const {
        return this->underlyingIntegerMap.get(value);
    }

    V* getObjectById(int id) const {
        return this->underlyingIntegerMap.getByValue(id);
    }

    // -------------------------------------------------------------------------
    // Iteration over values (via underlyingIntegerMap)
    // -------------------------------------------------------------------------

    // shadow underlyingIntegerMap
    typename ObjectIntIdentityMap<V>::iterator begin() {
        return this->underlyingIntegerMap.begin();
    }

    // shadow underlyingIntegerMap
    typename ObjectIntIdentityMap<V>::iterator end() {
        return this->underlyingIntegerMap.end();
    }

};


#endif //MCCLONE_REGISTRYNAMESPACED_H
