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
    static std::unordered_map<int, KeyBinding*> hash;
    static std::unordered_set<std::string> keybindSet;

    std::string keyDescription;
    std::string keyCategory;
    int keyCode;
    int keyCodeDefault;
    bool pressed;
    uint16_t pressTime;

public:

    KeyBinding() : keyCode(0), keyCodeDefault(0), pressed(false), pressTime(0) {}
    KeyBinding(const std::string& description, int keyCode, const std::string& category);

    static void registerBinding(KeyBinding* binding);
    static void unregisterAllBinds();
    static void onTick(int keyCode);
    static void setKeyBindState(int keyCode, bool pressed);
    static void unPressAllKeys();
    static void resetKeyBindingArrayAndHash();
    static const std::unordered_set<std::string>& getKeybinds();
    static size_t getRegisteredCount();

    bool isKeyDown() const;
    bool isPressed();
    bool isAny();
    std::string getKeyDescription() const;
    std::string getKeyCategory() const;
    std::string getKeyName() const;
    int getKeyCode() const;
    int getKeyCodeDefault() const;
    void setKeyCode(int keyCode);

private:

    void unpressKey();

};


#endif //MCCLONE_KEYBINDING_H