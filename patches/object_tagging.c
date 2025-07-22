#include "patches.h"

typedef enum ObjectList {
    /* 10 */ OBJECT_10 = 10,
    /* 10 */ OBJECT_11,
    /* 10 */ OBJECT_12
} ObjectList;

#if 1
RECOMP_PATCH void render_player(Player* player, s8 playerId, s8 screenId) {
    UNUSED s32 pad[2];
    s32 temp_t1;
    s32 var_v1;
    OSMesg* sp34;

    // @recomp Tag the transform
    gEXMatrixGroupDecomposed(gDisplayListHead++, (TAG_OBJECT(player) << 16) | screenId, G_EX_PUSH, G_MTX_MODELVIEW,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

    update_wheel_palette(player, playerId, screenId, D_801651D0[screenId][playerId]);
    if (!(player->unk_002 & (4 << (screenId * 4)))) {
        var_v1 = 0;
    } else {
        var_v1 = 8;
    }
    func_80023BF0(player, playerId, screenId, var_v1);
    temp_t1 = 8 << (screenId * 4);
    if ((temp_t1 == (player->unk_002 & temp_t1)) && (player->collision.surfaceDistance[2] <= 50.0f) &&
        (player->surfaceType != ICE)) {
        gEXMatrixGroupDecomposed(gDisplayListHead++, (TAG_OBJECT(player) << 16) & 0xFFFF0000 | playerId, G_EX_PUSH,
                                 G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

        if ((player->effects & BOO_EFFECT) == BOO_EFFECT) {
            if (playerId == screenId) {
                render_player_shadow(player, playerId, screenId);
            }
        } else {
            render_player_shadow(player, playerId, screenId);
        }
        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
    if ((player->type & PLAYER_INVISIBLE_OR_BOMB) != PLAYER_INVISIBLE_OR_BOMB) {
        gEXMatrixGroupDecomposed(gDisplayListHead++, (TAG_OBJECT(player) << 16) & 0xFFFF0000 | playerId, G_EX_PUSH,
                                 G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

        render_kart(player, playerId, screenId, var_v1);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    } else {
        gEXMatrixGroupDecomposed(gDisplayListHead++, (TAG_OBJECT(player) << 16) & 0xFFFF0000 | playerId, G_EX_PUSH,
                                 G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

        render_ghost(player, playerId, screenId, var_v1);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
    osRecvMesg(&gDmaMesgQueue, (OSMesg*) &sp34, OS_MESG_BLOCK);
    if ((temp_t1 == (player->unk_002 & temp_t1)) && (player->surfaceType == ICE) && ((player->unk_0CA & 1) != 1) &&
        (player->collision.surfaceDistance[2] <= 30.0f)) {
        gEXMatrixGroupDecomposed(gDisplayListHead++, (TAG_OBJECT(player) << 16) & 0xFFFF0000 | playerId, G_EX_PUSH,
                                 G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

        render_player_ice_reflection(player, playerId, screenId, var_v1);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
    if (player->boostPower >= 2.0f) {
        gEXMatrixGroupDecomposed(gDisplayListHead++, (TAG_OBJECT(player) << 16) & 0xFFFF0000 | playerId, G_EX_PUSH,
                                 G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

        func_80025DE8(player, playerId, screenId, var_v1);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
}
#endif

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
            gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_SMOKE_DUST((playerId << 8) + i), G_EX_PUSH,
                                           G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
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
            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

            // @recomp Tag the transform.
            gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_SMOKE_DUST((playerId << 8) + i + 30), G_EX_PUSH,
                                           G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

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
            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

            // @recomp Tag the transform.
            gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_SMOKE_DUST((playerId << 8) + i + 10), G_EX_PUSH,
                                           G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
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

#if 1
RECOMP_PATCH /**
              * @brief Renders the train engine actor.
              * Actor used in Kalimari Desert.
              * His update position are made in vehicle.
              *
              * @param camera
              * @param actor
              */
    void
    render_actor_train_engine(Camera* camera, struct TrainCar* actor) {
    UNUSED s32 pad[2];
    s32 maxObjectsReached;
    Vec3f sp160;
    Mat4 sp120;
    Mat4 spE0;
    Mat4 spA0;

    f32 distance = is_within_render_distance(camera->pos, actor->pos, camera->rot[1], 2500.0f,
                                             gCameraZoom[camera - camera1], 9000000.0f);

    if (distance < 0.0f) {
        return;
    }

    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    mtxf_pos_rotation_xyz(sp120, actor->pos, actor->rot);
    maxObjectsReached = render_set_position(sp120, 0) == 0;

    static char* sLocomotive = "Locomotive";

    gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT(sLocomotive) & 0xFFFFFFF0 | 1, G_EX_PUSH, G_MTX_MODELVIEW,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

    if (maxObjectsReached) {
        return;
    }

    if (1 /* distance < 122500.0f */) {
        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0601c0f0 /*d_course_kalimari_desert_dl_1C0F0*/);
        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0601b978 /*d_course_kalimari_desert_dl_1B978*/);

    } else if (distance < 640000.0f) {
        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0601d670 /* d_course_kalimari_desert_dl_1D670 */);
        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0601d160 /* d_course_kalimari_desert_dl_1D160 */);
    } else {
        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0601e910 /* d_course_kalimari_desert_dl_1E910 */);
        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0601e480 /* d_course_kalimari_desert_dl_1E480 */);
    }

    // if (1440000.0f < distance) {
    //     return;
    // }

    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06022d28 /* d_course_kalimari_desert_dl_22D28 */);

    mtxf_rotate_x(sp120, actor->wheelRot);
    vec3f_set(sp160, 17.0f, 6.0f, 32.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

    gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT(sLocomotive) & 0xFFFFFFF0 | 2, G_EX_PUSH, G_MTX_MODELVIEW,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

    maxObjectsReached = render_set_position(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06022db8 /* d_course_kalimari_desert_dl_22DB8 */);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, actor->wheelRot);
    vec3f_set(sp160, -17.0, 6.0f, 32.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

    gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT(sLocomotive) & 0xFFFFFFF0 | 3, G_EX_PUSH, G_MTX_MODELVIEW,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

    maxObjectsReached = render_set_position(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06022db8 /* d_course_kalimari_desert_dl_22DB8 */);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x16C));
    vec3f_set(sp160, 17.0f, 6.0f, 16.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

    gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT(sLocomotive) & 0xFFFFFFF0 | 4, G_EX_PUSH, G_MTX_MODELVIEW,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

    maxObjectsReached = render_set_position(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06022db8 /* d_course_kalimari_desert_dl_22DB8 */);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x16C));
    vec3f_set(sp160, -17.0f, 6.0f, 16.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

    gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT(sLocomotive) & 0xFFFFFFF0 | 5, G_EX_PUSH, G_MTX_MODELVIEW,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

    maxObjectsReached = render_set_position(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06022db8 /* d_course_kalimari_desert_dl_22DB8 */);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x444));
    vec3f_set(sp160, 17.0f, 12.0f, -12.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

    gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT(sLocomotive) & 0xFFFFFFF0 | 6, G_EX_PUSH, G_MTX_MODELVIEW,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

    maxObjectsReached = render_set_position(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06022d70 /* d_course_kalimari_desert_dl_22D70 */);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x444));
    vec3f_set(sp160, -17.0f, 12.0f, -12.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

    gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT(sLocomotive) & 0xFFFFFFF0 | 7, G_EX_PUSH, G_MTX_MODELVIEW,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

    maxObjectsReached = render_set_position(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06022d70 /* d_course_kalimari_desert_dl_22D70 */);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x2D8));
    vec3f_set(sp160, 17.0f, 12.0f, -34.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

    gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT(sLocomotive) & 0xFFFFFFF0 | 8, G_EX_PUSH, G_MTX_MODELVIEW,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

    maxObjectsReached = render_set_position(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06022d70 /* d_course_kalimari_desert_dl_22D70 */);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x2D8));
    vec3f_set(sp160, -17.0f, 12.0f, -34.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

    gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT(sLocomotive) & 0xFFFFFFF0 | 9, G_EX_PUSH, G_MTX_MODELVIEW,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

    maxObjectsReached = render_set_position(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06022d70 /* d_course_kalimari_desert_dl_22D70 */);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
}
#endif

#if 1
RECOMP_PATCH void render_actor_train_tender(Camera* camera, struct TrainCar* actor) {
    Mat4 sp120;
    Vec3f sp160;
    Mat4 spE0;
    Mat4 spA0;

    f32 temp_f0 = is_within_render_distance(camera->pos, actor->pos, camera->rot[1], 625.0f,
                                            gCameraZoom[camera - camera1], 9000000.0f);

    if (temp_f0 < 0.0f) {
        return;
    }

    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    static char* sTender = "tender";

    // @recomp Tag the transform
    gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT(sTender) & 0xFFFFFFF0 | 0, G_EX_PUSH, G_MTX_MODELVIEW,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

    mtxf_pos_rotation_xyz(sp120, actor->pos, actor->rot);
    if (render_set_position(sp120, 0) == 0) {
        return;
    }

    if (1 /* temp_f0 < 250000.0f*/) {

        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0601f228 /* d_course_kalimari_desert_dl_1F228*/);

    } else if (temp_f0 < 1000000.0f) {
        //  gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_1F708);
    } else {
        // gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_1FAF8);
    }
    if (1440000.0f < temp_f0) {
        return;
    }

    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06022d28 /* d_course_kalimari_desert_dl_22D28 */);

    mtxf_rotate_x(sp120, actor->wheelRot);
    vec3f_set(sp160, 17.0f, 6.0f, 8.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

    // @recomp Tag the transform
    gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT(sTender) & 0xFFFFFFF0 | 1, G_EX_PUSH, G_MTX_MODELVIEW,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06022db8 /* d_course_kalimari_desert_dl_22DB8 */);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, actor->wheelRot);
    vec3f_set(sp160, -17.0, 6.0f, 8.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

    // @recomp Tag the transform
    gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT(sTender) & 0xFFFFFFF0 | 2, G_EX_PUSH, G_MTX_MODELVIEW,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

    // @recomp Tag the transform
    gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT(sTender) & 0xFFFFFFF0 | 3, G_EX_PUSH, G_MTX_MODELVIEW,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06022db8 /* d_course_kalimari_desert_dl_22DB8 */);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x444));
    vec3f_set(sp160, 17.0f, 6.0f, -8.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

    // @recomp Tag the transform
    gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT(sTender) & 0xFFFFFFF0 | 4, G_EX_PUSH, G_MTX_MODELVIEW,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06022db8 /* d_course_kalimari_desert_dl_22DB8 */);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x444));
    vec3f_set(sp160, -17.0f, 6.0f, -8.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

    // @recomp Tag the transform
    gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT(sTender) & 0xFFFFFFF0 | 5, G_EX_PUSH, G_MTX_MODELVIEW,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }
    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06022db8 /* d_course_kalimari_desert_dl_22DB8 */);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
}
#endif

#if 1
RECOMP_PATCH void render_actor_train_passenger_car(Camera* camera, struct TrainCar* actor) {
    Mat4 sp120;
    Vec3f sp160;
    Mat4 spE0;
    Mat4 spA0;

    f32 temp_f0 = is_within_render_distance(camera->pos, actor->pos, camera->rot[1], 2025.0f,
                                            gCameraZoom[camera - camera1], 9000000.0f);

    if (temp_f0 < 0.0f) {
        return;
    }

    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    mtxf_pos_rotation_xyz(sp120, actor->pos, actor->rot);

    // recomp_printf("render_actor_train_passenger_car: actor addr: %x\n", actor);

    // @recomp Tag the transform
    gEXMatrixGroupDecomposed(gDisplayListHead++, (TAG_OBJECT(actor) << 16) & 0xFFFF0000 | 0, G_EX_PUSH, G_MTX_MODELVIEW,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

    if (render_set_position(sp120, 0) == 0) {
        return;
    }

    if (1 /* temp_f0 < 250000.0f*/) {

        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06020a20 /* d_course_kalimari_desert_dl_20A20 */);
        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06020a08 /* d_course_kalimari_desert_dl_20A08*/);

    } else if (temp_f0 < 1000000.0f) {

        // gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_21550);
        // gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_21220);
    } else {
        // gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_21C90);
        // gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_21A80);
    }

    if (1440000.0f < temp_f0) {
        return;
    }

    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06022d28 /* d_course_kalimari_desert_dl_22D28 */);

    mtxf_rotate_x(sp120, actor->wheelRot);
    vec3f_set(sp160, 17.0f, 6.0f, 28.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

    // @recomp Tag the transform
    gEXMatrixGroupDecomposed(gDisplayListHead++, (TAG_OBJECT(actor) << 16) & 0xFFFF0000 | 1, G_EX_PUSH, G_MTX_MODELVIEW,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06022db8 /* d_course_kalimari_desert_dl_22DB8 */);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, actor->wheelRot);
    vec3f_set(sp160, -17.0, 6.0f, 28.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

    // @recomp Tag the transform
    gEXMatrixGroupDecomposed(gDisplayListHead++, (TAG_OBJECT(actor) << 16) & 0xFFFF0000 | 2, G_EX_PUSH, G_MTX_MODELVIEW,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06022db8 /* d_course_kalimari_desert_dl_22DB8 */);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x222));
    vec3f_set(sp160, 17.0f, 6.0f, 12.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

    // @recomp Tag the transform
    gEXMatrixGroupDecomposed(gDisplayListHead++, (TAG_OBJECT(actor) << 16) & 0xFFFF0000 | 3, G_EX_PUSH, G_MTX_MODELVIEW,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06022db8 /* d_course_kalimari_desert_dl_22DB8 */);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x222));
    vec3f_set(sp160, -17.0f, 6.0f, 12.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

    // @recomp Tag the transform
    gEXMatrixGroupDecomposed(gDisplayListHead++, (TAG_OBJECT(actor) << 16) & 0xFFFF0000 | 4, G_EX_PUSH, G_MTX_MODELVIEW,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06022db8 /* d_course_kalimari_desert_dl_22DB8 */);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x5B0));
    vec3f_set(sp160, 17.0f, 6.0f, -8.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

    // @recomp Tag the transform
    gEXMatrixGroupDecomposed(gDisplayListHead++, (TAG_OBJECT(actor) << 16) & 0xFFFF0000 | 5, G_EX_PUSH, G_MTX_MODELVIEW,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06022db8 /* d_course_kalimari_desert_dl_22DB8 */);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x5B0));
    vec3f_set(sp160, -17.0f, 6.0f, -8.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

    // @recomp Tag the transform
    gEXMatrixGroupDecomposed(gDisplayListHead++, (TAG_OBJECT(actor) << 16) & 0xFFFF0000 | 6, G_EX_PUSH, G_MTX_MODELVIEW,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06022db8 /* d_course_kalimari_desert_dl_22DB8 */);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x16C));
    vec3f_set(sp160, 17.0f, 6.0f, -24.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

    // @recomp Tag the transform
    gEXMatrixGroupDecomposed(gDisplayListHead++, (TAG_OBJECT(actor) << 16) & 0xFFFF0000 | 7, G_EX_PUSH, G_MTX_MODELVIEW,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06022db8 /* d_course_kalimari_desert_dl_22DB8 */);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    mtxf_rotate_x(sp120, (s16) (actor->wheelRot + 0x16C));
    vec3f_set(sp160, -17.0f, 6.0f, -24.0f);
    mtxf_translate(spE0, sp160);
    mtxf_multiplication(spA0, sp120, spE0);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

    // @recomp Tag the transform
    gEXMatrixGroupDecomposed(gDisplayListHead++, (TAG_OBJECT(actor) << 16) & 0xFFFF0000 | 8, G_EX_PUSH, G_MTX_MODELVIEW,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06022db8 /* d_course_kalimari_desert_dl_22DB8 */);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
}
#endif

#if 1
RECOMP_PATCH // Trains smoke particles.
    void
    render_object_trains_smoke_particles(s32 cameraId) {
    UNUSED s32 pad;
    UNUSED s32 j;
    Camera* camera;
    s32 i;

    camera = &camera1[cameraId];
    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0D007AE0 /* D_0D007AE0 */);
    load_texture_block_i8_nomirror((u8*) 0x0D029458 /*D_0D029458 */, 32, 32);
    func_8004B72C(255, 255, 255, 255, 255, 255, 255);
    D_80183E80[0] = 0;
    D_80183E80[2] = 0x8000;

// Render smoke for any number of trains. Don't know enough about these variables yet.
#ifdef AVOID_UB_WIP
    for (j = 0; j < NUM_TRAINS; j++) {
        if ((gTrainList[j].someFlags != 0) &&
            (is_particle_on_screen(&gTrainList[j].locomotive.position, camera, 0x4000U) != 0)) {

            for (i = 0; i < 128; i++) {
                // Need to make a way to increase this array for each train.
                render_object_train_smoke_particle(gObjectParticle2[i], cameraId);
            }
        }
    }
#else

    if ((gTrainList[0].someFlags != 0) &&
        (is_particle_on_screen(gTrainList[0].locomotive.position, camera, 0x4000U) != 0)) {

        for (i = 0; i < gObjectParticle2_SIZE; i++) {
            // @recomp Tag the transform
            gEXMatrixGroupDecomposed(gDisplayListHead++, (TAG_OBJECT(&gTrainList[0]) << 16) & 0xFFFF0000 | i, G_EX_PUSH,
                                     G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

            render_object_train_smoke_particle(gObjectParticle2[i], cameraId);

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
        }
    }

    if ((gTrainList[1].someFlags != 0) &&
        (is_particle_on_screen(gTrainList[1].locomotive.position, camera, 0x4000U) != 0)) {
        for (i = 0; i < gObjectParticle3_SIZE; i++) {
            // @recomp Tag the transform
            gEXMatrixGroupDecomposed(gDisplayListHead++, (TAG_OBJECT(&gTrainList[1]) << 16) & 0xFFFF0000 | i, G_EX_PUSH,
                                     G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

            render_object_train_smoke_particle(gObjectParticle3[i], cameraId);

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
        }
    }
#endif
}
#endif

#if 1
/**
 * @brief Renders the a first variant of cactus in Kalimari Desert.
 *
 * @param camera
 * @param arg1
 * @param arg2
 */
RECOMP_PATCH void render_actor_tree_cactus1_kalimari_desert(Camera* camera, Mat4 arg1, struct Actor* arg2) {
    f32 temp_f0;
    s16 temp_v0 = arg2->flags;

    if ((temp_v0 & 0x800) != 0) {
        return;
    }

    temp_f0 =
        is_within_render_distance(camera->pos, arg2->pos, camera->rot[1], 0, gCameraZoom[camera - camera1], 4000000.0f);

    if (temp_f0 < 0.0f) {
        return;
    }

    if (((temp_v0 & 0x400) == 0) && (temp_f0 < 40000.0f)) {
        func_8029794C(arg2->pos, arg2->rot, 1.0f);
    }
    arg1[3][0] = arg2->pos[0];
    arg1[3][1] = arg2->pos[1];
    arg1[3][2] = arg2->pos[2];

    // @recomp Tag the transform
    gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT(arg2), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

    if (render_set_position(arg1, 0) != 0) {
        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06008528 /* d_course_kalimari_desert_dl_cactus1 */);
    }

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
}
#endif

#if 1
/**
 * @brief Renders the a second variant of cactus in Kalimari Desert.
 *
 * @param camera
 * @param arg1
 * @param arg2
 */
RECOMP_PATCH void render_actor_tree_cactus2_kalimari_desert(Camera* camera, Mat4 arg1, struct Actor* arg2) {
    f32 temp_f0;
    s16 temp_v0 = arg2->flags;

    if ((temp_v0 & 0x800) != 0) {
        return;
    }

    temp_f0 =
        is_within_render_distance(camera->pos, arg2->pos, camera->rot[1], 0, gCameraZoom[camera - camera1], 4000000.0f);

    if (temp_f0 < 0.0f) {
        return;
    }

    if (((temp_v0 & 0x400) == 0) && (temp_f0 < 40000.0f)) {
        func_8029794C(arg2->pos, arg2->rot, 1.0f);
    }
    arg1[3][0] = arg2->pos[0];
    arg1[3][1] = arg2->pos[1];
    arg1[3][2] = arg2->pos[2];

    // @recomp Tag the transform
    gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT(arg2), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

    if (render_set_position(arg1, 0) != 0) {
        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06008628 /* d_course_kalimari_desert_dl_cactus2 */);
    }

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
}
#endif

#if 1
/**
 * @brief Renders the a third variant of cactus in Kalimari Desert.
 *
 * @param camera
 * @param arg1
 * @param arg2
 */
RECOMP_PATCH void render_actor_tree_cactus3_kalimari_desert(Camera* camera, Mat4 arg1, struct Actor* arg2) {
    f32 temp_f0;
    s16 temp_v0 = arg2->flags;

    if ((temp_v0 & 0x800) != 0) {
        return;
    }

    temp_f0 =
        is_within_render_distance(camera->pos, arg2->pos, camera->rot[1], 0, gCameraZoom[camera - camera1], 4000000.0f);

    if (temp_f0 < 0.0f) {
        return;
    }

    if (((temp_v0 & 0x400) == 0) && (temp_f0 < 40000.0f)) {
        func_8029794C(arg2->pos, arg2->rot, 0.80000001f);
    }
    arg1[3][0] = arg2->pos[0];
    arg1[3][1] = arg2->pos[1];
    arg1[3][2] = arg2->pos[2];

    // @recomp Tag the transform
    gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT(arg2), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

    if (render_set_position(arg1, 0) != 0) {
        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06008728 /* d_course_kalimari_desert_dl_cactus3 */);
    }

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
}
#endif

#if 1
/**
 * @brief Renders the railroad crossing actor.
 * Actor used in Kalimari Desert.
 *
 * @param arg0
 * @param rr_crossing
 */
RECOMP_PATCH void render_actor_railroad_crossing(Camera* arg0, struct RailroadCrossing* rr_crossing) {
    UNUSED Vec3s sp80 = { 0, 0, 0 };
    Mat4 sp40;
    f32 unk = is_within_render_distance(arg0->pos, rr_crossing->pos, arg0->rot[1], 0.0f, gCameraZoom[arg0 - camera1],
                                        4000000.0f);

    if (!(unk < 0.0f)) {

        // @recomp Tag the transform
        gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT(rr_crossing), G_EX_PUSH, G_MTX_MODELVIEW,
                                 G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

        mtxf_pos_rotation_xyz(sp40, rr_crossing->pos, rr_crossing->rot);

        if (render_set_position(sp40, 0) != 0) {
            gSPSetGeometryMode(gDisplayListHead++, G_LIGHTING);
            gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);

            if (isCrossingTriggeredByIndex[rr_crossing->crossingId]) {
                if (rr_crossing->someTimer < 20) {
                    gSPDisplayList(gDisplayListHead++,
                                   (Gfx*) 0x06010ae8 /* d_course_kalimari_desert_dl_crossing_right_active */);
                } else {
                    gSPDisplayList(gDisplayListHead++,
                                   (Gfx*) 0x06010c10 /* d_course_kalimari_desert_dl_crossing_left_active */);
                }
            } else {
                gSPDisplayList(gDisplayListHead++,
                               (Gfx*) 0x06010d38 /* d_course_kalimari_desert_dl_crossing_both_inactive */);
            }
            gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
        }

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
}
#endif