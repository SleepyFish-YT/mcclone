//
// Created by SleepyFish on 06.06.2026.
// Project: mcclone
//

#ifndef MCCLONE_KEYBINDING_H
#define MCCLONE_KEYBINDING_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

/**
 * @author SleepyFish
 * @brief Class used for tracking keybindings
 */
class KeyBinding {

private:

    static std::vector<KeyBinding*> keybindArray;

    static std::unordered_map<uint16_t, KeyBinding*> hash;

    static std::unordered_set<std::string> keybindSet;

    std::string keyDescription;

    std::string keyCategory;

    uint16_t keyCode;

    uint16_t keyCodeDefault;

    bool pressed;

    uint16_t pressTime;

    void unpressKey() noexcept;

public:

    KeyBinding() noexcept : keyCode(0), keyCodeDefault(0), pressed(false), pressTime(0) {}

    KeyBinding(const std::string& description, uint16_t keyCode, const std::string& category) noexcept;

    // disable const copy
    KeyBinding(const KeyBinding&) = delete;

    static void registerBinding(KeyBinding* binding);

    static void unregisterAllBinds() noexcept;

    static void onTick(uint16_t keyCode);

    static void setKeyBindState(uint16_t keyCode, bool pressed);

    static void unPressAllKeys();

    static void resetKeyBindingArrayAndHash();

    static const std::unordered_set<std::string>& getKeybinds();

    static size_t getRegisteredCount() noexcept;



    bool isKeyDown() const noexcept;

    bool isPressed() noexcept;

    bool isAny() noexcept;

    std::string getKeyDescription() const noexcept;

    std::string getKeyCategory() const noexcept;

    std::string getKeyName() const;

    uint16_t getKeyCode() const noexcept;

    uint16_t getKeyCodeDefault() const noexcept;

    void setKeyCode(uint16_t keyCode) noexcept;

};


#endif //MCCLONE_KEYBINDING_H