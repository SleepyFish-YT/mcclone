//
// Created by SleepyFish on 02.09.2026.
// Project: mcclone
//

#include "Timer.h"

#include "../client/Minecraft.h"

// member initializer list instead of body assignments is not really needed, since Timer will only be created once
Timer::Timer(float tps) noexcept :
    ticksPerSecond(tps),
    timerSpeed(1.0f)
{
    this->lastHRTime = 0.0;
    this->lastSyncSysClock = Minecraft::getSystemTime();
    this->lastSyncHRClock = Minecraft::getHighResTime();
    this->counter = 0ll;
    this->timeSyncAdjustment = 1.0;

    this->elapsedTicks = 0;
    this->renderPartialTicks = 0.0f;
    this->elapsedPartialTicks = 0.0f;
}

void Timer::updateTimer() {
    const long long now = Minecraft::getSystemTime();
    const long long now_diff = now - this->lastSyncSysClock;
    const long long now_hr = Minecraft::getHighResTime();

    const double now_hr_secs = static_cast<double>(now_hr) / 1000.0;

    if (now_diff <= 1000ll && now_diff >= 0ll) {
        this->counter += now_diff;

        if (this->counter > 1000ll) {
            const long long now_hr_diff = now_hr - this->lastSyncHRClock;
            const double syncRatio = static_cast<double>(this->counter) / static_cast<double>(now_hr_diff);

            this->timeSyncAdjustment += (syncRatio - this->timeSyncAdjustment) * 0.2;
            this->lastSyncHRClock = now_hr;
            this->counter = 0ll;
        }

        if (this->counter < 0ll) {
            this->lastSyncHRClock = now_hr;
        }
    } else {
        this->lastHRTime = now_hr_secs;
    }

    this->lastSyncSysClock = now;

    const double rawDelta = (now_hr_secs - this->lastHRTime) * this->timeSyncAdjustment;
    this->lastHRTime = now_hr_secs;
    const double deltaTime = std::clamp(rawDelta, 0.0, 1.0);

    this->elapsedPartialTicks += static_cast<float>(deltaTime * this->timerSpeed * this->ticksPerSecond);
    this->elapsedTicks = static_cast<int>(this->elapsedPartialTicks);
    this->elapsedPartialTicks -= static_cast<float>(this->elapsedTicks);

    // your sh*t anti-freeze
    if (this->elapsedTicks > 10) {
        this->elapsedTicks = 10;
    }

    this->renderPartialTicks = this->elapsedPartialTicks;
}
