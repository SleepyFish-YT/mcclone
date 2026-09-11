//
// Created by SleepyFish on 10.09.2026.
// Project: mcclone
//

// not FULLY implemented yet.

#include "ModelRenderer.h"

#include "ModelBase.h"
#include "../../util/ResourceLocation.h"
#include "ModelSprite.h"
// #include "ModelUpdater.h"
#include "../renderer/GlStateManager.h"
#include "../renderer/GLAllocation.h"
// #include "Tessellator.h"
// #include "WorldRenderer.h"
// #include "Shaders.h" // countResetDisplayLists
#include "../../util/MathUtils.h"

#include <cmath>
#include <utility>

ModelRenderer::ModelRenderer(ModelBase& model, std::string boxNameIn) :
    textureWidth(64.0f),
    textureHeight(32.0f),
    boxName(std::move(boxNameIn)),
    baseModel(model)
{
    model.boxList.push_back(this);

    this->setTextureSize(static_cast<int>(model.textureWidth), static_cast<int>(model.textureHeight));
}

ModelRenderer::ModelRenderer(ModelBase& model) :
    ModelRenderer(model, std::string())
{}

ModelRenderer::ModelRenderer(ModelBase& model, int texOffX, int texOffY) :
    ModelRenderer(model)
{
    this->setTextureOffset(texOffX, texOffY);
}

ModelRenderer& ModelRenderer::addBox(const std::string& partName, float offX, float offY, float offZ, int width, int height, int depth) {
    /*
    const std::string fullName = this->boxName + "." + partName;
    const TextureOffset textureoffset_ = this->baseModel.getTextureOffset(fullName);

    this->setTextureOffset(textureoffset_.offsetX, textureoffset_.offsetY);

    auto box = std::make_unique<ModelBox>(
            *this,
            this->textureOffsetX, this->textureOffsetY,
            offX, offY, offZ,
            width, height, depth,
            0.0f
    );

    box->setBoxName(fullName);
    this->cubeList.push_back(std::move(box));
     */
    return *this;
}

ModelRenderer& ModelRenderer::addBox(float offX, float offY, float offZ, int width, int height, int depth) {
    /*this->cubeList.push_back(std::make_unique<ModelBox>(
            *this,
            this->textureOffsetX, this->textureOffsetY,
            offX, offY, offZ,
            width, height, depth,
            0.0f)
        );*/
    return *this;
}

ModelRenderer& ModelRenderer::addBox(float offX, float offY, float offZ, int width, int height, int depth, bool idk) {
    /*
    this->cubeList.push_back(std::make_unique<ModelBox>(
            *this,
            this->textureOffsetX, this->textureOffsetY,
            offX, offY, offZ,
            width, height, depth,
            0.0f, idk)
    );
    */
    return *this;
}

void ModelRenderer::addBox(float offX, float offY, float offZ, int width, int height, int depth, float scaleFactor) {
    /*
    this->cubeList.push_back(std::make_unique<ModelBox>(
            *this,
            this->textureOffsetX, this->textureOffsetY,
            offX, offY, offZ,
            width, height, depth,
            scaleFactor)
    );
    */
}

void ModelRenderer::addBox(const int boxCoords[6][4], float b, float c, float d, float e, float f, float g, float h) {
    /*
    this->cubeList.push_back(std::make_unique<ModelBox>(
            *this,
            boxCoords,
            b, c, d,
            e, f, g, h,
            this->mirror)
    );
    */
}

void ModelRenderer::render(float scale) {
    if (this->isHidden || !this->showModel) {
        return;
    }

    this->checkResetDisplayList();

    if (!this->compiled) {
        this->compileDisplayList(scale);
    }

    int i = 0;

    // (renderGlobal overlay checks omitted - vanilla-only path)
    if (this->textureLocation != nullptr) {
        i = GlStateManager::getBoundTexture_();
        // Config::getTextureManager().bindTexture(*this->textureLocation);
    }

    if (this->modelUpdater != nullptr) {
        // this->modelUpdater->update();
    }

    const bool flag = this->scaleX != 1.0f || this->scaleY != 1.0f || this->scaleZ != 1.0f;

    GlStateManager::translate_(this->offsetX, this->offsetY, this->offsetZ);

    if (this->rotateAngleX == 0.0f && this->rotateAngleY == 0.0f && this->rotateAngleZ == 0.0f) {
        if (this->rotationPointX == 0.0f && this->rotationPointY == 0.0f && this->rotationPointZ == 0.0f) {
            if (flag) {
                GlStateManager::scale_(this->scaleX, this->scaleY, this->scaleZ);
            }

            GlStateManager::callList_(this->displayList);

            for (ModelRenderer* child : this->childModels) {
                child->render(scale);
            }

            if (flag) {
                GlStateManager::scale_(1.0f / this->scaleX, 1.0f / this->scaleY, 1.0f / this->scaleZ);
            }
        } else {
            GlStateManager::translate_(this->rotationPointX * scale, this->rotationPointY * scale, this->rotationPointZ * scale);

            if (flag) {
                GlStateManager::scale_(this->scaleX, this->scaleY, this->scaleZ);
            }

            GlStateManager::callList_(this->displayList);

            for (ModelRenderer* child : this->childModels) {
                child->render(scale);
            }

            if (flag) {
                GlStateManager::scale_(1.0f / this->scaleX, 1.0f / this->scaleY, 1.0f / this->scaleZ);
            }

            GlStateManager::translate_(-this->rotationPointX * scale, -this->rotationPointY * scale, -this->rotationPointZ * scale);
        }
    } else {
        GlStateManager::pushMatrix_();
        GlStateManager::translate_(this->rotationPointX * scale, this->rotationPointY * scale, this->rotationPointZ * scale);

        if (this->rotateAngleZ != 0.0f) {
            GlStateManager::rotate_(MathUtils::toDeg(this->rotateAngleZ), 0.0f, 0.0f, 1.0f);
        }
        if (this->rotateAngleY != 0.0f) {
            GlStateManager::rotate_(MathUtils::toDeg(this->rotateAngleY), 0.0f, 1.0f, 0.0f);
        }
        if (this->rotateAngleX != 0.0f) {
            GlStateManager::rotate_(MathUtils::toDeg(this->rotateAngleX), 1.0f, 0.0f, 0.0f);
        }

        if (flag) {
            GlStateManager::scale_(this->scaleX, this->scaleY, this->scaleZ);
        }

        GlStateManager::callList_(this->displayList);

        for (ModelRenderer* child : this->childModels) {
            child->render(scale);
        }

        GlStateManager::popMatrix_();
    }

    GlStateManager::translate_(-this->offsetX, -this->offsetY, -this->offsetZ);

    if (i != 0) {
        GlStateManager::bindTexture_(i);
    }
}

void ModelRenderer::renderWithRotation(float scale) {
    if (this->isHidden || !this->showModel) {
        return;
    }

    this->checkResetDisplayList();

    if (!this->compiled) {
        this->compileDisplayList(scale);
    }

    int i = 0;

    if (this->textureLocation != nullptr) {
        i = GlStateManager::getBoundTexture_();
        // Config::getTextureManager().bindTexture(*this->textureLocation);
    }

    if (this->modelUpdater != nullptr) {
        // this->modelUpdater->update();
    }

    const bool flag = this->scaleX != 1.0f || this->scaleY != 1.0f || this->scaleZ != 1.0f;

    GlStateManager::pushMatrix_();
    GlStateManager::translate_(this->rotationPointX * scale, this->rotationPointY * scale, this->rotationPointZ * scale);

    if (this->rotateAngleY != 0.0f) {
        GlStateManager::rotate_(MathUtils::toDeg(this->rotateAngleY), 0.0f, 1.0f, 0.0f);
    }
    if (this->rotateAngleX != 0.0f) {
        GlStateManager::rotate_(MathUtils::toDeg(this->rotateAngleX), 1.0f, 0.0f, 0.0f);
    }
    if (this->rotateAngleZ != 0.0f) {
        GlStateManager::rotate_(MathUtils::toDeg(this->rotateAngleZ), 0.0f, 0.0f, 1.0f);
    }

    if (flag) {
        GlStateManager::scale_(this->scaleX, this->scaleY, this->scaleZ);
    }

    GlStateManager::callList_(this->displayList);

    for (ModelRenderer* child : this->childModels) {
        child->render(scale);
    }

    GlStateManager::popMatrix_();

    if (i != 0) {
        GlStateManager::bindTexture_(i);
    }
}

void ModelRenderer::postRender(float scale) {
    if (this->isHidden || !this->showModel) {
        return;
    }

    this->checkResetDisplayList();

    if (!this->compiled) {
        this->compileDisplayList(scale);
    }

    constexpr float RAD2DEG = 180.0f / 3.14159265358979323846f;

    if (this->rotateAngleX == 0.0f && this->rotateAngleY == 0.0f && this->rotateAngleZ == 0.0f) {
        if (this->rotationPointX != 0.0f || this->rotationPointY != 0.0f || this->rotationPointZ != 0.0f) {
            GlStateManager::translate_(this->rotationPointX * scale, this->rotationPointY * scale, this->rotationPointZ * scale);
        }
    } else {
        GlStateManager::translate_(this->rotationPointX * scale, this->rotationPointY * scale, this->rotationPointZ * scale);

        if (this->rotateAngleZ != 0.0f) {
            GlStateManager::rotate_(this->rotateAngleZ * RAD2DEG, 0.0f, 0.0f, 1.0f);
        }
        if (this->rotateAngleY != 0.0f) {
            GlStateManager::rotate_(this->rotateAngleY * RAD2DEG, 0.0f, 1.0f, 0.0f);
        }
        if (this->rotateAngleX != 0.0f) {
            GlStateManager::rotate_(this->rotateAngleX * RAD2DEG, 1.0f, 0.0f, 0.0f);
        }
    }
}

void ModelRenderer::compileDisplayList(float scale) {
    if (this->displayList == 0) {
        this->displayList = GLAllocation::generateDisplayLists(1);
    }

    GlStateManager::glNewList_(this->displayList, GL_COMPILE);
    // WorldRenderer& renderer = Tessellator::getInstance().getWorldRenderer();

    for (auto& box : this->cubeList) {
        // box->render(renderer, scale);
    }

    for (auto& sprite : this->spriteList) {
        // sprite->render(Tessellator::getInstance(), scale);
    }

    GlStateManager::glEndList_();
    this->compiled = true;
}

void ModelRenderer::addSprite(float a, float b, float c, int d, int e, int f, float g) {
    this->spriteList.push_back(std::make_unique<ModelSprite>(
            *this,
            this->textureOffsetX, this->textureOffsetY,
            a, b, c,
            d, e, f,
            g));
}

void ModelRenderer::checkResetDisplayList() {
    /*
    if (this->countResetDisplayList != Shaders::countResetDisplayLists) {
        this->compiled = false;
        this->countResetDisplayList = Shaders::countResetDisplayLists;
    }
    */
}

ModelRenderer* ModelRenderer::getChild(const std::string& childName) {
    if (childName.empty()) {
        return nullptr;
    }

    for (ModelRenderer* child : this->childModels) {
        if (childName == child->getId()) {
            return child;
        }
    }

    return nullptr;
}

ModelRenderer* ModelRenderer::getChildDeep(const std::string& childName) {
    if (childName.empty()) {
        return nullptr;
    }

    if (ModelRenderer* direct = this->getChild(childName)) {
        return direct;
    }

    for (ModelRenderer* child : this->childModels) {
        if (ModelRenderer* deep = child->getChildDeep(childName)) {
            return deep;
        }
    }

    return nullptr;
}

std::string ModelRenderer::toString() const {
    return "id: " + this->id +
        ", boxes: " + std::to_string(this->cubeList.size()) +
        ", submodels: " + std::to_string(this->childModels.size());
}
