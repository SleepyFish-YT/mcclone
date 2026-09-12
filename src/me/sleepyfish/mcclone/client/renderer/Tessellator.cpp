//
// Created by SleepyFish on 10.09.2026.
// Project: mcclone
//

// not shader implemented yet.

#include "Tessellator.h"

#include "WorldRenderer.h"
#include "WorldVertexBufferUploader.h"
// #include "SmartAnimations.h"

Tessellator::Tessellator(int bufferSize) {
    this->worldRenderer = new WorldRenderer(bufferSize);
    this->vboUploader = new WorldVertexBufferUploader();
}

void Tessellator::draw() {
    // if (this->worldRenderer->animatedSprites != nullptr) {
    //     SmartAnimations::spritesRendered(this->worldRenderer->animatedSprites);
    // }

    this->worldRenderer->finishDrawing();
    this->vboUploader->draw(*this->worldRenderer);
}

WorldRenderer &Tessellator::getWorldRenderer() {
    return *this->worldRenderer;
}
