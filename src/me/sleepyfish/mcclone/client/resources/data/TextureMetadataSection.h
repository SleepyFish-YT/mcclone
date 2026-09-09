//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_TEXTUREMETADATASECTION_H
#define MCCLONE_TEXTUREMETADATASECTION_H

#include <vector>

/**
 * @author SleepyFish
 * @brief Texture metadata section
 */
class TextureMetadataSection {

private:

    const bool textureBlur;
    const bool textureClamp;
    const std::vector<int> listMipmaps;

public:

    TextureMetadataSection(bool blur, bool clamp, const std::vector<int>& mipmaps) :
        textureBlur(blur),
        textureClamp(clamp),
        listMipmaps(mipmaps)
    {}

    bool getTextureBlur() const noexcept {
        return this->textureBlur;
    }

    bool getTextureClamp() const noexcept {
        return this->textureClamp;
    }

    const std::vector<int>& getListMipmaps() const noexcept {
        return this->listMipmaps;
    }

};


#endif //MCCLONE_TEXTUREMETADATASECTION_H
