//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ISTRINGSERIALIZABLE_H
#define MCCLONE_ISTRINGSERIALIZABLE_H

#include <string>

/**
 * @author SleepyFish
 */
class IStringSerializable {

public:

    virtual ~IStringSerializable() = default;

    virtual std::string getName() = 0;

};

#endif //MCCLONE_ISTRINGSERIALIZABLE_H
