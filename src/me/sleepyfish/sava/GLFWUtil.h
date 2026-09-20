//
// Created by SleepyFish on 20.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_GLFWUTIL_H
#define MCCLONE_GLFWUTIL_H

#include "BufferedImage.h"

#include <vector>

#include <GLFW/glfw3.h>

/**
 * @author SleepyFish - SleepyAVA
 * @version 1.0
 * @brief GLFW utilities
 */
class GLFWUtil {

public:

    static bool SetWindowIcon(::GLFWwindow *window, const std::vector<BufferedImage> &icons) {
        std::vector<::GLFWimage> images;
        images.reserve(icons.size());

        for (const BufferedImage &img : icons) {
            const size_t need = static_cast<size_t>(img.width) * img.height * 4;

            if (img.channels != 4 || img.data.size() < need)
                continue;

            images.push_back({ img.width, img.height, const_cast<unsigned char *>(img.data.data()) });
        }

        if (images.empty()) return false;
        ::glfwSetWindowIcon(window, static_cast<int>(images.size()), images.data());
        return true;
    }

};


#endif //MCCLONE_GLFWUTIL_H
