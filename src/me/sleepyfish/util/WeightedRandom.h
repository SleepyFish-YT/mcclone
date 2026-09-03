//
// Created by SleepyFish on 02.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_WEIGHTEDRANDOM_H
#define MCCLONE_WEIGHTEDRANDOM_H

#include <vector>
#include <random>

/**
 * @author SleepyFish
 * @brief This class is used to generate weighted random numbers
 */
class WeightedRandom {

public:

    class Item {

    public:

        int itemWeight;

        explicit Item(int weight) noexcept : itemWeight(weight) {}

        virtual ~Item() = default;

    };

    template<typename T>
    static int getTotalWeight(const std::vector<T>& collection);

    template<typename T>
    static T getRandomItem(std::mt19937& random, const std::vector<T>& collection, int totalWeight);

    template<typename T>
    static T getRandomItem(const std::vector<T>& collection, int weight);

    template<typename T>
    static T getRandomItem(std::mt19937& random, const std::vector<T>& collection);

};


#endif //MCCLONE_WEIGHTEDRANDOM_H
