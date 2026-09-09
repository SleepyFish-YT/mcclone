//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#include "OpenGlHelper.h"

#include <glad/glad.h>
#include <algorithm>
#include <string>

// -------------------------------------------------------------------------
// Static member definitions
// -------------------------------------------------------------------------

bool        OpenGlHelper::nvidia                      = false;
bool        OpenGlHelper::ati                         = false;
int         OpenGlHelper::GL_FRAMEBUFFER_             = 0;
int         OpenGlHelper::GL_RENDERBUFFER_            = 0;
int         OpenGlHelper::GL_COLOR_ATTACHMENT0_       = 0;
int         OpenGlHelper::GL_DEPTH_ATTACHMENT_        = 0;
int         OpenGlHelper::GL_FRAMEBUFFER_COMPLETE_    = 0;
int         OpenGlHelper::GL_FB_INCOMPLETE_ATTACHMENT_  = 0;
int         OpenGlHelper::GL_FB_INCOMPLETE_MISS_ATTACH_ = 0;
int         OpenGlHelper::GL_FB_INCOMPLETE_DRAW_BUFFER_ = 0;
int         OpenGlHelper::GL_FB_INCOMPLETE_READ_BUFFER_ = 0;
bool        OpenGlHelper::framebufferSupported        = false;
int         OpenGlHelper::GL_LINK_STATUS_             = 0;
int         OpenGlHelper::GL_COMPILE_STATUS_          = 0;
int         OpenGlHelper::GL_VERTEX_SHADER_           = 0;
int         OpenGlHelper::GL_FRAGMENT_SHADER_         = 0;
bool        OpenGlHelper::shadersSupported            = false;
int         OpenGlHelper::defaultTexUnit              = 0;
int         OpenGlHelper::lightmapTexUnit             = 0;
int         OpenGlHelper::GL_TEXTURE2_                = 0;
int         OpenGlHelper::GL_COMBINE_                 = 0;
int         OpenGlHelper::GL_INTERPOLATE_             = 0;
int         OpenGlHelper::GL_PRIMARY_COLOR_           = 0;
int         OpenGlHelper::GL_CONSTANT_                = 0;
int         OpenGlHelper::GL_PREVIOUS_                = 0;
int         OpenGlHelper::GL_COMBINE_RGB_             = 0;
int         OpenGlHelper::GL_SOURCE0_RGB_             = 0;
int         OpenGlHelper::GL_SOURCE1_RGB_             = 0;
int         OpenGlHelper::GL_SOURCE2_RGB_             = 0;
int         OpenGlHelper::GL_OPERAND0_RGB_            = 0;
int         OpenGlHelper::GL_OPERAND1_RGB_            = 0;
int         OpenGlHelper::GL_OPERAND2_RGB_            = 0;
int         OpenGlHelper::GL_COMBINE_ALPHA_           = 0;
int         OpenGlHelper::GL_SOURCE0_ALPHA_           = 0;
int         OpenGlHelper::GL_SOURCE1_ALPHA_           = 0;
int         OpenGlHelper::GL_SOURCE2_ALPHA_           = 0;
int         OpenGlHelper::GL_OPERAND0_ALPHA_          = 0;
int         OpenGlHelper::GL_OPERAND1_ALPHA_          = 0;
int         OpenGlHelper::GL_OPERAND2_ALPHA_          = 0;
bool        OpenGlHelper::vboSupported                = false;
int         OpenGlHelper::GL_ARRAY_BUFFER_            = 0;
int         OpenGlHelper::GL_STATIC_DRAW_             = 0;
float       OpenGlHelper::lastBrightnessX             = 0.0f;
float       OpenGlHelper::lastBrightnessY             = 0.0f;
int         OpenGlHelper::GL_COPY_READ_BUFFER_        = 0;
int         OpenGlHelper::GL_COPY_WRITE_BUFFER_       = 0;
bool        OpenGlHelper::vboRegions                  = false;
bool        OpenGlHelper::openGL31                    = false;
std::string OpenGlHelper::logText_;
std::string OpenGlHelper::cpu_                        = "<unknown>";

// -------------------------------------------------------------------------
// Init
// -------------------------------------------------------------------------

void OpenGlHelper::initializeTextures() {
    // Framebuffer
    framebufferSupported           = true;
    GL_FRAMEBUFFER_                = 0x8D40;
    GL_RENDERBUFFER_               = 0x8D41;
    GL_COLOR_ATTACHMENT0_          = 0x8CE0;
    GL_DEPTH_ATTACHMENT_           = 0x8D00;
    GL_FRAMEBUFFER_COMPLETE_       = 0x8CD5;
    GL_FB_INCOMPLETE_ATTACHMENT_   = 0x8CD6;
    GL_FB_INCOMPLETE_MISS_ATTACH_  = 0x8CD7;
    GL_FB_INCOMPLETE_DRAW_BUFFER_  = 0x8CDB;
    GL_FB_INCOMPLETE_READ_BUFFER_  = 0x8CDC;
    logText_ += "Using framebuffer objects (OpenGL 3.0 core).\n";

    // Shaders
    shadersSupported  = true;
    GL_LINK_STATUS_     = 0x8B82;
    GL_COMPILE_STATUS_  = 0x8B81;
    GL_VERTEX_SHADER_   = 0x8B31;
    GL_FRAGMENT_SHADER_ = 0x8B30;
    logText_ += "Shaders available (OpenGL 2.0 core).\n";

    // Texture units
    defaultTexUnit  = 0x84C0;
    lightmapTexUnit = 0x84C1;
    GL_TEXTURE2_    = 0x84C2;
    logText_ += "Using GL 1.3 multitexturing.\n";

    // Texture env combine
    GL_COMBINE_        = 0x8570;
    GL_INTERPOLATE_    = 0x8575;
    GL_PRIMARY_COLOR_  = 0x8577;
    GL_CONSTANT_       = 0x8576;
    GL_PREVIOUS_       = 0x8578;
    GL_COMBINE_RGB_    = 0x8571;
    GL_SOURCE0_RGB_    = 0x8580;
    GL_SOURCE1_RGB_    = 0x8581;
    GL_SOURCE2_RGB_    = 0x8582;
    GL_OPERAND0_RGB_   = 0x8590;
    GL_OPERAND1_RGB_   = 0x8591;
    GL_OPERAND2_RGB_   = 0x8592;
    GL_COMBINE_ALPHA_  = 0x8572;
    GL_SOURCE0_ALPHA_  = 0x8588;
    GL_SOURCE1_ALPHA_  = 0x8589;
    GL_SOURCE2_ALPHA_  = 0x858A;
    GL_OPERAND0_ALPHA_ = 0x8598;
    GL_OPERAND1_ALPHA_ = 0x8599;
    GL_OPERAND2_ALPHA_ = 0x859A;

    // VBO
    vboSupported    = true;
    GL_ARRAY_BUFFER_ = 0x8892;
    GL_STATIC_DRAW_  = 0x88B4;
    logText_ += "VBOs available (OpenGL 1.5 core).\n";

    // Copy buffer
    openGL31             = true;
    vboRegions           = true;
    GL_COPY_READ_BUFFER_  = 0x8F36;
    GL_COPY_WRITE_BUFFER_ = 0x8F37;

    // Vendor detection
    const char* vendor = reinterpret_cast<const char*>(::glGetString(GL_VENDOR));
    if (vendor) {
        std::string v(vendor);
        std::transform(v.begin(), v.end(), v.begin(), ::tolower);
        nvidia = v.find("nvidia") != std::string::npos;
        ati    = v.find("ati")    != std::string::npos;
    }
}

// -------------------------------------------------------------------------
// Info
// -------------------------------------------------------------------------

bool        OpenGlHelper::areShadersSupported_()  { return shadersSupported;  }
std::string OpenGlHelper::getLogText_()           { return logText_;          }
std::string OpenGlHelper::getCpu_()               { return cpu_;              }
bool        OpenGlHelper::isFramebufferEnabled_() { return framebufferSupported; }

// -------------------------------------------------------------------------
// Shader
// -------------------------------------------------------------------------

int OpenGlHelper::glGetProgrami_(int program, int pname) {
    GLint v; ::glGetProgramiv(program, pname, &v); return v;
}

void OpenGlHelper::glAttachShader_(int program, int shader)  { ::glAttachShader(program, shader); }
void OpenGlHelper::glDeleteShader_(int shader)               { ::glDeleteShader(shader);          }
int  OpenGlHelper::glCreateShader_(int type)                 { return ::glCreateShader(type);     }
void OpenGlHelper::glCompileShader_(int shader)              { ::glCompileShader(shader);         }
void OpenGlHelper::glUseProgram_(int program)                { ::glUseProgram(program);           }
int  OpenGlHelper::glCreateProgram_()                        { return ::glCreateProgram();        }
void OpenGlHelper::glDeleteProgram_(int program)             { ::glDeleteProgram(program);        }
void OpenGlHelper::glLinkProgram_(int program)               { ::glLinkProgram(program);          }
void OpenGlHelper::glUniform1i_(int location, int v0)        { ::glUniform1i(location, v0);       }

void OpenGlHelper::glShaderSource_(int shader, const std::string& source) {
    const char* src = source.c_str();
    ::glShaderSource(shader, 1, &src, nullptr);
}

int OpenGlHelper::glGetShaderi_(int shader, int pname) {
    GLint v; ::glGetShaderiv(shader, pname, &v); return v;
}

std::string OpenGlHelper::glGetShaderInfoLog_(int shader) {
    GLint len = 0;
    ::glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    std::string log(len, '\0');
    ::glGetShaderInfoLog(shader, len, nullptr, log.data());
    return log;
}

std::string OpenGlHelper::glGetProgramInfoLog_(int program) {
    GLint len = 0;
    ::glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
    std::string log(len, '\0');
    ::glGetProgramInfoLog(program, len, nullptr, log.data());
    return log;
}

int OpenGlHelper::glGetUniformLocation_(int program, const std::string& name) {
    return ::glGetUniformLocation(program, name.c_str());
}

int OpenGlHelper::glGetAttribLocation_(int program, const std::string& name) {
    return ::glGetAttribLocation(program, name.c_str());
}

void OpenGlHelper::glUniform1_(int loc, const int*   v, int n) { ::glUniform1iv(loc, n, v); }
void OpenGlHelper::glUniform1_(int loc, const float* v, int n) { ::glUniform1fv(loc, n, v); }
void OpenGlHelper::glUniform2_(int loc, const int*   v, int n) { ::glUniform2iv(loc, n, v); }
void OpenGlHelper::glUniform2_(int loc, const float* v, int n) { ::glUniform2fv(loc, n, v); }
void OpenGlHelper::glUniform3_(int loc, const int*   v, int n) { ::glUniform3iv(loc, n, v); }
void OpenGlHelper::glUniform3_(int loc, const float* v, int n) { ::glUniform3fv(loc, n, v); }
void OpenGlHelper::glUniform4_(int loc, const int*   v, int n) { ::glUniform4iv(loc, n, v); }
void OpenGlHelper::glUniform4_(int loc, const float* v, int n) { ::glUniform4fv(loc, n, v); }

void OpenGlHelper::glUniformMatrix2_(int loc, bool t, const float* m, int n) { ::glUniformMatrix2fv(loc, n, t, m); }
void OpenGlHelper::glUniformMatrix3_(int loc, bool t, const float* m, int n) { ::glUniformMatrix3fv(loc, n, t, m); }
void OpenGlHelper::glUniformMatrix4_(int loc, bool t, const float* m, int n) { ::glUniformMatrix4fv(loc, n, t, m); }

// -------------------------------------------------------------------------
// VBO
// -------------------------------------------------------------------------

int  OpenGlHelper::glGenBuffers_()                                                          { GLuint b; ::glGenBuffers(1, &b); return b;      }
void OpenGlHelper::glBindBuffer_(int target, int buffer)                                    { ::glBindBuffer(target, buffer);                 }
void OpenGlHelper::glDeleteBuffers_(int buffer)                                             { GLuint b = buffer; ::glDeleteBuffers(1, &b);    }
void OpenGlHelper::glBufferData_(int target, const void* data, size_t size, int usage)      { ::glBufferData(target, size, data, usage);      }
void OpenGlHelper::glBufferData_(int target, size_t size, int usage)                        { ::glBufferData(target, size, nullptr, usage);   }
void OpenGlHelper::glBufferSubData_(int target, intptr_t offset, const void* data, size_t size) { ::glBufferSubData(target, offset, size, data); }

void OpenGlHelper::glCopyBufferSubData_(int read, int write, intptr_t readOff, intptr_t writeOff, size_t size) {
    ::glCopyBufferSubData(read, write, readOff, writeOff, size);
}

// -------------------------------------------------------------------------
// Framebuffer
// -------------------------------------------------------------------------

void OpenGlHelper::glBindFramebuffer_(int target, int fb)                         { ::glBindFramebuffer(target, fb);                        }
void OpenGlHelper::glBindRenderbuffer_(int target, int rb)                        { ::glBindRenderbuffer(target, rb);                       }
void OpenGlHelper::glDeleteRenderbuffers_(int rb)                                 { GLuint r = rb; ::glDeleteRenderbuffers(1, &r);          }
void OpenGlHelper::glDeleteFramebuffers_(int fb)                                  { GLuint f = fb; ::glDeleteFramebuffers(1, &f);           }
int  OpenGlHelper::glGenFramebuffers_()                                           { GLuint f; ::glGenFramebuffers(1, &f); return f;          }
int  OpenGlHelper::glGenRenderbuffers_()                                          { GLuint r; ::glGenRenderbuffers(1, &r); return r;         }
void OpenGlHelper::glRenderbufferStorage_(int t, int fmt, int w, int h)           { ::glRenderbufferStorage(t, fmt, w, h);                  }
void OpenGlHelper::glFramebufferRenderbuffer_(int t, int a, int rbt, int rb)      { ::glFramebufferRenderbuffer(t, a, rbt, rb);             }
int  OpenGlHelper::glCheckFramebufferStatus_(int target)                          { return ::glCheckFramebufferStatus(target);              }
void OpenGlHelper::glFramebufferTexture2D_(int t, int a, int tt, int tex, int l)  { ::glFramebufferTexture2D(t, a, tt, tex, l);             }

// -------------------------------------------------------------------------
// Texture
// -------------------------------------------------------------------------

void OpenGlHelper::setActiveTexture_(int texture)       { ::glActiveTexture(texture); }
void OpenGlHelper::setClientActiveTexture_(int texture) { /* no-op in core profile */ }

void OpenGlHelper::setLightmapTextureCoords_(int target, float x, float y) {
    // TODO: pass via vertex attributes in core profile
    if (target == lightmapTexUnit) {
        lastBrightnessX = x;
        lastBrightnessY = y;
    }
}

// -------------------------------------------------------------------------
// Blend
// -------------------------------------------------------------------------

void OpenGlHelper::glBlendFunc_(int srcRGB, int dstRGB, int srcAlpha, int dstAlpha) {
    ::glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
}
