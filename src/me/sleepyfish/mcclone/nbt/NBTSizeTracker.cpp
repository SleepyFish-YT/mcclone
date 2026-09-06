//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#include "NBTSizeTracker.h"

namespace {

    class InfiniteNBTSizeTracker : public NBTSizeTracker {

    public:

        InfiniteNBTSizeTracker() :
            NBTSizeTracker(0ll)
        {}

        void read(int64_t bits) override {
        }

    };

}

const NBTSizeTracker NBTSizeTracker::INFINITE = InfiniteNBTSizeTracker();
