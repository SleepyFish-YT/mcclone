//
// Created by SleepyFish on 02.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_TUPLE_H
#define MCCLONE_TUPLE_H

/**
 * @author SleepyFish
 * @brief Tuple class
 */
template<typename A, typename B>
class Tuple {

public:

    const A first;
    const B second;

    Tuple(A first, B second) noexcept :
        first(first),
        second(second)
    {}

};

#endif //MCCLONE_TUPLE_H
