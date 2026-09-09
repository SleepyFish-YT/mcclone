//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ENUMBORDERSTATUS_H
#define MCCLONE_ENUMBORDERSTATUS_H

/**
 * @author SleepyFish
 * @brief World border status color enum
 */
enum class EnumBorderStatus {

    GROWING = 4259712,
    SHRINKING = 16724016,
    STATIONARY = 2138367

};

inline int getID(EnumBorderStatus status) noexcept {
    return static_cast<int>(status);
}


#endif //MCCLONE_ENUMBORDERSTATUS_H
