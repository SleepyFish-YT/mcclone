//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ENCHANTMENTNAMEPARTS_H
#define MCCLONE_ENCHANTMENTNAMEPARTS_H

#include <string>
#include <array>
#include <random>
#include <string_view>

/**
 * @author SleepyFish
 */
class EnchantmentNameParts {

private:

    std::mt19937_64 rand;

    static constexpr std::array<std::string_view, 55> namePartsArray = {
            "the", "elder", "scrolls", "klaatu", "berata", "niktu",
            "xyzzy", "bless", "curse", "light", "darkness", "fire",
            "air", "earth", "water", "hot", "dry", "cold",
            "wet", "ignite", "snuff", "embiggen", "twist", "shorten",
            "stretch", "fiddle", "destroy", "imbue", "galvanize", "enchant",
            "free", "limited", "range", "of", "towards", "inside",
            "sphere", "cube", "self", "other", "ball", "mental",
            "physical", "grow", "shrink", "demon", "elemental", "spirit",
            "animal", "creature", "beast", "humanoid", "undead", "fresh",
            "stale"
    };

    EnchantmentNameParts() :
        rand(std::random_device{}())
    {}

public:

    // disable copy and move
    EnchantmentNameParts(const EnchantmentNameParts&) = delete;
    EnchantmentNameParts& operator=(const EnchantmentNameParts&) = delete;

    static EnchantmentNameParts& getInstance() {
        static EnchantmentNameParts instance;
        return instance;
    }

    std::string generateNewRandomName() {
        const int i = std::uniform_int_distribution<int>(0, 1)(this->rand) + 3;
        std::string s;

        for (int j = 0; j < i; ++j) {
            if (j > 0)
                s += ' ';

            s += this->namePartsArray[std::uniform_int_distribution<int>(0, this->namePartsArray.size() - 1)(this->rand)];
        }

        return s;
    }

    void reseedRandomGenerator(long long seed) {
        this->rand.seed(static_cast<uint64_t>(seed));
    }

};

#endif //MCCLONE_ENCHANTMENTNAMEPARTS_H
