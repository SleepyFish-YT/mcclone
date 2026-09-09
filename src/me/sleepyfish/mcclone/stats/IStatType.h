//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ISTATTYPE_H
#define MCCLONE_ISTATTYPE_H

#include <string>

/**
 * @author SleepyFish
 */
class IStatType {

public:

    virtual ~IStatType() = default;

    virtual std::string format(int number) = 0;

};


#endif //MCCLONE_ISTATTYPE_H
