//
// Created by SleepyFish on 02.09.2026.
// Project: mcclone
//

#include "WeightedRandom.h"

#include <stdexcept>

template<typename T>
int WeightedRandom::getTotalWeight(const std::vector<T>& collection) {
    int total = 0;

    for (const auto& item : collection) {
        total += item.itemWeight;
    }

    return total;
}

template<typename T>
T WeightedRandom::getRandomItem(std::mt19937& random, const std::vector<T>& collection, int totalWeight) {
    if (totalWeight <= 0) {
        throw std::invalid_argument("Total weight must be positive");
    }

    std::uniform_int_distribution<int> dist(0, totalWeight - 1);
    int weight = dist(random);
    return getRandomItem(collection, weight);
}

template<typename T>
T WeightedRandom::getRandomItem(const std::vector<T>& collection, int weight) {
    for (const auto& item : collection) {
        weight -= item.itemWeight;
        if (weight < 0) {
            return item;
        }
    }

    return T{};
}

template<typename T>
T WeightedRandom::getRandomItem(std::mt19937& random, const std::vector<T>& collection) {
    return getRandomItem(random, collection, getTotalWeight(collection));
}
