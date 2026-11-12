#include "patches.h"
#include "sound.h"
#include "sounds.h"

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

// Fade out racing music for 3P/4P results screen
RECOMP_PATCH void func_800C76C0(u8 playerId) {
    if (D_800E9EA4[playerId] != 0) {
        if (D_800E9EA4[playerId] < 0x2BC) {
            D_800E9EA4[playerId]++;
        }
        if (D_800E9EA4[playerId] == 2) {
            func_800C9018(playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFA, 0x28));
            func_800C9018(playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFF, 0x2C));
            func_800C9018(playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFA, 0x4C));
            func_800C5578(&D_800E9F7C[playerId].pos, D_800E9E94[playerId]);
            D_800E9E74[playerId] = 0;
            switch (gModeSelection) {
                case GRAND_PRIX:
                    D_800EA0EC[playerId] = 2;
                    func_800C9060(playerId, SOUND_ARG_LOAD(0x19, 0x00, 0xF1, 0x03));
                    if (D_800EA1C0 == 0) {
                        func_800C3448(0x100100FF);
                        func_800C3448(0x110100FF);
                        func_800C5278(5U);
                        if (gPlayers[playerId].currentRank == 0) {
                            func_800C97C4(playerId);
                            D_800EA0F0 = 2;
                            func_800C9A88(playerId);
                            func_800CA414(0x000DU, 0x0010U);
                        } else if (gPlayers[playerId].currentRank < 4) {
                            func_800C97C4(playerId);
                            D_800EA0F0 = 2;
                            func_800C9A88(playerId);
                            func_800CA414(0x000EU, 0x0010U);
                        } else {
                            func_800C3448(-0x3E9F9C00);
                            func_800CA414(0x000FU, 0x0018U);
                        }
                    } else {
                        D_800EA0EC[playerId] = 2;
                        func_800C9060(playerId, 0x1900F103U);
                        if (gPlayers[playerId].currentRank == 0) {
                            func_800C3448(0x100100FF);
                            func_800C3448(0x110100FF);
                            func_800C97C4(playerId);
                            D_800EA0F0 = 2;
                            func_800C9A88(playerId);
                            func_800CA414(0x000DU, 0x0010U);
                        } else if (gPlayers[playerId].currentRank < 4) {
                            if (D_800EA104 == 0) {
                                func_800C3448(0x100100FF);
                                func_800C3448(0x110100FF);
                            }
                            func_800C97C4(playerId);
                            D_800EA0F0 = 2;
                            func_800C9A88(playerId);
                            func_800CA414(0x000EU, 0x0010U);
                        } else if (D_800EA104 == 0) {
                            func_800C3448(0x100100FF);
                            func_800C3448(0x110100FF);
                            func_800C3448(-0x3E9F9C00);
                            func_800CA414(0x000FU, 0x0018U);
                        }
                        if ((D_800EA104 != 0) || (D_800EA0EC[playerId] != 1)) {
                            func_800C5278(5U);
                        }
                    }
                    break;
                case TIME_TRIALS:
                    func_800C3448(0x100100FF);
                    func_800C3448(0x110100FF);
                    func_800C97C4(playerId);
                    D_800EA0F0 = 2;
                    func_800C9A88(0U);
                    D_800EA0EC[playerId] = 2;
                    func_800C9060(playerId, 0x1900F103U);
                    if (D_801657E5 == 1) {
                        func_800CA414(0x000DU, 0x0010U);
                    } else if (D_8018ED90 == 1) {
                        func_800CA414(0x000EU, 0x0010U);
                    } else {
                        func_800C3448(0x01640010);
                    }
                    break;
                case VERSUS:
                    D_800EA0EC[playerId] = 2;
                    func_800C9060(playerId, 0x1900F103U);
                    switch (D_800EA1C0) {
                        case 1:
                            func_800C3448(0x100100FF);
                            func_800C3448(0x110100FF);
                            func_800C97C4(playerId);
                            D_800EA0F0 = 2;
                            func_800C9A88(playerId);
                            func_800CA414(0x000DU, 0x0017U);
                            break;
                        case 2:
                            if ((D_800EA104 == 0) && (D_800EA0EC[playerId] == 1)) {
                                func_800C3448(0x100100FF);
                                func_800C3448(0x110100FF);
#ifdef VERSION_EU
                                func_800C8EF8(0x000DU);
#else
                                func_800CA414(0x000DU, 0x0017U);
#endif
                                D_800EA104 = 1;
                            } else if ((D_800EA104 == 1) && (D_800EA0EC[playerId] == 1)) {
                                func_800C5278(5U);
#ifndef VERSION_EU
                                if (func_800C3508(1) != 0x000D) {
#endif
                                    D_800EA104 = 0;
                                    func_800CA414(0x000EU, 0x0017U);
                                }
                                D_800EA104 = 2;
                            }
                            break;
                        case 3:
                            if ((D_800EA104 == 0) && (D_800EA0EC[playerId] == 1)) {
                                func_800C3448(0x100100FF);
                                func_800C3448(0x110100FF);
                                func_800C8EF8(0x000DU);
                                D_800EA104 = 1;
                            } else if ((D_800EA104 == 1) && (D_800EA0EC[playerId] == 1)) {
                                if (func_800C3508(1) != 0x000D) {
                                    D_800EA104 = 0;
                                    func_800C8EF8(0x000EU);
                                }
                                D_800EA104 = 2;
                            } else if ((D_800EA104 == 2) && (D_800EA0EC[playerId] == 1)) {
                                func_800C5278(5U);
#ifndef VERSION_EU
                                if (func_800C3508(1) != 0x000E) {
#endif
                                    D_800EA104 = 0;
                                    func_800CA414(0x000EU, 0x0017U);
                                }
                                D_800EA104 = 3;
                            }
                            break;
                    }
                    break;
                case BATTLE:
                    switch (D_800EA1C0) {
                        case 1:
                            func_800C3448(0x100100FF);
                            func_800C3448(0x110100FF);
                            func_800C5278(5U);
                            func_800C9018(playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xF9, 0x26));
                            func_800C8EF8(0x0017U);
                            D_800EA0EC[playerId] = 2;
                            func_800C90F4(playerId, (gPlayers[gPlayerWinningIndex].characterId * 0x10) +
                                                        SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                            break;
                        case 2:
                            if ((D_800EA0EC[0] == 1) && (D_800EA0EC[1] == 1) && (D_800EA0EC[2] == 1)) {

                                // @recomp: Fade out racing music
                                func_800C3448(0x100100FF);
                                func_800C3448(0x110100FF);

                                func_800C5278(5U);
                                func_800C9018(playerId, SOUND_ARG_LOAD(0x01, 0x00, 0x80, 0x26));
                                func_800C8EF8(0x0017U);
                                D_800EA0EC[playerId] = 2;
                                func_800C90F4(playerId, (gPlayers[gPlayerWinningIndex].characterId * 0x10) +
                                                            SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                            }
                            break;
                        case 3:
                            if ((D_800EA0EC[0] == 1) && (D_800EA0EC[1] == 1) && (D_800EA0EC[2] == 1) &&
                                (D_800EA0EC[3] == 1)) {

                                // @recomp: Fade out racing music
                                func_800C3448(0x100100FF);
                                func_800C3448(0x110100FF);

                                func_800C5278(5U);
                                func_800C9018(playerId, SOUND_ARG_LOAD(0x01, 0x00, 0x80, 0x26));
                                func_800C8EF8(0x0017U);
                                D_800EA0EC[playerId] = 2;
                                func_800C90F4(playerId, (gPlayers[gPlayerWinningIndex].characterId * 0x10) +
                                                            SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                            }
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        if (D_800E9EA4[playerId] == 0x0000001E) {
            switch (gModeSelection) {
                case BATTLE:
                    break;
                case GRAND_PRIX:
                    if (gPlayers[playerId].currentRank == 0) {
                        D_800EA0EC[playerId] = 2;
                        func_800C90F4(playerId,
                                      (gPlayers[playerId].characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x07));
                    } else if (gPlayers[playerId].currentRank < 4) {
                        D_800EA0EC[playerId] = 2;
                        func_800C90F4(playerId,
                                      (gPlayers[playerId].characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                    } else {
                        D_800EA0EC[playerId] = 2;
                        func_800C90F4(playerId,
                                      (gPlayers[playerId].characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x03));
                    }
                    break;
                case VERSUS:
                    if (gPlayers[playerId].currentRank == 0) {
                        D_800EA0EC[playerId] = 2;
                        func_800C90F4(playerId,
                                      (gPlayers[playerId].characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                    }
                    break;
                case TIME_TRIALS:
                    if (D_801657E5 == 1) {
                        D_800EA0EC[playerId] = 2;
                        func_800C90F4(playerId,
                                      (gPlayers[playerId].characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x07));
                    } else if (D_8018ED90 == (u8) 1) {
                        D_800EA0EC[playerId] = 2;
                        func_800C90F4(playerId,
                                      (gPlayers[playerId].characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                    }
                    break;
            }
        }
        switch (gModeSelection) {
            case GRAND_PRIX:
                if (gPlayers[playerId].currentRank == 0) {
                    if (D_800E9EA4[playerId] >= 0x15F) {
                        if (D_800E9EA4[playerId] == 0x0000015F) {
                            func_800C9D0C(playerId);
                        }
                    } else {
                        D_800EA130[playerId] = (f32) D_800E9EA4[playerId] / 400.0f;
                    }
                } else if (gPlayers[playerId].currentRank < 4) {
                    if (D_800E9EA4[playerId] >= 0x15F) {
                        if (D_800E9EA4[playerId] == 0x0000015F) {
                            func_800C9D0C(playerId);
                        }
                    } else {
                        D_800EA130[playerId] = (f32) D_800E9EA4[playerId] / 400.0f;
                    }
                } else if (D_800E9EA4[playerId] >= 0x12D) {
                    if (D_800E9EA4[playerId] == 0x0000012D) {
                        func_800C97C4(playerId);
                    }
                } else {
                    D_800EA130[playerId] = (f32) D_800E9EA4[playerId] / 330.0f;
                }
                break;
            case VERSUS:
                if (D_800EA1C0 == (u8) 1) {
                    if (D_800E9EA4[playerId] >= 0x65) {
                        if (D_800E9EA4[playerId] == 0x00000065) {
                            func_800C9D0C(playerId);
                        }
                    } else {
                        D_800EA130[playerId] = (f32) D_800E9EA4[playerId] / 120.0f;
                    }
                } else if (D_800E9EA4[playerId] >= 0x65) {
                    if (D_800E9EA4[playerId] == 0x00000065) {
                        func_800C97C4(playerId);
                    }
                } else {
                    D_800EA130[playerId] = (f32) D_800E9EA4[playerId] / 120.0f;
                }
                /* fallthrough */
            case TIME_TRIALS:
                if (D_800E9EA4[playerId] >= 0x12D) {
                    if (D_800E9EA4[playerId] == 0x0000012D) {
                        func_800C9D0C(0U);
                    }
                } else {
                    D_800EA130[playerId] = (f32) D_800E9EA4[playerId] / 350.0f;
                }
                break;
            case BATTLE:
                if (D_800E9EA4[playerId] >= 0x65) {
                    if (D_800E9EA4[playerId] == 0x00000065) {
                        func_800C97C4(playerId);
                    }
                } else {
                    D_800EA130[playerId] = (f32) D_800E9EA4[playerId] / 120.0f;
                }
                break;
        }
    }
}
