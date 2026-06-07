//
// Created by SleepyFish on 05.06.2026.
// Project: mcclone
//

#include <GLFW/glfw3.h>
#include "KeyBinding.h"
#include "../../debug/Logger.h"

std::vector<KeyBinding*> KeyBinding::keybindArray {};
std::unordered_map<int, KeyBinding*> KeyBinding::hash {};
std::unordered_set<std::string> KeyBinding::keybindSet {};

KeyBinding::KeyBinding(const std::string& description, int keyCode, const std::string& category) {
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

void KeyBinding::unregisterAllBinds() {
    KeyBinding::keybindArray.clear();
    KeyBinding::hash.clear();
    KeyBinding::keybindSet.clear();
}

void KeyBinding::onTick(int keyCode) {
    if (keyCode != 0) {
        auto it = KeyBinding::hash.find(keyCode);
        if (it != KeyBinding::hash.end()) {
            ++it->second->pressTime;
        }
    }
}

void KeyBinding::setKeyBindState(int keyCode, bool pressed) {
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

size_t KeyBinding::getRegisteredCount() {
    return KeyBinding::keybindArray.size();
}

bool KeyBinding::isKeyDown() const {
    return this->pressed;
}

bool KeyBinding::isPressed() {
    if (this->pressTime == 0) {
        return false;
    }
    --this->pressTime;
    return true;
}

bool KeyBinding::isAny() {
    return this->pressed || this->isPressed();
}

void KeyBinding::unpressKey() {
    this->pressTime = 0;
    this->pressed = false;
}

std::string KeyBinding::getKeyDescription() const {
    return this->keyDescription;
}

std::string KeyBinding::getKeyCategory() const {
    return this->keyCategory;
}

std::string KeyBinding::getKeyName() const {
    const char* name = glfwGetKeyName(this->keyCode, 0);
    return name ? std::string(name) : "UNKNOWN";
}

int KeyBinding::getKeyCode() const {
    return this->keyCode;
}

int KeyBinding::getKeyCodeDefault() const {
    return this->keyCodeDefault;
}

void KeyBinding::setKeyCode(int keyCode) {
    this->keyCode = keyCode;
}

