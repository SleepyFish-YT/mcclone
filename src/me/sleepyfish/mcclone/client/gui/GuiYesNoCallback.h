//
// Created by SleepyFish on 07.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_GUIYESNOCALLBACK_H
#define MCCLONE_GUIYESNOCALLBACK_H

/**
 * @author SleepyFish
 * @brief gui yes no callback
 */
class GuiYesNoCallback {

public:

    virtual ~GuiYesNoCallback() = default;

    virtual void confirmClicked(bool result, int id) = 0;

};


#endif //MCCLONE_GUIYESNOCALLBACK_H
