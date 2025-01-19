#include "patches.h"

#if 1
#include "menu_items.h"
#include "menus.h"
#include "sounds.h"

// Fixes a bug with the recompiler.
int dummy_val = 1;

bool recompDebugSwitch = 0;

RECOMP_PATCH void handle_menus_with_pri_arg(s32 priSpecial) {
    s32 j = 0;
    s32 isRendered = 0;
    s32 i = 0;
    s32 type = 0;
    MenuItem* entry = NULL;
    volatile u8* address;
    
    gDPSetTextureFilter(gDisplayListHead++, G_TF_POINT);
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
            recomp_printf("recompDebugSwitch: %d\n", recompDebugSwitch);
        }
        if (gControllers[0].button & D_JPAD) {
            recompDebugSwitch = 0;
            recomp_printf("recompDebugSwitch: %d\n", recompDebugSwitch);
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
            case MENU_ITEM_UI_LOGO_INTRO:
                gDPSetTextureFilter(gDisplayListHead++, G_TF_BILERP);
                if (sIntroModelTimer < 0x50) {
                    sIntroModelSpeed = 3.0f;
                } else if (sIntroModelTimer < 0x5A) {
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
                func_800AEC54(entry);
                break;
            case MENU_ITEM_PAUSE:
                func_800ADF48(entry);
                break;
            case MENU_ITEM_END_COURSE_OPTION:
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
RECOMP_PATCH Gfx* func_80097A14(Gfx* displayListHead, s8 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, u8* arg8,
                   u32 arg9, u32 argA) {
    gDPPipeSync(displayListHead++);
    gDPSetCycleType(displayListHead++, G_CYC_COPY);
    displayListHead = func_80095E10(displayListHead, arg1, 0x00001000, 0x00000400, arg2, arg3, arg4, arg5, arg6, arg7,
                                    arg8, arg9, argA);
    gDPPipeSync(displayListHead++);
    gDPSetCycleType(displayListHead++, G_CYC_1CYCLE);
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
    // return false;
    // @recomp: always visible
    return true;
}