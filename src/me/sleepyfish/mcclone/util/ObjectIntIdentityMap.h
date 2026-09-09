//
// Created by SleepyFish on 10.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_OBJECTINTIDENTITYMAP_H
#define MCCLONE_OBJECTINTIDENTITYMAP_H

#include <unordered_map>
#include <vector>
#include <iterator>

/**
 * @author SleepyFish
 * @brief Identity based object to int map with reverse lookup by ID
 */
template<typename T>
class ObjectIntIdentityMap {

private:

    std::unordered_map<T*, int> identityMap;
    std::vector<T*> objectList;

public:

    void put(T* key, int value) {
        this->identityMap[key] = value;

        while (static_cast<int>(this->objectList.size()) <= value) {
            this->objectList.push_back(nullptr);
        }

        this->objectList[value] = key;
    }

    int get(T* key) const {
        auto it = this->identityMap.find(key);
        return it == this->identityMap.end() ? -1 : it->second;
    }

    T* getByValue(int value) const {
        if (value < 0 || value >= static_cast<int>(this->objectList.size())) {
            return nullptr;
        }

        return this->objectList[value];
    }

    // -------------------------------------------------------------------------
    // Iterator — skips nulls, equivalent to Iterators.filter(notNull())
    // -------------------------------------------------------------------------

    struct iterator {

        using iterator_category = std::forward_iterator_tag;
        using value_type = T*;
        using pointer = T**;
        using reference = T*&;
        using difference_type = std::ptrdiff_t;

        const std::vector<T*>* list;
        size_t index;

        iterator(const std::vector<T*>* list, size_t index) :
            list(list),
            index(index)
        {
            this->skipNulls();
        }

        void skipNulls() {
            while (this->index < this->list->size() && (*this->list)[this->index] == nullptr) {
                ++this->index;
            }
        }

        T* operator*() const {
            return (*this->list)[this->index];
        }

        iterator& operator++() {
            ++this->index;
            this->skipNulls();
            return *this;
        }

        bool operator==(const iterator& other) const {
            return this->index == other.index;
        }

        bool operator!=(const iterator& other) const {
            return this->index != other.index;
        }

    };

    iterator begin() const {
        return iterator(&this->objectList, 0);
    }

    iterator end() const {
        return iterator(&this->objectList, this->objectList.size());
    }

    int size() const {
        return static_cast<int>(this->identityMap.size());
    }

};


#endif //MCCLONE_OBJECTINTIDENTITYMAP_H
