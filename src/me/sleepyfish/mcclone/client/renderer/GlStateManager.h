//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_GLSTATEMANAGER_H
#define MCCLONE_GLSTATEMANAGER_H

#include <vector>
#include <array>

/**
 * @author SleepyFish
 * @brief OpenGL state manager - caches GL state to avoid redundant calls
 */
class GlStateManager {

public:

    enum class TexGen { S, T, R, Q };

    struct BooleanState {
        int  capability;
        bool currentState = false;

        explicit BooleanState(int cap) : capability(cap) {}

        void setEnabled()  { setState(true);  }
        void setDisabled() { setState(false); }
        void setState(bool state);
    };

    struct Color {
        float red   = 1.0f;
        float green = 1.0f;
        float blue  = 1.0f;
        float alpha = 1.0f;

        Color() = default;
        Color(float r, float g, float b, float a) : red(r), green(g), blue(b), alpha(a) {}
    };

    struct AlphaState {
        BooleanState alphaTest { 3008 };
        int   func = 519;
        float ref  = -1.0f;
    };

    struct BlendState {
        BooleanState blend { 3042 };
        int srcFactor      = 1;
        int dstFactor      = 0;
        int srcFactorAlpha = 1;
        int dstFactorAlpha = 0;
    };

    struct DepthState {
        BooleanState depthTest { 2929 };
        bool maskEnabled = true;
        int  depthFunc   = 513;
    };

    struct FogState {
        BooleanState fog { 2912 };
        int   mode    = 2048;
        float density = 1.0f;
        float start   = 0.0f;
        float end     = 1.0f;
    };

    struct CullState {
        BooleanState cullFace { 2884 };
        int mode = 1029;
    };

    struct ColorMaterialState {
        BooleanState colorMaterial { 2903 };
        int face = 1032;
        int mode = 5634;
    };

    struct ColorLogicState {
        BooleanState colorLogicOp { 3058 };
        int opcode = 5379;
    };

    struct PolygonOffsetState {
        BooleanState polygonOffsetFill { 32823 };
        BooleanState polygonOffsetLine { 10754 };
        float factor = 0.0f;
        float units  = 0.0f;
    };

    struct StencilFunc {
        int func = 519;
        int ref  = 0;
        int mask = -1;
    };

    struct StencilState {
        StencilFunc  func;
        int writeMask  = -1;
        int failOp     = 7680;
        int zFailOp    = 7680;
        int zPassOp    = 7680;
    };

    struct TexGenCoord {
        BooleanState textureGen;
        int coord;
        int param = -1;

        TexGenCoord(int coordIn, int capIn) : textureGen(capIn), coord(coordIn) {}
    };

    struct TexGenState {
        TexGenCoord s { 8192, 3168 };
        TexGenCoord t { 8193, 3169 };
        TexGenCoord r { 8194, 3170 };
        TexGenCoord q { 8195, 3171 };
    };

    struct TextureState {
        BooleanState texture2DState { 3553 };
        int          textureName = 0;
    };

    struct ColorMask {
        bool red   = true;
        bool green = true;
        bool blue  = true;
        bool alpha = true;
    };

    struct ClearState {
        double depth = 1.0;
        Color  color { 0.0f, 0.0f, 0.0f, 0.0f };
    };

    static bool clearEnabled;

private:

    static AlphaState        alphaState;
    static BooleanState      lightingState;
    static BooleanState      lightState[8];
    static ColorMaterialState colorMaterialState;
    static BlendState        blendState;
    static DepthState        depthState;
    static FogState          fogState;
    static CullState         cullState;
    static PolygonOffsetState polygonOffsetState;
    static ColorLogicState   colorLogicState;
    static TexGenState       texGenState;
    static ClearState        clearState;
    static StencilState      stencilState;
    static BooleanState      normalizeState;
    static int               activeTextureUnit;
    static TextureState      textureState[32];
    static int               activeShadeModel;
    static BooleanState      rescaleNormalState;
    static ColorMask         colorMaskState;
    static Color             colorState;
    static bool              creatingDisplayList;

    static TexGenCoord& texGenCoord(TexGen gen);

public:

    static void pushAttrib_();
    static void popAttrib_();

    static void enableAlpha_();
    static void disableAlpha_();
    static void alphaFunc_(int func, float ref);

    static void enableLighting_();
    static void disableLighting_();
    static void enableLight_(int light);
    static void disableLight_(int light);

    static void enableColorMaterial_();
    static void disableColorMaterial_();
    static void colorMaterial_(int face, int mode);

    static void enableDepth_();
    static void disableDepth_();
    static void depthFunc_(int func);
    static void depthMask_(bool flag);

    static void enableBlend_();
    static void disableBlend_();
    static void blendFunc_(int src, int dst);
    static void tryBlendFuncSeparate_(int src, int dst, int srcAlpha, int dstAlpha);

    static void enableFog_();
    static void disableFog_();
    static void setFog_(int param);
    static void setFogDensity_(float param);
    static void setFogStart_(float param);
    static void setFogEnd_(float param);
    static void glFog_(int pname, const float* params);
    static void glFogi_(int pname, int param);
    static bool isFogEnabled_();
    static void setFogEnabled_(bool enabled);

    static void enableCull_();
    static void disableCull_();
    static void cullFace_(int mode);

    static void enablePolygonOffset_();
    static void disablePolygonOffset_();
    static void doPolygonOffset_(float factor, float units);

    static void enableColorLogic_();
    static void disableColorLogic_();
    static void colorLogicOp_(int opcode);

    static void enableTexGenCoord_(TexGen gen);
    static void disableTexGenCoord_(TexGen gen);
    static void texGen_(TexGen gen, int param);
    static void texGen_(TexGen gen, int pname, const float* params);

    static void setActiveTexture_(int texture);
    static void enableTexture2D_();
    static void disableTexture2D_();
    static int  generateTexture_();
    static void deleteTexture_(int texture);
    static void deleteTextures_(const std::vector<int>& textures);
    static void bindTexture_(int texture);
    static void bindCurrentTexture_();
    static int  getBoundTexture_();
    static int  getActiveTextureUnit_();

    static void enableNormalize_();
    static void disableNormalize_();
    static void shadeModel_(int mode);
    static void enableRescaleNormal_();
    static void disableRescaleNormal_();

    static void viewport_(int x, int y, int width, int height);
    static void colorMask_(bool red, bool green, bool blue, bool alpha);
    static void clearDepth_(double depth);
    static void clearColor_(float red, float green, float blue, float alpha);
    static void clear_(int mask);

    static void matrixMode_(int mode);
    static void loadIdentity_();
    static void pushMatrix_();
    static void popMatrix_();
    static void getFloat_(int pname, float* params);
    static void ortho_(double left, double right, double bottom, double top, double zNear, double zFar);
    static void rotate_(float angle, float x, float y, float z);
    static void scale_(float x, float y, float z);
    static void scale_(double x, double y, double z);
    static void translate_(float x, float y, float z);
    static void translate_(double x, double y, double z);
    static void multMatrix_(const float* matrix);

    static void color_(float r, float g, float b, float a);
    static void color_(float r, float g, float b);
    static void resetColor_();

    static void glNormalPointer_(int type, int stride, const void* pointer);
    static void glTexCoordPointer_(int size, int type, int stride, int offset);
    static void glTexCoordPointer_(int size, int type, int stride, const void* pointer);
    static void glVertexPointer_(int size, int type, int stride, int offset);
    static void glVertexPointer_(int size, int type, int stride, const void* pointer);
    static void glColorPointer_(int size, int type, int stride, int offset);
    static void glColorPointer_(int size, int type, int stride, const void* pointer);
    static void glDisableClientState_(int array);
    static void glEnableClientState_(int array);

    static void glBegin_(int mode);
    static void glEnd_();
    static void glDrawArrays_(int mode, int first, int count);

    static void callList_(int list);
    static void callLists_(const std::vector<int>& lists);
    static void glDeleteLists_(int list, int range);
    static void glNewList_(int list, int mode);
    static void glEndList_();

    static int glGetError_();

    static void glTexImage2D_(int target, int level, int internalFormat, int width, int height, int border, int format, int type, const int* pixels);
    static void glTexSubImage2D_(int target, int level, int xoffset, int yoffset, int width, int height, int format, int type, const int* pixels);
    static void glCopyTexSubImage2D_(int target, int level, int xoffset, int yoffset, int x, int y, int width, int height);
    static void glGetTexImage_(int target, int level, int format, int type, int* pixels);
    static void glTexParameterf_(int target, int pname, float param);
    static void glTexParameteri_(int target, int pname, int param);
    static int  glGetTexLevelParameteri_(int target, int level, int pname);

    static void glMultiDrawArrays_(int mode, const int* first, const int* count, int drawCount);

    static void drainGlErrors();

};


#endif //MCCLONE_GLSTATEMANAGER_H
