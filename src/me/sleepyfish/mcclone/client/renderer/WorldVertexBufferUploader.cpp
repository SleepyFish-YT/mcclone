//
// Created by SleepyFish on 10.09.2026.
// Project: mcclone
//

#include "WorldVertexBufferUploader.h"

#include "WorldRenderer.h"
#include "vertex/VertexFormat.h"
#include "OpenGlHelper.h"
#include "GlStateManager.h"

#include <glad/glad.h>

void WorldVertexBufferUploader::draw(WorldRenderer &renderer) {
    if (renderer.getVertexCount() <= 0) {
        renderer.reset();
        return;
    }

    bool flag0 = false; // Reflector.ForgeVertexFormatElementEnumUseage_preDraw.exists()
    bool flag1 = false; // Reflector.ForgeVertexFormatElementEnumUseage_postDraw.exists()

    if (renderer.getDrawMode() == 7 /* && Config.isQuadsToTriangles() */) {
        renderer.quadsToTriangles();
    }

    const VertexFormat vertexFormat = renderer.getVertexFormat();
    const int stride = vertexFormat.getNextOffset();
    const auto& bytebuffer = renderer.getByteBuffer();
    const auto& list = vertexFormat.getElements();

    for (size_t j = 0; j < list.size(); ++j) {
        const VertexFormatElement& vertexFormatElement = list.at(j);
        const VertexFormatElement::EnumUsage usage = vertexFormatElement.getUsage();

        if (flag0) {
            // forge hook (not implemented)
            continue;
        }

        const int typeGlConstant = VertexFormatElement::getTypeGLConstant(vertexFormatElement.getType());
        const int index = vertexFormatElement.getIndex();
        const int offset = vertexFormat.getOffset((int) j);

        const void* ptr = static_cast<const void*>(bytebuffer.data() + offset);

        switch (usage) {
            case VertexFormatElement::EnumUsage::POSITION:
                GlStateManager::glVertexPointer_(vertexFormatElement.getElementCount(), typeGlConstant, stride, ptr);
                GlStateManager::glEnableClientState_(GL_VERTEX_ARRAY);
                break;

            case VertexFormatElement::EnumUsage::UV:
                OpenGlHelper::setClientActiveTexture_(OpenGlHelper::defaultTexUnit + index);
                GlStateManager::glTexCoordPointer_(vertexFormatElement.getElementCount(), typeGlConstant, stride, ptr);
                GlStateManager::glEnableClientState_(GL_TEXTURE_COORD_ARRAY);
                OpenGlHelper::setClientActiveTexture_(OpenGlHelper::defaultTexUnit);
                break;

            case VertexFormatElement::EnumUsage::COLOR:
                GlStateManager::glColorPointer_(vertexFormatElement.getElementCount(), typeGlConstant, stride, ptr);
                GlStateManager::glEnableClientState_(GL_COLOR_ARRAY);
                break;

            case VertexFormatElement::EnumUsage::NORMAL:
                GlStateManager::glNormalPointer_(typeGlConstant, stride, ptr);
                GlStateManager::glEnableClientState_(GL_NORMAL_ARRAY);
                break;
        }
    }

    if (renderer.isMultiTexture()) {
        renderer.drawMultiTexture();
    } else if (false /*Config::isShaders()*/) {
        // SVertexBuilder.drawArrays(renderer.getDrawMode(), 0, renderer.getVertexCount(), renderer);
    } else {
        GlStateManager::glDrawArrays_(renderer.getDrawMode(), 0, renderer.getVertexCount());
    }

    for (auto vertexFormatElement : list) {
        const VertexFormatElement::EnumUsage usage = vertexFormatElement.getUsage();

        if (flag1) {
            // forge hook (not implemented)
            continue;
        }

        const int index = vertexFormatElement.getIndex();

        switch (usage) {
            case VertexFormatElement::EnumUsage::POSITION:
                GlStateManager::glDisableClientState_(GL_VERTEX_ARRAY);
                break;

            case VertexFormatElement::EnumUsage::UV:
                OpenGlHelper::setClientActiveTexture_(OpenGlHelper::defaultTexUnit + index);
                GlStateManager::glDisableClientState_(GL_TEXTURE_COORD_ARRAY);
                OpenGlHelper::setClientActiveTexture_(OpenGlHelper::defaultTexUnit);
                break;

            case VertexFormatElement::EnumUsage::COLOR:
                GlStateManager::glDisableClientState_(GL_COLOR_ARRAY);
                GlStateManager::resetColor_();
                break;

            case VertexFormatElement::EnumUsage::NORMAL:
                GlStateManager::glDisableClientState_(GL_NORMAL_ARRAY);
                break;
        }
    }

    renderer.reset();
}
