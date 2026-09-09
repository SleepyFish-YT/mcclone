//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_INTCACHE_H
#define MCCLONE_INTCACHE_H

#include <vector>
#include <mutex>
#include <string>

/**
 * @author SleepyFish
 * @brief Integer cache
 */
class IntCache {

private:

    static std::mutex mutex;

    static int intCacheSize;
    static std::vector<int *> freeSmallArrays;
    static std::vector<int *> inUseSmallArrays;
    static std::vector<int *> freeLargeArrays;
    static std::vector<int *> inUseLargeArrays;

public:

    static int *getIntCache(int size) {
        std::lock_guard<std::mutex> lock(IntCache::mutex);

        if (size <= 256) {
            if (IntCache::freeSmallArrays.empty()) {
                int *arr = new int[256];
                IntCache::inUseSmallArrays.push_back(arr);
                return arr;
            } else {
                int *arr = IntCache::freeSmallArrays.back();
                IntCache::freeSmallArrays.pop_back();
                IntCache::inUseSmallArrays.push_back(arr);
                return arr;
            }
        } else if (size > IntCache::intCacheSize) {
            IntCache::intCacheSize = size;
            for (int *arr: IntCache::freeLargeArrays) delete[] arr;
            for (int *arr: IntCache::inUseLargeArrays) delete[] arr;
            IntCache::freeLargeArrays.clear();
            IntCache::inUseLargeArrays.clear();
            int *arr = new int[IntCache::intCacheSize];
            IntCache::inUseLargeArrays.push_back(arr);
            return arr;
        } else if (IntCache::freeLargeArrays.empty()) {
            int *arr = new int[IntCache::intCacheSize];
            IntCache::inUseLargeArrays.push_back(arr);
            return arr;
        } else {
            int *arr = IntCache::freeLargeArrays.back();
            IntCache::freeLargeArrays.pop_back();
            IntCache::inUseLargeArrays.push_back(arr);
            return arr;
        }
    }

    static void resetIntCache() {
        std::lock_guard<std::mutex> lock(IntCache::mutex);

        if (!IntCache::freeLargeArrays.empty()) {
            delete[] IntCache::freeLargeArrays.back();
            IntCache::freeLargeArrays.pop_back();
        }

        if (!IntCache::freeSmallArrays.empty()) {
            delete[] IntCache::freeSmallArrays.back();
            IntCache::freeSmallArrays.pop_back();
        }

        for (int *arr: IntCache::inUseLargeArrays) IntCache::freeLargeArrays.push_back(arr);
        for (int *arr: IntCache::inUseSmallArrays) IntCache::freeSmallArrays.push_back(arr);
        IntCache::inUseLargeArrays.clear();
        IntCache::inUseSmallArrays.clear();
    }

    static std::string getCacheSizes() {
        std::lock_guard<std::mutex> lock(IntCache::mutex);
        return "cache: "      + std::to_string(IntCache::freeLargeArrays.size())  +
               ", tcache: "   + std::to_string(IntCache::freeSmallArrays.size())  +
               ", allocated: "+ std::to_string(IntCache::inUseLargeArrays.size()) +
               ", tallocated:"+ std::to_string(IntCache::inUseSmallArrays.size());
    }

};

inline std::mutex IntCache::mutex;
inline int IntCache::intCacheSize = 256;
inline std::vector<int*> IntCache::freeSmallArrays;
inline std::vector<int*> IntCache::inUseSmallArrays;
inline std::vector<int*> IntCache::freeLargeArrays;
inline std::vector<int*> IntCache::inUseLargeArrays;


#endif //MCCLONE_INTCACHE_H
