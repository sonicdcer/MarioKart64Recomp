#include "patches.h"

#if 1
#include "menu_items.h"
#include "menus.h"
#include "sounds.h"

// Fixes a bug with the recompiler.
int dummy_val = 1;

bool recompDebugSwitch = 0;

Gfx common_square_plain_render[] = {
    gsSP1Triangle(0, 1, 2, 0),
    gsSP1Triangle(0, 2, 3, 0),
    gsSPEndDisplayList(),
};
Gfx common_setting_render_character[] = {
    gsDPPipeSync(),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPClearGeometryMode(G_LIGHTING),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsSPEndDisplayList(),
};

RECOMP_PATCH void handle_menus_with_pri_arg(s32 priSpecial) {
    s32 j = 0;
    s32 isRendered = 0;
    s32 i = 0;
    s32 type = 0;
    MenuItem* entry = NULL;
    volatile u8* address;

    // gDPSetTextureFilter(gDisplayListHead++, G_TF_POINT);
    gEXEnable(gDisplayListHead++); // @recomp
    gEXSetRefreshRate(gDisplayListHead++, 60 / 2);

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
                        // @recomp: Change gControllerFive to gControllers[0] to avoid bug where something
                        // is pressing other controllers in memory.
                        if (gControllers[0].button & R_TRIG) {
                            entry->state = 1;
                            play_sound2(SOUND_ACTION_PING);
                            // gGamestateNext = 4;
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
                    case CHARACTER_SELECT_MENU_BOWSER:gDPSetTextureFilter(gDisplayListHead++, G_TF_POINT);
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
#endif

#if 1
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
        switch (arg0->type) {             /* switch 6; irregular */
            case MENU_ITEM_UI_LOGO_INTRO: /* switch 6 */
                func_80094660(gGfxPool, arg0->param1);
                break;
            case START_MENU_FLAG: /* switch 6 */
                render_checkered_flag(gGfxPool, arg0->param1);
                break;
            case MENU_ITEM_TYPE_0D2: /* switch 6 */
                gDisplayListHead = func_8009B9D0(gDisplayListHead, D_020014C8);
                break;
            case MENU_ITEM_TYPE_0D3: /* switch 6 */
                gDisplayListHead = func_8009B9D0(gDisplayListHead, D_02001540);
                break;
            case MENU_ITEM_TYPE_0D4: /* switch 6 */
                func_800A09E0(arg0);
                func_800A0AD0(arg0);
                func_800A0B80(arg0);
                break;
            case MENU_ITEM_TYPE_0D5: /* switch 6 */
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
            case MENU_ITEM_TYPE_0D6: /* switch 6 */
                func_8009A76C(arg0->D_8018DEE0_index, arg0->column, arg0->row, -1);
                break;
            case MENU_ITEM_TYPE_0D7: /* switch 6 */
                func_800A0DFC();
                break;
            case MENU_ITEM_TYPE_0D8: /* switch 6 */
            case MENU_ITEM_TYPE_0D9: /* switch 6 */
                func_800A0EB8(arg0, arg0->type - 0xD8);
                break;
            case MENU_ITEM_UI_START_BACKGROUND: /* switch 6 */
                gDisplayListHead = RenderMenuTextures(
                    gDisplayListHead, gMenuTexturesBackground[has_unlocked_extra_mode()], arg0->column, arg0->row);
                break;
            case MENU_ITEM_UI_LOGO_AND_COPYRIGHT: /* switch 6 */
                render_game_logo((arg0->column + 0xA0), (arg0->row + 0x47));
                gDisplayListHead =
                    render_menu_textures(gDisplayListHead, seg2_copyright_1996_texture, arg0->column, arg0->row);
                break;
            case MENU_ITEM_UI_PUSH_START_BUTTON: /* switch 6 */
                if (((gGlobalTimer / 8) % 3) != 0) {
                    gDisplayListHead =
                        render_menu_textures(gDisplayListHead, seg2_push_start_button_texture, arg0->column, arg0->row);
                }
                break;
            case MENU_ITEM_UI_START_RECORD_TIME: /* switch 6 */
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
            case MENU_ITEM_UI_NO_CONTROLLER: /* switch 6 */
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
            case MAIN_MENU_BACKGROUND:        /* switch 6 */
            case CHARACTER_SELECT_BACKGROUND: /* switch 6 */
            case COURSE_SELECT_BACKGROUND:    /* switch 6 */
                gDisplayListHead = func_8009BC9C(gDisplayListHead, gMenuTexturesBackground[has_unlocked_extra_mode()],
                                                 arg0->column, arg0->row, 3, 0, arg0);
                break;
            case MENU_ITEM_UI_GAME_SELECT: /* switch 6 */
                gDisplayListHead =
                    render_menu_textures(gDisplayListHead, seg2_game_select_texture, arg0->column, arg0->row);
                break;
            case MENU_ITEM_UI_1P_GAME: /* switch 6 */
            case MENU_ITEM_UI_2P_GAME: /* switch 6 */
            case MENU_ITEM_UI_3P_GAME: /* switch 6 */
            case MENU_ITEM_UI_4P_GAME: /* switch 6 */
                var_a1 = arg0->type - 0xB;
                func_800A8270(var_a1, arg0);
                func_800A0FA4(arg0, var_a1);
                break;
            case MENU_ITEM_UI_OK: /* switch 6 */
                func_800A8564(arg0);
                gDisplayListHead =
                    func_8009BC9C(gDisplayListHead, D_0200487C, arg0->column, arg0->row, 2, arg0->param1, NULL);
                break;
            case MAIN_MENU_OPTION_GFX: /* switch 6 */
            case MAIN_MENU_DATA_GFX:   /* switch 6 */
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
                gDisplayListHead =
                    func_8009BC9C(gDisplayListHead, D_800E8254[var_v1], arg0->column, arg0->row, 2, arg0->param1, NULL);
                break;
            case MAIN_MENU_50CC:              /* switch 6 */
            case MAIN_MENU_100CC:             /* switch 6 */
            case MAIN_MENU_150CC:             /* switch 6 */
            case MAIN_MENU_EXTRA_CC:          /* switch 6 */
            case MENU_ITEM_TYPE_016:          /* switch 6 */
            case MENU_ITEM_TYPE_017:          /* switch 6 */
            case MAIN_MENU_TIME_TRIALS_BEGIN: /* switch 6 */
            case MAIN_MENU_TIME_TRIALS_DATA:  /* switch 6 */
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
            case MENU_ITEM_TYPE_01B: /* switch 6 */
                func_800A10CC(arg0);
                break;
            case CHARACTER_SELECT_MENU_PLAYER_SELECT_BANNER: /* switch 6 */
                gDisplayListHead = render_menu_textures(gDisplayListHead, D_02004B4C, arg0->column, arg0->row);
                break;
            case CHARACTER_SELECT_MENU_1P_CURSOR: /* switch 6 */
            case CHARACTER_SELECT_MENU_2P_CURSOR: /* switch 6 */
            case CHARACTER_SELECT_MENU_3P_CURSOR: /* switch 6 */
            case CHARACTER_SELECT_MENU_4P_CURSOR: /* switch 6 */
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
            case CHARACTER_SELECT_MENU_OK: /* switch 6 */
                func_800A8564(arg0);
                gDisplayListHead =
                    func_8009BC9C(gDisplayListHead, D_02004B74, arg0->column, arg0->row, 2, arg0->param1, NULL);
                break;
            case CHARACTER_SELECT_MENU_MARIO:  /* switch 6 */
            case CHARACTER_SELECT_MENU_LUIGI:  /* switch 6 */
            case CHARACTER_SELECT_MENU_TOAD:   /* switch 6 */
            case CHARACTER_SELECT_MENU_PEACH:  /* switch 6 */
            case CHARACTER_SELECT_MENU_YOSHI:  /* switch 6 */
            case CHARACTER_SELECT_MENU_DK:     /* switch 6 */
            case CHARACTER_SELECT_MENU_WARIO:  /* switch 6 */
            case CHARACTER_SELECT_MENU_BOWSER: /* switch 6 */
                func_800A12BC(arg0, segmented_to_virtual_dupe(D_800E7D54[arg0->type - 0x2B]));
                /* fallthrough */
            case MENU_ITEM_TYPE_0A0: /* switch 6 */
            case MENU_ITEM_TYPE_0A1: /* switch 6 */
                func_8009A76C(arg0->D_8018DEE0_index, arg0->column, arg0->row, arg0->param1);
                break;
            case MENU_ITEM_TYPE_058:         /* switch 6 */
            case COURSE_SELECT_COURSE_NAMES: /* switch 6 */
            case MENU_ITEM_TYPE_05A:         /* switch 6 */
            case MENU_ITEM_TYPE_05B:         /* switch 6 */
            case COURSE_SELECT_BATTLE_NAMES: /* switch 6 */
                func_800A8A98(arg0);
                gDisplayListHead = render_menu_textures(
                    gDisplayListHead,
                    segmented_to_virtual_dupe(gMenuTexturesTrackSelection[arg0->type - COURSE_SELECT_MAP_SELECT]),
                    arg0->column, arg0->row);
                func_800A8CA4(arg0);
                break;
            case COURSE_SELECT_MAP_SELECT: /* switch 6 */
                gDisplayListHead = render_menu_textures(
                    gDisplayListHead,
                    segmented_to_virtual_dupe(gMenuTexturesTrackSelection[arg0->type - COURSE_SELECT_MAP_SELECT]),
                    arg0->column, arg0->row);
                break;
            case MENU_ITEM_TYPE_05F: /* switch 6 */
            case MENU_ITEM_TYPE_060: /* switch 6 */
            case MENU_ITEM_TYPE_061: /* switch 6 */
            case MENU_ITEM_TYPE_062: /* switch 6 */
                func_800A1500(arg0);
                break;
            case COURSE_SELECT_MUSHROOM_CUP: /* switch 6 */
            case COURSE_SELECT_FLOWER_CUP:   /* switch 6 */
            case COURSE_SELECT_STAR_CUP:     /* switch 6 */
            case COURSE_SELECT_SPECIAL_CUP:  /* switch 6 */
                var_a1 = arg0->type - 0x53;
                func_800A890C(var_a1, arg0);
                func_800A143C(arg0, var_a1);
                break;
            case COURSE_SELECT_OK: /* switch 6 */
                func_800A8564(arg0);
                gDisplayListHead =
                    func_8009BC9C(gDisplayListHead, D_02004E80, arg0->column, arg0->row, 2, arg0->param1, NULL);
                break;
            case MENU_ITEM_TYPE_065: /* switch 6 */
            case MENU_ITEM_TYPE_066: /* switch 6 */
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
            case MENU_ITEM_TYPE_06E: /* switch 6 */
                render_battle_introduction(arg0);
                break;
            case MENU_ITEM_TYPE_067: /* switch 6 */
                func_800A8EC0(arg0);
                break;
            case MENU_ITEM_TYPE_068: /* switch 6 */
                gDisplayListHead = draw_box_fill(gDisplayListHead, arg0->column, arg0->row, arg0->column + 0x3F,
                                                 arg0->row + 0x11, 1, 1, 1, 0x000000FF);
                gDisplayListHead = render_menu_textures(
                    gDisplayListHead, segmented_to_virtual_dupe(D_800E8294[gCCSelection]), arg0->column, arg0->row);
                break;
            case MENU_ITEM_TYPE_069: /* switch 6 */
                func_800A8F48(arg0);
                break;
            case MENU_ITEM_TYPE_078: /* switch 6 */
            case MENU_ITEM_TYPE_079: /* switch 6 */
            case MENU_ITEM_TYPE_07A: /* switch 6 */
            case MENU_ITEM_TYPE_07B: /* switch 6 */
                var_a1 = arg0->type - 0x78;
                func_800A90D4(var_a1, arg0);
                func_800A143C(arg0, var_a1);
                break;
            case MENU_ITEM_TYPE_08C: /* switch 6 */
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
            case MENU_ITEM_TYPE_08D: /* switch 6 */
                func_800A1780(arg0);
                break;
            case MENU_ITEM_TYPE_07C: /* switch 6 */
            case MENU_ITEM_TYPE_07D: /* switch 6 */
            case MENU_ITEM_TYPE_07E: /* switch 6 */
            case MENU_ITEM_TYPE_07F: /* switch 6 */
            case MENU_ITEM_TYPE_080: /* switch 6 */
            case MENU_ITEM_TYPE_081: /* switch 6 */
            case MENU_ITEM_TYPE_082: /* switch 6 */
            case MENU_ITEM_TYPE_083: /* switch 6 */
            case MENU_ITEM_TYPE_084: /* switch 6 */
            case MENU_ITEM_TYPE_085: /* switch 6 */
            case MENU_ITEM_TYPE_086: /* switch 6 */
            case MENU_ITEM_TYPE_087: /* switch 6 */
            case MENU_ITEM_TYPE_088: /* switch 6 */
            case MENU_ITEM_TYPE_089: /* switch 6 */
            case MENU_ITEM_TYPE_08A: /* switch 6 */
            case MENU_ITEM_TYPE_08B: /* switch 6 */
                func_800A15EC(arg0);
                break;
            case MENU_ITEM_TYPE_096: /* switch 6 */
                set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_1);
                print_text1_left(arg0->column, arg0->row, gCupNames[D_800DC540], arg0->param1, arg0->paramf, 1.0f);
                break;
            case MENU_ITEM_TYPE_097: /* switch 6 */
                set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_2);
                print_text_mode_1(arg0->column, arg0->row, gCourseNames[gCurrentCourseId], arg0->param1, arg0->paramf,
                                  1.0f);
                break;
            case MENU_ITEM_TYPE_098: /* switch 6 */
                func_800A2D1C(arg0);
                break;
            case MENU_ITEM_TYPE_05E: /* switch 6 */
                gDisplayListHead = func_80096CD8(gDisplayListHead, 0x00000019, 0x00000072, 0x0000007CU, 0x0000004AU);
                break;
            case MENU_ITEM_TYPE_0AA: /* switch 6 */
                func_800A2EB8(arg0);
                break;
            case MENU_ITEM_TYPE_0AB: /* switch 6 */
                func_800A34A8(arg0);
                break;
            case MENU_ITEM_TYPE_0AC: /* switch 6 */
                func_800A6154(arg0);
                break;
            case MENU_ITEM_TYPE_0AF: /* switch 6 */
                func_800A6034(arg0);
                break;
            case MENU_ITEM_TYPE_0B0: /* switch 6 */
                func_800A638C(arg0);
                break;
            case MENU_ITEM_TYPE_0B1: /* switch 6 */
            case MENU_ITEM_TYPE_0B2: /* switch 6 */
            case MENU_ITEM_TYPE_0B3: /* switch 6 */
            case MENU_ITEM_TYPE_0B4: /* switch 6 */
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
            case MENU_ITEM_TYPE_0B9: /* switch 6 */
                time_trials_finish_text_render(arg0);
                break;
            case MENU_ITEM_TYPE_0BA: /* switch 6 */
                func_800A3E60(arg0);
                break;
            case MENU_ITEM_ANNOUNCE_GHOST: /* switch 6 */
                render_menu_item_announce_ghost(arg0);
                break;
            case MENU_ITEM_PAUSE: /* switch 6 */
                render_pause_menu(arg0);
                break;
            case MENU_ITEM_END_COURSE_OPTION: /* switch 6 */
                render_menu_item_end_course_option(arg0);
                break;
            case MENU_ITEM_DATA_COURSE_IMAGE: /* switch 6 */
                render_menu_item_data_course_image(arg0);
                break;
            case MENU_ITEM_DATA_COURSE_INFO: /* switch 6 */
                render_menu_item_data_course_info(arg0);
                break;
            case MENU_ITEM_DATA_COURSE_SELECTABLE: /* switch 6 */
                menu_item_data_course_selectable(arg0);
                break;
            case MENU_ITEM_TYPE_0E9: /* switch 6 */
                func_800A1DE0(arg0);
                break;
            case MENU_ITEM_TYPE_0EA: /* switch 6 */
                func_800A1F30(arg0);
                break;
            case MENU_ITEM_TYPE_0F0: /* switch 6 */
                func_800A1FB0(arg0);
                break;
            case MENU_ITEM_TYPE_0F1: /* switch 6 */
                gDisplayListHead = render_menu_textures(gDisplayListHead, D_02004638, arg0->column, arg0->row);
                break;
            case MENU_ITEM_TYPE_10E: /* switch 6 */
                func_800A70E8(arg0);
                break;
            case MENU_ITEM_TYPE_12B: /* switch 6 */
                func_800A7258(arg0);
                break;
            case MENU_ITEM_TYPE_12C: /* switch 6 */
                func_800A72FC(arg0);
                break;
            case MENU_ITEM_TYPE_12D: /* switch 6 */
                func_800A7448(arg0);
                break;
            case MENU_ITEM_TYPE_12E: /* switch 6 */
                func_800A75A0(arg0);
                break;
            case MENU_ITEM_TYPE_12F: /* switch 6 */
                func_800A761C(arg0);
                break;
            case MENU_ITEM_TYPE_130: /* switch 6 */
                if (arg0->state != 0) {
                    gDisplayListHead = render_menu_textures(
                        gDisplayListHead, segmented_to_virtual_dupe(D_800E7D54[D_800EFD64[D_802874D8.unk1E]]),
                        arg0->column, arg0->row);
                    func_8009A7EC(arg0->D_8018DEE0_index, arg0->column, arg0->row, 0, arg0->param1);
                }
                break;
            case MENU_ITEM_TYPE_190: /* switch 6 */
            case MENU_ITEM_TYPE_191: /* switch 6 */
            case MENU_ITEM_TYPE_192: /* switch 6 */
            case MENU_ITEM_TYPE_193: /* switch 6 */
            case MENU_ITEM_TYPE_194: /* switch 6 */
            case MENU_ITEM_TYPE_195: /* switch 6 */
            case MENU_ITEM_TYPE_196: /* switch 6 */
            case MENU_ITEM_TYPE_197: /* switch 6 */
            case MENU_ITEM_TYPE_198: /* switch 6 */
            case MENU_ITEM_TYPE_199: /* switch 6 */
            case MENU_ITEM_TYPE_19A: /* switch 6 */
            case MENU_ITEM_TYPE_19B: /* switch 6 */
            case MENU_ITEM_TYPE_19C: /* switch 6 */
            case MENU_ITEM_TYPE_19D: /* switch 6 */
            case MENU_ITEM_TYPE_19E: /* switch 6 */
            case MENU_ITEM_TYPE_19F: /* switch 6 */
            case MENU_ITEM_TYPE_1A0: /* switch 6 */
            case MENU_ITEM_TYPE_1A1: /* switch 6 */
            case MENU_ITEM_TYPE_1A2: /* switch 6 */
            case MENU_ITEM_TYPE_1A3: /* switch 6 */
            case MENU_ITEM_TYPE_1A4: /* switch 6 */
            case MENU_ITEM_TYPE_1A5: /* switch 6 */
            case MENU_ITEM_TYPE_1A6: /* switch 6 */
            case MENU_ITEM_TYPE_1A7: /* switch 6 */
            case MENU_ITEM_TYPE_1A8: /* switch 6 */
            case MENU_ITEM_TYPE_1A9: /* switch 6 */
            case MENU_ITEM_TYPE_1AA: /* switch 6 */
            case MENU_ITEM_TYPE_1AB: /* switch 6 */
            case MENU_ITEM_TYPE_1AC: /* switch 6 */
            case MENU_ITEM_TYPE_1AD: /* switch 6 */
            case MENU_ITEM_TYPE_1AE: /* switch 6 */
            case MENU_ITEM_TYPE_1AF: /* switch 6 */
            case MENU_ITEM_TYPE_1B0: /* switch 6 */
            case MENU_ITEM_TYPE_1B1: /* switch 6 */
            case MENU_ITEM_TYPE_1B2: /* switch 6 */
            case MENU_ITEM_TYPE_1B3: /* switch 6 */
            case MENU_ITEM_TYPE_1B4: /* switch 6 */
            case MENU_ITEM_TYPE_1B5: /* switch 6 */
            case MENU_ITEM_TYPE_1B6: /* switch 6 */
            case MENU_ITEM_TYPE_1B7: /* switch 6 */
            case MENU_ITEM_TYPE_1B8: /* switch 6 */
            case MENU_ITEM_TYPE_1B9: /* switch 6 */
            case MENU_ITEM_TYPE_1BA: /* switch 6 */
            case MENU_ITEM_TYPE_1BB: /* switch 6 */
            case MENU_ITEM_TYPE_1BC: /* switch 6 */
            case MENU_ITEM_TYPE_1BD: /* switch 6 */
            case MENU_ITEM_TYPE_1BE: /* switch 6 */
            case MENU_ITEM_TYPE_1BF: /* switch 6 */
            case MENU_ITEM_TYPE_1C0: /* switch 6 */
            case MENU_ITEM_TYPE_1C1: /* switch 6 */
            case MENU_ITEM_TYPE_1C2: /* switch 6 */
            case MENU_ITEM_TYPE_1C3: /* switch 6 */
            case MENU_ITEM_TYPE_1C4: /* switch 6 */
            case MENU_ITEM_TYPE_1C5: /* switch 6 */
            case MENU_ITEM_TYPE_1C6: /* switch 6 */
            case MENU_ITEM_TYPE_1C7: /* switch 6 */
            case MENU_ITEM_TYPE_1C8: /* switch 6 */
            case MENU_ITEM_TYPE_1C9: /* switch 6 */
            case MENU_ITEM_TYPE_1CA: /* switch 6 */
            case MENU_ITEM_TYPE_1CB: /* switch 6 */
            case MENU_ITEM_TYPE_1CC: /* switch 6 */
            case MENU_ITEM_TYPE_1CD: /* switch 6 */
            case MENU_ITEM_TYPE_1CE: /* switch 6 */
                menu_item_credits_render(arg0);
                break;
        }
    }
}
#endif

#if 1
RECOMP_PATCH Gfx* func_8009BC9C(Gfx* arg0, MenuTexture* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, MenuItem* item) {
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

Gfx* drawBackground(Gfx* displayListHead, s8 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, u8* arg8,
                    u32 arg9, u32 argA) {
    gDPPipeSync(displayListHead++);
    gDPSetCycleType(displayListHead++, G_CYC_COPY);
    displayListHead = drawBackground2(displayListHead, arg1, 0x00001000, 0x00000400, arg2, arg3, arg4, arg5, arg6, arg7,
                                      arg8, arg9, argA);
    gDPPipeSync(displayListHead++);
    gDPSetCycleType(displayListHead++, G_CYC_1CYCLE);
    return displayListHead;
}

Gfx* RenderMenuTextures(Gfx* arg0, MenuTexture* arg1, s32 column, s32 row) {
    MenuTexture* temp_v0;
    u8* temp_v0_3;
    s8 var_s4;

    temp_v0 = segmented_to_virtual_dupe(arg1);
    while (temp_v0->textureData != NULL) {
        var_s4 = 0;
        switch (temp_v0->type) {
            case 0:
                gSPDisplayList(arg0++, 0x02007708);
                break;
            case 1:
                gSPDisplayList(arg0++, 0x02007728);
                break;
            case 2:
                gSPDisplayList(arg0++, 0x02007748);
                break;
            case 3:
                gSPDisplayList(arg0++, 0x02007768);
                var_s4 = 3;
                break;
            case 4:
                gSPDisplayList(arg0++, 0x02007788);
                break;
            default:
                gSPDisplayList(arg0++, 0x02007728);
                break;
        }
        temp_v0_3 = (u8*) func_8009B8C4(temp_v0->textureData);
        if (temp_v0_3 != 0) {
            if (D_8018E7AC[4] != 4) {
                arg0 =
                    RenderBackground(arg0, var_s4, 0x00000400, 0x00000400, 0, 0, temp_v0->width, temp_v0->height,
                                  temp_v0->dX + column, temp_v0->dY + row, temp_v0_3, temp_v0->width, temp_v0->height);
            } else {
                arg0 = func_800987D0(arg0, 0U, 0U, temp_v0->width, temp_v0->height, temp_v0->dX + column,
                                     temp_v0->dY + row, temp_v0_3, temp_v0->width, temp_v0->height);
            }
        }
        temp_v0++;
    }
    return arg0;
}

Gfx* RenderBackground(Gfx* displayListHead, s8 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9, u8* argA, u32 argB, u32 argC) {
    u32 var_a1_2 = arg4;
    u32 var_s3 = arg5;
    s32 sp7C;
    u32 var_s2;
    u32 var_s4;
    s32 var_t0 = 1;
    s32 temp_lo;
    s32 sp68 = 0;
    s32 sp64 = 0;
    s32 var_v0_2;

    while ((u32) var_t0 < argB) {
        var_t0 *= 2;
    }

    temp_lo = 0x400 / var_t0;

    while ((u32) (temp_lo / 2) > argC) {
        temp_lo /= 2;
    }

    var_v0_2 = var_t0;
    while (var_v0_2 > 1) {
        var_v0_2 /= 2;
        sp68 += 1;
    }
    var_v0_2 = temp_lo;

    while (var_v0_2 > 1) {
        var_v0_2 /= 2;
        sp64 += 1;
    }

    if (arg8 < 0) {
        arg4 -= arg8;
        arg8 = 0;
    } else if (((arg6 - arg4) + arg8) > SCREEN_WIDTH) {
        arg6 = (arg4 - arg8) + SCREEN_WIDTH;
    }

    if (arg9 < 0) {
        arg5 -= arg9;
        arg9 = 0;
    } else if (((arg7 - arg5) + arg9) > SCREEN_HEIGHT) {
        arg7 = (arg5 - arg9) + SCREEN_HEIGHT;
    }

    if (arg6 < arg4) {
        return displayListHead;
    }
    if (arg7 < arg5) {
        return displayListHead;
    }
    sp7C = arg8;
    for (var_s3 = arg5; var_s3 < (u32) arg7; var_s3 += temp_lo) {

        if ((u32) arg7 < temp_lo + var_s3) {
            var_s4 = arg7 - var_s3;
            if (!var_s4) {
                break;
            }
        } else {
            var_s4 = temp_lo;
        }

        for (var_a1_2 = arg4; var_a1_2 < (u32) arg6; var_a1_2 += var_t0) {

            if ((u32) arg6 < var_t0 + var_a1_2) {
                var_s2 = arg6 - var_a1_2;
                if (!var_s2) {
                    break;
                }
            } else {
                var_s2 = var_t0;
            }
            gDPLoadTextureTile(displayListHead++, argA, arg1, G_IM_SIZ_16b, argB, 0, var_a1_2, var_s3,
                               var_a1_2 + var_s2, var_s3 + var_s4, 0, G_TX_NOMIRROR | G_TX_WRAP,
                               G_TX_NOMIRROR | G_TX_WRAP, sp68, sp64, G_TX_NOLOD, G_TX_NOLOD);
            gEXTextureRectangle(displayListHead++, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, arg8 * 4, arg9 * 4, (arg8 + var_s2) * 4, (arg9 + var_s4) * 4, 0,
                                (var_a1_2 * 32) & 0xFFFF, (var_s3 * 32) & 0xFFFF, arg2, arg3);

            arg8 += var_t0;
        }

        arg8 = sp7C;
        arg9 += temp_lo;
    }
    return displayListHead;
}

// Balloons
#if 1
Vtx common_vtx_hedgehog[] = {
    { { { -32, -31, 0 }, 0, { 0, 0 }, { 255, 255, 255, 255 } } },
    { { { 31, -31, 0 }, 0, { 4032, 0 }, { 255, 255, 255, 255 } } },
    { { { 31, 0, 0 }, 0, { 4032, 1984 }, { 255, 255, 255, 255 } } },
    { { { -32, 0, 0 }, 0, { 0, 1984 }, { 255, 255, 255, 255 } } },
    { { { -32, 0, 0 }, 0, { 0, 0 }, { 255, 255, 255, 255 } } },
    { { { 31, 0, 0 }, 0, { 4032, 0 }, { 255, 255, 255, 255 } } },
    { { { 31, 31, 0 }, 0, { 4032, 1984 }, { 255, 255, 255, 255 } } },
    { { { -32, 31, 0 }, 0, { 0, 1984 }, { 255, 255, 255, 255 } } },
};

Gfx common_rectangle_display[] = {
    gsSP2Triangles(0, 2, 1, 0, 0, 3, 2, 0),
    gsSPEndDisplayList(),
};

extern Object gObjectList[];

RECOMP_PATCH void func_80053D74(s32 objectIndex, UNUSED s32 arg1, s32 vertexIndex) {
    Object* object;
    int i = 0;

    if (gMatrixHudCount <= MTX_HUD_POOL_SIZE_MAX) {
        object = &gObjectList[objectIndex];
        D_80183E80[2] = (s16) (object->unk_084[6] + 0x8000);

        gEXEnable(gDisplayListHead++);

        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gDisplayListHead++, (objectIndex << 8) + i++, G_EX_PUSH, G_MTX_MODELVIEW,
                                       G_EX_EDIT_ALLOW);

        rsp_set_matrix_transformation(object->pos, (u16*) D_80183E80, object->sizeScaling);
        set_color_render((s32) object->unk_084[0], (s32) object->unk_084[1], (s32) object->unk_084[2],
                         (s32) object->unk_084[3], (s32) object->unk_084[4], (s32) object->unk_084[5],
                         (s32) object->primAlpha);
        gSPVertex(gDisplayListHead++, &common_vtx_hedgehog[vertexIndex], 4, 0);
        gSPDisplayList(gDisplayListHead++, common_rectangle_display);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
}
#endif

RECOMP_PATCH void func_8028FC34(void) {
    if (D_802BA038 < 0) {
        D_802BA038 = 1920;
        return;
    }
    D_802BA038--;
    // @recomp: Change gControllerFive to gControllers[0] to avoid bug where something
    // is pressing other controllers in memory.
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

// Blur characters
#if 1

RECOMP_PATCH void render_kart(Player* player, s8 playerId, s8 arg2, s8 arg3) {
    UNUSED s32 pad;
    Mat4 sp1A4;
    UNUSED s32 pad2[17];
    Vec3f sp154;
    Vec3s sp14C;
    f32 sp148;
    f32 sp144;
    f32 sp140;
    s16 temp_v1;
    s16 thing;

    if (player->unk_044 & 0x2000) {
        sp14C[0] = 0;
        sp14C[1] = player->unk_048[arg2];
        sp14C[2] = 0;
        func_80062B18(&sp148, &sp144, &sp140, 0.0f, 1.5f, 0.0f, -player->unk_048[arg2], player->unk_050[arg2]);
        sp154[1] = (player->pos[1] - player->boundingBoxSize) + (sp144 - 2.0);
        sp154[0] = player->pos[0] + sp148;
        sp154[2] = player->pos[2] + sp140;
    } else {
        thing = (u16) (player->unk_048[arg2] + player->rotation[1] + player->unk_0C0);
        temp_v1 = player->unk_0CC[arg2] * sins(thing);
        if ((player->effects & 8) == 8) {
            sp14C[0] = cameras[arg2].rot[0] - 0x4000;
        } else {
            sp14C[0] = -temp_v1 * 0.8;
        }
        sp14C[1] = player->unk_048[arg2];
        sp14C[2] = player->unk_050[arg2];
        if (((s32) player->effects & HIT_EFFECT) == HIT_EFFECT) {
            func_80062B18(&sp148, &sp144, &sp140, 0.0f, 8.0f, 0.0f, -player->unk_048[arg2], player->unk_050[arg2]);
            sp154[1] = (player->pos[1] - player->boundingBoxSize) + player->unk_108;
            sp154[0] = player->pos[0] + sp148;
            sp154[2] = player->pos[2] + sp140;
        } else {
            func_80062B18(&sp148, &sp144, &sp140, 0.0f, 1.5f, 0.0f, -player->unk_048[arg2], player->unk_050[arg2]);
            sp154[1] = (player->pos[1] - player->boundingBoxSize) + player->unk_108 + (sp144 - 2.0);
            sp154[0] = player->pos[0] + sp148;
            sp154[2] = player->pos[2] + sp140;
        }
    }
#ifdef AVOID_UB
    gPlayerPalette = &gPlayerPalettesList[D_801651D0[arg2][playerId]][arg2][playerId];
#else
    gPlayerPalette = (struct_D_802F1F80*) &gPlayerPalettesList[D_801651D0[arg2][playerId]][arg2][playerId * 0x100];
#endif
    if ((arg2 == 0) || (arg2 == 1)) {
        sKartUpperTexture = &D_802BFB80.arraySize8[D_801651D0[arg2][playerId]][arg2][playerId].pixel_index_array[0];
        sKartLowerTexture = &D_802BFB80.arraySize8[D_801651D0[arg2][playerId]][arg2][playerId].pixel_index_array[0x7C0];
    } else {
        sKartUpperTexture =
            &D_802BFB80.arraySize8[D_801651D0[arg2][playerId]][arg2 - 1][playerId - 4].pixel_index_array[0];
        sKartLowerTexture =
            &D_802BFB80.arraySize8[D_801651D0[arg2][playerId]][arg2 - 1][playerId - 4].pixel_index_array[0x7C0];
    }
    mtxf_translate_rotate(sp1A4, sp154, sp14C);
    mtxf_scale2(sp1A4, gCharacterSize[player->characterId] * player->size);
    convert_to_fixed_point_matrix(&gGfxPool->mtxKart[playerId + (arg2 * 8)], sp1A4);

    if ((player->effects & BOO_EFFECT) == BOO_EFFECT) {
        if (arg2 == playerId) {
            gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxKart[playerId + (arg2 * 8)]),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(gDisplayListHead++, common_setting_render_character);
            gDPLoadTLUT_pal256(gDisplayListHead++, gPlayerPalette);
            gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
            func_8004B614(gPlayerRedEffect[playerId], gPlayerGreenEffect[playerId], gPlayerBlueEffect[playerId],
                          gPlayerCyanEffect[playerId], gPlayerMagentaEffect[playerId], gPlayerYellowEffect[playerId],
                          (s32) player->unk_0C6);
            gDPSetRenderMode(gDisplayListHead++,
                             AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                                 GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA),
                             AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                                 GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA));
        } else {
            gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxKart[playerId + (arg2 * 8)]),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(gDisplayListHead++, common_setting_render_character);
            gDPLoadTLUT_pal256(gDisplayListHead++, gPlayerPalette);
            gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
            func_8004B614(gPlayerRedEffect[playerId], gPlayerGreenEffect[playerId], gPlayerBlueEffect[playerId],
                          gPlayerCyanEffect[playerId], gPlayerMagentaEffect[playerId], gPlayerYellowEffect[playerId],
                          D_8018D970[playerId]);
            gDPSetRenderMode(gDisplayListHead++,
                             AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                                 GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA),
                             AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                                 GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA));
        }
    } else if (((player->unk_0CA & 4) == 4) || (player->soundEffects & 0x08000000) ||
               (player->soundEffects & 0x04000000)) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxKart[playerId + (arg2 * 8)]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gDisplayListHead++, common_setting_render_character);
        gDPLoadTLUT_pal256(gDisplayListHead++, gPlayerPalette);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
        func_8004B614(gPlayerRedEffect[playerId], gPlayerGreenEffect[playerId], gPlayerBlueEffect[playerId],
                      gPlayerCyanEffect[playerId], gPlayerMagentaEffect[playerId], gPlayerYellowEffect[playerId],
                      (s32) player->unk_0C6);
        gDPSetAlphaCompare(gDisplayListHead++, G_AC_DITHER);
        gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_XLU_SURF, G_RM_ZB_XLU_SURF2);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxKart[playerId + (arg2 * 8)]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gDisplayListHead++, common_setting_render_character);
        gDPLoadTLUT_pal256(gDisplayListHead++, gPlayerPalette);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
        func_8004B614(gPlayerRedEffect[playerId], gPlayerGreenEffect[playerId], gPlayerBlueEffect[playerId],
                      gPlayerCyanEffect[playerId], gPlayerMagentaEffect[playerId], gPlayerYellowEffect[playerId],
                      (s32) player->unk_0C6);

        // @recomp: blured kart looks better.
        // gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
        gDPSetRenderMode(gDisplayListHead++,
                         AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                             GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA),
                         AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                             GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA));
    }

    gDPLoadTextureBlock(gDisplayListHead++, sKartUpperTexture, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(gDisplayListHead++, &D_800DDBB4[playerId][arg3], 4, 0);
    gSPDisplayList(gDisplayListHead++, common_square_plain_render);

    gDPLoadTextureBlock(gDisplayListHead++, sKartLowerTexture, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(gDisplayListHead++, &D_800DDBB4[playerId][arg3 + 4], 4, 0);
    gSPDisplayList(gDisplayListHead++, common_square_plain_render);
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
    gDPSetAlphaCompare(gDisplayListHead++, G_AC_NONE);
}
#endif

#if 0
Gfx D_02007708[] = {
    gsDPPipeSync(),
    gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
    gsSPEndDisplayList(),
};
Gfx D_02007728[] = {
    gsDPPipeSync(),
    gsDPSetRenderMode(G_RM_XLU_SURF, G_RM_XLU_SURF2),
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
    gsSPEndDisplayList(),
};
Gfx D_02007768[] = {
    gsDPPipeSync(),
    gsDPSetRenderMode(G_RM_XLU_SURF, G_RM_XLU_SURF2),
    gsDPSetCombineMode(G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM),
    gsSPEndDisplayList(),
};

RECOMP_PATCH Gfx* func_8009C434(Gfx* arg0, struct_8018DEE0_entry* arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 var_t0;
    s32 var_t1;
    Gfx* temp;
    MenuTexture* var_s0 = segmented_to_virtual_dupe(arg1->textureSequence[arg1->sequenceIndex].mk64Texture);
    
    temp = D_02007728;
    while (var_s0->textureData != NULL) {
        var_t1 = 0;
        switch (var_s0->type) {
            default:
                gSPDisplayList(arg0++, D_02007728);
                break;
            case 0:
                gSPDisplayList(arg0++, D_02007708);
                break;
            case 1:
                gSPDisplayList(arg0++, D_02007728);
                break;
            case 3:
                gSPDisplayList(arg0++, D_02007768);
                var_t1 = 3;
                break;
        }

        if (arg1->unk14 != 0) {
            var_t0 = sMenuTextureMap[arg1->menuTextureIndex + 1].offset;
        } else {
            var_t0 = sMenuTextureMap[arg1->menuTextureIndex].offset;
            if (1) {}
            if (1) {}
            if (1) {}
        }
        
        if (arg4 >= 0) {
            arg0 =
                func_80097E58(arg0, var_t1, 0, 0U, var_s0->width, var_s0->height, var_s0->dX + arg2, var_s0->dY + arg3,
                              (u8*) &gMenuTextureBuffer[var_t0], var_s0->width, var_s0->height, (u32) arg4);
        } else {
            switch (arg4) {
                case -1:
                    arg0 = func_80095E10(arg0, var_t1, 0x00000400, 0x00000400, 0, 0, var_s0->width, var_s0->height,
                                         var_s0->dX + arg2, var_s0->dY + arg3, (u8*) &gMenuTextureBuffer[var_t0],
                                         var_s0->width, var_s0->height);
                    break;
                case -2:
                    arg0 = func_800963F0(arg0, var_t1, 0x00000400, 0x00000400, 0.5f, 0.5f, 0, 0, var_s0->width,
                                         var_s0->height, var_s0->dX + arg2, var_s0->dY + arg3,
                                         (u8*) &gMenuTextureBuffer[var_t0], var_s0->width, var_s0->height);
                    break;
            }
        }
        var_s0++;
    }
    return arg0;
}
#endif

#if 1
Gfx* drawBackground2(Gfx* displayListHead, s8 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7,
                     s32 arg8, s32 arg9, u8* argA, u32 argB, u32 argC) {
    u32 var_a1_2 = arg4;
    u32 var_s3 = arg5;
    s32 sp7C;
    u32 var_s2;
    u32 var_s4;
    s32 var_t0 = 1;
    s32 temp_lo;
    s32 sp68 = 0;
    s32 sp64 = 0;
    s32 var_v0_2;

    while ((u32) var_t0 < argB) {
        var_t0 *= 2;
    }

    temp_lo = 0x400 / var_t0;

    while ((u32) (temp_lo / 2) > argC) {
        temp_lo /= 2;
    }

    var_v0_2 = var_t0;
    while (var_v0_2 > 1) {
        var_v0_2 /= 2;
        sp68 += 1;
    }
    var_v0_2 = temp_lo;

    while (var_v0_2 > 1) {
        var_v0_2 /= 2;
        sp64 += 1;
    }

    if (arg8 < 0) {
        arg4 -= arg8;
        arg8 = 0;
    } else if (((arg6 - arg4) + arg8) > SCREEN_WIDTH) {
        arg6 = (arg4 - arg8) + SCREEN_WIDTH;
    }

    if (arg9 < 0) {
        arg5 -= arg9;
        arg9 = 0;
    } else if (((arg7 - arg5) + arg9) > SCREEN_HEIGHT) {
        arg7 = (arg5 - arg9) + SCREEN_HEIGHT;
    }

    if (arg6 < arg4) {
        return displayListHead;
    }
    if (arg7 < arg5) {
        return displayListHead;
    }
    sp7C = arg8;
    for (var_s3 = arg5; var_s3 < (u32) arg7; var_s3 += temp_lo) {

        if ((u32) arg7 < temp_lo + var_s3) {
            var_s4 = arg7 - var_s3;
            if (!var_s4) {
                break;
            }
        } else {
            var_s4 = temp_lo;
        }

        for (var_a1_2 = arg4; var_a1_2 < (u32) arg6; var_a1_2 += var_t0) {

            if ((u32) arg6 < var_t0 + var_a1_2) {
                var_s2 = arg6 - var_a1_2;
                if (!var_s2) {
                    break;
                }
            } else {
                var_s2 = var_t0;
            }
            gDPLoadTextureTile(displayListHead++, argA, arg1, G_IM_SIZ_16b, argB, 0, var_a1_2, var_s3,
                               var_a1_2 + var_s2, var_s3 + var_s4, 0, G_TX_NOMIRROR | G_TX_WRAP,
                               G_TX_NOMIRROR | G_TX_WRAP, sp68, sp64, G_TX_NOLOD, G_TX_NOLOD);
            gEXTextureRectangle(displayListHead++, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, arg8 * 4, arg9 * 4,
                                (arg8 + var_s2) * 4, (arg9 + var_s4) * 4, 0, (var_a1_2 * 32) & 0xFFFF,
                                (var_s3 * 32) & 0xFFFF, arg2, arg3);

            arg8 += var_t0;
        }

        arg8 = sp7C;
        arg9 += temp_lo;
    }
    return displayListHead;
}
#endif

#if 0
extern u16 gFramebuffer0[SCREEN_WIDTH * SCREEN_HEIGHT];
extern u16 gFramebuffer1[SCREEN_WIDTH * SCREEN_HEIGHT];
extern u16 gFramebuffer2[SCREEN_WIDTH * SCREEN_HEIGHT];

RECOMP_PATCH void thread3_video(UNUSED void* arg0) {
    s32 i;
    u64* framebuffer1;
    OSMesg msg;

    gPhysicalFramebuffers[0] = (u16*) &gFramebuffer0;
    gPhysicalFramebuffers[1] = (u16*) &gFramebuffer1;
    gPhysicalFramebuffers[2] = (u16*) &gFramebuffer2;

    // Clear framebuffer.
    framebuffer1 = (u64*) &gFramebuffer1;
    for (i = 0; i < 19200; i++) {
        framebuffer1[i] = 0;
    }
    setup_mesg_queues();
    setup_game_memory();

    create_thread(&gAudioThread, 4, &thread4_audio, 0, gAudioThreadStack + 8192, 20);
    osStartThread(&gAudioThread);
    create_thread(&gGameLoopThread, 5, &thread5_game_loop, 0, gGameLoopThreadStack + 8192, 10);
    osStartThread(&gGameLoopThread);

    while (true) {
        
        osRecvMesg(&gIntrMesgQueue, &msg, OS_MESG_BLOCK);
        switch ((u32) msg) {
            case MESG_VI_VBLANK:
                handle_vblank();
                break;
            case MESG_SP_COMPLETE:
                handle_sp_complete();
                break;
            case MESG_DP_COMPLETE:
                handle_dp_complete();
                break;
            case MESG_START_GFX_SPTASK:
                start_gfx_sptask();
                break;
        }
    }
}
#endif

// Kart Draw Distance
#if 1
RECOMP_PATCH u16 check_player_camera_collision(Player* player, Camera* camera, f32 arg2, f32 arg3) {
    UNUSED f32 pad[6];
    f32 sp64;
    f32 sp60;
    f32 sp5C;
    f32 sp58;
    f32 sp54;
    f32 sp50;
    f32 sp4C;
    f32 sp48;
    f32 sp44;
    s16 var_v0;
    u16 ret;

    // Define a multiplier to scale the draw distance
    const f32 distanceMultiplier = 3.0f; // Adjust this value as needed

    // Apply the multiplier to arg2 and arg3
    arg2 *= distanceMultiplier;
    arg3 *= distanceMultiplier/5;

    ret = false;
    switch (gActiveScreenMode) {
        case SCREEN_MODE_1P:
            var_v0 = 0x293C;
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            var_v0 = 0x3058;
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            var_v0 = 0x1FFE;
            break;
        default:
            var_v0 = 0x1FFE;
            break;
    }
    sp4C = (arg2 * coss((camera->rot[1] - var_v0))) + camera->pos[2];
    sp58 = (arg2 * sins((camera->rot[1] - var_v0))) + camera->pos[0];
    sp48 = (arg2 * coss((camera->rot[1] + var_v0))) + camera->pos[2];
    sp54 = (arg2 * sins((camera->rot[1] + var_v0))) + camera->pos[0];
    sp44 = (arg3 * coss((camera->rot[1] + 0x5FFA))) + camera->pos[2];
    sp50 = (arg3 * sins((camera->rot[1] + 0x5FFA))) + camera->pos[0];

    sp64 = ((sp4C - player->pos[2]) * (sp54 - player->pos[0])) - ((sp48 - player->pos[2]) * (sp58 - player->pos[0]));
    sp60 = ((sp48 - player->pos[2]) * (sp50 - player->pos[0])) - ((sp44 - player->pos[2]) * (sp54 - player->pos[0]));
    sp5C = ((sp44 - player->pos[2]) * (sp58 - player->pos[0])) - ((sp4C - player->pos[2]) * (sp50 - player->pos[0]));

    if (((sp64 >= 0) && (sp60 >= 0) && (sp5C >= 0)) || (((sp64) <= 0) && (sp60 <= 0) && (sp5C <= 0))) {
        ret = true;
    }
    return ret;
}
#endif

#if 0
Vtx D_0D008BF8[] = {
    {{{     2,      2,      0}, 0, {     0,      0}, {255, 255, 255, 255}}},
    {{{     2,     -2,      0}, 0, {     0,   1984}, {255, 255, 255, 255}}},
    {{{    -2,     -2,      0}, 0, {  1984,   1984}, {255, 255, 255, 255}}},
    {{{    -2,      2,      0}, 0, {  1984,      0}, {255, 255, 255, 255}}},
};
Gfx D_0D008DA0[] = {
    gsSPDisplayList(common_square_plain_render),
    gsSPTexture(0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF),
    gsSPEndDisplayList(),
};
Gfx D_0D008E48[] = {
    gsDPSetRenderMode(G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2),
    gsSPVertex(D_0D008BF8, 4, 0),
    gsSPDisplayList(D_0D008DA0),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPEndDisplayList(),
};
Gfx D_0D008DB8[] = {
    gsDPPipeSync(),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPNoOp(),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPEndDisplayList(),
};
u8* common_texture_particle_smoke = (u8*) 0xd02bc58;

RECOMP_PATCH void func_8006538C(Player* player, s8 arg1, s16 arg2, s8 arg3) {
    Vec3f spB4;
    Vec3s spAC;
    s32 primColors[] = { MAKE_RGB(0xFB, 0xFF, 0xFB), MAKE_RGB(0xFF, 0xFB, 0x86) };
    s32 envColors[] = { MAKE_RGB(0x89, 0x62, 0x8F), MAKE_RGB(0xFE, 0x01, 0x09) };
    s16 primRed;
    s16 primGreen;
    s16 primBlue;
    s16 primAlpha;
    s16 envRed;
    s16 envGreen;
    s16 envBlue;

    recomp_printf("address: %x\n", common_texture_particle_smoke);

    if (player->unk_258[arg2].unk_01C == 1) {
        spB4[0] = player->unk_258[arg2].unk_000[0];
        spB4[1] = player->unk_258[arg2].unk_000[1];
        spB4[2] = player->unk_258[arg2].unk_000[2];
        spAC[0] = 0;
        spAC[1] = player->unk_048[arg3];
        spAC[2] = 0;
        if ((player->effects & STAR_EFFECT) && (((s32) gCourseTimer - D_8018D930[arg1]) < 9)) {
            primRed = (primColors[1] >> 0x10) & 0xFF;
            primGreen = (primColors[1] >> 0x08) & 0xFF;
            primBlue = (primColors[1] >> 0x00) & 0xFF;
            envRed = (envColors[1] >> 0x10) & 0xFF;
            envGreen = (envColors[1] >> 0x08) & 0xFF;
            envBlue = (envColors[1] >> 0x00) & 0xFF;
            primAlpha = player->unk_258[arg2].unk_03E;
            func_800652D4(spB4, spAC, ((player->unk_258[arg2].unk_00C * player->size) * 1.4));
            gSPDisplayList(gDisplayListHead++, D_0D008DB8);
            gDPLoadTextureBlock(gDisplayListHead++, common_texture_particle_smoke[player->unk_258[arg2].unk_010],
                                G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
            func_8004B72C(primRed, primGreen, primBlue, envRed, envGreen, envBlue, primAlpha);
            gDPSetAlphaCompare(gDisplayListHead++, G_AC_DITHER);
            gSPDisplayList(gDisplayListHead++, D_0D008E48);
        } else {
            primRed = (primColors[player->unk_258[arg2].unk_038] >> 0x10) & 0xFF;
            primGreen = (primColors[player->unk_258[arg2].unk_038] >> 0x08) & 0xFF;
            primBlue = (primColors[player->unk_258[arg2].unk_038] >> 0x00) & 0xFF;
            envRed = (envColors[player->unk_258[arg2].unk_038] >> 0x10) & 0xFF;
            envGreen = (envColors[player->unk_258[arg2].unk_038] >> 0x08) & 0xFF;
            envBlue = (envColors[player->unk_258[arg2].unk_038] >> 0x00) & 0xFF;
            primAlpha = player->unk_258[arg2].unk_03E;
            func_800652D4(spB4, spAC, player->unk_258[arg2].unk_00C * player->size);
            gSPDisplayList(gDisplayListHead++, D_0D008DB8);
            gDPLoadTextureBlock(gDisplayListHead++, common_texture_particle_smoke[player->unk_258[arg2].unk_010],
                                G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
            func_8004B72C(primRed, primGreen, primBlue, envRed, envGreen, envBlue, primAlpha);
            gSPDisplayList(gDisplayListHead++, D_0D008E48);
        }
        gMatrixEffectCount += 1;
    }
}
#endif

// This doesn't work at all
#if 0
RECOMP_PATCH void render_kart_particle_on_screen_one(Player* player, s8 playerId, s8 screenId) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gDisplayListHead++, ((u32) player << 8) + (playerId << 16) + screenId, G_EX_PUSH,
                                   G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    if ((player->type & PLAYER_EXISTS) == PLAYER_EXISTS) {
        if ((player->effects & BOO_EFFECT) == BOO_EFFECT) {
            if (playerId == screenId) {
                func_8006D474(player, playerId, screenId);
            }
        } else {
            func_8006D474(player, playerId, screenId);
        }
        func_8006DC54(player, playerId, screenId);
    }

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
}
#endif

// Tag smoke and dust
#if 1
RECOMP_PATCH void func_8006D474(Player* player, s8 playerId, s8 screenId) {
    s16 i;
    gEXEnable(gDisplayListHead++); // @recomp

    if ((player->unk_002 & (8 << (screenId * 4))) == (8 << (screenId * 4))) {
        for (i = 0; i < 10; i++) {
            // @recomp Tag the transform.
            gEXMatrixGroupDecomposedNormal(
                gDisplayListHead++, TAG_SMOKE_DUST(((u32) player->unk_258[i].unk_012 << 8) + (playerId << 16) + i),
                G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
            switch (player->unk_258[i].unk_012) {
                case 1:
                    if (gActiveScreenMode == SCREEN_MODE_3P_4P_SPLITSCREEN) {
                        if (screenId == playerId) {
                            func_8006538C(player, playerId, i, screenId);
                        }
                    } else {

                        func_8006538C(player, playerId, i, screenId);
                    }
                    break;
                case 6:
                    if (gActiveScreenMode == SCREEN_MODE_3P_4P_SPLITSCREEN) {
                        if (screenId == playerId) {
                            func_80066BAC(player, playerId, i, screenId);
                        }
                    } else if (screenId == playerId) {
                        func_80066BAC(player, playerId, i, screenId);
                    }
                    break;
            }
            switch (player->unk_258[i + 30].unk_012) {
                case 1:
                case 9:
                    if (gActiveScreenMode == SCREEN_MODE_1P) {
                        func_800691B8(player, playerId, i, screenId);
                    } else if (screenId == playerId) {
                        func_800691B8(player, playerId, i, screenId);
                    }
                    break;
                case 2:
                    if (gActiveScreenMode == SCREEN_MODE_1P) {
                        func_800696CC(player, playerId, i, screenId, player->unk_258[i + 30].unk_00C);
                    } else if (screenId == playerId) {
                        func_800696CC(player, playerId, i, screenId, player->unk_258[i + 30].unk_00C);
                    }
                    break;
                case 3:
                    if (gActiveScreenMode == SCREEN_MODE_1P) {
                        func_80067280(player, playerId, i, screenId);
                    } else if (screenId == playerId) {
                        func_80067280(player, (s32) playerId, i, screenId);
                    }
                    break;
                case 4:
                    if (gActiveScreenMode == SCREEN_MODE_1P) {
                        func_80069444(player, playerId, i, screenId);
                    } else if (screenId == playerId) {
                        func_80069444(player, playerId, i, screenId);
                    }
                    break;
                case 5:
                    if (gActiveScreenMode == SCREEN_MODE_1P) {
                        func_80069938(player, playerId, i, screenId);
                    } else if (screenId == playerId) {
                        func_80069938(player, playerId, i, screenId);
                    }
                    break;
                case 6:
                    if (gActiveScreenMode == SCREEN_MODE_1P) {
                        func_80069BA8(player, playerId, i, screenId);
                    } else if (screenId == playerId) {
                        func_80069BA8(player, playerId, i, screenId);
                    }
                    break;
                case 7:
                    if (gActiveScreenMode == SCREEN_MODE_1P) {
                        func_80069DB8(player, playerId, i, screenId);
                    } else if (screenId == playerId) {
                        func_80069DB8(player, playerId, i, screenId);
                    }
                    break;
                case 8:
                    if (gActiveScreenMode == SCREEN_MODE_1P) {
                        func_80067604(player, playerId, i, screenId);
                    } else if (screenId == playerId) {
                        func_80067604(player, playerId, i, screenId);
                    }
                    break;
            }
            switch (player->unk_258[i + 10].unk_012) {
                case 1:
                    if (gActiveScreenMode == SCREEN_MODE_3P_4P_SPLITSCREEN) {
                        if (screenId == playerId) {
                            func_80065AB0(player, playerId, i, screenId);
                        }
                    } else {
                        func_80065AB0(player, playerId, i, screenId);
                    }
                    break;
                case 2:
                case 3:
                case 4:
                case 5:
                    if (gActiveScreenMode == SCREEN_MODE_3P_4P_SPLITSCREEN) {
                        if (screenId == playerId) {
                            func_80065F0C(player, playerId, i, screenId);
                        }
                    } else {
                        func_80065F0C(player, playerId, i, screenId);
                    }
                    break;
                case 9:
                    if (gActiveScreenMode == SCREEN_MODE_3P_4P_SPLITSCREEN) {
                        if (screenId == playerId) {
                            func_800664E0(player, (s32) playerId, i, screenId);
                        }
                    } else {
                        func_800664E0(player, (s32) playerId, i, screenId);
                    }
                    break;
                case 11:
                    if (gActiveScreenMode == SCREEN_MODE_3P_4P_SPLITSCREEN) {
                        if (screenId == playerId) {
                            func_8006A01C(player, playerId, i, screenId);
                        }
                    } else if (screenId == playerId) {
                        func_8006A01C(player, playerId, i, screenId);
                    }
                    break;
            }
            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
        }
    }
    if ((gModeSelection == BATTLE) && (player->unk_002 & (2 << (screenId * 4)))) {
        func_8006BA94(player, playerId, screenId);
    }
}
#endif

// EEEEEEEEEEEEEEEEEEEEEEEEEEEE
#if 1
RECOMP_PATCH void func_80065AB0(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3) {
    Vec3f spB4;
    Vec3s spAC;
    s32 var_s0;
    s16 primRed;
    s16 primGreen;
    s16 primBlue;
    s16 primAlpha;
    s16 envRed;
    s16 envGreen;
    s16 envBlue;
    s32 sp8C[] = { 0x00ffffff, 0x00ffff00, 0x00ff9600 };

    if (player->unk_258[10 + arg2].unk_01C == 1) {
        if (player->unk_204 >= 50) {
            var_s0 = 1;
        } else {
            var_s0 = 0;
        }

        primRed = player->unk_258[10 + arg2].unk_038;
        primGreen = player->unk_258[10 + arg2].unk_03A;
        primBlue = player->unk_258[10 + arg2].unk_03C;
        primAlpha = player->unk_258[10 + arg2].unk_03E;
        envRed = (sp8C[player->unk_258[10 + arg2].unk_040] >> 0x10) & 0xFF;
        envGreen = (sp8C[player->unk_258[10 + arg2].unk_040] >> 0x08) & 0xFF;
        envBlue = (sp8C[player->unk_258[10 + arg2].unk_040] >> 0x00) & 0xFF;

        spB4[0] = player->unk_258[10 + arg2].unk_000[0];
        spB4[1] = player->unk_258[10 + arg2].unk_000[1];
        spB4[2] = player->unk_258[10 + arg2].unk_000[2];
        spAC[0] = 0;
        spAC[1] = player->unk_048[arg3];
        spAC[2] = 0;

        // @recomp Tag the transform.
        gEXMatrixGroupDecomposed(gDisplayListHead++, player->type << 8 | arg2 | var_s0 << 16, G_EX_PUSH,
                                 G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

        func_800652D4(spB4, spAC, player->unk_258[10 + arg2].unk_00C * player->size);

        if (var_s0 == 0) {
            // Blured E
            gSPDisplayList(gDisplayListHead++, 0x0D008DB8);
            gDPLoadTextureBlock(gDisplayListHead++, D_800E4770[var_s0][0], G_IM_FMT_I, G_IM_SIZ_8b, 16, 16, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            func_8004B72C(primRed, primGreen, primBlue, envRed, envGreen, envBlue, primAlpha);
            gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_XLU_SURF, G_RM_ZB_XLU_SURF2);
            gSPDisplayList(gDisplayListHead++, 0xD008DF8);
        } else {
            // Normal E
            gSPDisplayList(gDisplayListHead++, 0x0D008DB8);
            gDPLoadTextureBlock(gDisplayListHead++, D_800E4770[var_s0][0], G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            func_8004B72C(primRed, primGreen, primBlue, envRed, envGreen, envBlue, primAlpha);
            gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_XLU_SURF, G_RM_ZB_XLU_SURF2);
            gSPDisplayList(gDisplayListHead++, 0x0D008E48);
        }
        gMatrixEffectCount += 1;

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
}
#endif

#if 0
RECOMP_PATCH void print_text_mode_1(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposed(gDisplayListHead++, (u32) text << 16 | column << 8 | row, G_EX_PUSH, G_MTX_MODELVIEW,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

    print_text0(column, row, text, tracking, scaleX, scaleY, 1);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
}
#endif
#if 1
RECOMP_PATCH void print_text0(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY, s32 mode) {
    s32 stringWidth = 0;
    s32 glyphIndex;

    gSPDisplayList(gDisplayListHead++, (u8*) 0x020077A8);
    if (*text != 0) {
        do {
            // @recomp Tag the transform.
            gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_LETTER(text), G_EX_PUSH, G_MTX_MODELVIEW,
                                     G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

            glyphIndex = char_to_glyph_index(text);
            if (glyphIndex >= 0) {
                load_menu_img((MenuTexture*) segmented_to_virtual_dupe((const void*) gGlyphTextureLUT[glyphIndex]));
                gDisplayListHead =
                    print_letter(gDisplayListHead,
                                 (MenuTexture*) segmented_to_virtual_dupe((const void*) gGlyphTextureLUT[glyphIndex]),
                                 column + (stringWidth * scaleX), row, mode, scaleX, scaleY);
                stringWidth += gGlyphDisplayWidth[glyphIndex] + tracking;
            } else if ((glyphIndex != -2) && (glyphIndex == -1)) {
                stringWidth += tracking + 7;
            } else {
                gSPDisplayList(gDisplayListHead++, (u8*) 0x020077D8);
                return;
            }
            if (glyphIndex >= 0x30) {
                text += 2;
            } else {
                text += 1;
            }

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

        } while (*text != 0);
    }
    gSPDisplayList(gDisplayListHead++, (u8*) 0x020077D8);
}

// "tracking" is a uniform spacing between all characters in a given word
RECOMP_PATCH void print_text1(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY, s32 arg6) {
    char* temp_string = text;
    s32 stringWidth = 0;
    s32 glyphIndex;
    s32 sp60;

    while (*temp_string != 0) {
        glyphIndex = char_to_glyph_index(temp_string);
        if (glyphIndex >= 0) {
            stringWidth += ((gGlyphDisplayWidth[glyphIndex] + tracking) * scaleX);
        } else if ((glyphIndex != -2) && (glyphIndex == -1)) {
            stringWidth += ((tracking + 7) * scaleX);
        } else {
            return;
        }
        if (glyphIndex >= 0x30) {
            temp_string += 2;
        } else {
            temp_string += 1;
        }
    }

    switch (arg6) {
        case LEFT_TEXT:
            // ???
            do {
            } while (0);
        case RIGHT_TEXT:
            column -= stringWidth;
            break;
        case CENTER_TEXT_MODE_1:
        case CENTER_TEXT_MODE_2:
            column -= stringWidth / 2;
            break;
        default:
            break;
    }

    if (arg6 < 3) {
        sp60 = 1;
    } else {
        sp60 = 2;
    }

    gSPDisplayList(gDisplayListHead++, (u8*) 0x020077A8);
    while (*text != 0) {
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_LETTER(text), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO,
                                 G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                                 G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

        glyphIndex = char_to_glyph_index(text);
        if (glyphIndex >= 0) {
            load_menu_img(segmented_to_virtual_dupe(gGlyphTextureLUT[glyphIndex]));
            gDisplayListHead = print_letter(gDisplayListHead, segmented_to_virtual_dupe(gGlyphTextureLUT[glyphIndex]),
                                            column, row, sp60, scaleX, scaleY);
            column = column + (s32) ((gGlyphDisplayWidth[glyphIndex] + tracking) * scaleX);
        } else if ((glyphIndex != -2) && (glyphIndex == -1)) {
            column = column + (s32) ((tracking + 7) * scaleX);
        } else {
            gSPDisplayList(gDisplayListHead++, (u8*) 0x020077D8);
            return;
        }
        if (glyphIndex >= 0x30) {
            text += 2;
        } else {
            text += 1;
        }

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
    gSPDisplayList(gDisplayListHead++, (u8*) 0x020077D8);
}

RECOMP_PATCH void print_text2(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY, s32 arg6) {
    MenuTexture* glyphTexture;
    s32 characterWidth;
    s32 glyphIndex;

    gSPDisplayList(gDisplayListHead++, (u8*) 0x020077A8);
    if (*text != 0) {
        do {
            // @recomp Tag the transform.
            gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_LETTER(text), G_EX_PUSH, G_MTX_MODELVIEW,
                                     G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

            glyphIndex = char_to_glyph_index(text);
            if (glyphIndex >= 0) {
                glyphTexture = (MenuTexture*) segmented_to_virtual_dupe((const void*) gGlyphTextureLUT[glyphIndex]);
                load_menu_img(glyphTexture);
                gDisplayListHead =
                    print_letter(gDisplayListHead, glyphTexture, column - (gGlyphDisplayWidth[glyphIndex] / 2), row,
                                 arg6, scaleX, scaleY);
                if ((glyphIndex >= 213) && (glyphIndex < 224)) {
                    characterWidth = 32;
                } else {
                    characterWidth = 12;
                }
                column = column + (s32) ((characterWidth + tracking) * scaleX);
            } else if ((glyphIndex != -2) && (glyphIndex == -1)) {
                column = column + (s32) ((tracking + 7) * scaleX);
            } else {
                gSPDisplayList(gDisplayListHead++, (u8*) 0x020077D8);
                return;
            }
            if (glyphIndex >= 0x30) {
                text += 2;
            } else {
                text += 1;
            }

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

        } while (*text != 0);
    }

    gSPDisplayList(gDisplayListHead++, (u8*) 0x020077D8);
}
#endif

#if 1
/**
 * Determines whether an object is within the render distance of a camera.
 *
 * @param cameraPos       The position of the camera in 3D space.
 * @param objectPos       The position of the object in 3D space.
 * @param orientationY    The orientation angle of the object around the Y-axis.
 * @param minDistance     The minimum distance at which the object is considered within render distance.
 * @param fov             The field of view (FOV) of the camera.
 * @param maxDistance     The maximum render distance.
 * @return                The distance between the camera and the object if it's within render distance,
 *                        or -1.0f if it exceeds the render distance.
 */

RECOMP_PATCH f32 is_within_render_distance(Vec3f cameraPos, Vec3f objectPos, u16 orientationY, f32 minDistance, f32 fov,
                                           f32 maxDistance) {
    u16 angleObject;
    u16 temp_v0;
    f32 distanceX;
    f32 distance;
    f32 distanceY;
    f32 scaleFov;
    f32 maxDistance2;
    s32 plus_fov_angle;
    s32 minus_fov_angle;
    u16 temp;
    s32 count = 0;

    // for (s32 i = 0; i < ACTOR_LIST_SIZE; i++) {
    //     if (gActorList[i].flags != 0) {
    //         count++;
    //     }
    // }
    // // recomp_printf("Actor Count: %d\n", count);
    // recomp_printf("gNumActors: %d\n", gNumActors);
    // increase maxDistance value and culling of the fov
    maxDistance *= 1.5f;
    maxDistance2 = 1.0f;
    scaleFov = 1.25;

    f32 extended_fov = ((f32) fov * 0xB6 * scaleFov); // Sets the Culling for objects on the left and right

    distanceX = objectPos[0] - cameraPos[0];
    distanceX = distanceX * distanceX;
    if (maxDistance < distanceX) {
        return -1.0f;
    }

    distanceY = objectPos[2] - cameraPos[2];
    distanceY = distanceY * distanceY;
    if (maxDistance < distanceY) {
        return -1.0f;
    }

    distance = distanceX + distanceY;
    if (distance < minDistance) {
        return distance;
    }

    if (distance > maxDistance) {
        return -1.0f;
    }

    angleObject = get_angle_between_two_vectors(cameraPos, objectPos);
    minus_fov_angle = (orientationY - extended_fov);
    plus_fov_angle = (orientationY + extended_fov);

    if (minDistance == 0.0f) {
        if (is_visible_between_angle((orientationY + extended_fov), (orientationY - extended_fov), angleObject) == 1) {
            if (gCurrentCourseId == COURSE_KALAMARI_DESERT) {
                return distance / 8.0f;
            } else {
                return distance / 10.0f; // Items
            }
        }
        return -1.0f;
    }

    if (is_visible_between_angle((u16) plus_fov_angle, (u16) minus_fov_angle, angleObject) == 1) {
        if (gCurrentCourseId == COURSE_KALAMARI_DESERT) {
            return distance / 3.0f;
        } else {
            return distance / 10.0f; // DD Vhicles
        }
    }

    temp_v0 = func_802B7CA8(minDistance / distance);
    temp = angleObject + temp_v0;

    if (is_visible_between_angle(plus_fov_angle, minus_fov_angle, temp) == 1) {
        return 0;
    }

    temp = angleObject - temp_v0;
    if (is_visible_between_angle(plus_fov_angle, minus_fov_angle, temp) == 1) {
        return distance;
    }
    return -1.0f;
}
#endif