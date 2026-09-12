//
// Created by SleepyFish on 12.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ABSTRACTTEXTURE_H
#define MCCLONE_ABSTRACTTEXTURE_H

#include "ITextureObject.h"

/**
 * @author SleepyFish
 * @brief Abstract texture
 */
class AbstractTexture : public ITextureObject {

protected:

    mutable int glTextureId = -1;

    bool blur       = false;
    bool mipmap     = false;
    bool blurLast   = false;
    bool mipmapLast = false;

public:

    virtual ~AbstractTexture() = default;

    void setBlurMipmapDirect(bool blur_, bool mipmap_);

    void loadTexture(IResourceManager& resourceManager) override {};

    void setBlurMipmap(bool blur_, bool mipmap_) override;

    void restoreLastBlurMipmap() override;

    int getGlTextureId() const override;

    void deleteGlTexture();

};


#endif //MCCLONE_ABSTRACTTEXTURE_H
