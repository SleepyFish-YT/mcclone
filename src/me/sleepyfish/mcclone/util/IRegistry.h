//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_IREGISTRY_H
#define MCCLONE_IREGISTRY_H

/**
 * @author SleepyFish
 */
template<typename K, typename V>
class IRegistry {

public:

    virtual ~IRegistry() = default;

    virtual V* getObject(const K& name) = 0;

    virtual void putObject(const K& key, V value) = 0;

};


#endif //MCCLONE_IREGISTRY_H
