//
// Created by SleepyFish on 02.09.2026.
// Project: mcclone
//

#include "Timer.h"

#include "../client/Minecraft.h"

#include <chrono>

// member initializer list instead of body assignments is not really needed, since Timer will only be created once
Timer::Timer(float tps) noexcept :
    ticksPerSecond(tps)
{
    this->lastHRTime = 0.0;
    this->lastSyncSysClock = Minecraft::getSystemTime();
    this->lastSyncHRClock = Minecraft::getHighResTime();
    this->counter = 0ll;
    this->timeSyncAdjustment = 0.0;

    this->elapsedTicks = 0;
    this->renderPartialTicks = 0.0f;
    this->timerSpeed = 1.0f;
    this->elapsedPartialTicks = 0.0f;
}

void Timer::updateTimer() {
    const long long i = Minecraft::getSystemTime();
    const long long j = i - this->lastSyncSysClock;
    const long long k = Minecraft::getHighResTime();

    const double d0 = static_cast<double>(k) / 1000.0;

    if (j <= 1000ll && j >= 0ll) {
        this->counter += j;

        if (this->counter > 1000ll) {
            const long long l = k - this->lastSyncHRClock;
            const double d1 = static_cast<double>(this->counter) / static_cast<double>(l);

            this->timeSyncAdjustment += (d1 - this->timeSyncAdjustment) * 0.20000000298023224;
            this->lastSyncHRClock = k;
            this->counter = 0ll;
        }

        if (this->counter < 0ll) {
            this->lastSyncHRClock = k;
        }
    } else {
        this->lastHRTime = d0;
    }

    this->lastSyncSysClock = i;

    double d2 = (d0 - this->lastHRTime) * this->timeSyncAdjustment;
    this->lastHRTime = d0;
    d2 = std::clamp(d2, 0.0, 1.0); // not sure if clamp is needed.

    this->elapsedPartialTicks += static_cast<float>(d2 * this->timerSpeed * this->ticksPerSecond);
    this->elapsedTicks = static_cast<int>(this->elapsedPartialTicks);
    this->elapsedPartialTicks -= static_cast<float>(this->elapsedTicks);

    if (this->elapsedTicks > 10) {
        this->elapsedTicks = 10;
    }

    this->renderPartialTicks = this->elapsedPartialTicks;
}

