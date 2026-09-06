//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#include "KeyBinding.h"

#include "../../debug/Logger.h"

#include <GLFW/glfw3.h>

std::vector<KeyBinding*> KeyBinding::keybindArray {};
std::unordered_map<uint16_t, KeyBinding*> KeyBinding::hash {};
std::unordered_set<std::string> KeyBinding::keybindSet {};

KeyBinding::KeyBinding(const std::string& description, uint16_t keyCode, const std::string& category) noexcept {
    this->keyDescription = description;
    this->keyCode = keyCode;
    this->keyCodeDefault = keyCode;
    this->keyCategory = category;
    this->pressed = false;
    this->pressTime = 0;
}

void KeyBinding::registerBinding(KeyBinding* binding) {
    KeyBinding::keybindArray.push_back(binding);
    KeyBinding::hash[binding->keyCode] = binding;
    KeyBinding::keybindSet.insert(binding->keyCategory);
}

void KeyBinding::unregisterAllBinds() noexcept {
    KeyBinding::keybindArray.clear();
    KeyBinding::hash.clear();
    KeyBinding::keybindSet.clear();
}

void KeyBinding::onTick(uint16_t keyCode) {
    if (keyCode != 0) {
        auto it = KeyBinding::hash.find(keyCode);
        if (it != KeyBinding::hash.end()) {
            ++it->second->pressTime;
        }
    }
}

void KeyBinding::setKeyBindState(uint16_t keyCode, bool pressed) {
    if (keyCode != 0) {
        auto it = KeyBinding::hash.find(keyCode);
        if (it != KeyBinding::hash.end()) {
            it->second->pressed = pressed;
        }
    }
}

void KeyBinding::unPressAllKeys() {
    for (KeyBinding* keybinding : KeyBinding::keybindArray) {
        keybinding->unpressKey();
    }
}

void KeyBinding::resetKeyBindingArrayAndHash() {
    KeyBinding::hash.clear();

    for (KeyBinding* keybinding : KeyBinding::keybindArray) {
        KeyBinding::hash[keybinding->keyCode] = keybinding;
    }
}

const std::unordered_set<std::string>& KeyBinding::getKeybinds() {
    return KeyBinding::keybindSet;
}

size_t KeyBinding::getRegisteredCount() noexcept {
    return KeyBinding::keybindArray.size();
}

bool KeyBinding::isKeyDown() const noexcept {
    return this->pressed;
}

bool KeyBinding::isPressed() noexcept {
    if (this->pressTime == 0) {
        return false;
    }

    --this->pressTime;
    return true;
}

bool KeyBinding::isAny() noexcept {
    return this->pressed || this->isPressed();
}

void KeyBinding::unpressKey() noexcept {
    this->pressTime = 0;
    this->pressed = false;
}

std::string KeyBinding::getKeyDescription() const noexcept {
    return this->keyDescription;
}

std::string KeyBinding::getKeyCategory() const noexcept {
    return this->keyCategory;
}

std::string KeyBinding::getKeyName() const {
    const char* name = ::glfwGetKeyName(this->keyCode, 0);
    return name ? std::string(name) : "UNKNOWN";
}

uint16_t KeyBinding::getKeyCode() const noexcept {
    return this->keyCode;
}

uint16_t KeyBinding::getKeyCodeDefault() const noexcept {
    return this->keyCodeDefault;
}

void KeyBinding::setKeyCode(uint16_t keyCode) noexcept {
    this->keyCode = keyCode;
}
