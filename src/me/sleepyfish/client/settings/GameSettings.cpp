//
// Created by SleepyFish on 06.06.2026.
// Project: mcclone
//

#include "GameSettings.h"
#include "../../debug/Logger.h"

#include <fstream>

GameSettings::GameSettings() {
    this->settingsFilePath = "";
}

GameSettings::GameSettings(const std::filesystem::path& settingsParentPath) {
    this->settingsFilePath = (std::filesystem::path(settingsParentPath) / "options.json");

    this->mouseSensitivity = 0.5F;
    this->invertMouse = false;

    this->limitFramerate = 240;

    this->renderDistanceChunks = 12;
    this->enableVsync = true;
    this->fboEnable = true;
    this->useVbo = false;

    this->fullScreen = false;
    this->pauseOnLostFocus = false;
    this->showDebugInfo = false;
    this->heldItemTooltips = true;
    this->advancedItemTooltips = false;

    this->fovSetting = 70;
    this->thirdPersonView = 0;
    this->gammaSetting = 1.0f;
    this->saturation = 0.0f;

    // Keybinds
    {
        this->keyBindForward      = KeyBinding("key.forward", GLFW_KEY_W, "key.categories.movement");
        this->keyBindLeft         = KeyBinding("key.left", GLFW_KEY_A, "key.categories.movement");
        this->keyBindBack         = KeyBinding("key.back", GLFW_KEY_S, "key.categories.movement");
        this->keyBindRight        = KeyBinding("key.right", GLFW_KEY_D, "key.categories.movement");
        this->keyBindJump         = KeyBinding("key.jump", GLFW_KEY_SPACE, "key.categories.movement");
        this->keyBindSneak        = KeyBinding("key.sneak", GLFW_KEY_LEFT_SHIFT, "key.categories.movement");
        this->keyBindSprint       = KeyBinding("key.sprint", GLFW_KEY_LEFT_CONTROL, "key.categories.movement");

        this->keyBindInventory    = KeyBinding("key.inventory", GLFW_KEY_E  , "key.categories.inventory");
        this->keyBindHotbar1      = KeyBinding("key.hotbar.1", GLFW_KEY_1, "key.categories.inventory");
        this->keyBindHotbar2      = KeyBinding("key.hotbar.2", GLFW_KEY_2, "key.categories.inventory");
        this->keyBindHotbar3      = KeyBinding("key.hotbar.3", GLFW_KEY_3, "key.categories.inventory");
        this->keyBindHotbar4      = KeyBinding("key.hotbar.4", GLFW_KEY_4, "key.categories.inventory");
        this->keyBindHotbar5      = KeyBinding("key.hotbar.5", GLFW_KEY_5, "key.categories.inventory");
        this->keyBindHotbar6      = KeyBinding("key.hotbar.6", GLFW_KEY_6, "key.categories.inventory");
        this->keyBindHotbar7      = KeyBinding("key.hotbar.7", GLFW_KEY_7, "key.categories.inventory");
        this->keyBindHotbar8      = KeyBinding("key.hotbar.8", GLFW_KEY_8, "key.categories.inventory");
        this->keyBindHotbar9      = KeyBinding("key.hotbar.9", GLFW_KEY_9, "key.categories.inventory");

        this->keyBindUseItem      = KeyBinding("key.use", 1000 + GLFW_MOUSE_BUTTON_RIGHT, "key.categories.gameplay");
        this->keyBindDrop         = KeyBinding("key.drop", GLFW_KEY_Q, "key.categories.gameplay");
        this->keyBindAttack       = KeyBinding("key.attack", 1000 + GLFW_MOUSE_BUTTON_LEFT, "key.categories.gameplay");
        this->keyBindPickItem     = KeyBinding("key.pickItem", 1000 + GLFW_MOUSE_BUTTON_MIDDLE, "key.categories.gameplay");
        this->keyBindMouseBack    = KeyBinding("key.mouseBack", 1000 + GLFW_MOUSE_BUTTON_4, "key.categories.gameplay");
        this->keyBindMouseForward = KeyBinding("key.mouseForward", 1000 + GLFW_MOUSE_BUTTON_5, "key.categories.gameplay");

        this->keyBindChat         = KeyBinding("key.chat", GLFW_KEY_T, "key.categories.multiplayer");
        this->keyBindPlayerList   = KeyBinding("key.playerlist", GLFW_KEY_TAB, "key.categories.multiplayer");
        this->keyBindCommand      = KeyBinding("key.command", GLFW_KEY_APOSTROPHE, "key.categories.multiplayer");

        this->keyBindScreenshot   = KeyBinding("key.screenshot", GLFW_KEY_F2, "key.categories.misc");
        this->keyBindPerspective  = KeyBinding("key.togglePerspective", GLFW_KEY_F5, "key.categories.misc");
        this->keyBindSmoothCamera = KeyBinding("key.smoothCamera", 0, "key.categories.misc");
        this->keyBindFullscreen   = KeyBinding("key.fullscreen", GLFW_KEY_F11, "key.categories.misc");
        this->keyBindHideGui      = KeyBinding("key.hideGui", GLFW_KEY_F1, "key.categories.misc");
        this->keyBindToggleDebugOverlay = KeyBinding("key.toggleDebugOverlay", GLFW_KEY_F3, "key.categories.misc");
        this->keyBindExitGame     = KeyBinding("key.exitGame", GLFW_KEY_F12, "key.categories.misc");
        this->keyBindZoom         = KeyBinding("key.zoom", GLFW_KEY_C, "key.categories.misc");
    }

    this->keyBindHotbar = {
            &this->keyBindHotbar1,
            &this->keyBindHotbar2,
            &this->keyBindHotbar3,
            &this->keyBindHotbar4,
            &this->keyBindHotbar5,
            &this->keyBindHotbar6,
            &this->keyBindHotbar7,
            &this->keyBindHotbar8,
            &this->keyBindHotbar9
    };

    this->keyBinds = {};
    this->keyBinds.push_back(&this->keyBindForward);
    this->keyBinds.push_back(&this->keyBindLeft);
    this->keyBinds.push_back(&this->keyBindBack);
    this->keyBinds.push_back(&this->keyBindRight);
    this->keyBinds.push_back(&this->keyBindJump);
    this->keyBinds.push_back(&this->keyBindSneak);
    this->keyBinds.push_back(&this->keyBindSprint);
    this->keyBinds.push_back(&this->keyBindInventory);
    this->keyBinds.push_back(&this->keyBindUseItem);
    this->keyBinds.push_back(&this->keyBindDrop);
    this->keyBinds.push_back(&this->keyBindAttack);
    this->keyBinds.push_back(&this->keyBindPickItem);
    this->keyBinds.push_back(&this->keyBindMouseBack);
    this->keyBinds.push_back(&this->keyBindMouseForward);
    this->keyBinds.push_back(&this->keyBindChat);
    this->keyBinds.push_back(&this->keyBindPlayerList);
    this->keyBinds.push_back(&this->keyBindCommand);
    this->keyBinds.push_back(&this->keyBindScreenshot);
    this->keyBinds.push_back(&this->keyBindPerspective);
    this->keyBinds.push_back(&this->keyBindSmoothCamera);
    this->keyBinds.push_back(&this->keyBindFullscreen);
    this->keyBinds.push_back(&this->keyBindHideGui);
    this->keyBinds.push_back(&this->keyBindToggleDebugOverlay);
    this->keyBinds.push_back(&this->keyBindExitGame);
    this->keyBinds.push_back(&this->keyBindZoom);

    for (auto* kb : this->keyBinds) {
        KeyBinding::registerBinding(kb);
    }

    for (auto* kb : this->keyBindHotbar) {
        KeyBinding::registerBinding(kb);
    }

    if (KeyBinding::getRegisteredCount() != (this->keyBinds.size() + this->keyBindHotbar.size())) {
        Logger::error("Failed to register one or more keybinds! Expected {}, got {}",
                      this->keyBinds.size() + this->keyBindHotbar.size(),
                      KeyBinding::getRegisteredCount());
    }

    if (std::filesystem::exists(this->settingsFilePath)) {
        this->loadSettings();
    } else {
        this->settingsJson = this->createDefaultSettings();
        this->saveSettings();
    }
}

nlohmann::ordered_json GameSettings::createDefaultSettings() {
    nlohmann::ordered_json json = {
            {"mouseSensitivity", 0.5f},
            {"invertMouse", false},
            {"limitFramerate", 240},
            {"renderDistanceChunks", 12},
            {"enableVsync", true},
            {"fboEnable", true},
            {"useVbo", false},
            {"fullScreen", false},
            {"pauseOnLostFocus", false},
            {"showDebugInfo", false},
            {"heldItemTooltips", true},
            {"advancedItemTooltips", false},
            {"fovSetting", 70},
            {"thirdPersonView", 0},
            {"gammaSetting", 1.0f},
            {"saturation", 0.0f}
    };

    for (auto& keybind : this->keyBinds) {
        json[keybind->getKeyDescription()] = keybind->getKeyCode();
    }

    for (auto& keybind : this->keyBindHotbar) {
        json[keybind->getKeyDescription()] = keybind->getKeyCode();
    }

    return json;
}

void GameSettings::loadSettings() {
    std::ifstream file(this->settingsFilePath);
    if (!file.is_open()) {
        Logger::log("No settings file found, creating defaults...");
        this->createDefaultSettings();
        return;
    }

    file >> this->settingsJson;
    file.close();

    this->mouseSensitivity     = this->settingsJson.value("mouseSensitivity", 0.5f);
    this->invertMouse          = this->settingsJson.value("invertMouse", false);
    this->limitFramerate       = this->settingsJson.value("limitFramerate", 240);
    this->renderDistanceChunks = this->settingsJson.value("renderDistanceChunks", 12);
    this->enableVsync          = this->settingsJson.value("enableVsync", true);
    this->fboEnable            = this->settingsJson.value("fboEnable", true);
    this->useVbo               = this->settingsJson.value("useVbo", false);
    this->fullScreen           = this->settingsJson.value("fullScreen", false);
    this->pauseOnLostFocus     = this->settingsJson.value("pauseOnLostFocus", false);
    this->showDebugInfo        = this->settingsJson.value("showDebugInfo", false);
    this->heldItemTooltips     = this->settingsJson.value("heldItemTooltips", true);
    this->advancedItemTooltips = this->settingsJson.value("advancedItemTooltips", false);
    this->fovSetting           = this->settingsJson.value("fovSetting", 70);
    this->thirdPersonView      = this->settingsJson.value("thirdPersonView", 0);
    this->gammaSetting         = this->settingsJson.value("gammaSetting", 1.0f);
    this->saturation           = this->settingsJson.value("saturation", 0.0f);

    KeyBinding::resetKeyBindingArrayAndHash();
    KeyBinding::unregisterAllBinds();

    for (auto& keybind : this->keyBinds) {
        if (this->settingsJson.contains(keybind->getKeyDescription())) {
            keybind->setKeyCode(this->settingsJson[keybind->getKeyDescription()]);
        }
    }

    for (auto& keybind : this->keyBindHotbar) {
        if (this->settingsJson.contains(keybind->getKeyDescription())) {
            keybind->setKeyCode(this->settingsJson[keybind->getKeyDescription()]);
        }
    }

    for (auto* kb : this->keyBinds) {
        KeyBinding::registerBinding(kb);
    }

    for (auto* kb : this->keyBindHotbar) {
        KeyBinding::registerBinding(kb);
    }

    if (KeyBinding::getRegisteredCount() != (this->keyBinds.size() + this->keyBindHotbar.size())) {
        Logger::error("Failed to register one or more keybinds! Expected {}, got {}",
                      this->keyBinds.size() + this->keyBindHotbar.size(),
                      KeyBinding::getRegisteredCount());
    }

    Logger::log("Settings loaded from {}", this->settingsFilePath.string());
}

void GameSettings::saveSettings() {
    // update settingsJson
    {
        this->settingsJson["mouseSensitivity"]     = this->mouseSensitivity;
        this->settingsJson["invertMouse"]          = this->invertMouse;
        this->settingsJson["limitFramerate"]       = this->limitFramerate;
        this->settingsJson["renderDistanceChunks"] = this->renderDistanceChunks;
        this->settingsJson["enableVsync"]          = this->enableVsync;
        this->settingsJson["fboEnable"]            = this->fboEnable;
        this->settingsJson["useVbo"]               = this->useVbo;
        this->settingsJson["fullScreen"]           = this->fullScreen;
        this->settingsJson["pauseOnLostFocus"]     = this->pauseOnLostFocus;
        this->settingsJson["showDebugInfo"]        = this->showDebugInfo;
        this->settingsJson["heldItemTooltips"]     = this->heldItemTooltips;
        this->settingsJson["advancedItemTooltips"] = this->advancedItemTooltips;
        this->settingsJson["fovSetting"]           = this->fovSetting;
        this->settingsJson["thirdPersonView"]      = this->thirdPersonView;
        this->settingsJson["gammaSetting"]         = this->gammaSetting;
        this->settingsJson["saturation"]           = this->saturation;
    }

    std::ofstream file(this->settingsFilePath);
    {
        file << this->settingsJson.dump(4);
    }
    file.close();

    if (file.fail()) {
        Logger::log("Failed to save settings to {}", this->settingsFilePath.string());
        return;
    }

    if (!file.is_open()) {
        Logger::log("Settings saved to {}", this->settingsFilePath.string());
    }
}

