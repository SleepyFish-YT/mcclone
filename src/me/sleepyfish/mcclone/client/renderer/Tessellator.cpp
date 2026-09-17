//
// Created by SleepyFish on 10.09.2026.
// Project: mcclone
//

// not shader implemented yet.

#include "Tessellator.h"

#include "WorldRenderer.h"
#include "WorldVertexBufferUploader.h"
// #include "SmartAnimations.h"

Tessellator::Tessellator(int bufferSize) :
    worldRenderer(std::make_unique<WorldRenderer>(bufferSize)),
    vboUploader(std::make_unique<WorldVertexBufferUploader>())
{}

Tessellator::~Tessellator() = default;

void Tessellator::draw() {
    WorldRenderer& wr = *this->worldRenderer;

#ifdef MCCLONE_DEBUG_TESSELLATOR
    static int frame = 0;
    frame++;

    bool triMode = (wr.getDrawMode() != wr.drawMode);
    fprintf(stderr,
            "[Tessellator] frame=%d  verts=%d  mode=%d  triMode=%d  multiTex=%d\n"
            "  rawIntBuffer:  size=%zu (%.1f KiB)  cap=%zu (%.1f KiB)\n"
            "  byteBuffer:    size=%zu (%.1f KiB)\n"
            "  bufferSize:    %d ints (%.1f KiB)\n",
            frame,
            wr.vertexCount,
            wr.drawMode,
            triMode,
            wr.isMultiTexture(),
            wr.rawIntBuffer.size(),     wr.rawIntBuffer.size()     * 4 / 1024.0,
            wr.rawIntBuffer.capacity(), wr.rawIntBuffer.capacity() * 4 / 1024.0,
            wr.getByteBuffer().size(),  wr.getByteBuffer().size()  / 1024.0,
            wr.getBufferSize(),         wr.getBufferSize() * 4 / 1024.0
    );
    fflush(stderr);
#endif //MCCLONE_DEBUG_TESSELLATOR

    wr.finishDrawing();
    this->vboUploader->draw(wr);
}

WorldRenderer &Tessellator::getWorldRenderer() {
    return *this->worldRenderer;
}
