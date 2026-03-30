#include "zelda_config.h"
#include "zelda_debug.h"
#include "recompui/recompui.h"
#include "recompui/config.h"
#include "recompinput/recompinput.h"
#include "zelda_sound.h"
#include "zelda_support.h"
#include "ultramodern/config.hpp"
#include "librecomp/files.hpp"
#include "librecomp/config.hpp"
#include "util/file.h"
#include <filesystem>
#include <fstream>
#include <iomanip>

#if defined(_WIN32)
#include <Shlobj.h>
#elif defined(__linux__)
#include <unistd.h>
#include <pwd.h>
#elif defined(__APPLE__)
#include "apple/rt64_apple.h"
#endif

bool get_general_config_bool_value(const std::string& option_id) {
    return std::get<bool>(recompui::config::get_general_config().get_option_value(option_id));
}

template <typename T = uint32_t>
T get_general_config_enum_value(const std::string& option_id) {
    return static_cast<T>(std::get<uint32_t>(recompui::config::get_general_config().get_option_value(option_id)));
}

template <typename T = uint32_t>
T get_general_config_number_value(const std::string& option_id) {
    return static_cast<T>(std::get<double>(recompui::config::get_general_config().get_option_value(option_id)));
}

template <typename T = uint32_t>
T get_graphics_config_enum_value(const std::string& option_id) {
    return static_cast<T>(std::get<uint32_t>(recompui::config::get_graphics_config().get_option_value(option_id)));
}

template <typename T = uint32_t>
T get_graphics_config_number_value(const std::string& option_id) {
    return static_cast<T>(std::get<double>(recompui::config::get_graphics_config().get_option_value(option_id)));
}

template <typename T = uint32_t>
T get_sound_config_enum_value(const std::string& option_id) {
    return static_cast<T>(std::get<uint32_t>(recompui::config::get_sound_config().get_option_value(option_id)));
}

template <typename T = uint32_t>
T get_sound_config_number_value(const std::string& option_id) {
    return static_cast<T>(std::get<double>(recompui::config::get_sound_config().get_option_value(option_id)));
}

static void add_general_options(recomp::config::Config &config) {
    using EnumOptionVector = const std::vector<recomp::config::ConfigOptionEnumOption>;

    // debug_mode
    config.add_bool_option(
        zelda64::configkeys::general::debug_mode,
        "Debug Mode",
        "Controls whether the debug menu is visible",
        false,
        true
    );

    #if 0

    // Film Grain
    static EnumOptionVector film_grain_options = {
        {zelda64::FilmGrainMode::On, "On"},
        {zelda64::FilmGrainMode::Off, "Off"},
    };

    config.add_enum_option(
        zelda64::configkeys::general::film_grain_mode,
        "Film Grain",
        "Adds a visual effect that simulates the random grain you see in old film or high-ISO camera footage. <recomp-color primary>On</recomp-color> is the default and matches the original game.",
        film_grain_options,
        zelda64::InvertYAxisMode::On
    );

    // Invert Y Axis
    static EnumOptionVector invert_y_axis_options = {
        {zelda64::InvertYAxisMode::On, "On"},
        {zelda64::InvertYAxisMode::Off, "Off"},
    };

    config.add_enum_option(
        zelda64::configkeys::general::invert_y_axis,
        "Invert Y Axis",
        "Inverts the Y axis aiming controls. <recomp-color primary>On</recomp-color> is the default and matches the original game.",
        invert_y_axis_options,
        zelda64::InvertYAxisMode::On
    );

    // Radio Communication Box
    static EnumOptionVector radio_comm_options = {
        {zelda64::RadioCommBoxMode::Expand, "Expand"},
        {zelda64::RadioCommBoxMode::Original, "Original"},
    };

    config.add_enum_option(
        zelda64::configkeys::general::radio_comm_mode,
        "Radio Communication Box",
        "Radio Communication Box Alignment.",
        radio_comm_options,
        zelda64::RadioCommBoxMode::Original
    );

    

    // autosave_mode
    static EnumOptionVector autosave_mode_options = {
        {zelda64::AutosaveMode::Off, "Off"},
        {zelda64::AutosaveMode::On, "On"},
    };
    config.add_enum_option(
        zelda64::configkeys::general::autosave_mode,
        "Autosaving",
        "Turns on autosaving and prevents owl saves from being deleted on load. Autosaves act as owl saves and take up the same slot as they do.\n"
        "\n"
        "Loading an autosave will place you in Clock Town or at the entrance of the current dungeon if you were in one.\n"
        "\n"
        "\n"
        "<recomp-color primary>If autosaving is disabled, existing autosaves will be deleted when loaded.</recomp-color>",
        autosave_mode_options,
        zelda64::AutosaveMode::On
    );

    // camera_invert_mode
    static EnumOptionVector camera_invert_mode_options = {
        {zelda64::CameraInvertMode::InvertNone, "None"},
        {zelda64::CameraInvertMode::InvertX, "Invert X"},
        {zelda64::CameraInvertMode::InvertY, "Invert Y"},
        {zelda64::CameraInvertMode::InvertBoth, "Invert Both"},
    };
    config.add_enum_option(
        zelda64::configkeys::general::camera_invert_mode,
        "Aiming Camera Mode",
        "Inverts the camera controls for first-person aiming. <recomp-color primary>Invert Y</recomp-color> is the default and matches the original game.",
        camera_invert_mode_options,
        zelda64::CameraInvertMode::InvertY
    );

    // analog_cam_mode
    static EnumOptionVector analog_cam_mode_options = {
        {zelda64::AnalogCamMode::Off, "Off"},
        {zelda64::AnalogCamMode::On, "On"},
    };
    config.add_enum_option(
        zelda64::configkeys::general::analog_cam_mode,
        "Analog Camera",
        "Enables an analog \"free\" camera similar to later entries in the series that's mapped to the right analog stick on the controller.\n"
        "\n"
        "When you move the right stick, the camera will enter free mode and stop centering behind Link. Press the <recomp-color primary>Target</recomp-color> button at any time to go back into the normal camera mode. The camera will also return to normal mode after a cutscene plays or when you move between areas.\n"
        "\n"
        "This option also enables right stick control while looking and aiming.",
        analog_cam_mode_options,
        zelda64::AnalogCamMode::Off
    );

    // analog_camera_invert_mode
    static EnumOptionVector analog_camera_invert_mode_options = {
        {zelda64::CameraInvertMode::InvertNone, "None"},
        {zelda64::CameraInvertMode::InvertX, "Invert X"},
        {zelda64::CameraInvertMode::InvertY, "Invert Y"},
        {zelda64::CameraInvertMode::InvertBoth, "Invert Both"},
    };
    config.add_enum_option(
        zelda64::configkeys::general::analog_camera_invert_mode,
        "Analog Camera Mode",
        "Inverts the camera controls for the analog camera if it's enabled. <recomp-color primary>None</recomp-color> is the default.",
        analog_camera_invert_mode_options,
        zelda64::CameraInvertMode::InvertNone
    );
    #endif
}

bool zelda64::get_debug_mode_enabled() {
    return get_general_config_bool_value(zelda64::configkeys::general::debug_mode);
}

#if 0
zelda64::FilmGrainMode zelda64::get_film_grain_mode() {
    return get_general_config_enum_value<zelda64::FilmGrainMode>(zelda64::configkeys::general::film_grain_mode);
}

zelda64::RadioCommBoxMode zelda64::get_radio_comm_mode() {
    return get_general_config_enum_value<zelda64::RadioCommBoxMode>(zelda64::configkeys::general::radio_comm_mode);
}

zelda64::InvertYAxisMode zelda64::get_invert_y_axis_mode() {
    return get_general_config_enum_value<zelda64::InvertYAxisMode>(zelda64::configkeys::general::invert_y_axis);
}
#endif

#if 0
zelda64::CameraInvertMode zelda64::get_camera_invert_mode() {
    return get_general_config_enum_value<zelda64::CameraInvertMode>(zelda64::configkeys::general::camera_invert_mode);
}

zelda64::AnalogCamMode zelda64::get_analog_cam_mode() {
    return get_general_config_enum_value<zelda64::AnalogCamMode>(zelda64::configkeys::general::analog_cam_mode);
}


zelda64::CameraInvertMode zelda64::get_analog_camera_invert_mode() {
    return get_general_config_enum_value<zelda64::CameraInvertMode>(zelda64::configkeys::general::analog_camera_invert_mode);
}
#endif

static void add_graphics_options(recomp::config::Config &config) {
    using EnumOptionVector = const std::vector<recomp::config::ConfigOptionEnumOption>;

    // Nothing to do here.
}

static void add_sound_options(recomp::config::Config &config) {
    using EnumOptionVector = const std::vector<recomp::config::ConfigOptionEnumOption>;

    // bgm_volume
    config.add_percent_number_option(
        zelda64::configkeys::sound::bgm_volume,
        "Background Music Volume",
        "Controls the overall volume of background music.",
        100.0f
    );

    // fanfare_volume
    config.add_percent_number_option(
        zelda64::configkeys::sound::fanfare_volume,
        "Fanfare Volume",
        "Controls the overall volume of voices.",
        100.0f
    );

    // sfx_volume
    config.add_percent_number_option(
        zelda64::configkeys::sound::sfx_volume,
        "Sound Effects Volume",
        "Controls the overall volume of sound effects.",
        100.0f
    );

    #if 0
    // low_health_beeps
    static EnumOptionVector low_health_beeps_mode_options = {
        {zelda64::LowHealthBeepsMode::Off, "Off"},
        {zelda64::LowHealthBeepsMode::On, "On"},
    };
    config.add_enum_option(
        zelda64::configkeys::sound::low_health_beeps,
        "Low Health Beeps",
        "Toggles whether or not the low-health beeping sound plays.",
        low_health_beeps_mode_options,
        zelda64::LowHealthBeepsMode::On
    );
    #endif
}

int zelda64::get_bgm_volume() {
    return get_sound_config_number_value<int>(zelda64::configkeys::sound::bgm_volume);
}

int zelda64::get_sfx_volume() {
    return get_sound_config_number_value<int>(zelda64::configkeys::sound::sfx_volume);
}

int zelda64::get_fanfare_volume() {
    return get_sound_config_number_value<int>(zelda64::configkeys::sound::fanfare_volume);
}

#if 0
bool zelda64::get_low_health_beeps_enabled() {
    return get_sound_config_enum_value<zelda64::LowHealthBeepsMode>(zelda64::configkeys::sound::low_health_beeps) == zelda64::LowHealthBeepsMode::On;
}
#endif

static void set_control_defaults() {
    using namespace recompinput;

    set_default_mapping_for_controller(
        GameInput::Z,
        { 
            InputField::controller_digital(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSHOULDER),
            InputField::controller_analog(SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERLEFT, true)
        }
    );

    set_default_mapping_for_controller(
        GameInput::R,
        { 
            InputField::controller_digital(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSHOULDER),
            InputField::controller_analog(SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERRIGHT, true)
        }
    );

 //   set_default_mapping_for_controller(
 //       GameInput::C_DOWN,
 //       { 
 //           InputField::controller_digital(SDL_CONTROLLER_BUTTON_EAST),
 //           InputField::controller_analog(SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERLEFT, true)
 //       }
 //   );
//
 //   set_default_mapping_for_controller(
 //       GameInput::C_LEFT,
 //       { 
 //           InputField::controller_digital(SDL_CONTROLLER_BUTTON_NORTH),
 //           InputField::controller_analog(SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERRIGHT, true)
 //       }
 //   );
//
 //   set_default_mapping_for_controller(
 //       GameInput::C_RIGHT,
 //       { 
 //           InputField::controller_analog(SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTX, true),
 //           InputField::controller_digital(SDL_CONTROLLER_BUTTON_EAST)
 //       }
 //   );
//
 //       set_default_mapping_for_controller(
 //       GameInput::Z,
 //       { 
 //           InputField::controller_digital(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
 //       }
 //   );
//
 //   set_default_mapping_for_controller(
 //       GameInput::R,
 //       { 
 //           InputField::controller_digital(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
 //       }
 //   );

    // L3 -> L | Unused in SF64 but can be used in mods
    set_default_mapping_for_controller(GameInput::L, { InputField::controller_digital(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSTICK) });
}

static void set_control_descriptions() {
    // recompinput::set_game_input_name(recompinput::GameInput::A, "Laser | (A)");
    // recompinput::set_game_input_name(recompinput::GameInput::B, "Smart Bomb | (B)");
    // recompinput::set_game_input_name(recompinput::GameInput::Z, "Tilt Left | (Z)");
    // recompinput::set_game_input_name(recompinput::GameInput::R, "Tilt Right | (R)");
    // recompinput::set_game_input_name(recompinput::GameInput::START, "Pause | (Start)");
    // recompinput::set_game_input_name(recompinput::GameInput::C_UP, "Camera | (C-UP)");
    // recompinput::set_game_input_name(recompinput::GameInput::C_LEFT, "Turbo | (C-LEFT)");
    // recompinput::set_game_input_name(recompinput::GameInput::C_DOWN, "Brake | (C-DOWN)");
    // recompinput::set_game_input_name(recompinput::GameInput::C_RIGHT, "ROB Message | (C-RIGHT)");
    // recompinput::set_game_input_name(recompinput::GameInput::L, "MOD | (L)");

    recompinput::set_game_input_description(recompinput::GameInput::Y_AXIS_NEG, "Used for reverse (holding B) and item launch direction.");
    recompinput::set_game_input_description(recompinput::GameInput::X_AXIS_NEG, "Used to Steer the kart.");
    recompinput::set_game_input_description(recompinput::GameInput::X_AXIS_POS, "Used to Steer the kart.");
    recompinput::set_game_input_description(recompinput::GameInput::A,          "Used to Accelerate.");
    recompinput::set_game_input_description(recompinput::GameInput::B,          "Used to Brake");
    recompinput::set_game_input_description(recompinput::GameInput::Z,          "Use item.");
    recompinput::set_game_input_description(recompinput::GameInput::R,          "Used to Jump and Drift (Hold)");
    recompinput::set_game_input_description(recompinput::GameInput::L,          "Unused. Mods may use it for additional features.");
    recompinput::set_game_input_description(recompinput::GameInput::START,      "Used for pausing.");
    recompinput::set_game_input_description(recompinput::GameInput::C_UP,       "Used to change the camera mode.");
    recompinput::set_game_input_description(recompinput::GameInput::C_DOWN,     "Use item.");
    recompinput::set_game_input_description(recompinput::GameInput::C_RIGHT,    "Used to toggle on-screen hud (Speedometer/Map/Etc).");
    recompinput::set_game_input_description(recompinput::GameInput::DPAD_UP,    "Used for in-game menu navigation.");
    recompinput::set_game_input_description(recompinput::GameInput::DPAD_DOWN,  "Used for in-game menu navigation.");
    recompinput::set_game_input_description(recompinput::GameInput::DPAD_LEFT,  "Used for in-game menu navigation.");
    recompinput::set_game_input_description(recompinput::GameInput::DPAD_RIGHT, "Used for in-game menu navigation.");
}

void zelda64::init_config() {
    std::filesystem::path recomp_dir = recompui::file::get_app_folder_path();

    if (!recomp_dir.empty()) {
        std::filesystem::create_directories(recomp_dir);
    }

    recompui::config::GeneralTabOptions general_options{};
    general_options.has_rumble_strength = false;
    general_options.has_gyro_sensitivity = false;
    general_options.has_mouse_sensitivity = false;

    auto &general_config = recompui::config::create_general_tab(general_options);
    add_general_options(general_config);

    set_control_defaults();
    set_control_descriptions();
    recompui::config::create_controls_tab();

    auto &graphics_config = recompui::config::create_graphics_tab();
    add_graphics_options(graphics_config);

    auto &sound_config = recompui::config::create_sound_tab();
    add_sound_options(sound_config);

    recompui::config::create_mods_tab();

    recompui::config::create_tab(
        zelda64::debug::tab_name,
        zelda64::debug::tab_id,
        [](recompui::ContextId context, recompui::Element* parent) {
            // TODO implement debug tab
        }
    );

    recompui::config::finalize();

    recompui::config::set_tab_visible(zelda64::debug::tab_id, get_debug_mode_enabled());
}
