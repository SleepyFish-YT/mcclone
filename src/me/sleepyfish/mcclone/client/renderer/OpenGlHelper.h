//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_OPENGLHELPER_H
#define MCCLONE_OPENGLHELPER_H

#include <string>
#include <vector>
#include <cstdint>

/**
 * @author SleepyFish
 * @brief OpenGL helper - capability detection and wrapper for framebuffer/shader/VBO calls
 */
class OpenGlHelper {

public:

    // -------------------------------------------------------------------------
    // Vendor flags
    // -------------------------------------------------------------------------
    static bool nvidia;
    static bool ati;

    // -------------------------------------------------------------------------
    // Framebuffer constants
    // -------------------------------------------------------------------------
    static int  GL_FRAMEBUFFER_;
    static int  GL_RENDERBUFFER_;
    static int  GL_COLOR_ATTACHMENT0_;
    static int  GL_DEPTH_ATTACHMENT_;
    static int  GL_FRAMEBUFFER_COMPLETE_;
    static int  GL_FB_INCOMPLETE_ATTACHMENT_;
    static int  GL_FB_INCOMPLETE_MISS_ATTACH_;
    static int  GL_FB_INCOMPLETE_DRAW_BUFFER_;
    static int  GL_FB_INCOMPLETE_READ_BUFFER_;
    static bool framebufferSupported;

    // -------------------------------------------------------------------------
    // Shader constants
    // -------------------------------------------------------------------------
    static int  GL_LINK_STATUS_;
    static int  GL_COMPILE_STATUS_;
    static int  GL_VERTEX_SHADER_;
    static int  GL_FRAGMENT_SHADER_;
    static bool shadersSupported;

    // -------------------------------------------------------------------------
    // Texture units
    // -------------------------------------------------------------------------
    static int defaultTexUnit;
    static int lightmapTexUnit;
    static int GL_TEXTURE2_;

    // -------------------------------------------------------------------------
    // Texture env combine constants
    // -------------------------------------------------------------------------
    static int GL_COMBINE_;
    static int GL_INTERPOLATE_;
    static int GL_PRIMARY_COLOR_;
    static int GL_CONSTANT_;
    static int GL_PREVIOUS_;
    static int GL_COMBINE_RGB_;
    static int GL_SOURCE0_RGB_;
    static int GL_SOURCE1_RGB_;
    static int GL_SOURCE2_RGB_;
    static int GL_OPERAND0_RGB_;
    static int GL_OPERAND1_RGB_;
    static int GL_OPERAND2_RGB_;
    static int GL_COMBINE_ALPHA_;
    static int GL_SOURCE0_ALPHA_;
    static int GL_SOURCE1_ALPHA_;
    static int GL_SOURCE2_ALPHA_;
    static int GL_OPERAND0_ALPHA_;
    static int GL_OPERAND1_ALPHA_;
    static int GL_OPERAND2_ALPHA_;

    // -------------------------------------------------------------------------
    // VBO constants
    // -------------------------------------------------------------------------
    static bool vboSupported;
    static int  GL_ARRAY_BUFFER_;
    static int  GL_STATIC_DRAW_;

    // -------------------------------------------------------------------------
    // Blend / brightness
    // -------------------------------------------------------------------------
    static float lastBrightnessX;
    static float lastBrightnessY;

    // -------------------------------------------------------------------------
    // Copy buffer
    // -------------------------------------------------------------------------
    static int  GL_COPY_READ_BUFFER_;
    static int  GL_COPY_WRITE_BUFFER_;
    static bool vboRegions;
    static bool openGL31;

    // -------------------------------------------------------------------------
    // Misc
    // -------------------------------------------------------------------------
    static constexpr int GL_QUADS_     = 7;
    static constexpr int GL_TRIANGLES_ = 4;

    // -------------------------------------------------------------------------
    // Init
    // -------------------------------------------------------------------------
    static void initializeTextures();

    // -------------------------------------------------------------------------
    // Info
    // -------------------------------------------------------------------------
    static bool        areShadersSupported_();
    static std::string getLogText_();
    static std::string getCpu_();
    static bool        isFramebufferEnabled_();

    // -------------------------------------------------------------------------
    // Shader
    // -------------------------------------------------------------------------
    static int         glGetProgrami_(int program, int pname);
    static void        glAttachShader_(int program, int shader);
    static void        glDeleteShader_(int shader);
    static int         glCreateShader_(int type);
    static void        glShaderSource_(int shader, const std::string& source);
    static void        glCompileShader_(int shader);
    static int         glGetShaderi_(int shader, int pname);
    static std::string glGetShaderInfoLog_(int shader);
    static std::string glGetProgramInfoLog_(int program);
    static void        glUseProgram_(int program);
    static int         glCreateProgram_();
    static void        glDeleteProgram_(int program);
    static void        glLinkProgram_(int program);
    static int         glGetUniformLocation_(int program, const std::string& name);
    static void        glUniform1i_(int location, int v0);
    static void        glUniform1_(int location, const int*   values, int count);
    static void        glUniform1_(int location, const float* values, int count);
    static void        glUniform2_(int location, const int*   values, int count);
    static void        glUniform2_(int location, const float* values, int count);
    static void        glUniform3_(int location, const int*   values, int count);
    static void        glUniform3_(int location, const float* values, int count);
    static void        glUniform4_(int location, const int*   values, int count);
    static void        glUniform4_(int location, const float* values, int count);
    static void        glUniformMatrix2_(int location, bool transpose, const float* matrices, int count);
    static void        glUniformMatrix3_(int location, bool transpose, const float* matrices, int count);
    static void        glUniformMatrix4_(int location, bool transpose, const float* matrices, int count);
    static int         glGetAttribLocation_(int program, const std::string& name);

    // -------------------------------------------------------------------------
    // VBO
    // -------------------------------------------------------------------------
    static int  glGenBuffers_();
    static void glBindBuffer_(int target, int buffer);
    static void glBufferData_(int target, const void* data, size_t size, int usage);
    static void glBufferData_(int target, size_t size, int usage);
    static void glBufferSubData_(int target, intptr_t offset, const void* data, size_t size);
    static void glDeleteBuffers_(int buffer);
    static void glCopyBufferSubData_(int readTarget, int writeTarget, intptr_t readOffset, intptr_t writeOffset, size_t size);

    // -------------------------------------------------------------------------
    // Framebuffer
    // -------------------------------------------------------------------------
    static void glBindFramebuffer_(int target, int framebuffer);
    static void glBindRenderbuffer_(int target, int renderbuffer);
    static void glDeleteRenderbuffers_(int renderbuffer);
    static void glDeleteFramebuffers_(int framebuffer);
    static int  glGenFramebuffers_();
    static int  glGenRenderbuffers_();
    static void glRenderbufferStorage_(int target, int internalFormat, int width, int height);
    static void glFramebufferRenderbuffer_(int target, int attachment, int renderbufferTarget, int renderbuffer);
    static int  glCheckFramebufferStatus_(int target);
    static void glFramebufferTexture2D_(int target, int attachment, int textarget, int texture, int level);

    // -------------------------------------------------------------------------
    // Texture
    // -------------------------------------------------------------------------
    static void setActiveTexture_(int texture);
    static void setClientActiveTexture_(int texture);
    static void setLightmapTextureCoords_(int target, float x, float y);

    // -------------------------------------------------------------------------
    // Blend
    // -------------------------------------------------------------------------
    static void glBlendFunc_(int srcRGB, int dstRGB, int srcAlpha, int dstAlpha);

private:

    static std::string logText_;
    static std::string cpu_;

};


#endif //MCCLONE_OPENGLHELPER_H
