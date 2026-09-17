//
// Created by SleepyFish on 10.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_TESSELLATOR_H
#define MCCLONE_TESSELLATOR_H

#include <memory>

class WorldRenderer;
class WorldVertexBufferUploader;

/**
 * @author SleepyFish
 * @brief Render tessellator
 */
class Tessellator {

private:

    std::unique_ptr<WorldRenderer> worldRenderer;
    std::unique_ptr<WorldVertexBufferUploader> vboUploader;

public:

    // disable copy
    Tessellator(const Tessellator&) = delete;
    Tessellator& operator=(const Tessellator&) = delete;

    ~Tessellator();

    static Tessellator &getInstance() {
        static Tessellator instance(2097152);
        return instance;
    }

    explicit Tessellator(int bufferSize);

    void draw();

    WorldRenderer &getWorldRenderer();

};


#endif //MCCLONE_TESSELLATOR_H
