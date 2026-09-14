//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#ifndef MCCLONE_RUNNABLE_H
#define MCCLONE_RUNNABLE_H

#include "ThreadSave.h"

#include <thread>

/**
 * @author SleepyFish - SleepyAVA
 * @version 1.3
 * @brief Runnable interface class for threads
 */
class Runnable {

private:

    std::thread thread;

    ThreadSave<bool> running;

protected:

    virtual void run() = 0; // subclass must implement this

    virtual void onStart() {} // subclass can implement this

    virtual void onJoin() {} // subclass can implement this

    virtual void onStop() {} // subclass can implement this

public:

    Runnable() noexcept :
        running(false)
    {}

    ~Runnable() {
        this->stop();
        this->join();
    }

    void start() {
        this->setRunning(true);
        this->onStart();

        this->thread = std::thread(&Runnable::run, this);
    }

    void stop() {
        this->setRunning(false);
        this->onStop();
    }

    void join() {
        if (this->thread.joinable()) {
            this->onJoin();
            this->thread.join();
        }
    }

    void setRunning(bool value, std::memory_order order = std::memory_order_release) noexcept {
        this->running.set(value, order);
    }

    bool isRunning(std::memory_order order = std::memory_order_acquire) const noexcept {
        return this->running.get(order);
    }

};

#endif //MCCLONE_RUNNABLE_H