//
// Created by SleepyFish on 06.06.2026.
// Project: mcclone
//

#ifndef MCCLONE_GAMESETTINGS_H
#define MCCLONE_GAMESETTINGS_H

#include "../audio/SoundCategory.h"

#include <filesystem>
#include <vector>
#include <array>

#include <nlohmann/json.hpp>

class KeyBinding;
class Minecraft;

/**
 * @author SleepyFish
 * @brief Class used for tracking game settings
 */
class GameSettings {

private:

    std::filesystem::path settingsFilePath{};

    ::nlohmann::ordered_json settingsJson{};

    Minecraft *mc{};

public:

    // disable copy and move
    GameSettings(const GameSettings&) = delete;
    GameSettings &operator=(const GameSettings&) = delete;
    GameSettings(GameSettings&&) = delete;
    GameSettings &operator=(GameSettings&&) = delete;

    float mouseSensitivity{};
    bool invertMouse{};
    uint16_t limitFramerate{};
    uint8_t clouds{};
    int renderDistanceChunks{};
    bool enableVsync{};
    bool fboEnable{};
    bool useVbo{};
    bool fullScreen{};
    bool pauseOnLostFocus{};
    bool showDebugInfo{};
    bool showDebugProfilerChart{};
    bool hideGUI{};
    bool heldItemTooltips{};
    bool advancedItemTooltips{};
    uint8_t thirdPersonView{};
    uint8_t fovSetting{};
    float gammaSetting{};
    float saturation{};
    int particleSetting{};
    int guiScale{};
    bool viewBobbing{};
    bool anaglyph{};
    bool fancyGraphics{};
    int ambientOcclusion{};
    std::vector<std::string> resourcePacks{};
    std::vector<std::string> incompatibleResourcePacks{};
    bool reducedDebugInfo{};
    bool forceUnicodeFont{};
    bool snooperEnabled{};
    int mipmapLevels{};
    bool useNativeTransport{};

    // Movement
    KeyBinding *keyBindForward{};
    KeyBinding *keyBindLeft{};
    KeyBinding *keyBindBack{};
    KeyBinding *keyBindRight{};
    KeyBinding *keyBindJump{};
    KeyBinding *keyBindSneak{};
    KeyBinding *keyBindSprint{};

    // Inventory
    KeyBinding *keyBindInventory{};
    KeyBinding *keyBindHotbar1{};
    KeyBinding *keyBindHotbar2{};
    KeyBinding *keyBindHotbar3{};
    KeyBinding *keyBindHotbar4{};
    KeyBinding *keyBindHotbar5{};
    KeyBinding *keyBindHotbar6{};
    KeyBinding *keyBindHotbar7{};
    KeyBinding *keyBindHotbar8{};
    KeyBinding *keyBindHotbar9{};

    // Gameplay
    KeyBinding *keyBindUseItem{};
    KeyBinding *keyBindDrop{};
    KeyBinding *keyBindAttack{};
    KeyBinding *keyBindPickItem{};
    KeyBinding *keyBindMouseBack{};
    KeyBinding *keyBindMouseForward{};

    // Multiplayer
    KeyBinding *keyBindChat{};
    KeyBinding *keyBindPlayerList{};
    KeyBinding *keyBindCommand{};

    // Misc
    KeyBinding *keyBindScreenshot{};
    KeyBinding *keyBindPerspective{};
    KeyBinding *keyBindSmoothCamera{};
    KeyBinding *keyBindHideGui{};
    KeyBinding *keyBindToggleDebugOverlay{};
    KeyBinding *keyBindFullscreen{};
    KeyBinding *keyBindExitGame{};
    KeyBinding *keyBindZoom{};
    KeyBinding *keyFreelook{};

    std::array<KeyBinding *, 9> keyBindHotbar{};
    std::vector<KeyBinding *> keyBinds{};

    GameSettings();

    explicit GameSettings(Minecraft &mc, const std::filesystem::path &settingsParentPath);

    ::nlohmann::ordered_json createDefaultSettings();

    void saveSettings();

    void loadSettings();

    float getSoundLevel(SoundCategory &category) const;

    uint8_t shouldRenderClouds() const noexcept;

    void sendSettingsToServer();

};


#endif //MCCLONE_GAMESETTINGS_H
