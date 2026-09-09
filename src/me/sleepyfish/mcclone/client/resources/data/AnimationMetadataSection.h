//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ANIMATIONMETADATASECTION_H
#define MCCLONE_ANIMATIONMETADATASECTION_H

#include "AnimationFrame.h"

#include <vector>
#include <unordered_set>

/**
 * @author SleepyFish
 * @brief Animation metadata section
 */
class AnimationMetadataSection {

private:

    const std::vector<AnimationFrame> animationFrames;
    const int frameWidth;
    const int frameHeight;
    const int frameTime;
    const bool interpolate;

    const AnimationFrame& getAnimationFrame(int index) const noexcept {
        return this->animationFrames[index];
    }

public:

    AnimationMetadataSection(const std::vector<AnimationFrame>& frames, int frameWidth, int frameHeight, int frameTime, bool interpolate) :
        animationFrames(frames),
        frameWidth(frameWidth),
        frameHeight(frameHeight),
        frameTime(frameTime),
        interpolate(interpolate)
    {}

    int getFrameWidth() const noexcept {
        return this->frameWidth;
    }

    int getFrameHeight() const noexcept {
        return this->frameHeight;
    }

    int getFrameCount() const noexcept {
        return static_cast<int>(this->animationFrames.size());
    }

    int getFrameTime() const noexcept {
        return this->frameTime;
    }

    bool isInterpolate() const noexcept {
        return this->interpolate;
    }

    int getFrameTimeSingle(int index) const noexcept {
        const AnimationFrame& frame = getAnimationFrame(index);
        return frame.hasNoTime() ? this->frameTime : frame.getFrameTime();
    }

    bool frameHasTime(int index) const noexcept {
        return !this->animationFrames[index].hasNoTime();
    }

    int getFrameIndex(int index) const noexcept {
        return this->animationFrames[index].getFrameIndex();
    }

    std::unordered_set<int> getFrameIndexSet() const {
        std::unordered_set<int> set;
        for (const auto& frame : this->animationFrames) {
            set.insert(frame.getFrameIndex());
        }
        return set;
    }

};


#endif //MCCLONE_ANIMATIONMETADATASECTION_H
