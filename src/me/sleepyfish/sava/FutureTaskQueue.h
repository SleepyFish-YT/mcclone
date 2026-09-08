//
// Created by SleepyFish on 07.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_FUTURETASKQUEUE_H
#define MCCLONE_FUTURETASKQUEUE_H

#include "../mcclone/util/Util.h"

#include <queue>
#include <functional>
#include <mutex>
#include <future>

/**
 * @author SleepyFish - SleepyAVA
 * @version 1.1
 * @brief Thread-safe queue of callable tasks, equivalent to Java's Queue<FutureTask<?>>
 * @template T The return type of the task
 */
template<typename T>
class FutureTaskQueue {

private:

    std::queue<std::packaged_task<T()>> tasks;

    std::mutex mutex;

public:

    void push(std::packaged_task<T()> task) {
        std::lock_guard<std::mutex> lock(this->mutex);
        this->tasks.push(std::move(task));
    }

    bool empty() {
        std::lock_guard<std::mutex> lock(this->mutex);
        return this->tasks.empty();
    }

    void runAll() {
        std::lock_guard<std::mutex> lock(this->mutex);
        while (!this->tasks.empty()) {
            Util::runTaskFunc(this->tasks.front());
            this->tasks.pop();
        }
    }

    std::future<T> pollAndRun() {
        std::lock_guard<std::mutex> lock(this->mutex);
        if (this->tasks.empty()) {
            return {};
        }

        auto future = this->tasks.front().get_future();
        this->tasks.front()();
        this->tasks.pop();
        return future;
    }

    std::size_t size() {
        std::lock_guard<std::mutex> lock(this->mutex);
        return this->tasks.size();
    }

    void clear() {
        std::lock_guard<std::mutex> lock(this->mutex);
        while (!this->tasks.empty()) {
            this->tasks.pop();
        }
    }

};

#endif //MCCLONE_FUTURETASKQUEUE_H
