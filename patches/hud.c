#include "patches.h"

// HUD
#if 1 // lap counter, Timer
RECOMP_PATCH void draw_simplified_lap_count(s32 playerId) {
    gEXSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, 0, 0, 0,
                  SCREEN_HEIGHT); // @recomp
    gEXSetRectAlign(gDisplayListHead++, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_LEFT, 0, 0, 0, 0);
    draw_hud_2d_texture_32x8((s32) playerHUD[playerId].lapX, playerHUD[playerId].lapY + 3, (u8*) 0x000000000d00a958);
    draw_hud_2d_texture_32x16(playerHUD[playerId].lapX + 0x1C, (s32) playerHUD[playerId].lapY,
                              (u8*) gHudLapTextures[playerHUD[playerId].alsoLapCount]);
    gEXSetRectAlign(gDisplayListHead++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0);
}

RECOMP_PATCH void func_8004EB38(s32 playerId) {
    hud_player* temp_s0;

    gEXSetRectAlign(gDisplayListHead++, G_EX_ORIGIN_RIGHT, G_EX_ORIGIN_RIGHT, -(SCREEN_WIDTH) * 4, 0,
                    -(SCREEN_WIDTH) * 4, 0); // @recomp

    temp_s0 = &playerHUD[playerId];

    if ((u8) temp_s0->unk_7B != 0) {
        func_8004C9D8(temp_s0->lap1CompletionTimeX - 0x13, temp_s0->timerY + 8, 0x00000080, (u8*) 0x000000000d00c158,
                      0x00000020, 0x00000010, 0x00000020, 0x00000010);
        func_8004F950((s32) temp_s0->lap1CompletionTimeX, (s32) temp_s0->timerY, 0x00000080, (s32) temp_s0->someTimer);
    }

    if ((u8) temp_s0->unk_7C != 0) {
        func_8004C9D8(temp_s0->lap2CompletionTimeX - 0x13, temp_s0->timerY + 8, 0x00000050, (u8*) 0x000000000d00c158,
                      0x00000020, 0x00000010, 0x00000020, 0x00000010);
        func_8004F950((s32) temp_s0->lap2CompletionTimeX, (s32) temp_s0->timerY, 0x00000050, (s32) temp_s0->someTimer);
    }

    gEXSetRectAlign(gDisplayListHead++, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_LEFT, 0, 0, 0, 0); // @recomp

    if ((u8) temp_s0->unk_7E != 0) {
        func_8004C9D8((s32) temp_s0->lapAfterImage1X, temp_s0->lapY + 3, 0x00000080, (u8*) 0x000000000d00a958,
                      0x00000020, 8, 0x00000020, 8);
        func_8004C9D8(temp_s0->lapAfterImage1X + 0x1C, (s32) temp_s0->lapY, 0x00000080,
                      (u8*) gHudLapTextures[temp_s0->alsoLapCount], 0x00000020, 0x00000010, 0x00000020, 0x00000010);
    }

    if ((u8) temp_s0->unk_7F != 0) {
        func_8004C9D8((s32) temp_s0->lapAfterImage2X, temp_s0->lapY + 3, 0x00000050, (u8*) 0x000000000d00a958,
                      0x00000020, 8, 0x00000020, 8);
        func_8004C9D8(temp_s0->lapAfterImage2X + 0x1C, (s32) temp_s0->lapY, 0x00000050,
                      (u8*) gHudLapTextures[temp_s0->alsoLapCount], 0x00000020, 0x00000010, 0x00000020, 0x00000010);
    }

    gEXSetRectAlign(gDisplayListHead++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0); // @recomp
}
#endif

// HUD On Screen Timer
#if 1
RECOMP_PATCH void render_hud_timer(s32 playerId) {
    s32 var_s0;
    gEXSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, 0, 0, 0,
                  SCREEN_HEIGHT); // @recomp
    gEXSetRectAlign(gDisplayListHead++, G_EX_ORIGIN_RIGHT, G_EX_ORIGIN_RIGHT, -(SCREEN_WIDTH) * 4, 0,
                    -(SCREEN_WIDTH) * 4, 0); // @recomp
    if ((gModeSelection != 2) && (gModeSelection != 3)) {
        if (D_8018D320 == playerHUD[playerId].lapCount) {
            if (D_8015F890 == 0) {
                for (var_s0 = 0; var_s0 < 3; var_s0++) {
                    if (D_80165658[var_s0] == 0) {
                        print_timer(playerHUD[playerId].lapCompletionTimeXs[var_s0],
                                    playerHUD[playerId].timerY + (var_s0 * 0x10),
                                    playerHUD[playerId].lapDurations[var_s0]);
                    } else {
                        print_timer_rainbow(playerHUD[playerId].lapCompletionTimeXs[var_s0],
                                            playerHUD[playerId].timerY + (var_s0 * 0x10),
                                            playerHUD[playerId].lapDurations[var_s0]);
                    }
                }
                draw_hud_2d_texture_32x16(playerHUD[playerId].totalTimeX - 0x13, playerHUD[playerId].timerY + 0x38,
                                          (u8*) 0xd00bd58);
                if (D_801657E5 != 0) {
                    print_timer_rainbow(playerHUD[playerId].totalTimeX, playerHUD[playerId].timerY + 0x30,
                                        playerHUD[playerId].someTimer);
                } else {
                    print_timer(playerHUD[playerId].totalTimeX, playerHUD[playerId].timerY + 0x30,
                                playerHUD[playerId].someTimer);
                }
            }
        } else {
            if (playerHUD[playerId].blinkTimer == 0) {
                draw_hud_2d_texture_32x16(playerHUD[playerId].timerX - 0x13, playerHUD[playerId].timerY + 8,
                                          (u8*) 0xd00c158);
                print_timer(playerHUD[playerId].timerX, playerHUD[playerId].timerY, playerHUD[playerId].someTimer);
            } else {
                draw_hud_2d_texture_32x16(playerHUD[playerId].timerX - 0x13, playerHUD[playerId].timerY + 8,
                                          (u8*) 0xd00ad58);
                if (D_801657E3 != 0) {
                    print_timer_rainbow(playerHUD[playerId].timerX, playerHUD[playerId].timerY,
                                        playerHUD[playerId].someTimer1);
                } else if (playerHUD[playerId].blinkState == 0) {
                    print_timer(playerHUD[playerId].timerX, playerHUD[playerId].timerY, playerHUD[playerId].someTimer1);
                }
            }
        }
    }
    gEXSetRectAlign(gDisplayListHead++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0); // @recomp
}
#endif

// Speedometer
#if 1
RECOMP_PATCH void func_8004ED40(s32 arg0) {
    gEXSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, 0, 0, 0,
                  SCREEN_HEIGHT); // @recomp
    gEXSetRectAlign(gDisplayListHead++, G_EX_ORIGIN_RIGHT, G_EX_ORIGIN_RIGHT, -(SCREEN_WIDTH) * 4, 0,
                    -(SCREEN_WIDTH) * 4, 0); // @recomp
    gEXSetViewportAlign(gDisplayListHead++, G_EX_ORIGIN_RIGHT, -(SCREEN_WIDTH) * 4, 0);
    gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL((void*) 0x802B8880));

    func_8004A2F4(playerHUD[arg0].speedometerX, playerHUD[arg0].speedometerY, 0U, 1.0f, D_8018D300, D_8018D308,
                  D_8018D310, 0xFF, (u8*) 0xd009958, (Vtx*) 0x0D0064B0, 64, 96, 64, 48);
    func_8004A258(D_8018CFEC, D_8018CFF4, D_8016579E, 1.0f, (u8*) 0xd00a558, (Vtx*) 0x0D005FF0, 0x40, 0x20, 0x40, 0x20);
    gEXSetViewportAlign(gDisplayListHead++, G_EX_ORIGIN_NONE, 0, 0);
    gEXSetRectAlign(gDisplayListHead++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0);
}
#endif

// Theboy181: Temp fix for HUD in MP (until proper widescreen is made for MP modes)
#if 1
RECOMP_PATCH void func_800591B4(void) {
    if ((gHUDDisable == 0) && (D_800DC5B8 != 0)) {
        func_80057C60();
        gSPDisplayList(gDisplayListHead++, 0x0d0076f8); // SETUP_DL

        if (gIsHUDVisible != 0) {
            if (D_801657D8 == 0) {
                if (D_801657F0 != false) {
                    func_800514BC();
                }

                if ((!gDemoMode) && (D_801657E8 != false)) {
                    if (D_80165800[0] != 0) {
                        if (gScreenModeSelection < SCREEN_MODE_3P_4P_SPLITSCREEN) {
                            gEXSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT,
                                          0, 0, 0, SCREEN_HEIGHT); // @recomp

                            gEXSetRectAlign(gDisplayListHead++, G_EX_ORIGIN_RIGHT, G_EX_ORIGIN_RIGHT,
                                            -(SCREEN_WIDTH) * 4, 0, -(SCREEN_WIDTH) * 4, 0); // @recomp
                        }

                        func_8004EE54(0); // map
                        if (gModeSelection != BATTLE) {
                            render_mini_map_finish_line(0);
                        }

                        if (gScreenModeSelection < SCREEN_MODE_3P_4P_SPLITSCREEN) {
                            gEXSetViewportAlign(gDisplayListHead++, G_EX_ORIGIN_RIGHT, -(SCREEN_WIDTH) * 4, 0);
                            gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL((void*) 0x802B8880));
                        }

                        func_8004F3E4(0); // racers on the map, including the player.

                        if (gScreenModeSelection < SCREEN_MODE_3P_4P_SPLITSCREEN) {
                            gEXSetViewportAlign(gDisplayListHead++, G_EX_ORIGIN_NONE, 0, 0);
                            gEXSetRectAlign(gDisplayListHead++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0,
                                            0); // @recomp
                        }
                    }

                    if ((gScreenModeSelection == SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL) && (D_80165800[1] != 0)) {
                        func_8004EE54(1); // map splitscreen
                        if (gModeSelection != BATTLE) {
                            render_mini_map_finish_line(1);
                        }
                        func_8004F3E4(1); // player splitscreen
                    }
                }
            }

            if (gScreenModeSelection < SCREEN_MODE_3P_4P_SPLITSCREEN) {
                gEXSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, 0, 0, 0,
                              SCREEN_HEIGHT); // @recomp

                if (D_801657E2 == 0) {
                    gEXSetViewportAlign(gDisplayListHead++, G_EX_ORIGIN_LEFT, 0, 0);
                } else {
                    gEXSetViewportAlign(gDisplayListHead++, G_EX_ORIGIN_NONE, 0, 0);
                }

                gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL((void*) 0x802B8880));
            }

            if ((D_801657E4 != 2) && (gModeSelection == GRAND_PRIX) && (D_8018D2BC != 0)) {
                func_80050320(); // racers ranking
            }

            func_800590D4(); // Player Ranking position (ex. 1st, 2nd, etc)

            if (gScreenModeSelection < SCREEN_MODE_3P_4P_SPLITSCREEN) {
                gEXSetViewportAlign(gDisplayListHead++, G_EX_ORIGIN_NONE, 0, 0);
                gEXSetRectAlign(gDisplayListHead++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0); // @recomp
            }
        }

        func_8005902C();
        func_80057DD0();
        func_80057CE4();
    }

    if (gScreenModeSelection < SCREEN_MODE_3P_4P_SPLITSCREEN) {
        gEXSetViewportAlign(gDisplayListHead++, G_EX_ORIGIN_NONE, 0, 0);
    }
}
#endif