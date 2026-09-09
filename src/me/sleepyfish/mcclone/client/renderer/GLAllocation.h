//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_GLALLOCATION_H
#define MCCLONE_GLALLOCATION_H

#include <vector>
#include <mutex>
#include <stdexcept>
#include <string>

#include <glad/glad.h>

/**
 * @author SleepyFish
 * @brief GL allocation
 */
class GLAllocation {

private:

    static std::mutex mutex;

public:

    static int generateDisplayLists(int range) {
        std::lock_guard<std::mutex> lock(GLAllocation::mutex);
        int id = ::glGenLists(range);

        if (id == 0) {
            int err = ::glGetError();
            throw std::runtime_error(
                    "glGenLists returned an ID of 0 for a count of " +
                    std::to_string(range) +
                    ", GL error (" + std::to_string(err) + ")"
            );
        }

        return id;
    }

    static void deleteDisplayLists(int list, int range) {
        std::lock_guard<std::mutex> lock(GLAllocation::mutex);
        ::glDeleteLists(list, range);
    }

    static void deleteDisplayLists(int list) {
        GLAllocation::deleteDisplayLists(list, 1);
    }

    static std::vector<uint8_t> createDirectByteBuffer(int capacity) {
        return std::vector<uint8_t>(capacity);
    }

    static std::vector<int32_t> createDirectIntBuffer(int capacity) {
        return std::vector<int32_t>(capacity);
    }

    static std::vector<float> createDirectFloatBuffer(int capacity) {
        return std::vector<float>(capacity);
    }

};

inline std::mutex GLAllocation::mutex;


#endif //MCCLONE_GLALLOCATION_H
