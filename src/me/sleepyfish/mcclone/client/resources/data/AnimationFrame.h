//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ANIMATIONFRAME_H
#define MCCLONE_ANIMATIONFRAME_H

/**
 * @author SleepyFish
 */
class AnimationFrame {

private:

    const int frameIndex;
    const int frameTime;

public:

    explicit AnimationFrame(int p_i1307_1_) noexcept :
        AnimationFrame(p_i1307_1_, -1)
    {}

    AnimationFrame(int p_i1308_1_, int p_i1308_2_) noexcept :
        frameIndex(p_i1308_1_),
        frameTime(p_i1308_2_)
    {}

    bool hasNoTime() const noexcept {
        return this->frameTime == -1;
    }

    int getFrameTime() const noexcept {
        return this->frameTime;
    }

    int getFrameIndex() const noexcept {
        return this->frameIndex;
    }

};

#endif //MCCLONE_ANIMATIONFRAME_H
