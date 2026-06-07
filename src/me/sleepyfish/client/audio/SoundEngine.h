//
// Created by SleepyFish on 06.06.2026.
// Project: mcclone
//

#ifndef MCCLONE_SOUNDENGINE_H
#define MCCLONE_SOUNDENGINE_H

#include <AL/al.h>
#include <AL/alc.h>
#include <string>
#include <vector>
#include <filesystem>
#include <mutex>

/**
 * @author SleepyFish
 * @brief This class is used to handle audio playback (thread-safe)
 */
class SoundEngine {

public:

    struct ActiveSound {
        ALuint source;
        ALuint buffer;
    };

    SoundEngine();
    SoundEngine(const std::filesystem::path& soundDir);
    SoundEngine& operator=(const SoundEngine&) = delete;
    SoundEngine(SoundEngine&&) = delete;
    SoundEngine& operator=(SoundEngine&&) = delete;

    ~SoundEngine();

    void playSound(const std::string& name, float volume = 1.0f, float pitch = 1.0f);
    void playSound3D(const std::string& name, float x, float y, float z, float volume = 1.0f, float pitch = 1.0f);
    void setListenerPosition3D(float x, float y, float z);
    void cleanup();
    void destory(bool msg = true);

private:

    ALuint loadBuffer(const std::string& name);

    ALCdevice* device;
    ALCcontext* context;
    std::filesystem::path soundDir;
    std::vector<ActiveSound> activeSources;
    std::mutex mutex;

};

#endif //MCCLONE_SOUNDENGINE_H

