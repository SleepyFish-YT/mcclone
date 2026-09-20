//
// Created by SleepyFish on 15.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_FONTRENDERER_H
#define MCCLONE_FONTRENDERER_H

#include "../resources/IResourceManagerReloadListener.h"
#include "../../util/ResourceLocation.h"

#include <string>
#include <vector>
#include <array>
#include <random>
#include <cstdint>

class GameSettings;
class TextureManager;
class IResourceManager;
class BufferedImage;
class Tessellator;
class WorldRenderer;

/*
 * @author SleepyFish
 * @brief Font renderer
 */
class FontRenderer : public IResourceManagerReloadListener {

private:

    static inline std::array<ResourceLocation*, 256> unicodePageLocations{};

    static const std::wstring CHAR_MAP;

    std::array<int, 256> charWidth{};

public:

    int FONT_HEIGHT = 9;
    std::mt19937 fontRandom;

private:

    std::array<uint8_t, 65536> glyphWidth{};
    std::array<int, 32> colorCode{};
    ResourceLocation locationFontTexture;
    TextureManager* renderEngine{};
    int fontTextureWidth = 128;
    int fontTextureHeight = 128;

    float posX{};
    float posY{};
    bool unicodeFlag{};
    bool bidiFlag{};

    float red{};
    float blue{};
    float green{};
    float alpha{};
    int textColor{};

    bool randomStyle{};
    bool boldStyle{};
    bool italicStyle{};
    bool underlineStyle{};
    bool strikethroughStyle{};

public:

    FontRenderer(GameSettings *gameSettingsIn, const ResourceLocation& location, TextureManager* textureManagerIn, bool unicode);

    void onResourceManagerReload(IResourceManager &resourceManager) override;

private:

    void readFontTexture();
    void readGlyphSizes();

    float renderChar(wchar_t ch, bool italic);
    float renderDefaultChar(int ch, bool italic);

    ResourceLocation *getUnicodePageLocation(int page);
    void loadGlyphTexture(int page);

    float renderUnicodeChar(wchar_t ch, bool italic);

public:

    int drawStringWithShadow(const std::string &text, float x, float y, int color);
    int drawString(const std::string &text, int x, int y, int color);
    int drawString(const std::string &text, float x, float y, int color, bool dropShadow);

private:

    std::string bidiReorder(const std::string &text);

    void resetStyles();
    void renderStringAtPos(const std::string &text, bool shadow);
    int renderStringAligned(const std::string &text, int x, int y, int width, int color, bool dropShadow);
    int renderString(const std::string &text, float x, float y, int color, bool dropShadow);

public:
    int getStringWidth(const std::string &text);
    int getCharWidth(wchar_t character);

    std::string trimStringToWidth(const std::string &text, int width);
    std::string trimStringToWidth(const std::string &text, int width, bool reverse);

private:

    std::string trimStringNewline(std::string text);

public:

    void drawSplitString(const std::string &str, int x, int y, int wrapWidth, int textColor);

private:

    void renderSplitString(const std::string &str, int x, int y, int wrapWidth, bool addShadow);

public:

    int splitStringWidth(const std::string &str, int maxLength);

    void setUnicodeFlag(bool unicodeFlagIn);
    bool getUnicodeFlag() const;

    void setBidiFlag(bool bidiFlagIn);
    bool getBidiFlag() const;

    std::vector<std::string> listFormattedStringToWidth(const std::string &str, int wrapWidth);

private:

    std::string wrapFormattedStringToWidth(const std::string &str, int wrapWidth);
    int sizeStringToWidth(const std::string &str, int wrapWidth);

    static bool isFormatColor(char colorChar);
    static bool isFormatSpecial(char formatChar);

public:

    static std::string getFormatFromString(const std::string &text);
    int getColorCode(char character) const;

};


#endif //MCCLONE_FONTRENDERER_H
