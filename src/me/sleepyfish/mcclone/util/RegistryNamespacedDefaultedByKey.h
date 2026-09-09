//
// Created by SleepyFish on 10.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_REGISTRYNAMESPACEDDEFAULTEDBYKEY_H
#define MCCLONE_REGISTRYNAMESPACEDDEFAULTEDBYKEY_H

#include "RegistryNamespaced.h"

#include <stdexcept>

/**
 * @author SleepyFish
 * @brief Namespaced registry with a default value fallback for unknown keys/ids
 */
template<typename K, typename V>
class RegistryNamespacedDefaultedByKey : public RegistryNamespaced<K, V> {

private:

    const K defaultValueKey;
    V *defaultValue = nullptr;

public:

    explicit RegistryNamespacedDefaultedByKey(const K& defaultValueKeyIn) :
        defaultValueKey(defaultValueKeyIn)
    {}

    // shadowing RegistryNamespaced
    void register_(int id, const K &key, V *value) {
        if (key == this->defaultValueKey) {
            this->defaultValue = value;
        }

        RegistryNamespaced<K, V>::register_(id, key, value);
    }

    void validateKey() const {
        if (this->defaultValue == nullptr) {
            throw std::runtime_error("Default value key was never registered");
        }
    }

    V *getObject(const K &name) override {
        V *v = RegistryNamespaced<K, V>::getObject(name);
        return v == nullptr ? this->defaultValue : v;
    }

    // shadowing RegistryNamespaced
    V *getObjectById(int id) const {
        V *v = RegistryNamespaced<K, V>::getObjectById(id);
        return v == nullptr ? this->defaultValue : v;
    }

};


#endif //MCCLONE_REGISTRYNAMESPACEDDEFAULTEDBYKEY_H
