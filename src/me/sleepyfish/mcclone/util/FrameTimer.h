//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_FRAMETIMER_H
#define MCCLONE_FRAMETIMER_H

#include <array>

/**
 * @author SleepyFish
 * @brief Tracks the last 240 frames for the lagometer
 */
class FrameTimer {

private:

    std::array<long long, 240> frames{};

    int lastIndex = 0;

    int counter = 0;

    int index = 0;

public:

    void addFrame(long long runningTime) noexcept {
        this->frames[this->index] = runningTime;
        ++this->index;

        if (this->index == 240) {
            this->index = 0;
        }

        if (this->counter < 240) {
            this->lastIndex = 0;
            ++this->counter;
        } else {
            this->lastIndex = this->parseIndex(this->index + 1);
        }
    }

    // never used in minecraft
    int getLagometerValue(long long time, int multiplier) const noexcept {
        const double d0 = static_cast<double>(time) / 1.6666666e7;
        return static_cast<int>(d0 * static_cast<double>(multiplier));
    }

    int getLastIndex() const noexcept {
        return this->lastIndex;
    }

    int getIndex() const noexcept {
        return this->index;
    }

    int parseIndex(int rawIndex) const noexcept {
        return rawIndex % 240;
    }

    const std::array<long long, 240>& getFrames() const noexcept {
        return this->frames;
    }

};


#endif //MCCLONE_FRAMETIMER_H
