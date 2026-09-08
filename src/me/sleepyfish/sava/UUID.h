//
// Created by SleepyFish on 06.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_UUID_H
#define MCCLONE_UUID_H

#include <cstdint>
#include <string>
#include <random>
#include <sstream>
#include <iomanip>

/**
 * @author SleepyFish - SleepyAVA
 * @brief UUID wrapper of 2 uint64_t
 */
class UUID {

public:

    uint64_t hi; // high bits
    uint64_t lo; // low bits

    UUID() noexcept :
        hi(0),
        lo(0)
    {};

    UUID(uint64_t hi, uint64_t lo) noexcept :
        hi(hi),
        lo(lo)
    {}

    explicit UUID(const std::string &str) {
        if (str.size() != 36 || str[8] != '-' || str[13] != '-' || str[18] != '-' || str[23] != '-') {
            throw std::invalid_argument("Invalid UUID string: " + str);
        }

        std::string clean;
        for (char c : str) {
            if (c != '-') {
                clean += c;
            }
        }

        this->hi = std::stoull(clean.substr(0, 16),  nullptr, 16);
        this->lo = std::stoull(clean.substr(16, 16), nullptr, 16);
    }

    static UUID random() {
        static std::mt19937_64 rng(std::random_device{}());
        UUID uuid(rng(), rng());
        uuid.hi = (uuid.hi & 0xFFFFFFFFFFFF0FFFULL) | 0x0000000000004000ULL;
        uuid.lo = (uuid.lo & 0x3FFFFFFFFFFFFFFFULL) | 0x8000000000000000ULL;
        return uuid;
    }

    static UUID fromString(const std::string &str) {
        return UUID(str);
    }

    bool operator==(const UUID &o) const noexcept {
        return this->hi == o.hi && this->lo == o.lo;
    }

    bool operator!=(const UUID &o) const noexcept {
        return !(*this == o);
    }

    friend std::ostream &operator<<(std::ostream &os, const UUID &u) {
        os << std::hex << std::setfill('0')
           << std::setw(8)  << (u.hi >> 32)              << '-'
           << std::setw(4)  << ((u.hi >> 16) & 0xFFFF)   << '-'
           << std::setw(4)  << (u.hi & 0xFFFF)            << '-'
           << std::setw(4)  << (u.lo >> 48)               << '-'
           << std::setw(12) << (u.lo & 0x0000FFFFFFFFFFFFULL);
        return os;
    }

    std::string toString() const {
        std::ostringstream oss;
        oss << *this;
        return oss.str();
    }

};

namespace std {

    template<>
    struct hash<UUID> {
        size_t operator()(const UUID &u) const noexcept {
            return u.hi ^ (u.lo * 2654435761ULL);
        }
    };

};


#endif //MCCLONE_UUID_H
