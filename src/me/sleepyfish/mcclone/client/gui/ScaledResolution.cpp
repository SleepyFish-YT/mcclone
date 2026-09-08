//
// Created by SleepyFish on 07.09.2026.
// Project: mcclone
//

#include "ScaledResolution.h"

#include "../../util/MathHelper.h"
#include "../../client/Minecraft.h"
#include "../../client/settings/GameSettings.h"

ScaledResolution::ScaledResolution(const Minecraft& mcIn) noexcept {
    this->scaledWidth = mcIn.displayWidth;
    this->scaledHeight = mcIn.displayHeight;

    this->scaleFactor = 1;
    const bool unicode = false; //mcIn.isUnicode();
    int i = mcIn.gameSettings->guiScale;

    if (i == 0) {
        i = 1000;
    }

    while (this->scaleFactor < i && std::min(this->scaledWidth, this->scaledHeight) / (this->scaleFactor + 1) >= 240) {
        ++this->scaleFactor;
    }

    if (unicode && this->scaleFactor % 2 != 0 && this->scaleFactor != 1) {
        --this->scaleFactor;
    }

    this->scaledWidthD = (double) this->scaledWidth / (double) this->scaleFactor;
    this->scaledHeightD = (double) this->scaledHeight / (double) this->scaleFactor;

    this->scaledWidth = MathHelper::ceiling_double_int(this->scaledWidthD);
    this->scaledHeight = MathHelper::ceiling_double_int(this->scaledHeightD);
}