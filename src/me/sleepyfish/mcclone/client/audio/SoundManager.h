//
// Created by SleepyFish on 16.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_SOUNDMANAGER_H
#define MCCLONE_SOUNDMANAGER_H

#include "SoundCategory.h"
#include "SoundPoolEntry.h"

#include <string>
#include <unordered_map>
#include <vector>
#include <mutex>

class SoundHandler;
class SoundEngine;
class GameSettings;
class EntityPlayer;
class ISound;
class ITickableSound;
class SoundEventAccessorComposite;

/**
 * @author SleepyFish
 * @brief SoundManager
 */
class SoundManager {

public:

    SoundManager(SoundHandler *sndHandler, GameSettings *options, SoundEngine *engine);
    ~SoundManager() = default;

    // disable copy
    SoundManager(const SoundManager &) = delete;
    SoundManager &operator=(const SoundManager &) = delete;

    void reloadSoundSystem();
    void unloadSoundSystem();
    void stopAllSounds();
    void updateAllSounds();
    void pauseAllSounds();
    void resumeAllSounds();
    void playSound(ISound *sound);
    void playDelayedSound(ISound *sound, int delay);
    void stopSound(ISound *sound);
    void setListener(EntityPlayer *player, float partialTicks);
    void setSoundCategoryVolume(SoundCategory category, float volume);
    bool isSoundPlaying(ISound *sound) const;

private:

    float getSoundCategoryVolume(SoundCategory category) const;
    float getNormalizedVolume(ISound *sound, const SoundPoolEntry &entry, SoundCategory category) const;
    float getNormalizedPitch(ISound *sound, const SoundPoolEntry &entry) const;

    std::string generateUUID() const;
    SoundHandler *sndHandler;
    GameSettings *options;
    SoundEngine *engine;

    bool loaded = false;
    int playTime = 0;

    std::unordered_map<std::string, ISound *> playingSounds;
    std::unordered_map<ISound *, std::string> invPlayingSounds;
    std::unordered_map<ISound *, SoundPoolEntry> playingSoundPoolEntries;
    std::unordered_map<SoundCategory, std::vector<std::string>> categorySounds;
    std::vector<ITickableSound *> tickableSounds;
    std::unordered_map<ISound *, int> delayedSounds;
    std::unordered_map<std::string, int> playingSoundsStopTime;

    mutable std::mutex mutex;

};


#endif //MCCLONE_SOUNDMANAGER_H
