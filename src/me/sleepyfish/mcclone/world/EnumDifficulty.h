//
// Created by SleepyFish on 08.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ENUMDIFFICULTY_H
#define MCCLONE_ENUMDIFFICULTY_H

#include <string>

/**
 * @author SleepyFish
 * @brief world difficulty enum
 */
enum class EnumDifficulty {

    PEACEFUL = 0,
    EASY,
    NORMAL,
    HARD

};


struct DifficultyInfo {

    int difficultyId;

    std::string difficultyResourceKey;

};

inline const DifficultyInfo& getDifficultyInfo(EnumDifficulty difficulty) {
    static const DifficultyInfo infos[] = {
            {0, "options.difficulty.peaceful"},
            {1, "options.difficulty.easy"},
            {2, "options.difficulty.normal"},
            {3, "options.difficulty.hard"},
    };

    return infos[static_cast<int>(difficulty)];
}

inline EnumDifficulty getDifficultyEnum(int id) {
    return static_cast<EnumDifficulty>(id % 4);
}


#endif //MCCLONE_ENUMDIFFICULTY_H
