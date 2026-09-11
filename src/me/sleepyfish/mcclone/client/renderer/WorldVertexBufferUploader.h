//
// Created by SleepyFish on 10.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_WORLDVERTEXBUFFERUPLOADER_H
#define MCCLONE_WORLDVERTEXBUFFERUPLOADER_H

class WorldRenderer;

/**
 * @author SleepyFish
 * @brief World vertex buffer uploader
 */
class WorldVertexBufferUploader {

public:

    virtual ~WorldVertexBufferUploader() = default;

    virtual void draw(WorldRenderer &renderer);

};


#endif //MCCLONE_WORLDVERTEXBUFFERUPLOADER_H
