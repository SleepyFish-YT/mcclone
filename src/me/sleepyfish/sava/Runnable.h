//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#ifndef MCCLONE_RUNNABLE_H
#define MCCLONE_RUNNABLE_H

#include <stop_token>
#include <thread>


/**
 * @author SleepyFish - SleepyAVA
 * @version 1.4
 * @brief Runnable interface class for threads
 */
class Runnable {

    std::jthread thread;

protected:

    virtual void run(std::stop_token st) = 0;
    virtual void onStart() {}
    virtual void onJoin()  {}
    virtual void onStop()  {}

public:

    Runnable() = default;
    virtual ~Runnable() { this->stop(); }

    // disable copy/move
    Runnable(const Runnable&)            = delete;
    Runnable &operator=(const Runnable&) = delete;
    Runnable(Runnable&&)                 = delete;
    Runnable &operator=(Runnable&&)      = delete;

    void start() {
        if (this->thread.joinable()) {
            return;
        }

        this->thread = std::jthread([this](std::stop_token st) {
            this->onStart();
            this->run(std::move(st));
            this->onJoin();
        });
    }

    void requestStop() {
        if (this->thread.joinable()) {
            this->thread.request_stop();
            this->onStop();
        }
    }

    void join() noexcept {
        if (this->thread.joinable()) {
            this->thread.join();
        }
    }

    void stop() {
        this->requestStop();
        this->join();
    }

    bool isRunning() const noexcept {
        return this->thread.joinable() && !this->getStopToken().stop_requested();
    }

    std::stop_token getStopToken() const noexcept {
        return this->thread.get_stop_token();
    }

};

#endif //MCCLONE_RUNNABLE_H