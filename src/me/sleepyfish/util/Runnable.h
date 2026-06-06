//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#ifndef MCCLONE_RUNNABLE_H
#define MCCLONE_RUNNABLE_H

#include <thread>
#include <atomic>

/**
 * @author SleepyFish
 * @brief Runnable interface class for threads
 */
class Runnable {

private:

    std::thread thread;

protected:

    std::atomic<bool> running;

    virtual void run() = 0; // subclass implements this

public:

    Runnable() : running(false) {}

    void start() {
        this->running = true;
        this->thread = std::thread(&Runnable::run, this);
    }

    void stop() {
        this->running = false;
    }

    void join() {
        if (this->thread.joinable()) {
            this->thread.join();
        }
    }

    bool isRunning() const {
        return this->running;
    }

};

#endif //MCCLONE_RUNNABLE_H