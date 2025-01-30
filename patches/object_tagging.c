#include "patches.h"

typedef enum ObjectList {
    /* 10 */ OBJECT_10 = 10,
    /* 10 */ OBJECT_11,
    /* 10 */ OBJECT_12
} ObjectList;

// Balloons
#if 1
RECOMP_PATCH void func_80053D74(s32 objectIndex, UNUSED s32 arg1, s32 vertexIndex) {
    Object* object;
    int i = 0;
    Vtx* vtx = (Vtx*) 0x0d0060b0;

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
        gSPVertex(gDisplayListHead++, &vtx[vertexIndex], 4, 0);
        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0d006940);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
}
#endif

// Star particles
#if 1
RECOMP_PATCH void func_800691B8(Player* player, s8 arg1, s16 arg2, s8 arg3) {
    Vec3f sp5C;
    Vec3s sp54;
    s16 alpha;

    if (player->unk_258[30 + arg2].unk_01C == 1) {
        alpha = player->unk_258[30 + arg2].unk_03E;
        sp5C[0] = player->unk_258[30 + arg2].unk_000[0];
        sp5C[1] = player->unk_258[30 + arg2].unk_000[1];
        sp5C[2] = player->unk_258[30 + arg2].unk_000[2];
        sp54[0] = 0;
        sp54[1] = player->unk_048[arg3];
        player->unk_258[30 + arg2].unk_03A += 0x1C71;
        sp54[2] = player->unk_258[30 + arg2].unk_03A;

        // @recomp Tag the transform.
        gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT(arg2) | OBJECT_10 << 16, G_EX_PUSH, G_MTX_MODELVIEW,
                                 G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

        func_800652D4(sp5C, sp54, player->size * 0.5);
        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0D008D58);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
        gDPLoadTextureBlock(gDisplayListHead++, D_8018D488, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        func_8004B35C(255, 255, 0, alpha);
        gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
        gSPVertex(gDisplayListHead++, (Vtx*) 0x800E87C0, 4, 0);
        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0D008DA0);
        gMatrixEffectCount++;

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
}
#endif

#if 1
RECOMP_PATCH void func_800696CC(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3, f32 arg4) {
    Vec3f sp5C;
    Vec3s sp54;
    s16 alpha;

    if (player->unk_258[30 + arg2].unk_01C == 1) {
        alpha = player->unk_258[30 + arg2].unk_03E;
        sp5C[0] = player->unk_258[30 + arg2].unk_000[0];
        sp5C[1] = player->unk_258[30 + arg2].unk_000[1];
        sp5C[2] = player->unk_258[30 + arg2].unk_000[2];
        sp54[0] = 0;
        sp54[1] = player->unk_048[arg3];
        sp54[2] = 0;

        // @recomp Tag the transform.
        gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT(arg2) | OBJECT_11 << 16, G_EX_PUSH, G_MTX_MODELVIEW,
                                 G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

        func_800652D4(sp5C, sp54, player->size * arg4);
        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0D008D58);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
        gDPLoadTextureBlock(gDisplayListHead++, D_8018D488, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        func_8004B35C(0x000000FF, 0x000000FF, 0, alpha);
        gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
        gSPVertex(gDisplayListHead++, (Vtx*) 0x800E87C0, 4, 0);
        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0D008DA0);
        gMatrixEffectCount += 1;

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
}
#endif

#if 1
RECOMP_PATCH void func_80069938(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3) {
    Vec3f sp5C;
    Vec3s sp54;
    s16 alpha;

    if (player->unk_258[30 + arg2].unk_01C == 1) {
        alpha = player->unk_258[30 + arg2].unk_03E;
        sp5C[0] = player->unk_258[30 + arg2].unk_000[0];
        sp5C[1] = player->unk_258[30 + arg2].unk_000[1];
        sp5C[2] = player->unk_258[30 + arg2].unk_000[2];
        sp54[0] = 0;
        sp54[1] = player->unk_048[arg3];
        sp54[2] = player->unk_258[30 + arg2].unk_038;

        // @recomp Tag the transform.
        gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT(arg2) | OBJECT_12 << 16, G_EX_PUSH, G_MTX_MODELVIEW,
                                 G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

        func_800652D4(sp5C, sp54, player->unk_258[30 + arg2].unk_00C * player->size);
        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0D008D58);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
        gDPLoadTextureBlock(gDisplayListHead++, D_8018D488, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        func_8004B35C(0x000000FF, 0x000000FF, 0, alpha);
        gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
        gSPVertex(gDisplayListHead++, (Vtx*) 0x800E87C0, 4, 0);
        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0D008DA0);
        gMatrixEffectCount += 1;

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
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

#if 1
RECOMP_PATCH void func_80051ABC(s16 arg0, s32 arg1) {
    s32 i;
    s32 objectIndex;
    Object* object;
    u32 margin = 0;

    D_8018D228 = 0xFF;
    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0D007A60);
    if ((u8) D_8018D230 != 0) {
        func_8004B414(255, 255, 255, 255);
        for (i = 0; i < D_8018D1F0; i++) {
            objectIndex = D_8018CC80[arg1 + i];
            object = &gObjectList[objectIndex];
            func_800519D4(objectIndex, object->unk_09C, arg0 - object->unk_09E);
        }
    } else {
        func_8004B6C4(255, 255, 255);
        for (i = 0; i < D_8018D1F0; i++) {
            objectIndex = D_8018CC80[arg1 + i];
            object = &gObjectList[objectIndex];
            gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT(object), G_EX_PUSH, G_MTX_MODELVIEW,
                                     G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

            gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0 + margin, 0 + margin, SCREEN_WIDTH - margin,
                          SCREEN_HEIGHT - margin);

            func_800518F8(objectIndex, object->unk_09C, arg0 - object->unk_09E);

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
        }
    }
}
#endif

// Farm Moles
#if 1
RECOMP_PATCH void func_80054D00(s32 objectIndex, s32 cameraId) {
    Camera* camera;

    camera = &camera1[cameraId];
    if (gObjectList[objectIndex].state >= 3) {
        func_8008A364(objectIndex, cameraId, 0x2AABU, 0x0000012C);
        if (is_obj_flag_status_active(objectIndex, VISIBLE) != 0) {
            gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT(&gObjectList[objectIndex]), G_EX_PUSH,
                                     G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

            D_80183E80[0] = (s16) gObjectList[objectIndex].orientation[0];
            D_80183E80[1] =
                func_800418AC(gObjectList[objectIndex].pos[0], gObjectList[objectIndex].pos[2], camera->pos);
            D_80183E80[2] = (u16) gObjectList[objectIndex].orientation[2];
            func_80048130(gObjectList[objectIndex].pos, (u16*) D_80183E80, gObjectList[objectIndex].sizeScaling,
                          (u8*) gObjectList[objectIndex].activeTLUT, gObjectList[objectIndex].activeTexture,
                          (Vtx*) 0x0D0062B0, 32, 64, 32, 64, 5);

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
        }
    }
}

RECOMP_PATCH void func_80054E10(s32 objectIndex) {
    if (gObjectList[objectIndex].state > 0) {
        if (is_obj_flag_status_active(objectIndex, 0x00800000) != 0) {
            gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT(&gObjectList[objectIndex]), G_EX_PUSH,
                                     G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

            D_80183E50[0] = gObjectList[objectIndex].pos[0];
            D_80183E50[1] = gObjectList[objectIndex].surfaceHeight + 0.8;
            D_80183E50[2] = gObjectList[objectIndex].pos[2];
            D_80183E70[0] = gObjectList[objectIndex].velocity[0];
            D_80183E70[1] = gObjectList[objectIndex].velocity[1];
            D_80183E70[2] = gObjectList[objectIndex].velocity[2];
            func_8004A9B8(gObjectList[objectIndex].sizeScaling);

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
        }
    }
}

RECOMP_PATCH void func_80054F04(s32 cameraId) {
    s32 i;
    s32 objectIndex;
    Camera* camera = &camera1[cameraId];
    Object* object;
    
    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0D0079C8);
    load_texture_block_rgba16_mirror((u8*) 0x6013670, 16, 16);

    for (i = 0; i < gObjectParticle2_SIZE; i++) {
        objectIndex = gObjectParticle2[i];
        object = &gObjectList[objectIndex];

        if (object->state > 0) {
            func_8008A364(objectIndex, cameraId, 0x2AABU, 200);
            if ((is_obj_flag_status_active(objectIndex, VISIBLE) != 0) && (gMatrixHudCount <= MTX_HUD_POOL_SIZE_MAX)) {
                gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT(object) | (i << 16), G_EX_PUSH, G_MTX_MODELVIEW,
                                         G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                         G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                         G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

                object->orientation[1] = func_800418AC(object->pos[0], object->pos[2], camera->pos);
                rsp_set_matrix_gObjectList(objectIndex);
                gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0D006980);

                // @recomp Pop the transform id.
                gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
            }
        }
    }
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}
#endif
