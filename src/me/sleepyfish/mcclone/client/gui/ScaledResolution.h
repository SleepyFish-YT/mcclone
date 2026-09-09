//
// Created by SleepyFish on 07.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_SCALEDRESOLUTION_H
#define MCCLONE_SCALEDRESOLUTION_H

class Minecraft;

/**
 * @author SleepyFish
 * @brief Scaled resolution
 */
class ScaledResolution {

private:

    double scaledWidthD;
    double scaledHeightD;

    int scaledWidth;
    int scaledHeight;
    int scaleFactor;

public:

    explicit ScaledResolution(const Minecraft &mcIn) noexcept;

    int getScaledWidth() const noexcept {
        return this->scaledWidth;
    }

    int getScaledHeight() const noexcept {
        return this->scaledHeight;
    }

    double getScaledWidth_double() const noexcept {
        return this->scaledWidthD;
    }

    double getScaledHeight_double() const noexcept {
        return this->scaledHeightD;
    }

    int getScaleFactor() const noexcept {
        return this->scaleFactor;
    }

};


#endif //MCCLONE_SCALEDRESOLUTION_H
