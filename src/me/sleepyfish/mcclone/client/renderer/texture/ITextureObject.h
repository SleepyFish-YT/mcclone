//
// Created by SleepyFish on 12.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ITEXTUREOBJECT_H
#define MCCLONE_ITEXTUREOBJECT_H

class IResourceManager;

/**
 * @author SleepyFish
 * @brief Texture object interface
 */
class ITextureObject {

public:

    virtual ~ITextureObject() = default;

    virtual void setBlurMipmap(bool blur, bool mipmap) = 0;

    virtual void restoreLastBlurMipmap() = 0;

    // throws std::ios_base::failure
    virtual void loadTexture(IResourceManager& resourceManager) = 0;

    virtual int getGlTextureId() const = 0;

};


#endif //MCCLONE_ITEXTUREOBJECT_H
