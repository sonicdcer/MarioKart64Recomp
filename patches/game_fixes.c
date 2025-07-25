#include "patches.h"

// Fixes a bug with the recompiler.
int dummy_val = 1;

RECOMP_PATCH void func_8028FC34(void) {
    if (D_802BA038 < 0) {
        D_802BA038 = 1920;
        return;
    }
    D_802BA038--;

    // @recomp: Change gControllerFive to gControllers[0] to avoid bug where something
    // is pressing other controllers in memory.

    // if (gControllerFive->buttonPressed != 0) {
    if (gControllers[0].buttonPressed != 0) {
        func_8028FBD4();
        gMenuSelection = START_MENU;
        return;
    }
    if (D_802BA038 == 0) {
        func_8028FBD4();
        gMenuSelection = LOGO_INTRO_MENU;
    }
}

#if 0
RECOMP_PATCH void read_controllers(void) {
    OSMesg msg;

    // Start reading controller data
    osContStartReadData(&gSIEventMesgQueue);
    osRecvMesg(&gSIEventMesgQueue, &msg, OS_MESG_BLOCK);

    // Retrieve the controller data
    osContGetReadData(gControllerPads);

    // Update each controller individually
    update_controller(0);
    update_controller(1);
    update_controller(2);
    update_controller(3);

    // Optionally, clear or reset gControllerFive if it's no longer needed
    if (gControllerFive) {
        gControllerFive->button = 0;
        gControllerFive->buttonPressed = 0;
        gControllerFive->buttonDepressed = 0;
        gControllerFive->stickDirection = 0;
        gControllerFive->stickPressed = 0;
        gControllerFive->stickDepressed = 0;
    }
}
#endif

// Fixed tickrate to 2 on all cases
#if 1
RECOMP_PATCH void race_logic_loop(void) {
    s16 i;
    u16 rotY;

    gMatrixObjectCount = 0;
    gMatrixEffectCount = 0;
    if (gIsGamePaused != 0) {
        func_80290B14();
    }
    if (gIsInQuitToMenuTransition != 0) {
        func_802A38B4();
        return;
    }

    if (sNumVBlanks >= 6) {
        sNumVBlanks = 5;
    }
    if (sNumVBlanks < 0) {
        sNumVBlanks = 1;
    }
    func_802A4EF4();

    switch (gActiveScreenMode) {
        case SCREEN_MODE_1P:
            gTickSpeed = 2;
            staff_ghosts_loop();
            if (gIsGamePaused == 0) {
                for (i = 0; i < gTickSpeed; i++) {
                    if (D_8015011E) {
                        gCourseTimer += COURSE_TIMER_ITER;
                    }
                    func_802909F0();
                    evaluate_collision_for_players_and_actors();
                    func_800382DC();
                    func_8001EE98(gPlayerOneCopy, camera1, 0);
                    func_80028F70();
                    func_8028F474();
                    func_80059AC8();
                    update_course_actors();
                    course_update_water();
                    func_8028FCBC();
                }
                func_80022744();
            }
            func_8005A070();
            sNumVBlanks = 0;
            profiler_log_thread5_time(1);
            D_8015F788 = 0;
            render_player_one_1p_screen();
            if (!gEnableDebugMode) {
                D_800DC514 = false;
            } else {
                if (D_800DC514) {

                    if ((gControllerOne->buttonPressed & R_TRIG) && (gControllerOne->button & A_BUTTON) &&
                        (gControllerOne->button & B_BUTTON)) {
                        D_800DC514 = false;
                    }

                    rotY = camera1->rot[1];
                    gDebugPathCount = D_800DC5EC->pathCounter;
                    if (rotY < 0x2000) {
                        func_80057A50(40, 100, "SOUTH  ", gDebugPathCount);
                    } else if (rotY < 0x6000) {
                        func_80057A50(40, 100, "EAST   ", gDebugPathCount);
                    } else if (rotY < 0xA000) {
                        func_80057A50(40, 100, "NORTH  ", gDebugPathCount);
                    } else if (rotY < 0xE000) {
                        func_80057A50(40, 100, "WEST   ", gDebugPathCount);
                    } else {
                        func_80057A50(40, 100, "SOUTH  ", gDebugPathCount);
                    }

                } else {
                    if ((gControllerOne->buttonPressed & L_TRIG) && (gControllerOne->button & A_BUTTON) &&
                        (gControllerOne->button & B_BUTTON)) {
                        D_800DC514 = true;
                    }
                }
            }
            break;

        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            if (gCurrentCourseId == COURSE_DK_JUNGLE) {
                gTickSpeed = 2;
            } else {
                gTickSpeed = 2;
            }
            if (gIsGamePaused == 0) {
                for (i = 0; i < gTickSpeed; i++) {
                    if (D_8015011E != 0) {
                        gCourseTimer += COURSE_TIMER_ITER;
                    }
                    func_802909F0();
                    evaluate_collision_for_players_and_actors();
                    func_800382DC();
                    func_8001EE98(gPlayerOneCopy, camera1, 0);
                    func_80029060();
                    func_8001EE98(gPlayerTwoCopy, camera2, 1);
                    func_80029150();
                    func_8028F474();
                    func_80059AC8();
                    update_course_actors();
                    course_update_water();
                    func_8028FCBC();
                }
                func_80022744();
            }
            func_8005A070();
            profiler_log_thread5_time(1);
            sNumVBlanks = 0;
            move_segment_table_to_dmem();
            init_rdp();
            if (D_800DC5B0 != 0) {
                select_framebuffer();
            }
            D_8015F788 = 0;
            if (gPlayerWinningIndex == 0) {
                render_player_two_2p_screen_vertical();
                render_player_one_2p_screen_vertical();
            } else {
                render_player_one_2p_screen_vertical();
                render_player_two_2p_screen_vertical();
            }
            break;

        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:

            if (gCurrentCourseId == COURSE_DK_JUNGLE) {
                gTickSpeed = 2;
            } else {
                gTickSpeed = 2;
            }

            if (gIsGamePaused == 0) {
                for (i = 0; i < gTickSpeed; i++) {
                    if (D_8015011E != 0) {
                        gCourseTimer += COURSE_TIMER_ITER;
                    }
                    func_802909F0();
                    evaluate_collision_for_players_and_actors();
                    func_800382DC();
                    func_8001EE98(gPlayerOneCopy, camera1, 0);
                    func_80029060();
                    func_8001EE98(gPlayerTwoCopy, camera2, 1);
                    func_80029150();
                    func_8028F474();
                    func_80059AC8();
                    update_course_actors();
                    course_update_water();
                    func_8028FCBC();
                }
                func_80022744();
            }
            profiler_log_thread5_time(1);
            sNumVBlanks = (u16) 0;
            func_8005A070();
            move_segment_table_to_dmem();
            init_rdp();
            if (D_800DC5B0 != 0) {
                select_framebuffer();
            }
            D_8015F788 = 0;
            if (gPlayerWinningIndex == 0) {
                render_player_two_2p_screen_horizontal();
                render_player_one_2p_screen_horizontal();
            } else {
                render_player_one_2p_screen_horizontal();
                render_player_two_2p_screen_horizontal();
            }

            break;

        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            if (gPlayerCountSelection1 == 3) {
                switch (gCurrentCourseId) {
                    case COURSE_BOWSER_CASTLE:
                    case COURSE_MOO_MOO_FARM:
                    case COURSE_SKYSCRAPER:
                    case COURSE_DK_JUNGLE:
                        gTickSpeed = 2;
                        break;
                    default:
                        gTickSpeed = 2;
                        break;
                }
            } else {
                // Four players
                switch (gCurrentCourseId) {
                    case COURSE_BLOCK_FORT:
                    case COURSE_DOUBLE_DECK:
                    case COURSE_BIG_DONUT:
                        gTickSpeed = 2;
                        break;
                    case COURSE_DK_JUNGLE:
                        gTickSpeed = 2;
                        break;
                    default:
                        gTickSpeed = 2;
                        break;
                }
            }
            if (gIsGamePaused == 0) {
                for (i = 0; i < gTickSpeed; i++) {
                    if (D_8015011E != 0) {
                        gCourseTimer += COURSE_TIMER_ITER;
                    }
                    func_802909F0();
                    evaluate_collision_for_players_and_actors();
                    func_800382DC();
                    func_8001EE98(gPlayerOneCopy, camera1, 0);
                    func_80029158();
                    func_8001EE98(gPlayerTwo, camera2, 1);
                    func_800291E8();
                    func_8001EE98(gPlayerThree, camera3, 2);
                    func_800291F0();
                    func_8001EE98(gPlayerFour, camera4, 3);
                    func_800291F8();
                    func_8028F474();
                    func_80059AC8();
                    update_course_actors();
                    course_update_water();
                    func_8028FCBC();
                }
                func_80022744();
            }
            func_8005A070();
            sNumVBlanks = 0;
            profiler_log_thread5_time(1);
            move_segment_table_to_dmem();
            init_rdp();
            if (D_800DC5B0 != 0) {
                select_framebuffer();
            }
            D_8015F788 = 0;
            if (gPlayerWinningIndex == 0) {
                render_player_two_3p_4p_screen();
                render_player_three_3p_4p_screen();
                render_player_four_3p_4p_screen();
                render_player_one_3p_4p_screen();
            } else if (gPlayerWinningIndex == 1) {
                render_player_one_3p_4p_screen();
                render_player_three_3p_4p_screen();
                render_player_four_3p_4p_screen();
                render_player_two_3p_4p_screen();
            } else if (gPlayerWinningIndex == 2) {
                render_player_one_3p_4p_screen();
                render_player_two_3p_4p_screen();
                render_player_four_3p_4p_screen();
                render_player_three_3p_4p_screen();
            } else {
                render_player_one_3p_4p_screen();
                render_player_two_3p_4p_screen();
                render_player_three_3p_4p_screen();
                render_player_four_3p_4p_screen();
            }
            break;
    }

    if (!gEnableDebugMode) {
        gEnableResourceMeters = 0;
    } else {
        if (gEnableResourceMeters) {
            resource_display();
            if ((!(gControllerOne->button & L_TRIG)) && (gControllerOne->button & R_TRIG) &&
                (gControllerOne->buttonPressed & B_BUTTON)) {
                gEnableResourceMeters = 0;
            }
        } else {
            if ((!(gControllerOne->button & L_TRIG)) && (gControllerOne->button & R_TRIG) &&
                (gControllerOne->buttonPressed & B_BUTTON)) {
                gEnableResourceMeters = 1;
            }
        }
    }
    func_802A4300();
    func_800591B4();
    func_80093E20();
#if DVDL
    display_dvdl();
#endif
    gDPFullSync(gDisplayListHead++);
    gSPEndDisplayList(gDisplayListHead++);
}
#endif

#if 1
// play_music_for_current_track
RECOMP_PATCH void func_8028EC98(s32 track) {
    // @recomp: play music in 3P/4P modes
    // if (gScreenModeSelection == SCREEN_MODE_3P_4P_SPLITSCREEN) {
    //     return; // If 3P/4P splitscreen mode is currently on, don't play the music for the current track.
    // }

    func_800029B0();

    switch (track) {
        case COURSE_MARIO_RACEWAY: // Raceways, Wario Stadium
        case COURSE_ROYAL_RACEWAY:
        case COURSE_LUIGI_RACEWAY:
        case COURSE_WARIO_STADIUM:
            func_800C8EAC(SEQ_TRACK_RACEWAY);
            break;

        case COURSE_TOADS_TURNPIKE: // Toad's Turnpike
            func_800C8EAC(SEQ_TRACK_TURNPIKE);
            break;

        case COURSE_YOSHI_VALLEY: // Yoshi Valley, Moo Moo Farm
        case COURSE_MOO_MOO_FARM:
            func_800C8EAC(SEQ_TRACK_FARM);
            break;

        case COURSE_CHOCO_MOUNTAIN: // Choco Mountain, Various Battle Stages
        case COURSE_BLOCK_FORT:
        case COURSE_DOUBLE_DECK:
            func_800C8EAC(SEQ_TRACK_MOUNTAIN);
            break;

        case COURSE_KALAMARI_DESERT: // Kalimari Desert
            func_800C8EAC(SEQ_TRACK_DESERT);
            break;

        case COURSE_KOOPA_BEACH: // Koopa Troopa Beach
            func_800C8EAC(SEQ_TRACK_BEACH);
            break;

        case COURSE_BOWSER_CASTLE: // Bowser Castle
            func_800C8EAC(SEQ_TRACK_CASTLE);
            break;

        case COURSE_BANSHEE_BOARDWALK: // Banshee Boardwalk
            func_800C8EAC(SEQ_TRACK_SCARY);
            break;

        case COURSE_FRAPPE_SNOWLAND: // Frappe Snowland, Sherbet Land
        case COURSE_SHERBET_LAND:
            func_800C8EAC(SEQ_TRACK_SNOW);
            break;

        case COURSE_RAINBOW_ROAD: // Rainbow Road
            func_800C8EAC(SEQ_TRACK_RAINBOW);
            break;

        case COURSE_DK_JUNGLE: // DK's Jungle Parkway
            func_800C8EAC(SEQ_TRACK_JUNGLE);
            break;

        case COURSE_SKYSCRAPER: // Other Battle Stages
        case COURSE_BIG_DONUT:
            func_800C8EAC(SEQ_TRACK_BATTLE);
            break;
    }
}
#endif