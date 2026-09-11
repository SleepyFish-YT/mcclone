//
// Created by SleepyFish on 11.09.2026.
// Project: mcclone
//

// not FULLY implemented yet.

#include "TextureCompass.h"

#include "../../Minecraft.h"
#include "../../../util/MathHelper.h"
#include "../../../util/BlockPos.h"
#include "TextureUtil.h"
// #include "World.h"

void TextureCompass::updateAnimation() {
    if (this->framesTextureData.empty()) {
        return;
    }

    Minecraft *minecraft = Minecraft::getMinecraft();

    /* if (minecraft->theWorld != nullptr && minecraft->thePlayer != nullptr) {
        this->updateCompass(minecraft->theWorld, minecraft->thePlayer.posX, minecraft->thePlayer.posZ, (double) minecraft->thePlayer.rotationYaw, false, false);
    } else */ {
        this->updateCompass(nullptr, 0.0, 0.0, 0.0, true, false);
    }
}

void TextureCompass::updateCompass(World *worldIn, double xIn, double zIn, double angle, bool reqWorld, bool setAngle) {
    if (!this->framesTextureData.empty()) {
        double d0 = 0.0;

        /*
        if (worldIn != nullptr && !reqWorld) {
            const BlockPos blockpos = worldIn->getSpawnPoint();

            const double d1 = static_cast<double>(blockpos.x) - xIn;
            const double d2 = static_cast<double>(blockpos.z) - zIn;

            angle = std::fmod(angle, 360.0);
            if (angle < 0.0) {
                angle += 360.0;
            }

            d0 = -((angle - 90.0) * (MathHelper::PId / 180.0) - MathHelper::atan2(d2, d1));
            if (!worldIn->provider->isSurfaceWorld()) {
                d0 = MathHelper::getRandomDouble() * (MathHelper::PId * 2.0);
            }
        }
        */

        if (setAngle) {
            this->currentAngle = d0;
        } else {
            double d3 = d0 - this->currentAngle;
            const double twoPi = MathHelper::PId * 2.0;
            d3 -= twoPi * std::floor((d3 + MathHelper::PId) / twoPi);
            d3 = MathHelper::clamp_double(d3, -1.0, 1.0);
            this->angleDelta += d3 * 0.1;
            this->angleDelta *= 0.8;
            this->currentAngle += this->angleDelta;
        }

        const int frameCount = this->framesTextureData.size();
        if (frameCount == 0) return;

        const int rawIndex = static_cast<int>((this->currentAngle / (MathHelper::PId * 2.0) + 1.0) * static_cast<double>(frameCount));
        const int i = ((rawIndex % frameCount) + frameCount) % frameCount;

        if (i != this->frameCounter) {
            this->frameCounter = i;
            TextureUtil::uploadTextureMipmap_(this->framesTextureData[i], this->width, this->height, this->originX, this->originY, false, false);
        }
    }
}
