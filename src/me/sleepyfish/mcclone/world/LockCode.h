//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_LOCKCODE_H
#define MCCLONE_LOCKCODE_H

#include "../nbt/NBTTagCompound.h"

#include <string>

/**
 * @author SleepyFish
 * @brief Lock code for lockable tile entities
 */
class LockCode {

private:

    std::string lock;

public:

    static const LockCode EMPTY_CODE;

    explicit LockCode(std::string code) :
        lock(std::move(code)) // might need to change to 'std::string &code' reference
    {}

    bool isEmpty() const noexcept {
        return this->lock.empty();
    }

    const std::string& getLock() const noexcept {
        return this->lock;
    }

    void toNBT(NBTTagCompound& nbt) const {
        nbt.setString("Lock", this->lock);
    }

    static LockCode fromNBT(const NBTTagCompound& nbt) {
        if (nbt.hasKey("Lock", 8)) {
            return LockCode(nbt.getString("Lock"));
        }
        return LockCode::EMPTY_CODE;
    }

};

inline const LockCode LockCode::EMPTY_CODE = LockCode("");

#endif //MCCLONE_LOCKCODE_H
