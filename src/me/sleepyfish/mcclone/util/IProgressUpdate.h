//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_IPROGRESSUPDATE_H
#define MCCLONE_IPROGRESSUPDATE_H

#include <string>

/**
 * @author SleepyFish
 */
class IProgressUpdate {

public:

    virtual ~IProgressUpdate() = default;

    virtual void displaySavingString(std::string message) = 0;

    virtual void resetProgressAndMessage(std::string message) = 0;

    virtual void displayLoadingString(std::string message) = 0;

    virtual void setLoadingProgress(int progress) = 0;

    virtual void setDoneWorking() = 0;

};


#endif //MCCLONE_IPROGRESSUPDATE_H
