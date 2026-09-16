//
// Created by SleepyFish on 16.09.2026.
// Project: mcclone
//

#include "SoundManager.h"

#include "SoundEngine.h"
#include "SoundEventAccessorComposite.h"
#include "ISound.h"
#include "ITickableSound.h"
#include "SoundHandler.h"
#include "../settings/GameSettings.h"
#include "../../util/MathHelper.h"
#include "../../debug/Logger.h"

#include <algorithm>
#include <random>
#include <sstream>

SoundManager::SoundManager(SoundHandler *sndHandler, GameSettings *options, SoundEngine *engine) :
    sndHandler(sndHandler),
    options(options),
    engine(engine)
{}

void SoundManager::reloadSoundSystem() {
    this->unloadSoundSystem();
    this->loaded = true;
    Logger::log("Sound engine started");
}

void SoundManager::unloadSoundSystem() {
    if (this->loaded) {
        this->stopAllSounds();
        this->loaded = false;
    }
}

void SoundManager::stopAllSounds() {
    if (!this->loaded) return;

    std::lock_guard<std::mutex> lock(this->mutex);

    for (auto &[id, sound]: this->playingSounds) {
        // OpenAL stop by source name is tracked via playingSoundPoolEntries
        // engine cleanup handles actual AL sources
    }

    this->playingSounds.clear();
    this->invPlayingSounds.clear();
    this->delayedSounds.clear();
    this->tickableSounds.clear();
    this->categorySounds.clear();
    this->playingSoundPoolEntries.clear();
    this->playingSoundsStopTime.clear();
}

void SoundManager::updateAllSounds() {
    if (!this->loaded) return;

    std::lock_guard<std::mutex> lock(this->mutex);
    ++this->playTime;

    // update tickable sounds
    for (auto *tickable: this->tickableSounds) {
        tickable->update();

        if (tickable->isDonePlaying()) {
            this->stopSound(tickable);
        } else {
            auto it = this->invPlayingSounds.find(tickable);
            if (it != this->invPlayingSounds.end()) {
                auto *composite = this->sndHandler->getSound(tickable->getResourceLocation());
                if (composite) {
                    auto entryIt = this->playingSoundPoolEntries.find(tickable);
                    if (entryIt != this->playingSoundPoolEntries.end()) {
                        float vol = this->getNormalizedVolume(tickable, entryIt->second, composite->getSoundCategory());
                        float pitch = this->getNormalizedPitch(tickable, entryIt->second);
                        // update running OpenAL source volume/pitch if you track source IDs
                        // engine->updateSource(it->second, vol, pitch, ...);
                    }
                }
            }
        }
    }

    // remove stopped sounds
    for (auto it = this->playingSounds.begin(); it != this->playingSounds.end();) {
        const std::string &id = it->first;
        ISound *sound = it->second;

        auto stopIt = this->playingSoundsStopTime.find(id);
        if (stopIt != this->playingSoundsStopTime.end() && stopIt->second <= this->playTime) {
            int repeatDelay = sound->getRepeatDelay();
            if (sound->canRepeat() && repeatDelay > 0) {
                this->delayedSounds[sound] = this->playTime + repeatDelay;
            }

            this->playingSoundsStopTime.erase(id);
            this->playingSoundPoolEntries.erase(sound);
            this->invPlayingSounds.erase(sound);

            auto *composite = this->sndHandler->getSound(sound->getResourceLocation());
            if (composite) {
                auto &catVec = this->categorySounds[composite->getSoundCategory()];
                catVec.erase(std::remove(catVec.begin(), catVec.end(), id), catVec.end());
            }

            if (auto *tickable = dynamic_cast<ITickableSound *>(sound)) {
                this->tickableSounds.erase(
                        std::remove(this->tickableSounds.begin(), this->tickableSounds.end(), tickable),
                        this->tickableSounds.end());
            }

            it = this->playingSounds.erase(it);
        } else {
            ++it;
        }
    }

    // play delayed sounds
    for (auto it = this->delayedSounds.begin(); it != this->delayedSounds.end();) {
        if (this->playTime >= it->second) {
            ISound *sound = it->first;
            if (auto *tickable = dynamic_cast<ITickableSound *>(sound)) {
                tickable->update();
            }
            this->playSound(sound);
            it = this->delayedSounds.erase(it);
        } else {
            ++it;
        }
    }
}

void SoundManager::playSound(ISound *sound) {
    if (!this->loaded) return;

    float masterVol = this->getSoundCategoryVolume(SoundCategory::MASTER);
    if (masterVol <= 0.0f) {
        Logger::log("Skipped playing soundEvent: {}, master volume was zero", sound->getResourceLocation().toString());
        return;
    }

    auto *composite = this->sndHandler->getSound(sound->getResourceLocation());
    if (!composite) {
        Logger::warn("Unable to play unknown soundEvent: {}", sound->getResourceLocation().toString());
        return;
    }

    SoundPoolEntry entry = composite->cloneEntry();
    if (&entry == SoundHandler::missing_sound) {
        Logger::warn("Unable to play empty soundEvent: {}", composite->getSoundEventLocation().toString());
        return;
    }

    SoundCategory category = composite->getSoundCategory();
    float vol = this->getNormalizedVolume(sound, entry, category);
    float pitch = this->getNormalizedPitch(sound, entry);

    if (vol == 0.0f) {
        Logger::log("Skipped playing sound {}, volume was zero.", entry.location.toString());
        return;
    }

    std::string id = this->generateUUID();
    bool loops = sound->canRepeat() && sound->getRepeatDelay() == 0;

    if (entry.streamingSound) {
        this->engine->playSound(entry.location.getResourcePath(), vol, pitch);
    } else {
        this->engine->playSound3D(entry.location.getResourcePath(), sound->getXPosF(), sound->getYPosF(),
                                  sound->getZPosF(), vol, pitch);
    }

    Logger::log("Playing sound {} for event {} as channel {}", entry.location.toString(),
                composite->getSoundEventLocation().toString(), id);

    std::lock_guard<std::mutex> lock(this->mutex);
    this->playingSoundsStopTime[id] = this->playTime + 20;
    this->playingSounds[id] = sound;
    this->invPlayingSounds[sound] = id;
    this->playingSoundPoolEntries.emplace(sound, entry);

    if (category != SoundCategory::MASTER) {
        this->categorySounds[category].push_back(id);
    }

    if (auto *tickable = dynamic_cast<ITickableSound *>(sound)) {
        this->tickableSounds.push_back(tickable);
    }
}

void SoundManager::stopSound(ISound *sound) {
    if (!this->loaded) return;

    auto it = this->invPlayingSounds.find(sound);
    if (it != this->invPlayingSounds.end()) {
        // engine stops it on next cleanup when source finishes
        this->playingSounds.erase(it->second);
        this->playingSoundsStopTime.erase(it->second);
        this->invPlayingSounds.erase(it);
        this->playingSoundPoolEntries.erase(sound);
    }
}

void SoundManager::playDelayedSound(ISound *sound, int delay) {
    std::lock_guard<std::mutex> lock(this->mutex);
    this->delayedSounds[sound] = this->playTime + delay;
}

void SoundManager::pauseAllSounds() {
    // OpenAL: pause all active sources via engine
    Logger::log("Pausing all sounds");
}

void SoundManager::resumeAllSounds() {
    Logger::log("Resuming all sounds");
}

void SoundManager::setListener(EntityPlayer *player, float partialTicks) {
    if (!this->loaded || !player) return;

    float pitch = 0.0f; // player->prevRotationPitch + (player->rotationPitch - player->prevRotationPitch) * partialTicks;
    float yaw = 0.0f; // player->prevRotationYaw   + (player->rotationYaw   - player->prevRotationYaw)   * partialTicks;

    double x = 0.0; // player->prevPosX + (player->posX - player->prevPosX) * partialTicks;
    double y = 0.0; // player->prevPosY + (player->posY - player->prevPosY) * partialTicks + player->getEyeHeight();
    double z = 0.0; // player->prevPosZ + (player->posZ - player->prevPosZ) * partialTicks;

    float f2 = MathHelper::cos((yaw + 90.0f) * 0.017453292f);
    float f3 = MathHelper::sin((yaw + 90.0f) * 0.017453292f);
    float f4 = MathHelper::cos(-pitch * 0.017453292f);
    float f5 = MathHelper::sin(-pitch * 0.017453292f);
    float f6 = MathHelper::cos((-pitch + 90.0f) * 0.017453292f);
    float f7 = MathHelper::sin((-pitch + 90.0f) * 0.017453292f);

    float atX = f2 * f4;
    float atY = f5;
    float atZ = f3 * f4;
    float upX = f2 * f6;
    float upY = f7;
    float upZ = f3 * f6;

    this->engine->setListenerPosition3D((float) x, (float) y, (float) z);
    this->engine->setListenerOrientation(atX, atY, atZ, upX, upY, upZ);
}

void SoundManager::setSoundCategoryVolume(SoundCategory category, float volume) {
    if (!this->loaded) return;

    if (category == SoundCategory::MASTER) {
        // set master volume on all active AL sources
        return;
    }

    auto it = this->categorySounds.find(category);
    if (it == this->categorySounds.end()) return;

    for (const auto &id: it->second) {
        auto soundIt = this->playingSounds.find(id);
        if (soundIt == this->playingSounds.end()) continue;

        ISound *sound = soundIt->second;
        auto entryIt = this->playingSoundPoolEntries.find(sound);
        if (entryIt == this->playingSoundPoolEntries.end()) continue;

        float vol = this->getNormalizedVolume(sound, entryIt->second, category);
        if (vol <= 0.0f) {
            this->stopSound(sound);
        }
        // else: engine->setVolume(id, vol) if you add that to SoundEngine
    }
}

bool SoundManager::isSoundPlaying(ISound *sound) const {
    if (!this->loaded) return false;
    return this->invPlayingSounds.count(sound) > 0;
}

float SoundManager::getSoundCategoryVolume(SoundCategory category) const {
    if (category == SoundCategory::MASTER) return 1.0f;
    return this->options->getSoundLevel(category);
}

float SoundManager::getNormalizedVolume(ISound *sound, const SoundPoolEntry &entry, SoundCategory category) const {
    return (float) MathHelper::clamp_double((double) sound->getVolume() * entry.getVolume(), 0.0, 1.0) *
           this->getSoundCategoryVolume(category);
}

float SoundManager::getNormalizedPitch(ISound *sound, const SoundPoolEntry &entry) const {
    return (float) MathHelper::clamp_double((double) sound->getPitch() * entry.getPitch(), 0.5, 2.0);
}

std::string SoundManager::generateUUID() const {
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_int_distribution<uint32_t> dist(0, 0xFFFFFFFF);

    std::ostringstream ss;
    ss << std::hex << dist(rng) << "-" << (dist(rng) & 0xFFFF) << "-" << (dist(rng) & 0xFFFF) << "-"
       << (dist(rng) & 0xFFFF) << "-" << dist(rng) << (dist(rng) & 0xFFFF);
    return ss.str();
}
