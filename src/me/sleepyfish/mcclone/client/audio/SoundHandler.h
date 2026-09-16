//
// Created by SleepyFish on 16.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_SOUNDHANDLER_H
#define MCCLONE_SOUNDHANDLER_H

#include "SoundCategory.h"
#include "../resources/IResourceManagerReloadListener.h"
#include "../renderer/texture/ITickableTexture.h"

#include <string>
#include <vector>
#include <map>
#include <random>

class SoundManager;
class SoundRegistry;
class SoundPoolEntry;
class SoundEventAccessorComposite;
class IResourceManager;
class ResourceLocation;
class GameSettings;
class EntityPlayer;
class ISound;
class SoundList;
class SoundEngine;

/**
 * @author SleepyFish
 * @brief SoundHandler
 */
class SoundHandler : public IResourceManagerReloadListener, public ITickableTexture {

public:

    static SoundPoolEntry *missing_sound;

    SoundHandler(IResourceManager *manager, GameSettings *gameSettingsIn, SoundEngine *engine);
    void onResourceManagerReload(IResourceManager& resourceManager) override;
    SoundEventAccessorComposite *getSound(const ResourceLocation &location);

    void playSound(ISound *sound);
    void playDelayedSound(ISound *sound, int delay);
    void setListener(EntityPlayer *player, float p_147691_2_);
    void pauseSounds();
    void stopSounds();
    void unloadSounds();
    void update() override;
    void resumeSounds();
    void setSoundLevel(SoundCategory *category, float volume);
    void stopSound(ISound *sound);
    bool isSoundPlaying(ISound *sound);

    SoundEventAccessorComposite *getRandomSoundFromCategories(const std::vector<SoundCategory>& categories);

protected:

    std::map<std::string, SoundList *> getSoundMap(std::istream &stream);

private:

    void loadSoundResource(const ResourceLocation &location, SoundList *sounds);

    SoundRegistry *sndRegistry;
    SoundManager *sndManager;
    IResourceManager *mcResourceManager;

};


#endif //MCCLONE_SOUNDHANDLER_H
