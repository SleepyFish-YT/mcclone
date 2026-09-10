//
// Created by SleepyFish on 10.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_MODELBASE_H
#define MCCLONE_MODELBASE_H

#include "ModelRenderer.h"
#include "TextureOffset.h"

#include <vector>
#include <unordered_map>
#include <string>
#include <random>
#include <memory>

class Entity;
class EntityLivingBase;

/**
 * @author SleepyFish
 * @brief Base model
 */
class ModelBase {

private:

    std::unordered_map<std::string, TextureOffset> modelTextureMap{};

protected:

    void setTextureOffset(const std::string& partName, int x, int y) {
        this->modelTextureMap[partName] = TextureOffset(x, y);
    }

public:

    float swingProgress = 0.0f;
    bool isRiding = false;
    bool isChild = true;

    std::vector<ModelRenderer*> boxList;

    int textureWidth  = 64;
    int textureHeight = 32;

    virtual ~ModelBase() = default;

    virtual void render(Entity& entityIn, float x, float y, float yaw, float pitch, float partialTicks, float scale) {}

    virtual void setRotationAngles(float limbSwing, float limbSwingAmount, float ageInTicks, float netHeadYaw, float headPitch, float scaleFactor, Entity& entityIn) {}

    virtual void setLivingAnimations(EntityLivingBase& entitylivingbaseIn, float x, float y, float partialTicks) {}

    ModelRenderer* getRandomModelBox(std::mt19937& rand) {
        std::uniform_int_distribution<std::size_t> dist(0, this->boxList.size() - 1);
        return this->boxList[dist(rand)];
    }

    static void copyModelAngles(const ModelRenderer& source, ModelRenderer& dest) {
        dest.rotateAngleX = source.rotateAngleX;
        dest.rotateAngleY = source.rotateAngleY;
        dest.rotateAngleZ = source.rotateAngleZ;
        dest.rotationPointX = source.rotationPointX;
        dest.rotationPointY = source.rotationPointY;
        dest.rotationPointZ = source.rotationPointZ;
    }

    void setModelAttributes(const ModelBase& model) {
        this->swingProgress = model.swingProgress;
        this->isRiding = model.isRiding;
        this->isChild = model.isChild;
    }

    TextureOffset getTextureOffset(const std::string& partName) const {
        auto it = this->modelTextureMap.find(partName);
        return (it != this->modelTextureMap.end()) ? it->second : TextureOffset(0, 0);
    }

};


#endif //MCCLONE_MODELBASE_H
