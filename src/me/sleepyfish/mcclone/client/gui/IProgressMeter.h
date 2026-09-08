//
// Created by SleepyFish on 07.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_IPROGRESSMETER_H
#define MCCLONE_IPROGRESSMETER_H

#include <string>
#include <array>

/**
 * @author SleepyFish
 * @brief Progress meter interface
 */
class IProgressMeter {

public:

    virtual ~IProgressMeter() = default;

    const std::array<std::string, 7> lanSearchStates = {"oooooo", "Oooooo", "oOoooo", "ooOooo", "oooOoo", "ooooOo", "oooooO"};

    virtual void doneLoading() = 0;

};


#endif //MCCLONE_IPROGRESSMETER_H
