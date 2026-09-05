//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_REGISTRYSIMPLE_H
#define MCCLONE_REGISTRYSIMPLE_H

#include "IRegistry.h"
#include "../debug/Logger.h"

#include <unordered_map>
#include <unordered_set>
#include <stdexcept>
#include <string>
#include <iostream>

/**
 * @author SleepyFish
 */
template<typename K, typename V, typename Hash = std::hash<K>, typename Equal = std::equal_to<K>>
class RegistrySimple : public IRegistry<K, V> {

protected:

    std::unordered_map<K, V, Hash, Equal> registryObjects;

public:

    V *getObject(const K &name) override {
        auto it = this->registryObjects.find(name);
        return it != this->registryObjects.end() ? &it->second : nullptr;
    }

    void putObject(const K &key, V value) override {
        /*
        // this will cause a compile error for ResourceLocation since it has no default constructor
        if (key == K{})
            throw std::invalid_argument("key must not be null");
        */

        if (this->registryObjects.count(key)) {
            Logger::warn("Adding duplicate key to registry");
        }

        this->registryObjects.insert_or_assign(key, std::move(value));
    }

    std::unordered_set<K, Hash, Equal> getKeys() const {
        std::unordered_set<K, Hash, Equal> keys;
        for (const auto &[k, v]: this->registryObjects)
            keys.insert(k);
        return keys;
    }

    bool containsKey(const K &key) const {
        return this->registryObjects.count(key) > 0;
    }

    auto begin() noexcept {
        return this->registryObjects.begin();
    }

    auto end() noexcept {
        return this->registryObjects.end();
    }

    auto begin() const noexcept {
        return this->registryObjects.begin();
    }

    auto end() const noexcept {
        return this->registryObjects.end();
    }

};


#endif //MCCLONE_REGISTRYSIMPLE_H
