//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_CONFIG_H
#define MCCLONE_CONFIG_H

#include <string>
#include <glad/glad.h>

/**
 * @author SleepyFish
 */
class Config {

public:

    static std::string getGlErrorString(int error) {
        std::string result;

        switch (error) {
            case GL_NO_ERROR: result = "No error"; break;
            case GL_INVALID_ENUM: result = "Invalid enum"; break;
            case GL_INVALID_VALUE: result = "Invalid value"; break;
            case GL_INVALID_OPERATION: result = "Invalid operation"; break;
            case GL_CONTEXT_LOST: result = "Context lost"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: result = "Invalid framebuffer operation"; break;
            case GL_STACK_OVERFLOW: result = "Stack overflow"; break;
            case GL_STACK_UNDERFLOW: result = "Stack underflow"; break;
            case GL_OUT_OF_MEMORY: result = "Out of memory"; break;
            case 0x8031: result = "Table too large"; break;
            case 0x8032: result = "Invalid texture"; break;
            case 0x8033: result = "Invalid texture internal format"; break;
            case 0x8034: result = "Invalid texture format"; break;
            case 0x8035: result = "Invalid texture type"; break;
            case 0x8036: result = "Invalid texture size"; break;
            default: result = "Unknown: " + std::to_string(error); break;
        }

        return result;
    }

};


#endif //MCCLONE_CONFIG_H
