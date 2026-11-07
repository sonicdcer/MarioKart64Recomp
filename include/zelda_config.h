#ifndef __ZELDA_CONFIG_H__
#define __ZELDA_CONFIG_H__

#include <filesystem>
#include <string_view>
#include "ultramodern/config.hpp"
#include "recomp_input.h"

namespace zelda64 {
    inline const std::u8string program_id = u8"MarioKart64Recompiled";
    inline const std::string program_name = "MarioKart 64: Recompiled";

    namespace configkeys {
        namespace general {
            inline const std::string debug_mode = "debug_mode";
            inline const std::string keyboard_players = "keyboard_players";
            #if 0
            inline const std::string autosave_mode = "autosave_mode";
            inline const std::string camera_invert_mode = "camera_invert_mode";
            inline const std::string analog_cam_mode = "analog_cam_mode";
            inline const std::string analog_camera_invert_mode = "analog_camera_invert_mode";
            #endif
        }

        namespace sound {
            inline const std::string bgm_volume = "bgm_volume";
            inline const std::string sfx_volume = "sfx_volume";
            inline const std::string fanfare_volume = "fanfare_volume";
            #if 0
            inline const std::string low_health_beeps = "low_health_beeps";
            #endif
        }

        namespace graphics {
        }
    }

    // TODO: Move loading configs to the runtime once we have a way to allow per-project customization.
    void init_config();
    
    bool get_debug_mode_enabled();
    bool get_keyboard_players_enabled();

    // Keyboard players (src/game/keyboard_players.cpp). Several players can share one keyboard, each with keys of their own.
    // At startup, after the config loads: default keys for the keyboard profiles, and the Assign Players key handling.
    void init_keyboard_players();
    // Before the config loads: each player's keys as the frontend's per-player defaults (new profiles, Reset to defaults), when it has them.
    void register_keyboard_player_defaults();
    // Gives every multiplayer keyboard profile (players 1 to 4) its default keys when it has none bound yet.
    void set_keyboard_player_defaults();
    // Called whenever the game switches to a 2 to 4 player mode: with the Keyboard Players option on, every player is assigned the keyboard.
    void on_multiplayer_mode(size_t players);

    enum class AutosaveMode {
        Off,
        On,
    };

    AutosaveMode get_autosave_mode();

    #if 0
    enum class FilmGrainMode {
        On,
        Off
    };
    
    FilmGrainMode get_film_grain_mode();

    enum class RadioCommBoxMode {
        Expand,
        Original
    };
    
    RadioCommBoxMode get_radio_comm_mode();
    
    enum class InvertYAxisMode {
        On,
        Off
    };
    
    InvertYAxisMode get_invert_y_axis_mode();
    #endif

    enum class CameraInvertMode {
        InvertNone,
        InvertX,
        InvertY,
        InvertBoth
    };
    
    CameraInvertMode get_camera_invert_mode();
    CameraInvertMode get_analog_camera_invert_mode();

    enum class AnalogCamMode {
        Off,
        On,
    };
    
    AnalogCamMode get_analog_cam_mode();

    #if 0
    enum class LowHealthBeepsMode {
        Off,
        On,
    };
    #endif
};

#endif
