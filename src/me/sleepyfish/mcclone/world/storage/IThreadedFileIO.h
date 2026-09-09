//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ITHREADEDFILEIO_H
#define MCCLONE_ITHREADEDFILEIO_H

/**
 * @author SleepyFish
 * @brief Threaded file IO interface
 */
class IThreadedFileIO {

public:

    virtual ~IThreadedFileIO() {}

    virtual bool writeNextIO() = 0;

};


#endif //MCCLONE_ITHREADEDFILEIO_H
