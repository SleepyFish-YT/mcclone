//
// Created by SleepyFish on 05.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_PCB_H
#define MCCLONE_PCB_H

#define ERROR_CODES \
    X(NONE, 0) \
    X(ARGUMENTS, -1) \
    X(EXECUTE, -2) \
    X(LOGGER, -3) \
    X(RESOLUTION, -4) \
    X(DIMENSIONS, -5) \
    X(GAME_CONFIG, -6)\
    X(OPENGL_INIT, -7)

#define MCCLONE_ERR_SIZE (-7)

#define X(name, value) MCCLONE_ERR_##name = value,
enum { ERROR_CODES };
#undef X //define X

#define X(name, value) case MCCLONE_ERR_##name: return #name;
inline const char* MCCLONE_ERR_NAME_FUNC(int code) {
    switch(code) {
        ERROR_CODES
        default: return "UNKNOWN";
    }
}
#undef X //define X


#endif //MCCLONE_PCB_H
