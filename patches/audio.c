#include "patches.h"
#include "sound.h"

void func_800C36C4(u8 arg0, u8 arg1, u8 arg2, u8 arg3);
void func_800C8F44(u8);

#if 1
RECOMP_PATCH void func_8028F970(void) {
    s32 i;

    if (D_8015F890) {
        return;
    }

    //! @todo increasing players past four would require increase this loop iterator.
    for (i = 0; i < 4; i++) {

        Player* player = &gPlayers[i];
        struct Controller* controller = &gControllers[i];

        if (!(player->type & PLAYER_HUMAN)) {
            continue;
        }
        if (player->type & PLAYER_KART_AI) {
            continue;
        }

        if (gActiveScreenMode != SCREEN_MODE_3P_4P_SPLITSCREEN) {
            if ((controller->buttonPressed & L_TRIG) && !(controller->button & R_TRIG)) {
                controller->buttonPressed &= 0xFFDF;

                D_800DC5A8++;
                if (D_800DC5A8 >= 3) {
                    D_800DC5A8 = 0;
                }
                // play_sound2(SOUND_ACTION_PING);
                // func_800029B0();
            }
        }
        if ((controller->buttonPressed & START_BUTTON) && (!(controller->button & R_TRIG)) &&
            (!(controller->button & L_TRIG))) {
            func_8028DF00();
            gIsGamePaused = (controller - gControllerOne) + 1;
            controller->buttonPressed = 0;
            func_800C9F90(1);
            D_80162DF0 = 1;
            if (gModeSelection == TIME_TRIALS) {
                if (gPlayerOne->type & (PLAYER_EXISTS | PLAYER_INVISIBLE_OR_BOMB)) {
                    func_80005AE8(gPlayerOne);
                }
                if (gPlayerTwo->type & (PLAYER_EXISTS | PLAYER_INVISIBLE_OR_BOMB)) {
                    func_80005AE8(gPlayerTwo);
                }
                if (gPlayerThree->type & (PLAYER_EXISTS | PLAYER_INVISIBLE_OR_BOMB)) {
                    func_80005AE8(gPlayerThree);
                }
            }
            return;
        }
    }

    if (gEnableDebugMode) {
        if (gModeSelection == BATTLE) {
            // do stuff?
        } else {
            if (gControllerOne->buttonPressed & U_JPAD) {
                gLapCountByPlayerId[0] = 2;
            }
            if (gControllerOne->buttonPressed & R_JPAD) {
                gLapCountByPlayerId[0] = 2;
                gLapCountByPlayerId[1] = 2;
            }
            if (gControllerOne->buttonPressed & D_JPAD) {
                gLapCountByPlayerId[0] = 2;
                gLapCountByPlayerId[1] = 2;
                gLapCountByPlayerId[2] = 2;
                gLapCountByPlayerId[3] = 2;
                gLapCountByPlayerId[4] = 2;
                gLapCountByPlayerId[5] = 2;
                gLapCountByPlayerId[6] = 2;
                gLapCountByPlayerId[7] = 2;
            }
        }
    }
}
#endif

#if 1
RECOMP_PATCH void func_800029B0(void) {
    // @recomp: Disable change in volume
    return;

    switch (D_800DC5A8) {
        case 0:
            // func_800C8F44(127);
            // recomp_set_bgm_volume_100();
            break;
        case 1:
            // func_800C8F44(75);
            // recomp_set_bgm_volume_59();
            break;
        case 2:
            // func_800C8F44(0);
            // recomp_set_bgm_volume_0();
            break;
    }
}
#endif

void audio_set_volume(void) {
    func_800C36C4(SEQ_PLAYER_LEVEL, 0, recomp_get_bgm_volume() * 127, 1);
    func_800C36C4(SEQ_PLAYER_ENV, 0, recomp_get_fanfare_volume() * 127, 1);
    func_800C36C4(SEQ_PLAYER_SFX, 0, recomp_get_sfx_volume() * 127, 1);
}
