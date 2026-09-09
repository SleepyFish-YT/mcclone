//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ISTATSTRINGFORMAT_H
#define MCCLONE_ISTATSTRINGFORMAT_H

#include <string>

/**
 * @author SleepyFish
 * @brief Stat string format interface
 */
class IStatStringFormat {

public:

    virtual ~IStatStringFormat() {}

    virtual std::string formatString(int value) = 0;

};


#endif //MCCLONE_ISTATSTRINGFORMAT_H
