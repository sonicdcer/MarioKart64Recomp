#include "patches.h"

bool recompDebugSwitch = 0;

RECOMP_PATCH void handle_menus_with_pri_arg(s32 priSpecial) {
    s32 j = 0;
    s32 isRendered = 0;
    s32 i = 0;
    s32 type = 0;
    MenuItem* entry = NULL;
    volatile u8* address;

    // gDPSetTextureFilter(gDisplayListHead++, G_TF_POINT);
    // gEXEnable(gDisplayListHead++); // @recomp
    // gEXSetRefreshRate(gDisplayListHead++, 60 / 2);

    // @recomp: Patch the sequence bug that prevented the whole end results song from playing.
    {
        address = (volatile u8*) 0x803B16DC;

        if (*address == 0x40) {
            *address = 0x2;
            recomp_printf("Sequence PATCHED!\n");
        }
    }

#if 0
    {
        if (gControllers[0].button & U_JPAD) {
            recompDebugSwitch = 1;
            recomp_printf("recompDebugSwitch: ON : %d\n", recompDebugSwitch);
        }
        if (gControllers[0].button & D_JPAD) {
            recompDebugSwitch = 0;
            recomp_printf("recompDebugSwitch: OFF : %d\n", recompDebugSwitch);
        }
    }
#endif

    for (i = 0; i < MENU_ITEMS_MAX; i++) {
        isRendered = false;
        entry = &gMenuItems[i];
        type = entry->type;
        if ((type == MENU_ITEM_UI_NO_CONTROLLER) || (type == MENU_ITEM_UI_START_RECORD_TIME) ||
            (type == MENU_ITEM_PAUSE)) {
            if (priSpecial != 0) {
                isRendered = true;
            }
        } else if (priSpecial == 0) {
            isRendered = true;
        }

        if (isRendered == false) {
            continue;
        }

        switch (type) {
            case 240:
                gDPSetTextureFilter(gDisplayListHead++, G_TF_BILERP);
                break;
            case MENU_ITEM_UI_LOGO_INTRO:
                gDPSetTextureFilter(gDisplayListHead++, G_TF_BILERP);
                if (sIntroModelTimer < 0x50) {
                    sIntroModelSpeed = 3.0f;
                } else if (sIntroModelTimer < 90) {
                    if (sIntroModelMotionSpeed < 1.0) {
                        sIntroModelMotionSpeed += 0.1;
                    }
                    sIntroModelSpeed += 0.1;
                } else if (sIntroModelTimer < 0xA0) {
                    sIntroModelSpeed += 0.1;
                } else if (sIntroModelTimer < 0x190) {
                    sIntroModelSpeed += 0.3;
                }
                gIntroModelRotY -= sIntroModelSpeed;
                sIntroModelTimer += 1;
                if (gIntroModelRotY < -360.0f) {
                    gIntroModelRotY += 360.0f;
                }
                entry->param1++;
                if (entry->param1 == 0x000000B4) {
                    func_8009E000(40);
                    func_800CA388(0x64U);
                    gMenuFadeType = MENU_FADE_TYPE_MAIN;
                }
                if ((entry->param2 != 0) && (entry->param1 >= 3)) {
                    entry->param2 = 0;
                    play_sound2(SOUND_INTRO_LOGO);
                }
                break;
            case MENU_ITEM_TYPE_0DA:
                func_800A954C(entry);
                break;
            case MENU_ITEM_TYPE_0D6:
                func_800A9710(entry);
                break;
            case MENU_ITEM_TYPE_0D4:
                func_800A97BC(entry);
                break;
            case MENU_ITEM_UI_START_RECORD_TIME:
                gDPSetTextureFilter(gDisplayListHead++, G_TF_BILERP);
                switch (entry->state) {
                    case 0:
                        if (gControllers[0].button & L_TRIG) {
                            gDebugMenuSelection = 3;
                        }
#if DEBUG_ENDING == 1
                        if (gControllers[0].button & U_CBUTTONS) {
                            gGamestateNext = CREDITS_SEQUENCE;
                        }
#endif
#if DEBUG_CEREMONY == 1
                        if (gControllers[0].button & R_CBUTTONS) {
                            gGamestateNext = ENDING;
                        }
#endif
                        // @recomp: Change gControllerFive to gControllers[0] to avoid bug where something
                        // is pressing other controllers in memory.
                        if (gControllers[0].button & R_TRIG) {
                            entry->state = 1;
                            play_sound2(SOUND_ACTION_PING);
                        } else {
                            entry->visible = 0;
                        }
                        break;
                    case 1:
                    default:
                        entry->visible = 1;
                        break;
                }
                break;
            case MENU_ITEM_UI_GAME_SELECT:
                func_800AA280(entry);
                break;
            case MAIN_MENU_OPTION_GFX:
            case MAIN_MENU_DATA_GFX:
                switch (gMainMenuSelection) {
                    case MAIN_MENU_OPTION:
                    case MAIN_MENU_DATA:
                    case MAIN_MENU_PLAYER_SELECT:
                        func_800A9B9C(entry);
                        break;
                    case MAIN_MENU_MODE_SELECT:
                    case MAIN_MENU_MODE_SUB_SELECT:
                    case MAIN_MENU_OK_SELECT:
                    case MAIN_MENU_OK_SELECT_GO_BACK:
                    case MAIN_MENU_MODE_SUB_SELECT_GO_BACK:
                        func_800A9C40(entry);
                        break;
                }
                break;
            case MENU_ITEM_UI_OK:
                func_800AA280(entry);
                update_ok_menu_item(entry);
                break;
            case MENU_ITEM_UI_1P_GAME:
            case MENU_ITEM_UI_2P_GAME:
            case MENU_ITEM_UI_3P_GAME:
            case MENU_ITEM_UI_4P_GAME:
                switch (gMainMenuSelection) {
                    case MAIN_MENU_OPTION:
                    case MAIN_MENU_DATA:
                    case MAIN_MENU_PLAYER_SELECT:
                        gDPSetTextureFilter(gDisplayListHead++, G_TF_POINT);
                        func_800A9B9C(entry);
                        break;
                    case MAIN_MENU_MODE_SELECT:
                    case MAIN_MENU_MODE_SUB_SELECT:
                    case MAIN_MENU_OK_SELECT:
                    case MAIN_MENU_OK_SELECT_GO_BACK:
                    case MAIN_MENU_MODE_SUB_SELECT_GO_BACK:
                        gDPSetTextureFilter(gDisplayListHead++, G_TF_POINT);
                        func_800A9C40(entry);
                        break;
                }
                func_800A9D5C(entry);
                break;
            case MAIN_MENU_50CC:
            case MAIN_MENU_100CC:
            case MAIN_MENU_150CC:
            case MAIN_MENU_EXTRA_CC:
            case MENU_ITEM_TYPE_016:
            case MENU_ITEM_TYPE_017:
            case MAIN_MENU_TIME_TRIALS_BEGIN:
            case MAIN_MENU_TIME_TRIALS_DATA:
                func_800A9E58(entry);
                break;
            case MENU_ITEM_TYPE_01B:
                gDPSetTextureFilter(gDisplayListHead++, G_TF_POINT);
                func_800AA2EC(entry);
                break;
            case CHARACTER_SELECT_MENU_1P_CURSOR:
            case CHARACTER_SELECT_MENU_2P_CURSOR:
            case CHARACTER_SELECT_MENU_3P_CURSOR:
            case CHARACTER_SELECT_MENU_4P_CURSOR:
                update_cursor(entry);
                break;
            case CHARACTER_SELECT_MENU_MARIO:
            case CHARACTER_SELECT_MENU_LUIGI:
            case CHARACTER_SELECT_MENU_TOAD:
            case CHARACTER_SELECT_MENU_PEACH:
            case CHARACTER_SELECT_MENU_YOSHI:
            case CHARACTER_SELECT_MENU_DK:
            case CHARACTER_SELECT_MENU_WARIO:
            case CHARACTER_SELECT_MENU_BOWSER:
                func_800AAC18(entry);
                switch (entry->type) {
                    case CHARACTER_SELECT_MENU_MARIO:
                    case CHARACTER_SELECT_MENU_LUIGI:
                    case CHARACTER_SELECT_MENU_TOAD:
                    case CHARACTER_SELECT_MENU_PEACH:
                    case CHARACTER_SELECT_MENU_YOSHI:
                    case CHARACTER_SELECT_MENU_DK:
                    case CHARACTER_SELECT_MENU_WARIO:
                    case CHARACTER_SELECT_MENU_BOWSER:
                        gDPSetTextureFilter(gDisplayListHead++, G_TF_POINT);
                        func_800AA69C(entry);
                        break;
                }
                switch (gPlayerSelectMenuSelection) {
                    case PLAYER_SELECT_MENU_MAIN:
                        func_800AAB90(entry);
                        break;
                    case PLAYER_SELECT_MENU_OK:
                    case PLAYER_SELECT_MENU_OK_GO_BACK:
                        func_800AAA9C(entry);
                        break;
                }
                func_800AAE18(entry);
                break;
            case CHARACTER_SELECT_MENU_OK:
            case COURSE_SELECT_OK:
                update_ok_menu_item(entry);
                break;
            case COURSE_SELECT_MUSHROOM_CUP:
            case COURSE_SELECT_FLOWER_CUP:
            case COURSE_SELECT_STAR_CUP:
            case COURSE_SELECT_SPECIAL_CUP:
                func_800AB164(entry);
                switch (gSubMenuSelection) {
                    case SUB_MENU_MAP_SELECT_CUP:
                        func_800AB020(entry);
                        break;
                    case SUB_MENU_MAP_SELECT_COURSE:
                    case SUB_MENU_MAP_SELECT_OK:
                        func_800AB098(entry);
                        break;
                }
                break;
            case MENU_ITEM_TYPE_058:
            case COURSE_SELECT_COURSE_NAMES:
            case MENU_ITEM_TYPE_05A:
            case MENU_ITEM_TYPE_05B:
                func_800AB260(entry);
                break;
            case MENU_ITEM_TYPE_064:
                func_800AB314(entry);
                break;
            case MENU_ITEM_TYPE_05F:
            case MENU_ITEM_TYPE_060:
            case MENU_ITEM_TYPE_061:
            case MENU_ITEM_TYPE_062:
                gDPSetTextureFilter(gDisplayListHead++, G_TF_POINT);
                func_800AB290(entry);
                break;
            case MENU_ITEM_TYPE_065:
            case MENU_ITEM_TYPE_066:
                func_800AB904(entry);
                break;
            case MENU_ITEM_TYPE_067:
                func_800AB9B0(entry);
                break;
            case MENU_ITEM_TYPE_078:
            case MENU_ITEM_TYPE_079:
            case MENU_ITEM_TYPE_07A:
            case MENU_ITEM_TYPE_07B:
            case MENU_ITEM_TYPE_08C:
                func_800ABAE8(entry);
                break;
            case MENU_ITEM_TYPE_08D:
                func_800ABB24(entry);
                break;
            case MENU_ITEM_TYPE_07C:
            case MENU_ITEM_TYPE_07D:
            case MENU_ITEM_TYPE_07E:
            case MENU_ITEM_TYPE_07F:
            case MENU_ITEM_TYPE_080:
            case MENU_ITEM_TYPE_081:
            case MENU_ITEM_TYPE_082:
            case MENU_ITEM_TYPE_083:
            case MENU_ITEM_TYPE_084:
            case MENU_ITEM_TYPE_085:
            case MENU_ITEM_TYPE_086:
            case MENU_ITEM_TYPE_087:
            case MENU_ITEM_TYPE_088:
            case MENU_ITEM_TYPE_089:
            case MENU_ITEM_TYPE_08A:
            case MENU_ITEM_TYPE_08B:
                gDPSetTextureFilter(gDisplayListHead++, G_TF_POINT);
                func_800ABBCC(entry);
                break;
            case MENU_ITEM_TYPE_096:
                func_800ABC38(entry);
                break;
            case MENU_ITEM_TYPE_097:
                func_800ABEAC(entry);
                break;
            case MENU_ITEM_TYPE_05E:
                func_800AC300(entry);
                break;
            case MENU_ITEM_TYPE_0AA:
                func_800AC324(entry);
                break;
            case MENU_ITEM_TYPE_0AB:
                func_800AC458(entry);
                break;
            case MENU_ITEM_TYPE_0AC:
                func_800ACA14(entry);
                break;
            case MENU_ITEM_TYPE_0AF:
                func_800AC978(entry);
                break;
            case MENU_ITEM_TYPE_0B0:
                func_800ACC50(entry);
                break;
            case MENU_ITEM_TYPE_0B1:
            case MENU_ITEM_TYPE_0B2:
            case MENU_ITEM_TYPE_0B3:
            case MENU_ITEM_TYPE_0B4:
                func_800ACF40(entry);
                break;
            case MENU_ITEM_TYPE_0B9:
                func_800AD1A4(entry);
                break;
            case MENU_ITEM_TYPE_0BA:
                func_800AD2E8(entry);
                break;
            case MENU_ITEM_ANNOUNCE_GHOST:
                gDPSetTextureFilter(gDisplayListHead++, G_TF_BILERP);
                func_800AEC54(entry);
                break;
            case MENU_ITEM_PAUSE:
                gDPSetTextureFilter(gDisplayListHead++, G_TF_BILERP);
                func_800ADF48(entry);
                break;
            case MENU_ITEM_END_COURSE_OPTION:
                gDPSetTextureFilter(gDisplayListHead++, G_TF_BILERP);
                func_800AE218(entry);
                break;
            case MENU_ITEM_DATA_COURSE_IMAGE:
                func_800AEDBC(entry);
                break;
            case MENU_ITEM_DATA_COURSE_SELECTABLE:
                func_800AEE90(entry);
                break;
            case MENU_ITEM_TYPE_0E9:
                func_800AEEBC(entry);
                break;
            case MENU_ITEM_TYPE_0EA:
                func_800AEEE8(entry);
                break;
            case MENU_ITEM_TYPE_0BE:
                func_800AEF14(entry);
                break;
            case MENU_ITEM_TYPE_10E:
                func_800AEF74(entry);
                break;
            case MENU_ITEM_TYPE_12B:
                func_800AF004(entry);
                break;
            case MENU_ITEM_TYPE_12C:
            case MENU_ITEM_TYPE_12D:
            case MENU_ITEM_TYPE_12E:
            case MENU_ITEM_TYPE_12F:
                func_800AF1AC(entry);
                break;
            case MENU_ITEM_TYPE_130:
                func_800AF270(entry);
                break;
            case MENU_ITEM_TYPE_190:
            case MENU_ITEM_TYPE_191:
            case MENU_ITEM_TYPE_192:
            case MENU_ITEM_TYPE_193:
            case MENU_ITEM_TYPE_194:
            case MENU_ITEM_TYPE_195:
            case MENU_ITEM_TYPE_196:
            case MENU_ITEM_TYPE_197:
            case MENU_ITEM_TYPE_198:
            case MENU_ITEM_TYPE_199:
            case MENU_ITEM_TYPE_19A:
            case MENU_ITEM_TYPE_19B:
            case MENU_ITEM_TYPE_19C:
            case MENU_ITEM_TYPE_19D:
            case MENU_ITEM_TYPE_19E:
            case MENU_ITEM_TYPE_19F:
            case MENU_ITEM_TYPE_1A0:
            case MENU_ITEM_TYPE_1A1:
            case MENU_ITEM_TYPE_1A2:
            case MENU_ITEM_TYPE_1A3:
            case MENU_ITEM_TYPE_1A4:
            case MENU_ITEM_TYPE_1A5:
            case MENU_ITEM_TYPE_1A6:
            case MENU_ITEM_TYPE_1A7:
            case MENU_ITEM_TYPE_1A8:
            case MENU_ITEM_TYPE_1A9:
            case MENU_ITEM_TYPE_1AA:
            case MENU_ITEM_TYPE_1AB:
            case MENU_ITEM_TYPE_1AC:
            case MENU_ITEM_TYPE_1AD:
            case MENU_ITEM_TYPE_1AE:
            case MENU_ITEM_TYPE_1AF:
            case MENU_ITEM_TYPE_1B0:
            case MENU_ITEM_TYPE_1B1:
            case MENU_ITEM_TYPE_1B2:
            case MENU_ITEM_TYPE_1B3:
            case MENU_ITEM_TYPE_1B4:
            case MENU_ITEM_TYPE_1B5:
            case MENU_ITEM_TYPE_1B6:
            case MENU_ITEM_TYPE_1B7:
            case MENU_ITEM_TYPE_1B8:
            case MENU_ITEM_TYPE_1B9:
            case MENU_ITEM_TYPE_1BA:
            case MENU_ITEM_TYPE_1BB:
            case MENU_ITEM_TYPE_1BC:
            case MENU_ITEM_TYPE_1BD:
            case MENU_ITEM_TYPE_1BE:
            case MENU_ITEM_TYPE_1BF:
            case MENU_ITEM_TYPE_1C0:
            case MENU_ITEM_TYPE_1C1:
            case MENU_ITEM_TYPE_1C2:
            case MENU_ITEM_TYPE_1C3:
            case MENU_ITEM_TYPE_1C4:
            case MENU_ITEM_TYPE_1C5:
            case MENU_ITEM_TYPE_1C6:
            case MENU_ITEM_TYPE_1C7:
            case MENU_ITEM_TYPE_1C8:
            case MENU_ITEM_TYPE_1C9:
            case MENU_ITEM_TYPE_1CA:
            case MENU_ITEM_TYPE_1CB:
            case MENU_ITEM_TYPE_1CC:
            case MENU_ITEM_TYPE_1CD:
            case MENU_ITEM_TYPE_1CE:
                func_800AF480(entry);
                break;
            case MENU_ITEM_UI_NONE:
            case MENU_ITEM_UI_START_BACKGROUND:
            case MENU_ITEM_UI_LOGO_AND_COPYRIGHT:
            case MENU_ITEM_UI_PUSH_START_BUTTON:
            case MENU_ITEM_UI_NO_CONTROLLER:
                break;
        }
    }

    for (j = 0; j < MENU_ITEM_PRIORITY_MAX; j++) {
        for (i = 0; i < MENU_ITEMS_MAX; i++) {
            isRendered = false;
            entry = &gMenuItems[i];
            type = entry->type;
            if ((type == MENU_ITEM_UI_NO_CONTROLLER) || (type == MENU_ITEM_UI_START_RECORD_TIME) ||
                (type == MENU_ITEM_PAUSE)) {
                if (priSpecial != 0) {
                    isRendered = true;
                }
            } else if (priSpecial == 0) {
                isRendered = true;
            }
            if ((isRendered != 0) && (j == (s8) entry->priority)) {
                render_menus(entry);
            }
        }
    }

#if 0
    for (int num = 0; num < 8; num++) {
        if (gControllers[num].button != 0) {
            recomp_printf("BUTTON: %x\n", gControllers[num].button);
            recomp_printf("BUTTON: %x\n", gControllers[num].button);
        }
    }
#endif
}

#if 1
Gfx* func_8009BC9C_2(Gfx* arg0, MenuTexture* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, MenuItem* item);
Gfx* RenderMenuTextures(Gfx* arg0, MenuTexture* arg1, s32 column, s32 row);
RECOMP_PATCH void render_menus(MenuItem* arg0) {
    s32 var_a1 = 0;
    s32 var_v1 = 0;
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    MenuTexture* sp9C;
    UNUSED s32 stackPadding2;
    s32 temp_a0;
    s32 temp_t2;
    s32 temp_t5;
    s32 temp_t9;
    s32 temp_v1;
    char sp80[3];
    s32 var_t0;
    UNUSED s32 var_a2;
    f32 why = 0.75f;
    s32 one = 1;
    f32 floatone = 1;
    UNUSED s32 stackPadding3;
    UNUSED s32 stackPadding4;
    f32 scaleX;

    if (arg0->visible) {
        gDPPipeSync(gDisplayListHead++);
        switch (arg0->type) { /* switch 6; irregular */
            case MENU_ITEM_UI_LOGO_INTRO:
                func_80094660(gGfxPool, arg0->param1);
                break;
            case START_MENU_FLAG:
                render_checkered_flag(gGfxPool, arg0->param1);
                break;
            case MENU_ITEM_TYPE_0D2:
                gDisplayListHead = func_8009B9D0(gDisplayListHead, D_020014C8);
                break;
            case MENU_ITEM_TYPE_0D3:
                gDisplayListHead = func_8009B9D0(gDisplayListHead, D_02001540);
                break;
            case MENU_ITEM_TYPE_0D4:
                func_800A09E0(arg0);
                func_800A0AD0(arg0);
                func_800A0B80(arg0);
                break;
            case MENU_ITEM_TYPE_0D5:
                // Sets the text color of the text on the Controller Pak menu
                // Also sets the color of the shading at the top and bottom of the menu
                // Does not set color of the text in the table itself
                gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x00, 0x00, 0x00, 0xFF);
                gDisplayListHead = func_8009B9D0(gDisplayListHead, D_020015A4);
                gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x32, 0x00, 0x00, 0xFF);
                gDisplayListHead = func_8009B9D0(gDisplayListHead, D_020015CC);
                gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x00, 0x32, 0x00, 0xFF);
                gDisplayListHead = func_8009B9D0(gDisplayListHead, D_02001630);
                gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x32, 0x32, 0x00, 0xFF);
                gDisplayListHead = func_8009B9D0(gDisplayListHead, D_02001658);
                break;
            case MENU_ITEM_TYPE_0D6:
                func_8009A76C(arg0->D_8018DEE0_index, arg0->column, arg0->row, -1);
                break;
            case MENU_ITEM_TYPE_0D7:
                func_800A0DFC();
                break;
            case MENU_ITEM_TYPE_0D8:
            case MENU_ITEM_TYPE_0D9:
                func_800A0EB8(arg0, arg0->type - 0xD8);
                break;
            case MENU_ITEM_UI_START_BACKGROUND:
                gDisplayListHead = RenderMenuTextures(
                    gDisplayListHead, gMenuTexturesBackground[has_unlocked_extra_mode()], arg0->column, arg0->row);
                break;
            case MENU_ITEM_UI_LOGO_AND_COPYRIGHT:
                render_game_logo((arg0->column + 0xA0), (arg0->row + 0x47));
                gDisplayListHead =
                    render_menu_textures(gDisplayListHead, seg2_copyright_1996_texture, arg0->column, arg0->row);
                break;
            case MENU_ITEM_UI_PUSH_START_BUTTON:
                if (((gGlobalTimer / 8) % 3) != 0) {
                    gDisplayListHead =
                        render_menu_textures(gDisplayListHead, seg2_push_start_button_texture, arg0->column, arg0->row);
                }
                break;
            case MENU_ITEM_UI_START_RECORD_TIME:
                var_t0 = (s32) ((f32) (get_string_width(gCourseNamesDup[0]) + 5) * 0.9f) / 2;
                gDisplayListHead = draw_box(gDisplayListHead, 0xA0 - var_t0, 0x0000007B, var_t0 + 0xA0, 0x000000A4, 0,
                                            0, 0, 0x00000096);
                set_text_color(TEXT_GREEN);
                print_text1_center_mode_1(0x0000009B, 0x0000008C, gCourseNamesDup[0], 0, 0.9f, 0.9f);
                temp_v1 = func_800B4EB4(0, 7) & 0xFFFFF;
                if (temp_v1 < 0x1EAA) {
                    set_text_color((s32) gGlobalTimer % 2);
                } else if (temp_v1 < 0x2329) {
                    set_text_color((s32) gGlobalTimer % 3);
                } else {
                    set_text_color(TEXT_YELLOW);
                }
                get_time_record_minutes(temp_v1, sp80);
                func_800939C8(0x00000077, 0x000000A0, sp80, 0, 1.0f, 1.0f);
                print_text_mode_1(0x0000008B, 0x000000A0, "'", 0, 1.0f, 1.0f);
                get_time_record_seconds(temp_v1, sp80);
                func_800939C8(0x00000094, 0x000000A0, sp80, 0, 1.0f, 1.0f);
                print_text_mode_1(0x000000A7, 0x000000A0, "\"", 0, 1.0f, 1.0f);
                get_time_record_centiseconds(temp_v1, sp80);
                func_800939C8(0x000000B4, 0x000000A0, sp80, 0, 1.0f, 1.0f);
                break;
            case MENU_ITEM_UI_NO_CONTROLLER:
                var_t0 = get_string_width(gTextNoController[0]);
                temp_v1 = get_string_width(gTextNoController[1]);
                if (var_t0 < temp_v1) {
                    var_t0 = temp_v1;
                }
                temp_t2 = (s32) (var_t0 * why) / 2;
                temp_t5 = (s32) (((why * 2) + 0.5) * 16.0) / 2;
                gDisplayListHead = draw_box(gDisplayListHead, 0xA0 - temp_t2, 0xB6 - temp_t5, temp_t2 + 0xA0,
                                            temp_t5 + 0xB6, 0, 0, 0, 0x00000096);
                set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_1);
                for (temp_t9 = 0; temp_t9 < 2; temp_t9++) {
                    print_text1_center_mode_1(0xA0 * one - floatone * why,
                                              (s32) (0xB4 * one + ((f32) (temp_t9 * 0x12) * why)),
                                              gTextNoController[temp_t9], 0, why, why);
                }
                break;
            case MAIN_MENU_BACKGROUND:
            case CHARACTER_SELECT_BACKGROUND:
            case COURSE_SELECT_BACKGROUND:
                gDisplayListHead = func_8009BC9C_2(gDisplayListHead, gMenuTexturesBackground[has_unlocked_extra_mode()],
                                                   arg0->column, arg0->row, 3, 0, arg0);
                break;
            case MENU_ITEM_UI_GAME_SELECT:
                gDisplayListHead =
                    render_menu_textures(gDisplayListHead, seg2_game_select_texture, arg0->column, arg0->row);
                break;
            case MENU_ITEM_UI_1P_GAME:
            case MENU_ITEM_UI_2P_GAME:
            case MENU_ITEM_UI_3P_GAME:
            case MENU_ITEM_UI_4P_GAME:
                var_a1 = arg0->type - 0xB;
                func_800A8270(var_a1, arg0);
                func_800A0FA4(arg0, var_a1);
                break;
            case MENU_ITEM_UI_OK:
                func_800A8564(arg0);
                gDisplayListHead =
                    func_8009BC9C_2(gDisplayListHead, D_0200487C, arg0->column, arg0->row, 2, arg0->param1, NULL);
                break;
            case MAIN_MENU_OPTION_GFX:
            case MAIN_MENU_DATA_GFX:
                var_a1 = arg0->type - 0xF;
                if (arg0->param1 < 0x20) {
                    temp_t9 = (arg0->param1 * 0x3A) / 64;
                    if (var_a1 == gMainMenuSelection) {
                        gDisplayListHead =
                            draw_flash_select_case_fast(gDisplayListHead, arg0->column + temp_t9, arg0->row,
                                                        (arg0->column - temp_t9) + 0x39, arg0->row + 0x12);
                    } else {
                        gDisplayListHead =
                            draw_box_fill(gDisplayListHead, arg0->column + temp_t9, arg0->row,
                                          (arg0->column - temp_t9) + 0x39, arg0->row + 0x12, 1, 1, 1, 0x000000FF);
                    }
                }
                var_v1 = arg0->type - 0xA;
                gDisplayListHead = func_8009BC9C_2(gDisplayListHead, D_800E8254[var_v1], arg0->column, arg0->row, 2,
                                                   arg0->param1, NULL);
                break;
            case MAIN_MENU_50CC:
            case MAIN_MENU_100CC:
            case MAIN_MENU_150CC:
            case MAIN_MENU_EXTRA_CC:
            case MENU_ITEM_TYPE_016:
            case MENU_ITEM_TYPE_017:
            case MAIN_MENU_TIME_TRIALS_BEGIN:
            case MAIN_MENU_TIME_TRIALS_DATA:
                var_v1 = gGameModeSubMenuColumn[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]];
                var_a1 = gGameModePlayerSelection[gPlayerCount - 1][gGameModeMenuColumn[gPlayerCount - 1]];
                switch (arg0->type) {        /* switch 5 */
                    case MAIN_MENU_50CC:     /* switch 5 */
                    case MAIN_MENU_100CC:    /* switch 5 */
                    case MAIN_MENU_150CC:    /* switch 5 */
                    case MAIN_MENU_EXTRA_CC: /* switch 5 */
                        switch (var_a1) {
                            case 0:
                            case 2:
                                break;
                            default:
                                var_v1 = -1;
                                break;
                        }

                        var_a1 = MAIN_MENU_50CC;
                        sp9C = segmented_to_virtual_dupe(D_800E8294[arg0->type - MAIN_MENU_50CC]);
                        break;
                    case MENU_ITEM_TYPE_016: /* switch 5 */
                    case MENU_ITEM_TYPE_017: /* switch 5 */
                        if (var_a1 != 2) {
                            var_v1 = -1;
                        } else {
                            var_a1 = MENU_ITEM_TYPE_016;
                            sp9C = segmented_to_virtual_dupe(D_800E82A4[arg0->type - MENU_ITEM_TYPE_016]);
                        }
                        break;
                    case MAIN_MENU_TIME_TRIALS_BEGIN: /* switch 5 */
                    case MAIN_MENU_TIME_TRIALS_DATA:  /* switch 5 */
                        if (var_a1 != 1) {
                            var_v1 = -1;
                        } else {
                            var_a1 = MAIN_MENU_TIME_TRIALS_BEGIN;
                            sp9C = segmented_to_virtual_dupe(D_800E82AC[arg0->type - MAIN_MENU_TIME_TRIALS_BEGIN]);
                        }
                        break;
                }
                if (var_v1 == -1) {
                    break;
                }
                if ((gMainMenuSelection >= MAIN_MENU_MODE_SUB_SELECT) && (var_v1 == (arg0->type - var_a1))) {
                    if (gMainMenuSelection > MAIN_MENU_MODE_SUB_SELECT) {
                        gDisplayListHead =
                            draw_box_fill(gDisplayListHead, arg0->column, arg0->row, arg0->column + 0x3F,
                                          arg0->row + 0x11, 0x000000FF, 0x000000F9, 0x000000DC, 0x000000FF);
                    } else {
                        gDisplayListHead = draw_flash_select_case_slow(
                            gDisplayListHead, arg0->column ^ 0, arg0->row ^ 0, arg0->column + 0x3F, arg0->row + 0x11);
                    }
                } else {
                    gDisplayListHead = draw_box_fill(gDisplayListHead, arg0->column, arg0->row, arg0->column + 0x3F,
                                                     arg0->row + 0x11, 1, 1, 1, 0x000000FF);
                }
                gDisplayListHead = render_menu_textures(gDisplayListHead, sp9C, arg0->column, arg0->row);
                break;
            case MENU_ITEM_TYPE_01B:
                if (arg0->state == 2) {
                    gDPSetTextureFilter(gDisplayListHead++, G_TF_BILERP);
                } else {
                    gDPSetTextureFilter(gDisplayListHead++, G_TF_POINT);
                }
                func_800A10CC(arg0);
                break;
            case CHARACTER_SELECT_MENU_PLAYER_SELECT_BANNER:
                gDisplayListHead = render_menu_textures(gDisplayListHead, D_02004B4C, arg0->column, arg0->row);
                break;
            case CHARACTER_SELECT_MENU_1P_CURSOR:
            case CHARACTER_SELECT_MENU_2P_CURSOR:
            case CHARACTER_SELECT_MENU_3P_CURSOR:
            case CHARACTER_SELECT_MENU_4P_CURSOR:
                temp_a0 = arg0->type - CHARACTER_SELECT_MENU_1P_CURSOR;
                if (gCharacterGridSelections[temp_a0]) {
                    if (gCharacterGridIsSelected[temp_a0] == 0) {
                        temp_t2 = 0x000000FF;
                    } else {
                        temp_t2 = gGlobalTimer % 16;
                        if (temp_t2 >= 8) {
                            temp_t2 = (-temp_t2 * 8) + 0x80;
                        } else {
                            temp_t2 *= 8;
                        }
                        temp_t2 += 0xBF;
                    }
                    render_cursor_player(arg0, temp_a0, temp_t2);
                }
                break;
            case CHARACTER_SELECT_MENU_OK:
                func_800A8564(arg0);
                gDisplayListHead =
                    func_8009BC9C_2(gDisplayListHead, D_02004B74, arg0->column, arg0->row, 2, arg0->param1, NULL);
                break;
            case CHARACTER_SELECT_MENU_MARIO:
            case CHARACTER_SELECT_MENU_LUIGI:
            case CHARACTER_SELECT_MENU_TOAD:
            case CHARACTER_SELECT_MENU_PEACH:
            case CHARACTER_SELECT_MENU_YOSHI:
            case CHARACTER_SELECT_MENU_DK:
            case CHARACTER_SELECT_MENU_WARIO:
            case CHARACTER_SELECT_MENU_BOWSER:
                func_800A12BC(arg0, segmented_to_virtual_dupe(D_800E7D54[arg0->type - 0x2B]));
                /* fallthrough */
            case MENU_ITEM_TYPE_0A0:
            case MENU_ITEM_TYPE_0A1:
                func_8009A76C(arg0->D_8018DEE0_index, arg0->column, arg0->row, arg0->param1);
                break;
            case MENU_ITEM_TYPE_058:
            case COURSE_SELECT_COURSE_NAMES:
            case MENU_ITEM_TYPE_05A:
            case MENU_ITEM_TYPE_05B:
            case COURSE_SELECT_BATTLE_NAMES:
                func_800A8A98(arg0);
                gDisplayListHead = render_menu_textures(
                    gDisplayListHead,
                    segmented_to_virtual_dupe(gMenuTexturesTrackSelection[arg0->type - COURSE_SELECT_MAP_SELECT]),
                    arg0->column, arg0->row);
                func_800A8CA4(arg0);
                break;
            case COURSE_SELECT_MAP_SELECT:
                gDisplayListHead = render_menu_textures(
                    gDisplayListHead,
                    segmented_to_virtual_dupe(gMenuTexturesTrackSelection[arg0->type - COURSE_SELECT_MAP_SELECT]),
                    arg0->column, arg0->row);
                break;
            case MENU_ITEM_TYPE_05F:
            case MENU_ITEM_TYPE_060:
            case MENU_ITEM_TYPE_061:
            case MENU_ITEM_TYPE_062:
                func_800A1500(arg0);
                break;
            case COURSE_SELECT_MUSHROOM_CUP:
            case COURSE_SELECT_FLOWER_CUP:
            case COURSE_SELECT_STAR_CUP:
            case COURSE_SELECT_SPECIAL_CUP:
                var_a1 = arg0->type - 0x53;
                func_800A890C(var_a1, arg0);
                func_800A143C(arg0, var_a1);
                break;
            case COURSE_SELECT_OK:
                func_800A8564(arg0);
                gDisplayListHead =
                    func_8009BC9C_2(gDisplayListHead, D_02004E80, arg0->column, arg0->row, 2, arg0->param1, NULL);
                break;
            case MENU_ITEM_TYPE_065:
            case MENU_ITEM_TYPE_066:
                if (arg0->type == MENU_ITEM_TYPE_065) {
                    scaleX = 0.6f;
                } else {
                    scaleX = 0.8f;
                }
                func_800A86E8(arg0);
                set_text_color(TEXT_YELLOW);
                print_text_mode_1(arg0->column + 8, arg0->row + 0x10, gBestTimeText[arg0->type - 0x65], 0, scaleX,
                                  0.8f);
                func_800A874C(arg0);
                break;
            case MENU_ITEM_TYPE_06E:
                render_battle_introduction(arg0);
                break;
            case MENU_ITEM_TYPE_067:
                func_800A8EC0(arg0);
                break;
            case MENU_ITEM_TYPE_068:
                gDisplayListHead = draw_box_fill(gDisplayListHead, arg0->column, arg0->row, arg0->column + 0x3F,
                                                 arg0->row + 0x11, 1, 1, 1, 0x000000FF);
                gDisplayListHead = render_menu_textures(
                    gDisplayListHead, segmented_to_virtual_dupe(D_800E8294[gCCSelection]), arg0->column, arg0->row);
                break;
            case MENU_ITEM_TYPE_069:
                func_800A8F48(arg0);
                break;
            case MENU_ITEM_TYPE_078:
            case MENU_ITEM_TYPE_079:
            case MENU_ITEM_TYPE_07A:
            case MENU_ITEM_TYPE_07B:
                var_a1 = arg0->type - 0x78;
                func_800A90D4(var_a1, arg0);
                func_800A143C(arg0, var_a1);
                break;
            case MENU_ITEM_TYPE_08C:
                if ((gMainMenuSelection >= MAIN_MENU_MODE_SUB_SELECT) && (var_a1 == (arg0->type - var_v1))) {
                    if (gMainMenuSelection > MAIN_MENU_MODE_SUB_SELECT) {
                        gDisplayListHead =
                            draw_box_fill(gDisplayListHead, arg0->column, arg0->row, arg0->column + 0x3F,
                                          arg0->row + 0x11, 0x000000FF, 0x000000F9, 0x000000DC, 0x000000FF);
                    } else {
                        gDisplayListHead = draw_flash_select_case_slow(
                            gDisplayListHead, arg0->column ^ 0, arg0->row ^ 0, arg0->column + 0x3F, arg0->row + 0x11);
                    }
                } else {
                    gDisplayListHead = draw_box_fill(gDisplayListHead, arg0->column, arg0->row, arg0->column + 0x3F,
                                                     arg0->row + 0x11, 1, 1, 1, 0x000000FF);
                }
                gDisplayListHead = render_menu_textures(gDisplayListHead, seg2_data_texture, arg0->column, arg0->row);
                set_text_color(TEXT_YELLOW);
                print_text1_left(0x00000125, 0x0000001C, gTextMenuData, 0, 0.55f, 0.55f);
                break;
            case MENU_ITEM_TYPE_08D:
                func_800A1780(arg0);
                break;
            case MENU_ITEM_TYPE_07C:
            case MENU_ITEM_TYPE_07D:
            case MENU_ITEM_TYPE_07E:
            case MENU_ITEM_TYPE_07F:
            case MENU_ITEM_TYPE_080:
            case MENU_ITEM_TYPE_081:
            case MENU_ITEM_TYPE_082:
            case MENU_ITEM_TYPE_083:
            case MENU_ITEM_TYPE_084:
            case MENU_ITEM_TYPE_085:
            case MENU_ITEM_TYPE_086:
            case MENU_ITEM_TYPE_087:
            case MENU_ITEM_TYPE_088:
            case MENU_ITEM_TYPE_089:
            case MENU_ITEM_TYPE_08A:
            case MENU_ITEM_TYPE_08B:
                func_800A15EC(arg0);
                break;
            case MENU_ITEM_TYPE_096:
                set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_1);
                print_text1_left(arg0->column, arg0->row, gCupNames[D_800DC540], arg0->param1, arg0->paramf, 1.0f);
                break;
            case MENU_ITEM_TYPE_097:
                set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_2);
                print_text_mode_1(arg0->column, arg0->row, gCourseNames[gCurrentCourseId], arg0->param1, arg0->paramf,
                                  1.0f);
                break;
            case MENU_ITEM_TYPE_098:
                func_800A2D1C(arg0);
                break;
            case MENU_ITEM_TYPE_05E:
                gDisplayListHead = func_80096CD8(gDisplayListHead, 0x00000019, 0x00000072, 0x0000007CU, 0x0000004AU);
                break;
            case MENU_ITEM_TYPE_0AA:
                // scissor
                gEXSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, 0, 0,
                              SCREEN_WIDTH, SCREEN_HEIGHT);
                // color
                gDPSetPrimColor(gDisplayListHead++, 0, 0, 0, 0, 0, 255);
                // black box in the top-right
                gEXTextureRectangle(gDisplayListHead++, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, (SCREEN_WIDTH / 2) * 4,
                                    0 * 4, SCREEN_WIDTH * 4, (SCREEN_HEIGHT / 2) * 4, 0, 0, 0, 0, 0);
                // black box in the bottom-left
                gEXTextureRectangle(gDisplayListHead++, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, 0 * 4,
                                    (SCREEN_HEIGHT / 2) * 4, (-SCREEN_WIDTH / 2) * 4, SCREEN_HEIGHT * 4, 0, 0, 0, 0,
                                    0);
                func_800A2EB8(arg0);
                break;
            case MENU_ITEM_TYPE_0AB:
                // scissor
                gEXSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, 0, 0,
                              SCREEN_WIDTH, SCREEN_HEIGHT);
                // color
                gDPSetPrimColor(gDisplayListHead++, 0, 0, 0, 0, 0, 255);
                // black box in the top-left
                gEXTextureRectangle(gDisplayListHead++, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, 0 * 4, 0 * 4,
                                    (-SCREEN_WIDTH / 2) * 4, (SCREEN_HEIGHT / 2) * 4, 0, 0, 0, 0, 0);
                // black box in the bottom-right
                gEXTextureRectangle(gDisplayListHead++, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, (SCREEN_WIDTH / 2) * 4,
                                    (SCREEN_HEIGHT / 2) * 4, SCREEN_WIDTH * 4, SCREEN_HEIGHT * 4, 0, 0, 0, 0, 0);

                func_800A34A8(arg0);

                break;
            case MENU_ITEM_TYPE_0AC:
                func_800A6154(arg0);
                break;
            case MENU_ITEM_TYPE_0AF:
                func_800A6034(arg0);
                break;
            case MENU_ITEM_TYPE_0B0:
                func_800A638C(arg0);
                break;
            case MENU_ITEM_TYPE_0B1:
            case MENU_ITEM_TYPE_0B2:
            case MENU_ITEM_TYPE_0B3:
            case MENU_ITEM_TYPE_0B4:
                if (arg0->state != 0) {
                    var_v1 = arg0->type - MENU_ITEM_TYPE_0B1;
                    gDisplayListHead = render_menu_textures(
                        gDisplayListHead,
                        segmented_to_virtual_dupe(D_800E7D54[D_800EFD64[gCharacterSelections[var_v1]]]), arg0->column,
                        arg0->row);
                    func_8009A7EC(arg0->D_8018DEE0_index, arg0->column, arg0->row, var_v1, arg0->param1);
                    render_cursor_player(arg0, var_v1, 0x000000FF);
                }
                break;
            case MENU_ITEM_TYPE_0B9:
                time_trials_finish_text_render(arg0);
                break;
            case MENU_ITEM_TYPE_0BA:
                func_800A3E60(arg0);
                break;
            case MENU_ITEM_ANNOUNCE_GHOST:
                render_menu_item_announce_ghost(arg0);
                break;
            case MENU_ITEM_PAUSE:
                render_pause_menu(arg0);
                break;
            case MENU_ITEM_END_COURSE_OPTION:
                render_menu_item_end_course_option(arg0);
                break;
            case MENU_ITEM_DATA_COURSE_IMAGE:
                render_menu_item_data_course_image(arg0);
                break;
            case MENU_ITEM_DATA_COURSE_INFO:
                render_menu_item_data_course_info(arg0);
                break;
            case MENU_ITEM_DATA_COURSE_SELECTABLE:
                menu_item_data_course_selectable(arg0);
                break;
            case MENU_ITEM_TYPE_0E9:
                func_800A1DE0(arg0);
                break;
            case MENU_ITEM_TYPE_0EA:
                func_800A1F30(arg0);
                break;
            case MENU_ITEM_TYPE_0F0:
                func_800A1FB0(arg0);
                break;
            case MENU_ITEM_TYPE_0F1:
                gDisplayListHead = render_menu_textures(gDisplayListHead, D_02004638, arg0->column, arg0->row);
                break;
            case MENU_ITEM_TYPE_10E:
                func_800A70E8(arg0);
                break;
            case MENU_ITEM_TYPE_12B:
                func_800A7258(arg0);
                break;
            case MENU_ITEM_TYPE_12C:
                func_800A72FC(arg0);
                break;
            case MENU_ITEM_TYPE_12D:
                func_800A7448(arg0);
                break;
            case MENU_ITEM_TYPE_12E:
                func_800A75A0(arg0);
                break;
            case MENU_ITEM_TYPE_12F:
                func_800A761C(arg0);
                break;
            case MENU_ITEM_TYPE_130:
                if (arg0->state != 0) {
                    gDisplayListHead = render_menu_textures(
                        gDisplayListHead, segmented_to_virtual_dupe(D_800E7D54[D_800EFD64[D_802874D8.unk1E]]),
                        arg0->column, arg0->row);
                    func_8009A7EC(arg0->D_8018DEE0_index, arg0->column, arg0->row, 0, arg0->param1);
                }
                break;
            case MENU_ITEM_TYPE_190:
            case MENU_ITEM_TYPE_191:
            case MENU_ITEM_TYPE_192:
            case MENU_ITEM_TYPE_193:
            case MENU_ITEM_TYPE_194:
            case MENU_ITEM_TYPE_195:
            case MENU_ITEM_TYPE_196:
            case MENU_ITEM_TYPE_197:
            case MENU_ITEM_TYPE_198:
            case MENU_ITEM_TYPE_199:
            case MENU_ITEM_TYPE_19A:
            case MENU_ITEM_TYPE_19B:
            case MENU_ITEM_TYPE_19C:
            case MENU_ITEM_TYPE_19D:
            case MENU_ITEM_TYPE_19E:
            case MENU_ITEM_TYPE_19F:
            case MENU_ITEM_TYPE_1A0:
            case MENU_ITEM_TYPE_1A1:
            case MENU_ITEM_TYPE_1A2:
            case MENU_ITEM_TYPE_1A3:
            case MENU_ITEM_TYPE_1A4:
            case MENU_ITEM_TYPE_1A5:
            case MENU_ITEM_TYPE_1A6:
            case MENU_ITEM_TYPE_1A7:
            case MENU_ITEM_TYPE_1A8:
            case MENU_ITEM_TYPE_1A9:
            case MENU_ITEM_TYPE_1AA:
            case MENU_ITEM_TYPE_1AB:
            case MENU_ITEM_TYPE_1AC:
            case MENU_ITEM_TYPE_1AD:
            case MENU_ITEM_TYPE_1AE:
            case MENU_ITEM_TYPE_1AF:
            case MENU_ITEM_TYPE_1B0:
            case MENU_ITEM_TYPE_1B1:
            case MENU_ITEM_TYPE_1B2:
            case MENU_ITEM_TYPE_1B3:
            case MENU_ITEM_TYPE_1B4:
            case MENU_ITEM_TYPE_1B5:
            case MENU_ITEM_TYPE_1B6:
            case MENU_ITEM_TYPE_1B7:
            case MENU_ITEM_TYPE_1B8:
            case MENU_ITEM_TYPE_1B9:
            case MENU_ITEM_TYPE_1BA:
            case MENU_ITEM_TYPE_1BB:
            case MENU_ITEM_TYPE_1BC:
            case MENU_ITEM_TYPE_1BD:
            case MENU_ITEM_TYPE_1BE:
            case MENU_ITEM_TYPE_1BF:
            case MENU_ITEM_TYPE_1C0:
            case MENU_ITEM_TYPE_1C1:
            case MENU_ITEM_TYPE_1C2:
            case MENU_ITEM_TYPE_1C3:
            case MENU_ITEM_TYPE_1C4:
            case MENU_ITEM_TYPE_1C5:
            case MENU_ITEM_TYPE_1C6:
            case MENU_ITEM_TYPE_1C7:
            case MENU_ITEM_TYPE_1C8:
            case MENU_ITEM_TYPE_1C9:
            case MENU_ITEM_TYPE_1CA:
            case MENU_ITEM_TYPE_1CB:
            case MENU_ITEM_TYPE_1CC:
            case MENU_ITEM_TYPE_1CD:
            case MENU_ITEM_TYPE_1CE:
                menu_item_credits_render(arg0);
                break;
        }
    }
}
#endif

#if 1
Gfx* func_8009BC9C_2(Gfx* arg0, MenuTexture* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, MenuItem* item) {
    MenuTexture* var_s0;
    u8* temp_v0_3;

    var_s0 = segmented_to_virtual_dupe(arg1);
    while (var_s0->textureData != NULL) {
        switch (var_s0->type) {
            case 0:
                gSPDisplayList(arg0++, (Gfx*) 0x2007708);
                break;
            case 1:
                gSPDisplayList(arg0++, (Gfx*) 0x2007728);
                break;
        }
        temp_v0_3 = (u8*) func_8009B8C4(var_s0->textureData);
        if (temp_v0_3 != 0) {
            switch (arg4) {
                case 1:
                    arg0 = func_80097AE4(arg0, 0, var_s0->dX + arg2, var_s0->dY + arg3, temp_v0_3, arg5);
                    break;
                case 2:
                    arg0 = func_80097E58(arg0, 0, 0, 0U, var_s0->width, var_s0->height, var_s0->dX + arg2,
                                         var_s0->dY + arg3, temp_v0_3, var_s0->width, var_s0->height, arg5);
                    break;
                case 3:
                    // Background
                    if (item == NULL)
                        goto defaultDraw;
                    switch (item->type) {
                        case 35:
                        case 36:
                        case 37:
                            arg0 = drawBackground(arg0, 0, 0, 0, var_s0->width, var_s0->height, var_s0->dX + arg2,
                                                  var_s0->dY + arg3, temp_v0_3, var_s0->width, var_s0->height);
                            break;
                        default:
                        defaultDraw:
                            arg0 = func_80097A14(arg0, 0, 0, 0, var_s0->width, var_s0->height, var_s0->dX + arg2,
                                                 var_s0->dY + arg3, temp_v0_3, var_s0->width, var_s0->height);
                            break;
                    }
                    break;
                case 4:
                    arg0 = func_80097274(arg0, 0, 0x00000400, 0x00000400, 0, 0, var_s0->width, var_s0->height,
                                         var_s0->dX + arg2, var_s0->dY + arg3, (u16*) temp_v0_3, var_s0->width,
                                         var_s0->height, arg5);
                    break;
            }
        }
        var_s0++;
    }
    return arg0;
}
#endif