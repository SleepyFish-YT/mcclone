//
// Created by SleepyFish on 06.06.2026.
// Project: mcclone
//

#ifndef MCCLONE_GAMESETTINGS_H
#define MCCLONE_GAMESETTINGS_H

#include "KeyBinding.h"

#include <vector>
#include <array>
#include <glfw/glfw3.h>
#include <nlohmann/json.hpp>
#include <filesystem>

/**
 * @author SleepyFish
 * @brief Class used for tracking game settings
 */
class GameSettings {

private:

    std::filesystem::path settingsFilePath;
    nlohmann::ordered_json settingsJson;

public:

    GameSettings(const GameSettings&) = delete;
    GameSettings& operator=(const GameSettings&) = delete;
    GameSettings(GameSettings&&) = delete;
    GameSettings& operator=(GameSettings&&) = delete;

    float mouseSensitivity;
    bool  invertMouse;

    int   limitFramerate;

    int   renderDistanceChunks;
    bool  enableVsync;
    bool  fboEnable;
    bool  useVbo;

    bool  fullScreen;
    bool  pauseOnLostFocus;
    bool  showDebugInfo;
    bool  heldItemTooltips;
    bool  advancedItemTooltips;

    int   thirdPersonView;
    int   fovSetting;
    float gammaSetting;
    float saturation;

    // Movement
    KeyBinding keyBindForward {"", 0, ""};
    KeyBinding keyBindLeft {"", 0, ""};
    KeyBinding keyBindBack {"", 0, ""};
    KeyBinding keyBindRight {"", 0, ""};
    KeyBinding keyBindJump {"", 0, ""};
    KeyBinding keyBindSneak {"", 0, ""};
    KeyBinding keyBindSprint {"", 0, ""};

    // Inventory
    KeyBinding keyBindInventory {"", 0, ""};
    KeyBinding keyBindHotbar1 {"", 0, ""};
    KeyBinding keyBindHotbar2 {"", 0, ""};
    KeyBinding keyBindHotbar3 {"", 0, ""};
    KeyBinding keyBindHotbar4 {"", 0, ""};
    KeyBinding keyBindHotbar5 {"", 0, ""};
    KeyBinding keyBindHotbar6 {"", 0, ""};
    KeyBinding keyBindHotbar7 {"", 0, ""};
    KeyBinding keyBindHotbar8 {"", 0, ""};
    KeyBinding keyBindHotbar9 {"", 0, ""};

    // Gameplay
    KeyBinding keyBindUseItem {"", 0, ""};
    KeyBinding keyBindDrop {"", 0, ""};
    KeyBinding keyBindAttack {"", 0, ""};
    KeyBinding keyBindPickItem {"", 0, ""};
    KeyBinding keyBindMouseBack {"", 0, ""};
    KeyBinding keyBindMouseForward {"", 0, ""};

    // Multiplayer
    KeyBinding keyBindChat {"", 0, ""};
    KeyBinding keyBindPlayerList {"", 0, ""};
    KeyBinding keyBindCommand {"", 0, ""};

    // Misc
    KeyBinding keyBindScreenshot {"", 0, ""};
    KeyBinding keyBindPerspective {"", 0, ""};
    KeyBinding keyBindSmoothCamera {"", 0, ""};
    KeyBinding keyBindHideGui {"", 0, ""};
    KeyBinding keyBindToggleDebugOverlay {"", 0, ""};
    KeyBinding keyBindFullscreen {"", 0, ""};
    KeyBinding keyBindExitGame {"", 0, ""};
    KeyBinding keyBindZoom {"", 0, ""};

    std::array<KeyBinding*, 9> keyBindHotbar;
    std::vector<KeyBinding*> keyBinds;

    GameSettings();
    GameSettings(const std::filesystem::path& settingsParentPath);

    nlohmann::ordered_json createDefaultSettings();

    void saveSettings();

    void loadSettings();

};


#endif //MCCLONE_GAMESETTINGS_H
