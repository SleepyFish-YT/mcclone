//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_LAZYLOADBASE_H
#define MCCLONE_LAZYLOADBASE_H

/**
 * @author SleepyFish
 * @brief Base class for lazy loading
 */
template<typename T>
class LazyLoadBase {

private:

    T value;
    bool isLoaded{};

public:

    T getValue() {
        if (!this->isLoaded) {
            this->isLoaded = true;
            this->value = this->load();
        }

        return this->value;
    }

    virtual T load() = 0;

    virtual ~LazyLoadBase() = default;

};


#endif //MCCLONE_LAZYLOADBASE_H
