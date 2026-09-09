//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#include "GlStateManager.h"

#include <glad/glad.h>

bool GlStateManager::clearEnabled       = true;
bool GlStateManager::creatingDisplayList = false;
int  GlStateManager::activeTextureUnit  = 0;
int  GlStateManager::activeShadeModel   = 7425;

GlStateManager::AlphaState   GlStateManager::alphaState;
GlStateManager::BooleanState GlStateManager::lightingState { 2896 };
GlStateManager::BooleanState GlStateManager::lightState[8] = {
        GlStateManager::BooleanState{16384},
        GlStateManager::BooleanState{16385},
        GlStateManager::BooleanState{16386},
        GlStateManager::BooleanState{16387},
        GlStateManager::BooleanState{16388},
        GlStateManager::BooleanState{16389},
        GlStateManager::BooleanState{16390},
        GlStateManager::BooleanState{16391}
};
GlStateManager::ColorMaterialState GlStateManager::colorMaterialState;
GlStateManager::BlendState         GlStateManager::blendState;
GlStateManager::DepthState         GlStateManager::depthState;
GlStateManager::FogState           GlStateManager::fogState;
GlStateManager::CullState          GlStateManager::cullState;
GlStateManager::PolygonOffsetState GlStateManager::polygonOffsetState;
GlStateManager::ColorLogicState    GlStateManager::colorLogicState;
GlStateManager::TexGenState        GlStateManager::texGenState;
GlStateManager::ClearState         GlStateManager::clearState;
GlStateManager::StencilState       GlStateManager::stencilState;
GlStateManager::BooleanState       GlStateManager::normalizeState     { 2977 };
GlStateManager::TextureState       GlStateManager::textureState[32];
GlStateManager::BooleanState       GlStateManager::rescaleNormalState { 32826 };
GlStateManager::ColorMask          GlStateManager::colorMaskState;
GlStateManager::Color              GlStateManager::colorState;

void GlStateManager::BooleanState::setState(bool state) {
    if (state != currentState) {
        currentState = state;
        if (state) ::glEnable(capability);
        else       ::glDisable(capability);
    }
}

void GlStateManager::pushAttrib_() { ::glPushAttrib(8256); }
void GlStateManager::popAttrib_()  { ::glPopAttrib();      }

void GlStateManager::enableAlpha_()  { alphaState.alphaTest.setEnabled();  }
void GlStateManager::disableAlpha_() { alphaState.alphaTest.setDisabled(); }

void GlStateManager::alphaFunc_(int func, float ref) {
    if (func != alphaState.func || ref != alphaState.ref) {
        alphaState.func = func;
        alphaState.ref  = ref;
        ::glAlphaFunc(func, ref);
    }
}

void GlStateManager::enableLighting_()    { lightingState.setEnabled();     }
void GlStateManager::disableLighting_()   { lightingState.setDisabled();    }
void GlStateManager::enableLight_(int i)  { lightState[i].setEnabled();     }
void GlStateManager::disableLight_(int i) { lightState[i].setDisabled();    }

void GlStateManager::enableColorMaterial_()  { colorMaterialState.colorMaterial.setEnabled();  }
void GlStateManager::disableColorMaterial_() { colorMaterialState.colorMaterial.setDisabled(); }

void GlStateManager::colorMaterial_(int face, int mode) {
    if (face != colorMaterialState.face || mode != colorMaterialState.mode) {
        colorMaterialState.face = face;
        colorMaterialState.mode = mode;
        ::glColorMaterial(face, mode);
    }
}

void GlStateManager::enableDepth_()  { depthState.depthTest.setEnabled();  }
void GlStateManager::disableDepth_() { depthState.depthTest.setDisabled(); }

void GlStateManager::depthFunc_(int func) {
    if (func != depthState.depthFunc) {
        depthState.depthFunc = func;
        ::glDepthFunc(func);
    }
}

void GlStateManager::depthMask_(bool flag) {
    if (flag != depthState.maskEnabled) {
        depthState.maskEnabled = flag;
        ::glDepthMask(flag);
    }
}

void GlStateManager::enableBlend_()  { blendState.blend.setEnabled();  }
void GlStateManager::disableBlend_() { blendState.blend.setDisabled(); }

void GlStateManager::blendFunc_(int src, int dst) {
    if (src != blendState.srcFactor || dst != blendState.dstFactor ||
        src != blendState.srcFactorAlpha || dst != blendState.dstFactorAlpha)
    {
        blendState.srcFactor      = src;
        blendState.dstFactor      = dst;
        blendState.srcFactorAlpha = src;
        blendState.dstFactorAlpha = dst;
        ::glBlendFunc(src, dst);
    }
}

void GlStateManager::tryBlendFuncSeparate_(int src, int dst, int srcA, int dstA) {
    if (src != blendState.srcFactor || dst != blendState.dstFactor ||
        srcA != blendState.srcFactorAlpha || dstA != blendState.dstFactorAlpha)
    {
        blendState.srcFactor      = src;
        blendState.dstFactor      = dst;
        blendState.srcFactorAlpha = srcA;
        blendState.dstFactorAlpha = dstA;
        ::glBlendFuncSeparate(src, dst, srcA, dstA);
    }
}

void GlStateManager::enableFog_()  { fogState.fog.setEnabled();  }
void GlStateManager::disableFog_() { fogState.fog.setDisabled(); }
bool GlStateManager::isFogEnabled_()          { return fogState.fog.currentState; }
void GlStateManager::setFogEnabled_(bool e)   { fogState.fog.setState(e); }

void GlStateManager::setFog_(int param) {
    if (param != fogState.mode) {
        fogState.mode = param;
        ::glFogi(GL_FOG_MODE, param);
    }
}

void GlStateManager::setFogDensity_(float param) {
    if (param < 0.0f) param = 0.0f;
    if (param != fogState.density) {
        fogState.density = param;
        ::glFogf(GL_FOG_DENSITY, param);
    }
}

void GlStateManager::setFogStart_(float param) {
    if (param != fogState.start) {
        fogState.start = param;
        ::glFogf(GL_FOG_START, param);
    }
}

void GlStateManager::setFogEnd_(float param) {
    if (param != fogState.end) {
        fogState.end = param;
        ::glFogf(GL_FOG_END, param);
    }
}

void GlStateManager::glFog_(int pname, const float* params)  { ::glFogfv(pname, params); }
void GlStateManager::glFogi_(int pname, int param)           { ::glFogi(pname, param); }

void GlStateManager::enableCull_()  { cullState.cullFace.setEnabled();  }
void GlStateManager::disableCull_() { cullState.cullFace.setDisabled(); }

void GlStateManager::cullFace_(int mode) {
    if (mode != cullState.mode) {
        cullState.mode = mode;
        ::glCullFace(mode);
    }
}

void GlStateManager::enablePolygonOffset_()  { polygonOffsetState.polygonOffsetFill.setEnabled();  }
void GlStateManager::disablePolygonOffset_() { polygonOffsetState.polygonOffsetFill.setDisabled(); }

void GlStateManager::doPolygonOffset_(float factor, float units) {
    if (factor != polygonOffsetState.factor || units != polygonOffsetState.units) {
        polygonOffsetState.factor = factor;
        polygonOffsetState.units  = units;
        ::glPolygonOffset(factor, units);
    }
}

void GlStateManager::enableColorLogic_()  { colorLogicState.colorLogicOp.setEnabled();  }
void GlStateManager::disableColorLogic_() { colorLogicState.colorLogicOp.setDisabled(); }

void GlStateManager::colorLogicOp_(int opcode) {
    if (opcode != colorLogicState.opcode) {
        colorLogicState.opcode = opcode;
        ::glLogicOp(opcode);
    }
}

GlStateManager::TexGenCoord& GlStateManager::texGenCoord(TexGen gen) {
    switch (gen) {
        case TexGen::S: return texGenState.s;
        case TexGen::T: return texGenState.t;
        case TexGen::R: return texGenState.r;
        case TexGen::Q: return texGenState.q;
        default:        return texGenState.s;
    }
}

void GlStateManager::enableTexGenCoord_(TexGen gen)  { texGenCoord(gen).textureGen.setEnabled();  }
void GlStateManager::disableTexGenCoord_(TexGen gen) { texGenCoord(gen).textureGen.setDisabled(); }

void GlStateManager::texGen_(TexGen gen, int param) {
    TexGenCoord& coord = texGenCoord(gen);
    if (param != coord.param) {
        coord.param = param;
        ::glTexGeni(coord.coord, GL_TEXTURE_GEN_MODE, param);
    }
}

void GlStateManager::texGen_(TexGen gen, int pname, const float* params) {
    ::glTexGenfv(texGenCoord(gen).coord, pname, params);
}

void GlStateManager::setActiveTexture_(int texture) {
    // TODO: subtract OpenGlHelper::defaultTexUnit (33984)
    if (activeTextureUnit != texture) {
        activeTextureUnit = texture;
        ::glActiveTexture(GL_TEXTURE0 + texture);
    }
}

void GlStateManager::enableTexture2D_()  { textureState[activeTextureUnit].texture2DState.setEnabled();  }
void GlStateManager::disableTexture2D_() { textureState[activeTextureUnit].texture2DState.setDisabled(); }
int  GlStateManager::generateTexture_()  { GLuint t; ::glGenTextures(1, &t); return t; }

void GlStateManager::deleteTexture_(int texture) {
    if (texture == 0) return;
    GLuint t = texture;
    ::glDeleteTextures(1, &t);
    for (auto& ts : textureState)
        if (ts.textureName == texture)
            ts.textureName = 0;
}

void GlStateManager::deleteTextures_(const std::vector<int>& textures) {
    for (int t : textures)
        deleteTexture_(t);
}

void GlStateManager::bindTexture_(int texture) {
    if (texture != textureState[activeTextureUnit].textureName) {
        textureState[activeTextureUnit].textureName = texture;
        ::glBindTexture(GL_TEXTURE_2D, texture);
    }
}

void GlStateManager::bindCurrentTexture_() { ::glBindTexture(GL_TEXTURE_2D, textureState[activeTextureUnit].textureName); }
int  GlStateManager::getBoundTexture_()    { return textureState[activeTextureUnit].textureName; }
int  GlStateManager::getActiveTextureUnit_() { return activeTextureUnit; }

void GlStateManager::enableNormalize_()    { normalizeState.setEnabled();      }
void GlStateManager::disableNormalize_()   { normalizeState.setDisabled();     }
void GlStateManager::enableRescaleNormal_()  { rescaleNormalState.setEnabled();  }
void GlStateManager::disableRescaleNormal_() { rescaleNormalState.setDisabled(); }

void GlStateManager::shadeModel_(int mode) {
    if (mode != activeShadeModel) {
        activeShadeModel = mode;
        ::glShadeModel(mode);
    }
}

void GlStateManager::viewport_(int x, int y, int w, int h) { ::glViewport(x, y, w, h); }

void GlStateManager::colorMask_(bool r, bool g, bool b, bool a) {
    if (r != colorMaskState.red || g != colorMaskState.green || b != colorMaskState.blue || a != colorMaskState.alpha) {
        colorMaskState.red   = r;
        colorMaskState.green = g;
        colorMaskState.blue  = b;
        colorMaskState.alpha = a;
        ::glColorMask(r, g, b, a);
    }
}

void GlStateManager::clearDepth_(double depth) {
    if (depth != clearState.depth) {
        clearState.depth = depth;
        ::glClearDepth(depth);
    }
}

void GlStateManager::clearColor_(float r, float g, float b, float a) {
    if (r != clearState.color.red || g != clearState.color.green || b != clearState.color.blue || a != clearState.color.alpha) {
        clearState.color = { r, g, b, a };
        ::glClearColor(r, g, b, a);
    }
}

void GlStateManager::clear_(int mask) {
    if (clearEnabled) ::glClear(mask);
}

void GlStateManager::matrixMode_(int mode)                           { ::glMatrixMode(mode);               }
void GlStateManager::loadIdentity_()                                 { ::glLoadIdentity();                  }
void GlStateManager::pushMatrix_()                                   { ::glPushMatrix();                    }
void GlStateManager::popMatrix_()                                    { ::glPopMatrix();                     }
void GlStateManager::getFloat_(int pname, float* params)             { ::glGetFloatv(pname, params);        }
void GlStateManager::ortho_(double l, double r, double b, double t, double n, double f) { ::glOrtho(l, r, b, t, n, f); }
void GlStateManager::rotate_(float angle, float x, float y, float z) { ::glRotatef(angle, x, y, z);        }
void GlStateManager::scale_(float x, float y, float z)               { ::glScalef(x, y, z);                }
void GlStateManager::scale_(double x, double y, double z)            { ::glScaled(x, y, z);                }
void GlStateManager::translate_(float x, float y, float z)           { ::glTranslatef(x, y, z);            }
void GlStateManager::translate_(double x, double y, double z)        { ::glTranslated(x, y, z);            }
void GlStateManager::multMatrix_(const float* matrix)                { ::glMultMatrixf(matrix);             }

void GlStateManager::color_(float r, float g, float b, float a) {
    if (r != colorState.red || g != colorState.green || b != colorState.blue || a != colorState.alpha) {
        colorState = { r, g, b, a };
        ::glColor4f(r, g, b, a);
    }
}

void GlStateManager::color_(float r, float g, float b) { color_(r, g, b, 1.0f); }

void GlStateManager::resetColor_() {
    colorState.red = colorState.green = colorState.blue = colorState.alpha = -1.0f;
}

void GlStateManager::glNormalPointer_(int type, int stride, const void* ptr)                    { ::glNormalPointer(type, stride, ptr);           }
void GlStateManager::glTexCoordPointer_(int size, int type, int stride, int offset)             { ::glTexCoordPointer(size, type, stride, (void*)(intptr_t)offset); }
void GlStateManager::glTexCoordPointer_(int size, int type, int stride, const void* ptr)        { ::glTexCoordPointer(size, type, stride, ptr);   }
void GlStateManager::glVertexPointer_(int size, int type, int stride, int offset)               { ::glVertexPointer(size, type, stride, (void*)(intptr_t)offset); }
void GlStateManager::glVertexPointer_(int size, int type, int stride, const void* ptr)          { ::glVertexPointer(size, type, stride, ptr);     }
void GlStateManager::glColorPointer_(int size, int type, int stride, int offset)                { ::glColorPointer(size, type, stride, (void*)(intptr_t)offset); }
void GlStateManager::glColorPointer_(int size, int type, int stride, const void* ptr)           { ::glColorPointer(size, type, stride, ptr);      }
void GlStateManager::glDisableClientState_(int array)                                           { ::glDisableClientState(array);                  }
void GlStateManager::glEnableClientState_(int array)                                            { ::glEnableClientState(array);                   }

void GlStateManager::glBegin_(int mode) { ::glBegin(mode); }
void GlStateManager::glEnd_()           { ::glEnd();       }

void GlStateManager::glDrawArrays_(int mode, int first, int count) {
    ::glDrawArrays(mode, first, count);
}

void GlStateManager::callList_(int list) { ::glCallList(list); }

void GlStateManager::callLists_(const std::vector<int>& lists) {
    ::glCallLists(static_cast<GLsizei>(lists.size()), GL_INT, lists.data());
}

void GlStateManager::glDeleteLists_(int list, int range) { ::glDeleteLists(list, range); }

void GlStateManager::glNewList_(int list, int mode) {
    ::glNewList(list, mode);
    creatingDisplayList = true;
}

void GlStateManager::glEndList_() {
    ::glEndList();
    creatingDisplayList = false;
}

int GlStateManager::glGetError_() { return ::glGetError(); }

void GlStateManager::glTexImage2D_(int target, int level, int internalFormat, int width, int height, int border, int format, int type, const int* pixels) {
    ::glTexImage2D(target, level, internalFormat, width, height, border, format, type, pixels);
}

void GlStateManager::glTexSubImage2D_(int target, int level, int xoffset, int yoffset, int width, int height, int format, int type, const int* pixels) {
    ::glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
}

void GlStateManager::glCopyTexSubImage2D_(int target, int level, int xoffset, int yoffset, int x, int y, int width, int height) {
    ::glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
}

void GlStateManager::glGetTexImage_(int target, int level, int format, int type, int* pixels) {
    ::glGetTexImage(target, level, format, type, pixels);
}

void GlStateManager::glTexParameterf_(int target, int pname, float param) { ::glTexParameterf(target, pname, param); }
void GlStateManager::glTexParameteri_(int target, int pname, int param)   { ::glTexParameteri(target, pname, param); }

int GlStateManager::glGetTexLevelParameteri_(int target, int level, int pname) {
    GLint result;
    ::glGetTexLevelParameteriv(target, level, pname, &result);
    return result;
}

void GlStateManager::glMultiDrawArrays_(int mode, const int* first, const int* count, int drawCount) {
    ::glMultiDrawArrays(mode, first, count, drawCount);
}

void GlStateManager::drainGlErrors() {
    while (::GlStateManager::glGetError_() != GL_NO_ERROR) {}
}
