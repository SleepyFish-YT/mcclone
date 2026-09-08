//
// Created by SleepyFish on 07.09.2026.
// Project: mcclone
//

#include "SavaUtil.h"

#include "../mcclone/util/MathHelper.h"

#include <random>

std::string SavaUtil::StringUtil::GetRandomPlayerName() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    const int rand = MathHelper::getRandomIntegerInRange(gen, 1, 999);
    return "Player" + std::to_string(rand);
}
