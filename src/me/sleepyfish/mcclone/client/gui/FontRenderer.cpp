//
// Created by SleepyFish on 15.09.2026.
// Project: mcclone
//

#include "FontRenderer.h"

#include "../../client/Minecraft.h"
#include "../../client/renderer/GlStateManager.h"
#include "../../client/renderer/Tessellator.h"
#include "../../client/renderer/WorldRenderer.h"
#include "../../client/renderer/vertex/DefaultVertexFormats.h"
#include "../../client/settings/GameSettings.h"
#include "../../client/renderer/texture/TextureManager.h"
#include "../../client/renderer/texture/TextureUtil.h"
#include "../../util/ResourceLocation.h"
#include "../resources/IResourceManager.h"
#include "../resources/IResource.h"
#include "../../../sava/Bufferedimage.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <sstream>

#include <glad/glad.h>

const std::wstring FontRenderer::CHAR_MAP = L"\u00c0\u00c1\u00c2\u00c8\u00ca\u00cb\u00cd\u00d3\u00d4\u00d5\u00da\u00df\u00e3\u00f5\u011f\u0130\u0131\u0152\u0153\u015e\u015f\u0174\u0175\u017e\u0207\u0000\u0000\u0000\u0000\u0000\u0000\u0000 !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\u0000\u00c7\u00fc\u00e9\u00e2\u00e4\u00e0\u00e5\u00e7\u00ea\u00eb\u00e8\u00ef\u00ee\u00ec\u00c4\u00c5\u00c9\u00e6\u00c6\u00f4\u00f6\u00f2\u00fb\u00f9\u00ff\u00d6\u00dc\u00f8\u00a3\u00d8\u00d7\u0192\u00e1\u00ed\u00f3\u00fa\u00f1\u00d1\u00aa\u00ba\u00bf\u00ae\u00ac\u00bd\u00bc\u00a1\u00ab\u00bb\u2591\u2592\u2593\u2502\u2524\u2561\u2562\u2556\u2555\u2563\u2551\u2557\u255d\u255c\u255b\u2510\u2514\u2534\u252c\u251c\u2500\u253c\u255e\u255f\u255a\u2554\u2569\u2566\u2560\u2550\u256c\u2567\u2568\u2564\u2565\u2559\u2558\u2552\u2553\u256b\u256a\u2518\u250c\u2588\u2584\u258c\u2590\u2580\u03b1\u03b2\u0393\u03c0\u03a3\u03c3\u03bc\u03c4\u03a6\u0398\u03a9\u03b4\u221e\u2205\u2208\u2229\u2261\u00b1\u2265\u2264\u2320\u2321\u00f7\u2248\u00b0\u2219\u00b7\u221a\u207f\u00b2\u25a0\u0000";

FontRenderer::FontRenderer(GameSettings* gameSettingsIn, ResourceLocation* location, TextureManager* textureManagerIn, bool unicode) {
    this->locationFontTexture = location;
    this->renderEngine = textureManagerIn;
    this->unicodeFlag = unicode;
    textureManagerIn->bindTexture(*this->locationFontTexture);

    for (int i = 0; i < 32; ++i) {
        int j = (i >> 3 & 1) * 85;
        int k = (i >> 2 & 1) * 170 + j;
        int l = (i >> 1 & 1) * 170 + j;
        int i1 = (i >> 0 & 1) * 170 + j;

        if (i == 6) {
            k += 85;
        }

        if (gameSettingsIn->anaglyph) {
            int j1 = (k * 30 + l * 59 + i1 * 11) / 100;
            int k1 = (k * 30 + l * 70) / 100;
            int l1 = (k * 30 + i1 * 70) / 100;
            k = j1;
            l = k1;
            i1 = l1;
        }

        if (i >= 16) {
            k /= 4;
            l /= 4;
            i1 /= 4;
        }

        this->colorCode[i] = (k & 255) << 16 | (l & 255) << 8 | i1 & 255;
    }

    this->readGlyphSizes();
}

void FontRenderer::onResourceManagerReload(IResourceManager* resourceManager) {
    this->readFontTexture();
}

void FontRenderer::readFontTexture() {
    try {
        int width = 0;
        int height = 0;

        auto stream = Minecraft::getMinecraft()
                ->getResourceManager()
                ->getResource(*locationFontTexture)
                ->getInputStream();

        // directly read the packed ARGB pixel data
        std::vector<int> pixelData = TextureUtil::readImageData_(*stream, width, height);

        // use the vector's data directly, no need to copy to a new int* array
        const int* aint = pixelData.data();
        const int i = width;
        const int j = height;

        const int k = j / 16;
        const int l = i / 16;
        const int i1 = 1;
        const float f = 8.0F / static_cast<float>(l);

        for (int j1 = 0; j1 < 256; ++j1) {
            const int k1 = j1 % 16;
            const int l1 = j1 / 16;

            if (j1 == 32) {
                this->charWidth[j1] = 3 + i1;
            }

            int i2;

            for (i2 = l - 1; i2 >= 0; --i2) {
                const int j2 = k1 * l + i2;
                bool flag = true;

                for (int k2 = 0; k2 < k && flag; ++k2) {
                    const int l2 = (l1 * l + k2) * i;

                    if ((aint[j2 + l2] >> 24 & 255) != 0) {
                        flag = false;
                        break;
                    }
                }

                if (!flag) {
                    break;
                }
            }

            ++i2;
            this->charWidth[j1] = static_cast<int>(0.5 + static_cast<double>(static_cast<float>(i2) * f)) + i1;
        }
    } catch (const std::exception& e) {
        throw std::runtime_error(e.what());
    }
}

void FontRenderer::readGlyphSizes() {
    try {
        ResourceLocation loc("font/glyph_sizes.bin");

        auto stream = Minecraft::getMinecraft()
                ->getResourceManager()
                ->getResource(loc)
                ->getInputStream();

        stream->read(reinterpret_cast<char*>(this->glyphWidth.data()), 65536);
    } catch (const std::exception& e) {
        throw std::runtime_error(e.what());
    }
}

float FontRenderer::renderChar(wchar_t ch, bool italic) {
    if (ch == 32) {
        return 4.0f;
    } else {
        size_t i = CHAR_MAP.find(ch);
        return i != std::wstring::npos && !this->unicodeFlag ? this->renderDefaultChar(static_cast<int>(i), italic) : this->renderUnicodeChar(ch, italic);
    }
}

float FontRenderer::renderDefaultChar(int ch, bool italic) {
    int i = ch % 16 * 8;
    int j = ch / 16 * 8;
    int k = italic ? 1 : 0;
    this->renderEngine->bindTexture(*this->locationFontTexture);
    int l = this->charWidth[ch];
    float f = (float) l - 0.01f;

    ::glBegin(GL_TRIANGLE_STRIP);
    ::glTexCoord2f((float) i / 128.0f, (float) j / 128.0f);
    ::glVertex3f(this->posX + (float) k, this->posY, 0.0f);
    ::glTexCoord2f((float) i / 128.0f, ((float) j + 7.99f) / 128.0f);
    ::glVertex3f(this->posX - (float) k, this->posY + 7.99f, 0.0f);
    ::glTexCoord2f(((float) i + f - 1.0f) / 128.0f, (float) j / 128.0f);
    ::glVertex3f(this->posX + f - 1.0f + (float) k, this->posY, 0.0f);
    ::glTexCoord2f(((float) i + f - 1.0f) / 128.0f, ((float) j + 7.99f) / 128.0f);
    ::glVertex3f(this->posX + f - 1.0f - (float) k, this->posY + 7.99f, 0.0f);
    ::glEnd();

    return (float) l;
}

ResourceLocation* FontRenderer::getUnicodePageLocation(int page) {
    if (unicodePageLocations[page] == nullptr) {
        char buffer[64];
        std::snprintf(buffer, sizeof(buffer), "textures/font/unicode_page_%02x.png", page);
        unicodePageLocations[page] = new ResourceLocation(buffer);
    }
    return unicodePageLocations[page];
}

void FontRenderer::loadGlyphTexture(int page) {
    this->renderEngine->bindTexture(*this->getUnicodePageLocation(page));
}

float FontRenderer::renderUnicodeChar(wchar_t ch, bool italic) {
    if (this->glyphWidth[ch] == 0) {
        return 0.0f;
    } else {
        int i = ch / 256;
        this->loadGlyphTexture(i);
        int j = this->glyphWidth[ch] >> 4;
        int k = this->glyphWidth[ch] & 15;
        float f = (float) j;
        float f1 = (float) (k + 1);
        float f2 = (float) (ch % 16 * 16) + f;
        float f3 = (float) ((ch & 255) / 16 * 16);
        float f4 = f1 - f - 0.02f;
        float f5 = italic ? 1.0f : 0.0f;

        ::glBegin(GL_TRIANGLE_STRIP);
        ::glTexCoord2f(f2 / 256.0f, f3 / 256.0f);
        ::glVertex3f(this->posX + f5, this->posY, 0.0f);
        ::glTexCoord2f(f2 / 256.0f, (f3 + 15.98f) / 256.0f);
        ::glVertex3f(this->posX - f5, this->posY + 7.99f, 0.0f);
        ::glTexCoord2f((f2 + f4) / 256.0f, f3 / 256.0f);
        ::glVertex3f(this->posX + f4 / 2.0f + f5, this->posY, 0.0f);
        ::glTexCoord2f((f2 + f4) / 256.0f, (f3 + 15.98f) / 256.0f);
        ::glVertex3f(this->posX + f4 / 2.0f - f5, this->posY + 7.99f, 0.0f);
        ::glEnd();

        return (f1 - f) / 2.0f + 1.0f;
    }
}

int FontRenderer::drawStringWithShadow(const std::string& text, float x, float y, int color) {
    return this->drawString(text, x, y, color, true);
}

int FontRenderer::drawString(const std::string& text, int x, int y, int color) {
    return this->drawString(text, (float) x, (float) y, color, false);
}

int FontRenderer::drawString(const std::string& text, float x, float y, int color, bool dropShadow) {
    GlStateManager::enableAlpha_();
    this->resetStyles();
    int i;

    if (dropShadow) {
        i = this->renderString(text, x + 1.0f, y + 1.0f, color, true);
        i = std::max(i, this->renderString(text, x, y, color, false));
    } else {
        i = this->renderString(text, x, y, color, false);
    }

    return i;
}

std::string FontRenderer::bidiReorder(const std::string& text) {
    // TODO: Implement using an ICU library equivalent if you need BiDi support
    return text;
}

void FontRenderer::resetStyles() {
    this->randomStyle = false;
    this->boldStyle = false;
    this->italicStyle = false;
    this->underlineStyle = false;
    this->strikethroughStyle = false;
}

void FontRenderer::renderStringAtPos(const std::string& text, bool shadow) {
    for (int i = 0; i < text.length(); ++i) {
        unsigned char c0 = static_cast<unsigned char>(text[i]);

        // 167 is the § (section sign) in Latin-1/extended ASCII
        if (c0 == 167 && i + 1 < text.length()) {
            char lower_c1 = static_cast<char>(std::tolower(text[i + 1]));
            int i1 = std::string("0123456789abcdefklmnor").find(lower_c1);

            if (i1 < 16) {
                this->randomStyle = false;
                this->boldStyle = false;
                this->strikethroughStyle = false;
                this->underlineStyle = false;
                this->italicStyle = false;

                if (i1 < 0 || i1 > 15) {
                    i1 = 15;
                }

                if (shadow) {
                    i1 += 16;
                }

                int j1 = this->colorCode[i1];
                this->textColor = j1;
                GlStateManager::color_((float) (j1 >> 16) / 255.0f, (float) (j1 >> 8 & 255) / 255.0f, (float) (j1 & 255) / 255.0f, this->alpha);
            } else if (i1 == 16) {
                this->randomStyle = true;
            } else if (i1 == 17) {
                this->boldStyle = true;
            } else if (i1 == 18) {
                this->strikethroughStyle = true;
            } else if (i1 == 19) {
                this->underlineStyle = true;
            } else if (i1 == 20) {
                this->italicStyle = true;
            } else if (i1 == 21) {
                this->randomStyle = false;
                this->boldStyle = false;
                this->strikethroughStyle = false;
                this->underlineStyle = false;
                this->italicStyle = false;
                GlStateManager::color_(this->red, this->blue, this->green, this->alpha);
            }

            ++i;
        } else {
            wchar_t wc0 = static_cast<wchar_t>(c0);
            size_t j = CHAR_MAP.find(wc0);
            int j_idx = (j != std::wstring::npos) ? static_cast<int>(j) : -1;

            if (this->randomStyle && j_idx != -1) {
                int k = this->getCharWidth(wc0);
                wchar_t c1;

                while (true) {
                    int random_index = this->fontRandom() % CHAR_MAP.length();
                    c1 = CHAR_MAP[random_index];

                    if (k == this->getCharWidth(c1)) {
                        break;
                    }
                }

                wc0 = c1;
            }

            float f1 = this->unicodeFlag ? 0.5f : 1.0f;
            bool flag = (wc0 == 0 || j_idx == -1 || this->unicodeFlag) && shadow;

            if (flag) {
                this->posX -= f1;
                this->posY -= f1;
            }

            float f = this->renderChar(wc0, this->italicStyle);

            if (flag) {
                this->posX += f1;
                this->posY += f1;
            }

            if (this->boldStyle) {
                this->posX += f1;

                if (flag) {
                    this->posX -= f1;
                    this->posY -= f1;
                }

                this->renderChar(wc0, this->italicStyle);
                this->posX -= f1;

                if (flag) {
                    this->posX += f1;
                    this->posY += f1;
                }

                ++f;
            }

            if (this->strikethroughStyle) {
                Tessellator &tess = Tessellator::getInstance();
                WorldRenderer &renderer = tess.getWorldRenderer();
                GlStateManager::disableTexture2D_();
                renderer.begin(7, DefaultVertexFormats::POSITION);
                renderer.pos(this->posX, this->posY + (float) (this->FONT_HEIGHT / 2), 0.0).endVertex();
                renderer.pos(this->posX + f, this->posY + (float) (this->FONT_HEIGHT / 2), 0.0).endVertex();
                renderer.pos(this->posX + f, this->posY + (float) (this->FONT_HEIGHT / 2) - 1.0f, 0.0).endVertex();
                renderer.pos(this->posX, this->posY + (float) (this->FONT_HEIGHT / 2) - 1.0f, 0.0).endVertex();
                tess.draw();
                GlStateManager::enableTexture2D_();
            }

            if (this->underlineStyle) {
                Tessellator &tess1 = Tessellator::getInstance();
                WorldRenderer &renderer1 = tess1.getWorldRenderer();
                GlStateManager::disableTexture2D_();
                renderer1.begin(7, DefaultVertexFormats::POSITION);
                int l = this->underlineStyle ? -1 : 0;
                renderer1.pos(this->posX + (float) l, this->posY + (float) this->FONT_HEIGHT, 0.0).endVertex();
                renderer1.pos(this->posX + f, this->posY + (float) this->FONT_HEIGHT, 0.0).endVertex();
                renderer1.pos(this->posX + f, this->posY + (float) this->FONT_HEIGHT - 1.0, 0.0).endVertex();
                renderer1.pos(this->posX + (float) l, this->posY + (float) this->FONT_HEIGHT - 1.0, 0.0).endVertex();
                tess1.draw();
                GlStateManager::enableTexture2D_();
            }

            this->posX += (float) ((int) f);
        }
    }
}

int FontRenderer::renderStringAligned(const std::string& text, int x, int y, int width, int color, bool dropShadow) {
    if (this->bidiFlag) {
        int i = this->getStringWidth(this->bidiReorder(text));
        x = x + width - i;
    }
    return this->renderString(text, (float) x, (float) y, color, dropShadow);
}

int FontRenderer::renderString(const std::string& text, float x, float y, int color, bool dropShadow) {
    if (text.empty()) {
        return 0;
    } else {
        std::string actualText = text;
        if (this->bidiFlag) {
            actualText = this->bidiReorder(actualText);
        }

        if ((color & -67108864) == 0) {
            color |= -16777216;
        }

        if (dropShadow) {
            color = (color & 16579836) >> 2 | color & -16777216;
        }

        this->red = (float) (color >> 16 & 255) / 255.0f;
        this->blue = (float) (color >> 8 & 255) / 255.0f;
        this->green = (float) (color & 255) / 255.0f;
        this->alpha = (float) (color >> 24 & 255) / 255.0f;
        GlStateManager::color_(this->red, this->blue, this->green, this->alpha);
        this->posX = x;
        this->posY = y;
        this->renderStringAtPos(actualText, dropShadow);
        return (int) this->posX;
    }
}

int FontRenderer::getStringWidth(const std::string& text) {
    if (text.empty()) {
        return 0;
    } else {
        int i = 0;
        bool flag = false;

        for (int j = 0; j < text.length(); ++j) {
            unsigned char c0 = static_cast<unsigned char>(text[j]);
            int k = this->getCharWidth(static_cast<wchar_t>(c0));

            if (k < 0 && j < text.length() - 1) {
                ++j;
                c0 = static_cast<unsigned char>(text[j]);

                if (c0 != 108 && c0 != 76) { // 'l' and 'L'
                    if (c0 == 114 || c0 == 82) { // 'r' and 'R'
                        flag = false;
                    }
                } else {
                    flag = true;
                }

                k = 0;
            }

            i += k;

            if (flag && k > 0) {
                ++i;
            }
        }

        return i;
    }
}

int FontRenderer::getCharWidth(wchar_t character) {
    if (character == 167) { // §
        return -1;
    } else if (character == 32) {
        return 4;
    } else {
        size_t i = CHAR_MAP.find(character);

        if (character > 0 && i != std::wstring::npos && !this->unicodeFlag) {
            return this->charWidth[static_cast<int>(i)];
        } else if (this->glyphWidth[character] != 0) {
            int j = this->glyphWidth[character] >> 4;
            int k = this->glyphWidth[character] & 15;

            if (k > 7) {
                k = 15;
                j = 0;
            }

            ++k;
            return (k - j) / 2 + 1;
        } else {
            return 0;
        }
    }
}

std::string FontRenderer::trimStringToWidth(const std::string& text, int width) {
    return this->trimStringToWidth(text, width, false);
}

std::string FontRenderer::trimStringToWidth(const std::string& text, int width, bool reverse) {
    std::string stringbuilder;
    int i = 0;
    int j = reverse ? text.length() - 1 : 0;
    int k = reverse ? -1 : 1;
    bool flag = false;
    bool flag1 = false;

    for (int l = j; l >= 0 && l < text.length() && i < width; l += k) {
        unsigned char c0 = static_cast<unsigned char>(text[l]);
        int i1 = this->getCharWidth(static_cast<wchar_t>(c0));

        if (flag) {
            flag = false;

            if (c0 != 108 && c0 != 76) {
                if (c0 == 114 || c0 == 82) {
                    flag1 = false;
                }
            } else {
                flag1 = true;
            }
        } else if (i1 < 0) {
            flag = true;
        } else {
            i += i1;

            if (flag1) {
                ++i;
            }
        }

        if (i > width) {
            break;
        }

        if (reverse) {
            stringbuilder.insert(stringbuilder.begin(), static_cast<char>(c0));
        } else {
            stringbuilder += static_cast<char>(c0);
        }
    }

    return stringbuilder;
}

std::string FontRenderer::trimStringNewline(std::string text) {
    while (!text.empty() && text.back() == '\n') {
        text.pop_back();
    }
    return text;
}

void FontRenderer::drawSplitString(const std::string& str, int x, int y, int wrapWidth, int textColor) {
    this->resetStyles();
    this->textColor = textColor;
    std::string trimmedStr = this->trimStringNewline(str);
    this->renderSplitString(trimmedStr, x, y, wrapWidth, false);
}

void FontRenderer::renderSplitString(const std::string& str, int x, int y, int wrapWidth, bool addShadow) {
    for (const std::string& s : this->listFormattedStringToWidth(str, wrapWidth)) {
        this->renderStringAligned(s, x, y, wrapWidth, this->textColor, addShadow);
        y += this->FONT_HEIGHT;
    }
}

int FontRenderer::splitStringWidth(const std::string& str, int maxLength) {
    return this->FONT_HEIGHT * this->listFormattedStringToWidth(str, maxLength).size();
}

void FontRenderer::setUnicodeFlag(bool unicodeFlagIn) {
    this->unicodeFlag = unicodeFlagIn;
}

bool FontRenderer::getUnicodeFlag() const {
    return this->unicodeFlag;
}

void FontRenderer::setBidiFlag(bool bidiFlagIn) {
    this->bidiFlag = bidiFlagIn;
}

bool FontRenderer::getBidiFlag() const {
    return this->bidiFlag;
}

std::vector<std::string> FontRenderer::listFormattedStringToWidth(const std::string& str, int wrapWidth) {
    std::vector<std::string> result;
    std::string wrapped = this->wrapFormattedStringToWidth(str, wrapWidth);

    std::string token;
    std::istringstream tokenStream(wrapped);
    while (std::getline(tokenStream, token, '\n')) {
        result.push_back(token);
    }

    return result;
}

std::string FontRenderer::wrapFormattedStringToWidth(const std::string& str, int wrapWidth) {
    int i = this->sizeStringToWidth(str, wrapWidth);

    if (str.length() <= i) {
        return str;
    } else {
        std::string s = str.substr(0, i);
        char c0 = str[i];
        bool flag = c0 == 32 || c0 == 10;
        std::string s1 = getFormatFromString(s) + str.substr(i + (flag ? 1 : 0));
        return s + "\n" + this->wrapFormattedStringToWidth(s1, wrapWidth);
    }
}

int FontRenderer::sizeStringToWidth(const std::string& str, int wrapWidth) {
    int i = str.length();
    int j = 0;
    int k = 0;
    int l = -1;
    bool flag = false;

    for (; k < i; ++k) {
        char c0 = str[k];

        if (c0 == '\n') {
            --k;
            break;
        }

        if (c0 == ' ') {
            l = k;
        }

        if (c0 == '\xA7' && k < i - 1) {
            ++k;
            char c1 = str[k];

            if (c1 != 108 && c1 != 76) {
                if (c1 == 114 || c1 == 82 || isFormatColor(c1)) {
                    flag = false;
                }
            } else {
                flag = true;
            }
        } else {
            j += this->getCharWidth(static_cast<wchar_t>(c0));

            if (flag) {
                ++j;
            }
        }

        if (c0 == '\n') {
            ++k;
            l = k;
            break;
        }

        if (j > wrapWidth) {
            break;
        }
    }

    return k != i && l != -1 && l < k ? l : k;
}

bool FontRenderer::isFormatColor(char colorChar) {
    return (colorChar >= '0' && colorChar <= '9') || (colorChar >= 'a' && colorChar <= 'f') || (colorChar >= 'A' && colorChar <= 'F');
}

bool FontRenderer::isFormatSpecial(char formatChar) {
    return (formatChar >= 'k' && formatChar <= 'o') || (formatChar >= 'K' && formatChar <= 'O') || formatChar == 'r' || formatChar == 'R';
}

std::string FontRenderer::getFormatFromString(const std::string& text) {
    std::string s = "";
    int i = -1;
    int j = text.length();

    while ((i = text.find('\xA7', i + 1)) != std::string::npos) {
        if (i < j - 1) {
            char c0 = text[i + 1];

            if (isFormatColor(c0)) {
                s = "\u00a7" + std::string(1, c0);
            } else if (isFormatSpecial(c0)) {
                s = s + "\u00a7" + std::string(1, c0);
            }
        }
    }

    return s;
}

int FontRenderer::getColorCode(char character) const {
    size_t index = std::string("0123456789abcdef").find(character);
    if (index != std::string::npos) {
        return this->colorCode[index];
    }
    return 0;
}
