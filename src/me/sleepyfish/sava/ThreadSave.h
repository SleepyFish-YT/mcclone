//
// Created by SleepyFish on 14.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_THREADSAVE_H
#define MCCLONE_THREADSAVE_H

#include <atomic>

/**
 * @author SleepyFish - SleepyAVA
 * @version 1.0
 * @brief Thread save object wrapper
 */
template<typename T>
class ThreadSave {

    static_assert(std::is_trivially_copyable_v<T>, "ThreadSafe<T> requires T to be trivially copyable");

private:

    std::atomic<T> threadValue{};

public:

    explicit ThreadSave(T value) noexcept :
        threadValue(value)
    {}

    ThreadSave(const ThreadSave &other, std::memory_order order = std::memory_order_acquire) noexcept :
        threadValue(other.get(order))
    {}

    void set(T value, std::memory_order order = std::memory_order_release) noexcept {
        this->threadValue.store(value, order);
    }

    T get(std::memory_order order = std::memory_order_acquire) const noexcept {
        return this->threadValue.load(order);
    }

    ThreadSave &operator=(const ThreadSave &other) noexcept {
        this->set(other.get());
        return *this;
    }

    ThreadSave &operator=(T value) noexcept {
        this->set(value);
        return *this;
    }

    bool operator==(const ThreadSave &other) const noexcept {
        return this->get() == other.get();
    }

    explicit operator T() const noexcept {
        return this->get();
    }

};


#endif //MCCLONE_THREADSAVE_H
