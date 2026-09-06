//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_NBTSIZETRACKER_H
#define MCCLONE_NBTSIZETRACKER_H

#include <stdexcept>
#include <format>
#include <cstdint>

/**
 * @author SleepyFish
 * @brief Tracks the size of NBT data being read
 */
class NBTSizeTracker {

private:

    const int64_t max;

    int64_t read_var = 0;

public:

    static const NBTSizeTracker INFINITE;

    explicit NBTSizeTracker(int64_t max) : max(max) {}

    virtual void read(int64_t bits) {
        this->read_var += bits / 8ll;

        if (this->read_var > this->max) {
            throw std::runtime_error(std::format(
                    "Tried to read NBT tag that was too big; tried to allocate: {}bytes where max allowed: {}",
                    this->read_var, this->max
            ));
        }
    }

    virtual ~NBTSizeTracker() = default;

};


#endif //MCCLONE_NBTSIZETRACKER_H
