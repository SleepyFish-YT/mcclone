//
// Created by SleepyFish on 09.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_ACTIVERENDERINFO_H
#define MCCLONE_ACTIVERENDERINFO_H

#include "GLAllocation.h"
#include "../../util/BlockPos.h"
#include "../../util/Vec3.h"
#include "../../util/MathHelper.h"

/* not FULLY implemented yet.
#include "GlStateManager.h"
#include "../world/World.h"
#include "../entity/Entity.h"
#include "../entity/player/EntityPlayer.h"
#include "../block/Block.h"
#include "../block/BlockLiquid.h"
 */

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

/**
 * @author SleepyFish
 * @brief Active render info
 */
class ActiveRenderInfo {

private:

    static std::vector<int> VIEWPORT;
    static std::vector<float> MODELVIEW;
    static std::vector<float> PROJECTION;

    static Vec3 position;
    static float rotationX;
    static float rotationXZ;
    static float rotationZ;
    static float rotationYZ;
    static float rotationXY;

public:

    /*
    static void updateRenderInfo(EntityPlayer& player, bool thirdPerson) {
        glGetFloatv(GL_MODELVIEW_MATRIX,  MODELVIEW.data());
        glGetFloatv(GL_PROJECTION_MATRIX, PROJECTION.data());
        glGetIntegerv(GL_VIEWPORT,        VIEWPORT.data());

        float f  = (float)((VIEWPORT[0] + VIEWPORT[2]) / 2);
        float f1 = (float)((VIEWPORT[1] + VIEWPORT[3]) / 2);

        // glm::unProject replaces gluUnProject
        glm::mat4 modelview  = glm::make_mat4(MODELVIEW.data());
        glm::mat4 projection = glm::make_mat4(PROJECTION.data());
        glm::vec4 viewport   = glm::vec4(VIEWPORT[0], VIEWPORT[1], VIEWPORT[2], VIEWPORT[3]);

        glm::vec3 obj = glm::unProject(glm::vec3(f, f1, 0.0f), modelview, projection, viewport);
        position = Vec3(obj.x, obj.y, obj.z);

        int i    = thirdPerson ? 1 : 0;
        float f2 = player.rotationPitch;
        float f3 = player.rotationYaw;

        rotationX  =  MathHelper::cos(f3 * (float)M_PI / 180.0f) * (float)(1 - i * 2);
        rotationZ  =  MathHelper::sin(f3 * (float)M_PI / 180.0f) * (float)(1 - i * 2);
        rotationYZ = -rotationZ  * MathHelper::sin(f2 * (float)M_PI / 180.0f) * (float)(1 - i * 2);
        rotationXY =  rotationX  * MathHelper::sin(f2 * (float)M_PI / 180.0f) * (float)(1 - i * 2);
        rotationXZ =  MathHelper::cos(f2 * (float)M_PI / 180.0f);
    }

    static Vec3 projectViewFromEntity(Entity& entity, double partialTicks) {
        double d0 = entity.prevPosX + (entity.posX - entity.prevPosX) * partialTicks;
        double d1 = entity.prevPosY + (entity.posY - entity.prevPosY) * partialTicks;
        double d2 = entity.prevPosZ + (entity.posZ - entity.prevPosZ) * partialTicks;
        return Vec3(d0 + position.xCoord, d1 + position.yCoord, d2 + position.zCoord);
    }

    static Block* getBlockAtEntityViewpoint(World& world, Entity& entity, float partialTicks) {
        Vec3     vec3      = projectViewFromEntity(entity, partialTicks);
        BlockPos blockpos  = BlockPos(vec3);
        IBlockState state  = world.getBlockState(blockpos);
        Block* block       = state.getBlock();

        if (block->getMaterial().isLiquid()) {
            float f = 0.0f;

            BlockLiquid* liquid = dynamic_cast<BlockLiquid*>(block);
            if (liquid)
                f = BlockLiquid::getLiquidHeightPercent(state.getValue(BlockLiquid::LEVEL)) - 0.11111111f;

            float f1 = (float)(blockpos.getY() + 1) - f;

            if (vec3.yCoord >= (double)f1)
                block = world.getBlockState(blockpos.up()).getBlock();
        }

        return block;
    }
    */

    static Vec3 getPosition() {
        return ActiveRenderInfo::position;
    }

    static float getRotationX() {
        return ActiveRenderInfo::rotationX;
    }

    static float getRotationXZ() {
        return ActiveRenderInfo::rotationXZ;
    }

    static float getRotationZ()  {
        return ActiveRenderInfo::rotationZ;
    }

    static float getRotationYZ() {
        return ActiveRenderInfo::rotationYZ;
    }

    static float getRotationXY() {
        return ActiveRenderInfo::rotationXY;
    }

};

inline std::vector<int> ActiveRenderInfo::VIEWPORT = GLAllocation::createDirectIntBuffer(16);
inline std::vector<float> ActiveRenderInfo::MODELVIEW  = GLAllocation::createDirectFloatBuffer(16);
inline std::vector<float> ActiveRenderInfo::PROJECTION = GLAllocation::createDirectFloatBuffer(16);
inline Vec3 ActiveRenderInfo::position   = Vec3(0.0, 0.0, 0.0);
inline float ActiveRenderInfo::rotationX  = 0.0f;
inline float ActiveRenderInfo::rotationXZ = 0.0f;
inline float ActiveRenderInfo::rotationZ  = 0.0f;
inline float ActiveRenderInfo::rotationYZ = 0.0f;
inline float ActiveRenderInfo::rotationXY = 0.0f;

#endif //MCCLONE_ACTIVERENDERINFO_H
