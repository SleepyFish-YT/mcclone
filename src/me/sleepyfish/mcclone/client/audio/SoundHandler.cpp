//
// Created by SleepyFish on 16.09.2026.
// Project: mcclone
//

// not FULLY implemented yet.

#include "SoundHandler.h"

#include "SoundRegistry.h"
#include "SoundPoolEntry.h"
#include "SoundEventAccessorComposite.h"
#include "ISoundEventAccessor.h"
#include "SoundList.h"
#include "../resources/IResourceManager.h"
#include "../resources/IResource.h"
#include "../../util/ResourceLocation.h"
#include "SoundCategory.h"
#include "../settings/GameSettings.h"
// #include "EntityPlayer.h"
#include "ISound.h"
#include "SoundManager.h"

#include "../../debug/Logger.h"
#include "SoundEventAccessor.h"
#include "SoundEventAccessorDelegate.h"

#include <algorithm>
#include <random>
#include <stdexcept>

#include <nlohmann/json.hpp>

SoundPoolEntry *SoundHandler::missing_sound = new SoundPoolEntry(ResourceLocation("meta:missing_sound"), 0.0, 0.0, false);

SoundHandler::SoundHandler(IResourceManager *manager, GameSettings *gameSettingsIn, SoundEngine *engine) {
    this->mcResourceManager = manager;
    this->sndRegistry = new SoundRegistry();
    this->sndManager = new SoundManager(this, gameSettingsIn, engine);
}

void SoundHandler::onResourceManagerReload(IResourceManager &manager) {
    this->sndManager->reloadSoundSystem();
    this->sndRegistry->clearMap();

    for (const std::string &s: manager.getResourceDomains()) {
        try {
            for (IResource *iResource: manager.getAllResources(ResourceLocation(s, "sounds.json"))) {
                try {
                    auto map = this->getSoundMap(*iResource->getInputStream());
                    for (auto &[key, value]: map) {
                        this->loadSoundResource(ResourceLocation(s, key), value);
                    }
                } catch (const std::exception &e) {
                    Logger::warn("Invalid sounds.json: {}", e.what());
                }
            }
        } catch (const std::exception &) {}
    }
}

std::map<std::string, SoundList *> SoundHandler::getSoundMap(std::istream &stream) {
    nlohmann::json j = nlohmann::json::parse(stream);
    std::map<std::string, SoundList *> result;

    for (auto &[key, value]: j.items()) {
        auto *soundList = new SoundList();

        if (value.contains("replace") && value["replace"].is_boolean()) {
            soundList->setReplaceExisting(value["replace"].get<bool>());
        }

        if (value.contains("category") && value["category"].is_string()) {
            std::string cat = value["category"].get<std::string>();
            SoundCategory *cat_ptr = getSoundCategory(cat);
            if (cat_ptr != nullptr) {
                soundList->setSoundCategory(*cat_ptr);
            }
        }

        if (value.contains("sounds") && value["sounds"].is_array()) {
            for (auto &soundEntry: value["sounds"]) {
                SoundList::SoundEntry entry;

                if (soundEntry.is_string()) {
                    // shorthand: just a filename string
                    entry.setSoundEntryName(soundEntry.get<std::string>());
                    entry.setSoundEntryType(SoundList::SoundEntry::Type::FILE);
                } else if (soundEntry.is_object()) {
                    if (soundEntry.contains("name") && soundEntry["name"].is_string()) {
                        entry.setSoundEntryName(soundEntry["name"].get<std::string>());
                    }

                    if (soundEntry.contains("volume") && soundEntry["volume"].is_number()) {
                        entry.setSoundEntryVolume(soundEntry["volume"].get<float>());
                    }

                    if (soundEntry.contains("pitch") && soundEntry["pitch"].is_number()) {
                        entry.setSoundEntryPitch(soundEntry["pitch"].get<float>());
                    }

                    if (soundEntry.contains("weight") && soundEntry["weight"].is_number_integer()) {
                        entry.setSoundEntryWeight(soundEntry["weight"].get<int>());
                    }

                    if (soundEntry.contains("stream") && soundEntry["stream"].is_boolean()) {
                        entry.setStreaming(soundEntry["stream"].get<bool>());
                    }

                    if (soundEntry.contains("type") && soundEntry["type"].is_string()) {
                        std::string typeStr = soundEntry["type"].get<std::string>();
                        auto *type = SoundList::SoundEntry::getType(typeStr);
                        if (type != nullptr) {
                            entry.setSoundEntryType(*type);
                        }
                    } else {
                        entry.setSoundEntryType(SoundList::SoundEntry::Type::FILE);
                    }
                }

                soundList->getSoundList().push_back(entry);
            }
        }

        result[key] = soundList;
    }

    return result;
}

void SoundHandler::loadSoundResource(const ResourceLocation &location, SoundList *sounds) {
    SoundEventAccessorComposite *existing = this->sndRegistry->getObject(location);
    bool flag = (existing == nullptr);

    SoundEventAccessorComposite *composite;

    if (!flag && !sounds->canReplaceExisting()) {
        composite = existing;
    } else {
        if (!flag) {
            Logger::log("Replaced sound event location {}", location.toString());
        }

        composite = new SoundEventAccessorComposite(location, 1.0, 1.0, sounds->getSoundCategory());
        this->sndRegistry->registerSound(composite);
    }

    for (auto &entry: sounds->getSoundList()) {
        std::string s = entry.getSoundEntryName();
        ResourceLocation entry_location(s);
        std::string s1 = s.find(':') != std::string::npos ? entry_location.getResourceDomain() : location.getResourceDomain();
        ISoundEventAccessor<SoundPoolEntry> *pool_accessor = nullptr;

        switch (entry.getSoundEntryType()) {
            case SoundList::SoundEntry::Type::FILE: {
                ResourceLocation file_entry_location(s1, "sounds/" + entry_location.getResourcePath() + ".ogg");

                try {
                    auto *res = this->mcResourceManager->getResource(file_entry_location);
                    (void) res;
                } catch (const std::exception &e) {
                    Logger::warn("File {} does not exist, cannot add it to event {}", file_entry_location.toString(), location.toString());
                    continue;
                }

                pool_accessor = new SoundEventAccessor(
                        SoundPoolEntry(file_entry_location, entry.getSoundEntryPitch(), entry.getSoundEntryVolume(), entry.isStreaming()),
                        entry.getSoundEntryWeight()
                );
                break;
            }

            case SoundList::SoundEntry::Type::SOUND_EVENT: {
                ResourceLocation target(s1, entry.getSoundEntryName());
                pool_accessor = new SoundEventAccessorDelegate(this->sndRegistry, target, missing_sound);
                break;
            }

            default:
                throw std::runtime_error("IN YOU FACE");
        }

        composite->addSoundToEventPool(pool_accessor);
    }
}

SoundEventAccessorComposite *SoundHandler::getSound(const ResourceLocation &location) {
    return this->sndRegistry->getObject(location);
}

void SoundHandler::playSound(ISound *sound) {
    this->sndManager->playSound(sound);
}

void SoundHandler::playDelayedSound(ISound *sound, int delay) {
    this->sndManager->playDelayedSound(sound, delay);
}

void SoundHandler::setListener(EntityPlayer *player, float p_147691_2_) {
    this->sndManager->setListener(player, p_147691_2_);
}

void SoundHandler::pauseSounds() {
    this->sndManager->pauseAllSounds();
}

void SoundHandler::stopSounds() {
    this->sndManager->stopAllSounds();
}

void SoundHandler::unloadSounds() {
    this->sndManager->unloadSoundSystem();
}

void SoundHandler::update() {
    this->sndManager->updateAllSounds();
}

void SoundHandler::resumeSounds() {
    this->sndManager->resumeAllSounds();
}

void SoundHandler::setSoundLevel(SoundCategory *category, float volume) {
    this->sndManager->setSoundCategoryVolume(*category, volume);
}

void SoundHandler::stopSound(ISound *sound) {
    this->sndManager->stopSound(sound);
}

bool SoundHandler::isSoundPlaying(ISound *sound) {
    return this->sndManager->isSoundPlaying(sound);
}

SoundEventAccessorComposite *SoundHandler::getRandomSoundFromCategories(const std::vector<SoundCategory> &categories) {
    std::vector<SoundEventAccessorComposite *> list;

    for (const auto &location: this->sndRegistry->getKeys()) {
        auto *composite = this->sndRegistry->getObject(location);
        if (std::find(categories.begin(), categories.end(), composite->getSoundCategory()) != categories.end()) {
            list.push_back(composite);
        }
    }

    if (list.empty()) return nullptr;

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, static_cast<int>(list.size()) - 1);
    return list[dist(rng)];
}
