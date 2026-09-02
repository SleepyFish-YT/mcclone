//
// Created by SleepyFish on 06.06.2026.
// Project: mcclone
//

#include "SoundEngine.h"
#include "../../debug/Logger.h"

extern "C" {

    #include "stb_vorbis.c"

    int stb_vorbis_decode_filename(const char *filename, int *channels, int *sample_rate, short **output);

}

SoundEngine::SoundEngine() {
    this->device = nullptr;
    this->context = nullptr;
    this->soundDir = "";
    this->activeSources = {};
    Logger::log("Created empty sound engine");
}

SoundEngine::SoundEngine(const std::filesystem::path& sound_dir) {
    this->soundDir = sound_dir;

    try {
        if (!std::filesystem::exists(this->soundDir)) {
            std::filesystem::create_directory(this->soundDir);
            Logger::log("Created sound directory: " + this->soundDir.string());
        }
    } catch (const std::exception& e) {
        Logger::error("Failed to create sound directory: " + std::string(e.what()));
        return;
    }

    this->device = alcOpenDevice(nullptr);
    if (!this->device) {
        Logger::error("Failed to open audio device");
        return;
    }

    this->context = alcCreateContext(this->device, nullptr);
    if (!this->context) {
        ALCenum error = alcGetError(this->device);
        Logger::error("Failed to create audio context: " + std::to_string(error));
        alcCloseDevice(this->device);
        return;
    }

    if (!alcMakeContextCurrent(this->context)) {
        ALCenum error = alcGetError(this->device);
        Logger::error("Failed to make audio context current: " + std::to_string(error));
        alcDestroyContext(this->context);
        alcCloseDevice(this->device);
        return;
    }

    Logger::log("Created sound engine");
}

SoundEngine::~SoundEngine() {
    this->destory(false);
}

void SoundEngine::destory(bool msg) {
    std::lock_guard<std::mutex> lock(this->mutex);
    for (auto& sound : this->activeSources) {
        alDeleteSources(1, &sound.source);
        alDeleteBuffers(1, &sound.buffer);
    }
    this->activeSources.clear();

    alcMakeContextCurrent(nullptr);
    alcDestroyContext(this->context);
    alcCloseDevice(this->device);

    if (msg) {
        Logger::log("Destroyed sound engine");
    }
}

ALuint SoundEngine::loadBuffer(const std::string& audio_name) {
    std::string file_name = audio_name + ".ogg";

    std::filesystem::path soundPath = this->soundDir / file_name;
    if (!std::filesystem::exists(soundPath)) {
        Logger::warn("Sound not found in soundDir, falling back to path: " + file_name);
        soundPath = std::filesystem::path(file_name);
    }

    if (!std::filesystem::exists(soundPath)) {
        Logger::error("Failed to find sound: " + file_name);
        return 0;
    }

    std::string path = soundPath.string();
    int channels, sampleRate;
    short* output;
    int samples = stb_vorbis_decode_filename(path.c_str(), &channels, &sampleRate, &output);

    if (samples < 0 || output == nullptr) {
        Logger::error("Failed to load sound: " + path);
        return 0;
    }

    ALuint buffer;
    alGenBuffers(1, &buffer);
    ALenum format = (channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
    alBufferData(buffer, format, output, samples * channels * sizeof(short), sampleRate);
    free(output);
    return buffer;
}

void SoundEngine::setListenerPosition3D(float x, float y, float z) {
    std::lock_guard<std::mutex> lock(this->mutex);
    alListener3f(AL_POSITION, x, y, z);
}

void SoundEngine::playSound(const std::string& name, float volume, float pitch) {
    if (volume <= 0.0f || volume > 1.0f) {
        return;
    }

    if (pitch <= 0.0f) {
        pitch = 1.0f;
    }

    std::lock_guard<std::mutex> lock(this->mutex);

    ALuint buffer = this->loadBuffer(name);
    if (buffer == 0) {
        Logger::error("Failed to load buffer from sound: " + name);
        return;
    }

    ALuint source;
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
    alSourcef(source, AL_GAIN, volume);
    alSourcef(source, AL_PITCH, pitch);
    alSource3f(source, AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE);
    alSourcePlay(source);

    this->activeSources.push_back({source, buffer});
}

void SoundEngine::playSound3D(const std::string& name, float x, float y, float z, float volume, float pitch) {
    if (volume <= 0.0f || volume > 1.0f) {
        return;
    }

    if (pitch <= 0.0f) {
        pitch = 1.0f;
    }

    std::lock_guard<std::mutex> lock(this->mutex);

    ALuint buffer = this->loadBuffer(name);
    if (buffer == 0) {
        Logger::error("Failed to load buffer from sound: " + name);
        return;
    }

    ALuint source;
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
    alSourcef(source, AL_GAIN, volume);
    alSourcef(source, AL_PITCH, pitch);
    alSource3f(source, AL_POSITION, x, y, z);
    alSourcei(source, AL_SOURCE_RELATIVE, AL_FALSE);
    alSourcePlay(source);

    this->activeSources.push_back({source, buffer});
}

void SoundEngine::cleanup() {
    std::lock_guard<std::mutex> lock(this->mutex);

    for (auto it = this->activeSources.begin(); it != this->activeSources.end();) {
        ALint state;
        alGetSourcei(it->source, AL_SOURCE_STATE, &state);

        if (state != AL_PLAYING) {
            alDeleteSources(1, &it->source);
            alDeleteBuffers(1, &it->buffer);
            it = this->activeSources.erase(it);
        } else {
            ++it;
        }
    }
}

