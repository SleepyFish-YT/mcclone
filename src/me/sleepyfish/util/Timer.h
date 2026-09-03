//
// Created by SleepyFish on 02.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_TIMER_H
#define MCCLONE_TIMER_H

/**
 * @author SleepyFish
 * @brief Timer class
 */
class Timer {

private:

    double lastHRTime;

    long long lastSyncSysClock;

    long long lastSyncHRClock;

    long long counter;

    double timeSyncAdjustment;

public:

    const float ticksPerSecond;

    int elapsedTicks;

    float renderPartialTicks;

    float timerSpeed;

    float elapsedPartialTicks;

    explicit Timer(float tps) noexcept;

    void updateTimer();

};


#endif //MCCLONE_TIMER_H
