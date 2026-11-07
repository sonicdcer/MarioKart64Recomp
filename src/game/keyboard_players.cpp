// Keyboard players: up to four players on one keyboard, each with keys of their own.
//
// The frontend already keeps a keyboard profile per player ("Keyboard (Player N)") and reads the keyboard for any player whose profile binds a
// key, but its player assignment gives the keyboard to one player only, and a new multiplayer keyboard profile starts with nothing bound.
// This gives those profiles default keys that do not overlap, and with the Keyboard Players option on:
// - assigns the keyboard to every player when the game switches to a 2 to 4 player mode (from its menu, or from a mod through the base
//   exports), unless that many players are already assigned;
// - in Assign Players, makes each further key press add the next keyboard player (the frontend takes the keyboard once), so gamepads and
//   keyboard players mix: A on a gamepad for player 1, then any key for players 2, 3 and 4.
#include <array>

#include "SDL.h"

#include "zelda_config.h"
#include "recompinput/recompinput.h"
#include "recompinput/players.h"
#include "recompinput/profiles.h"

using recompinput::GameInput;
using recompinput::InputField;

namespace {
    constexpr size_t keyboard_players = 4;

    struct KeyLayout {
        SDL_Scancode up, left, down, right, a, b, z, r, l, start;
    };

    // Players 2 to 4. Each keeps off the others and off player 1's keys (the game's own keyboard profile: WASD, the arrows, IJKL, Space,
    // Left Shift, Q, E, R, F, Enter, Esc), so no key moves two karts.
    constexpr std::array<KeyLayout, keyboard_players - 1> other_players = {{
        // Player 2, the numpad: steer 8 4 5 6, A 0, B ., Z Enter, R +, L -, Start *.
        { SDL_SCANCODE_KP_8, SDL_SCANCODE_KP_4, SDL_SCANCODE_KP_5, SDL_SCANCODE_KP_6, SDL_SCANCODE_KP_0, SDL_SCANCODE_KP_PERIOD,
          SDL_SCANCODE_KP_ENTER, SDL_SCANCODE_KP_PLUS, SDL_SCANCODE_KP_MINUS, SDL_SCANCODE_KP_MULTIPLY },
        // Player 3: steer G V B N, A M, B comma, Z period, R slash, L T, Start Tab.
        { SDL_SCANCODE_G, SDL_SCANCODE_V, SDL_SCANCODE_B, SDL_SCANCODE_N, SDL_SCANCODE_M, SDL_SCANCODE_COMMA,
          SDL_SCANCODE_PERIOD, SDL_SCANCODE_SLASH, SDL_SCANCODE_T, SDL_SCANCODE_TAB },
        // Player 4, the block above the arrows: steer Home Delete End Page Down, A Right Ctrl, B Right Shift, Z Insert, R Page Up,
        // L Backspace, Start Pause.
        { SDL_SCANCODE_HOME, SDL_SCANCODE_DELETE, SDL_SCANCODE_END, SDL_SCANCODE_PAGEDOWN, SDL_SCANCODE_RCTRL, SDL_SCANCODE_RSHIFT,
          SDL_SCANCODE_INSERT, SDL_SCANCODE_PAGEUP, SDL_SCANCODE_BACKSPACE, SDL_SCANCODE_PAUSE },
    }};

    bool profile_is_unbound(int profile_index) {
        for (size_t input = 0; input < recompinput::num_game_inputs; input++) {
            for (size_t binding = 0; binding < recompinput::num_bindings_per_input; binding++) {
                if (!recompinput::profiles::get_input_binding(profile_index, static_cast<GameInput>(input), binding).is_empty()) {
                    return false;
                }
            }
        }
        return true;
    }

    void bind(int profile_index, GameInput input, SDL_Scancode key) {
        recompinput::profiles::set_input_binding(profile_index, input, 0, InputField::keyboard(key));
    }

    size_t players_in_assignment() {
        size_t count = 0;
        while (count < recompinput::max_num_players_supported && recompinput::players::get_player_is_assigned(static_cast<int>(count), true)) {
            count++;
        }
        return count;
    }

    // Runs when SDL queues an event, on the thread that pumps events, before the frontend's own handling of it
    // (playerassignment::process_sdl_event). The frontend adds a keyboard player for the first key press; a key press after that only
    // flashes that player's card, so here each new press adds the next keyboard player instead, up to the game's player count.
    int SDLCALL assignment_key_watch(void*, SDL_Event* event) {
        if (event->type != SDL_KEYDOWN || event->key.repeat != 0) {
            return 1;
        }
        if (!recompinput::playerassignment::is_active() || !zelda64::get_keyboard_players_enabled()) {
            return 1;
        }
        switch (event->key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE:   // the frontend stops assigning
            case SDL_SCANCODE_UP:       // the arrows move between the window's buttons
            case SDL_SCANCODE_DOWN:
            case SDL_SCANCODE_LEFT:
            case SDL_SCANCODE_RIGHT:
                return 1;
            default:
                break;
        }
        if (!recompinput::playerassignment::was_keyboard_assigned()) {
            return 1;   // the frontend adds the first keyboard player itself
        }
        if (players_in_assignment() < recompinput::players::get_max_number_of_players()) {
            recompinput::playerassignment::add_keyboard_player();
        }
        return 1;
    }
}

void zelda64::register_keyboard_player_defaults() {
#ifdef RECOMPINPUT_HAS_PLAYER_KEYBOARD_DEFAULTS
    // Player 1: the game's own keyboard defaults. Players 2 to 4: their layouts above (other inputs unbound).
    for (size_t input = 0; input < recompinput::num_game_inputs; input++) {
        GameInput game_input = static_cast<GameInput>(input);
        recompinput::set_default_mapping_for_player_keyboard(0, game_input,
            recompinput::get_default_mapping_for_input(recompinput::InputDevice::Keyboard, game_input));
    }
    for (size_t player = 1; player < keyboard_players; player++) {
        const KeyLayout &keys = other_players[player - 1];
        const std::pair<GameInput, SDL_Scancode> bindings[] = {
            { GameInput::Y_AXIS_POS, keys.up }, { GameInput::X_AXIS_NEG, keys.left }, { GameInput::Y_AXIS_NEG, keys.down },
            { GameInput::X_AXIS_POS, keys.right }, { GameInput::A, keys.a }, { GameInput::B, keys.b }, { GameInput::Z, keys.z },
            { GameInput::R, keys.r }, { GameInput::L, keys.l }, { GameInput::START, keys.start },
        };
        for (const auto &binding : bindings) {
            recompinput::set_default_mapping_for_player_keyboard(static_cast<int>(player), binding.first, { InputField::keyboard(binding.second) });
        }
    }
#endif
}

void zelda64::init_keyboard_players() {
    set_keyboard_player_defaults();
    SDL_AddEventWatch(assignment_key_watch, nullptr);
}

void zelda64::set_keyboard_player_defaults() {
    for (size_t player = 0; player < keyboard_players; player++) {
        int profile_index = recompinput::profiles::get_or_create_mp_keyboard_profile_index(static_cast<int>(player));
        if (profile_index < 0 || !profile_is_unbound(profile_index)) {
            continue;   // keys the player chose (or bound earlier) are kept
        }

        if (player == 0) {
            // Player 1: the game's own keyboard defaults, menu keys included.
            for (size_t input = 0; input < recompinput::num_game_inputs; input++) {
                const auto &fields = recompinput::get_default_mapping_for_input(recompinput::InputDevice::Keyboard, static_cast<GameInput>(input));
                for (size_t binding = 0; binding < fields.size() && binding < recompinput::num_bindings_per_input; binding++) {
                    recompinput::profiles::set_input_binding(profile_index, static_cast<GameInput>(input), binding, fields[binding]);
                }
            }
            continue;
        }

        const KeyLayout &keys = other_players[player - 1];
        bind(profile_index, GameInput::Y_AXIS_POS, keys.up);
        bind(profile_index, GameInput::X_AXIS_NEG, keys.left);
        bind(profile_index, GameInput::Y_AXIS_NEG, keys.down);
        bind(profile_index, GameInput::X_AXIS_POS, keys.right);
        bind(profile_index, GameInput::A, keys.a);
        bind(profile_index, GameInput::B, keys.b);
        bind(profile_index, GameInput::Z, keys.z);
        bind(profile_index, GameInput::R, keys.r);
        bind(profile_index, GameInput::L, keys.l);
        bind(profile_index, GameInput::START, keys.start);
    }
}

void zelda64::on_multiplayer_mode(size_t players) {
    set_keyboard_player_defaults();
    if (!zelda64::get_keyboard_players_enabled()) {
        return;   // each player picks a device in Assign Players, as before
    }

    // Already that many players (all on the keyboard, or a mix set up in Assign Players): keep them. The menu calls this each time the
    // player count is chosen.
    if (recompinput::players::get_number_of_assigned_players() == players) {
        return;
    }

    // The same steps as the Assign Players window, with the keyboard pressed for each player in turn: each gets their own
    // "Keyboard (Player N)" profile.
    recompinput::playerassignment::start();
    for (size_t player = 0; player < players; player++) {
        recompinput::playerassignment::add_keyboard_player();
    }
    recompinput::playerassignment::commit_player_assignment();
}
