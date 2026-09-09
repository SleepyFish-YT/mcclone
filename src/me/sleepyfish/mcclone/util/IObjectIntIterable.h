//
// Created by SleepyFish on 10.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_IOBJECTINTITERABLE_H
#define MCCLONE_IOBJECTINTITERABLE_H

#include <vector>

/**
 * @author SleepyFish
 * @brief Marker interface for object-int iterables
 */
template<typename T>
class IObjectIntIterable {

public:

    virtual ~IObjectIntIterable() = default;

    virtual typename std::vector<T>::iterator begin() = 0;

    virtual typename std::vector<T>::iterator end() = 0;

};


#endif //MCCLONE_IOBJECTINTITERABLE_H
