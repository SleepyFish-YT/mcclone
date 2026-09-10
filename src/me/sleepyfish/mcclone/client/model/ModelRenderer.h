//
// Created by SleepyFish on 10.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_MODELRENDERER_H
#define MCCLONE_MODELRENDERER_H

#include "ModelBox.h"
#include "TextureOffset.h"

#include <string>
#include <vector>
#include <memory>

class ModelBase;
class ResourceLocation;
class ModelSprite;
class ModelUpdater;

/**
 * @author SleepyFish
 * @brief Model renderer
 */
class ModelRenderer {

private:

    int textureOffsetX;
    int textureOffsetY;
    bool compiled = false;
    int displayList = 0;
    const ModelBase& baseModel;
    int countResetDisplayList = 0;

    ResourceLocation* textureLocation = nullptr; // non-owning, may be null
    std::string id; // empty == null
    ModelUpdater* modelUpdater = nullptr; // non-owning, may be null

    void compileDisplayList(float scale);
    void checkResetDisplayList();

public:

    float textureWidth;
    float textureHeight;

    float rotationPointX = 0.0f;
    float rotationPointY = 0.0f;
    float rotationPointZ = 0.0f;
    float rotateAngleX = 0.0f;
    float rotateAngleY = 0.0f;
    float rotateAngleZ = 0.0f;

    bool mirror = false;
    bool showModel = true;
    bool isHidden = false;

    std::vector<std::unique_ptr<ModelBox>> cubeList;
    std::vector<ModelRenderer*> childModels; // non-owning

    const std::string boxName;

    float offsetX = 0.0f;
    float offsetY = 0.0f;
    float offsetZ = 0.0f;

    std::vector<std::unique_ptr<ModelSprite>> spriteList;

    bool mirrorV = false;
    float scaleX = 1.0f;
    float scaleY = 1.0f;
    float scaleZ = 1.0f;

    ModelRenderer(ModelBase& model, std::string  boxNameIn);
    explicit ModelRenderer(ModelBase& model);
    ModelRenderer(ModelBase& model, int texOffX, int texOffY);

    void addChild(ModelRenderer& renderer) {
        this->childModels.push_back(&renderer);
    }

    ModelRenderer& setTextureOffset(int x, int y) {
        this->textureOffsetX = x;
        this->textureOffsetY = y;
        return *this;
    }

    ModelRenderer& addBox(const std::string& partName, float offX, float offY, float offZ, int width, int height, int depth);

    ModelRenderer& addBox(float offX, float offY, float offZ, int width, int height, int depth);

    ModelRenderer& addBox(float offX, float offY, float offZ, int width, int height, int depth, bool idk);

    void addBox(float offX, float offY, float offZ, int width, int height, int depth, float scaleFactor);

    void addBox(const int boxCoords[6][4], float b, float c, float d, float e, float f, float g, float h);

    void setRotationPoint(float pointXIn, float pointYIn, float pointZIn) {
        this->rotationPointX = pointXIn;
        this->rotationPointY = pointYIn;
        this->rotationPointZ = pointZIn;
    }

    void render(float scale);
    void renderWithRotation(float scale);
    void postRender(float scale);

    ModelRenderer& setTextureSize(int textureWidthIn, int textureHeightIn) {
        this->textureWidth  = static_cast<float>(textureWidthIn);
        this->textureHeight = static_cast<float>(textureHeightIn);
        return *this;
    }

    void addSprite(float a, float b, float c, int d, int e, int f, float g);

    bool getCompiled() const {
        return this->compiled;
    }

    int  getDisplayList() const {
        return this->displayList;
    }

    ResourceLocation* getTextureLocation() const {
        return this->textureLocation;
    }

    void setTextureLocation(ResourceLocation* location) {
        this->textureLocation = location;
    }

    const std::string& getId() const {
        return this->id;
    }

    void setId(const std::string& name) {
        this->id = name;
    }

    ModelRenderer* getChild(const std::string& childName);
    ModelRenderer* getChildDeep(const std::string& childName);

    void setModelUpdater(ModelUpdater* updater) {
        this->modelUpdater = updater;
    }

    std::string toString() const;

    int getTextureOffsetX() const { return textureOffsetX; }
    int getTextureOffsetY() const { return textureOffsetY; }

};

#endif //MCCLONE_MODELRENDERER_H
