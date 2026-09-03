//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_MOUSEFILTER_H
#define MCCLONE_MOUSEFILTER_H

/**
 * @author SleepyFish
 */
class MouseFilter {

private:

    float a, b, c;

public:

    MouseFilter() noexcept :
        a(0.0f),
        b(0.0f),
        c(0.0f)
    {}

    float smooth(float value, float margin) noexcept {
        this->a += value;
        value = (this->a - this->b) * margin;
        this->c += (value - this->c) * 0.5f;

        if (value > 0.0f && value > this->c || value < 0.0f && value < this->c) {
            value = this->c;
        }

        this->b += value;
        return value;
    }

    void reset() noexcept {
        this->a = 0.0f;
        this->b = 0.0f;
        this->c = 0.0f;
    }

};

#endif //MCCLONE_MOUSEFILTER_H
