//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_PLAYERCAPABILITIES_H
#define MCCLONE_PLAYERCAPABILITIES_H

#include "../../nbt/NBTTagCompound.h"

/**
 * @author SleepyFish
 * @brief Player capabilities
 */
class PlayerCapabilities {

private:

    float flySpeed = 0.05f;

    float walkSpeed = 0.1f;

public:

    bool disableDamage{};

    bool isFlying{};

    bool allowFlying{};

    bool isCreativeMode{};

    bool allowEdit = true;

    void writeCapabilitiesToNBT(NBTTagCompound& tagCompound) {
        auto nbttagCompound = std::make_unique<NBTTagCompound>();
        nbttagCompound->setBoolean("invulnerable", this->disableDamage);
        nbttagCompound->setBoolean("flying", this->isFlying);
        nbttagCompound->setBoolean("mayfly", this->allowFlying);
        nbttagCompound->setBoolean("instabuild", this->isCreativeMode);
        nbttagCompound->setBoolean("mayBuild", this->allowEdit);
        nbttagCompound->setFloat("flySpeed", this->flySpeed);
        nbttagCompound->setFloat("walkSpeed", this->walkSpeed);
        tagCompound.setTag("abilities", std::move(nbttagCompound));
    }

    void readCapabilitiesFromNBT(const NBTTagCompound& tagCompound) {
        if (tagCompound.hasKey("abilities", 10)) {
            const NBTTagCompound& nbttagCompound = tagCompound.getCompoundTag("abilities");
            this->disableDamage  = nbttagCompound.getBoolean("invulnerable");
            this->isFlying       = nbttagCompound.getBoolean("flying");
            this->allowFlying    = nbttagCompound.getBoolean("mayfly");
            this->isCreativeMode = nbttagCompound.getBoolean("instabuild");

            if (nbttagCompound.hasKey("flySpeed", 99)) {
                this->flySpeed  = nbttagCompound.getFloat("flySpeed");
                this->walkSpeed = nbttagCompound.getFloat("walkSpeed");
            }

            if (nbttagCompound.hasKey("mayBuild", 1)) {
                this->allowEdit = nbttagCompound.getBoolean("mayBuild");
            }
        }
    }

    float getFlySpeed() const {
        return this->flySpeed;
    }

    void setFlySpeed(float speed) {
        this->flySpeed = speed;
    }

    float getWalkSpeed() const {
        return this->walkSpeed;
    }

    void setWalkSpeed(float speed) {
        this->walkSpeed = speed;
    }

};


#endif //MCCLONE_PLAYERCAPABILITIES_H
