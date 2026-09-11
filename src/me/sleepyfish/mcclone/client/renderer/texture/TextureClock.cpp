//
// Created by SleepyFish on 11.09.2026.
// Project: mcclone
//

// not FULLY implemented yet.

#include "TextureClock.h"

#include "../../Minecraft.h"
#include "../../../util/MathHelper.h"
#include "TextureUtil.h"

void TextureClock::updateAnimation() {
    if (this->framesTextureData.empty()) {
        return;
    }

    Minecraft *minecraft = Minecraft::getMinecraft();
    double d0 = 0.0;

    /*
    if (minecraft->theWorld != nullptr && minecraft->thePlayer != nullptr) {
        d0 = (double) minecraft->theWorld.getCelestialAngle(1.0f);

        if (!minecraft->theWorld->provider.isSurfaceWorld()) {
            d0 = MathHelper::getRandomDouble();
        }
    }
    */

    double d1 = d0 - this->currentAngle;
    d1 -= std::floor(d1 + 0.5);
    d1 = MathHelper::clamp_double(d1, -1.0, 1.0);

    this->angleDelta += d1 * 0.1;
    this->angleDelta *= 0.8;
    this->currentAngle += this->angleDelta;

    const int frameCount = this->framesTextureData.size();
    if (frameCount == 0) return;

    const int rawIndex = static_cast<int>((this->currentAngle + 1.0) * static_cast<double>(frameCount));

    const int i = ((rawIndex % frameCount) + frameCount) % frameCount;
    if (i != this->frameCounter) {
        this->frameCounter = i;
        TextureUtil::uploadTextureMipmap_(this->framesTextureData[i], this->width, this->height, this->originX, this->originY, false, false);
    }
}
