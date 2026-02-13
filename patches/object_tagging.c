#include "patches.h"

#if 1 // GLOBAL
typedef enum ObjectList {
    /* 10 */ OBJECT_10 = 10,
    /* 10 */ OBJECT_11,
    /* 10 */ OBJECT_12
} ObjectList;

// Sets statis visibility for some objects
// used by:
// render_object_trash_bin
// render_object_snowmans_list_2
// render_object_snowmans_list_1
// render_object_smoke_particles
// func_80054D00 (moles)
// func_80054F04 (moles)
// func_80055228 (Unk Yoshi Valley object)
// render_object_seagulls
// render_object_crabs
// render_object_hedgehogs
// render_object_train_penguins
// func_80057114 (VS bomb karts?)
#if 0 // CRASHES THE GAME IN THE 4 PLAYER SHERBET LAND DEMO
RECOMP_PATCH s32 func_8008A364(s32 objectIndex, s32 cameraId, u16 arg2, s32 arg3) {
    Camera* camera;
    u32 dist;
    u16 var_a2;

    camera = &camera1[cameraId];
    set_object_flag_status_false(objectIndex, 0x00020000 | VISIBLE);
    dist = get_horizontal_distance_to_camera(objectIndex, camera);
    if (1 /* dist < (arg3 * arg3)*/) {
        set_object_flag_status_true(objectIndex, 0x00020000);
        // if (dist < 0x2711U) {
        //     var_a2 = 0x5555;
        // } else if (dist < 0x9C41U) {
        //     var_a2 = 0x4000;
        // } else {
        //     var_a2 = arg2;
        // }
        if (1 /* is_object_visible_on_camera(objectIndex, camera, var_a2) != 0 */) {
            set_object_flag_status_true(objectIndex, VISIBLE);
        }
    }
    return dist;
}
#endif

#if 1
RECOMP_PATCH void render_player(Player* player, s8 playerId, s8 screenId) {
    UNUSED s32 pad[2];
    s32 temp_t1;
    s32 var_v1;
    OSMesg* sp34;

    if ((gCamera1Skipped && (screenId == 0)) || (gCamera2Skipped && (screenId == 1)) ||
        (gCamera3Skipped && (screenId == 2)) || (gCamera4Skipped && (screenId == 3))) {
        // Skip
        // @recomp Tag the transform
        gEXMatrixGroupDecomposed(gDisplayListHead++, (TAG_OBJECT(player) << 16) | playerId << 8 | screenId, G_EX_PUSH, G_MTX_MODELVIEW,
                                 G_EX_COMPONENT_AUTO, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_AUTO,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);
    } else {
        // @recomp Tag the transform
        gEXMatrixGroupDecomposed(gDisplayListHead++, (TAG_OBJECT(player) << 16) | playerId << 8 | screenId, G_EX_PUSH, G_MTX_MODELVIEW,
                                 G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);
    }

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
        if ((gCamera1Skipped && (screenId == 0)) || (gCamera2Skipped && (screenId == 1)) ||
            (gCamera3Skipped && (screenId == 2)) || (gCamera4Skipped && (screenId == 3))) {
            // Skip
            // @recomp Tag the transform
            gEXMatrixGroupDecomposed(gDisplayListHead++,
                                     (TAG_OBJECT(player) << 16) & 0xFFFF0000 | playerId << 8 | screenId, G_EX_PUSH,
                                     G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_AUTO,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);
        } else {
            gEXMatrixGroupDecomposed(gDisplayListHead++,
                                     (TAG_OBJECT(player) << 16) & 0xFFFF0000 | playerId << 8 | screenId, G_EX_PUSH,
                                     G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);
        }
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
        if ((gCamera1Skipped && (screenId == 0)) || (gCamera2Skipped && (screenId == 1)) ||
            (gCamera3Skipped && (screenId == 2)) || (gCamera4Skipped && (screenId == 3))) {
            // Skip
            // @recomp Tag the transform
            gEXMatrixGroupDecomposed(gDisplayListHead++,
                                     (TAG_OBJECT(player) << 16) & 0xFFFF0000 | playerId << 8 | screenId, G_EX_PUSH,
                                     G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_AUTO,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);
        } else {
            gEXMatrixGroupDecomposed(gDisplayListHead++,
                                     (TAG_OBJECT(player) << 16) & 0xFFFF0000 | playerId << 8 | screenId, G_EX_PUSH,
                                     G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);
        }
        render_kart(player, playerId, screenId, var_v1);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    } else {
        if ((gCamera1Skipped && (screenId == 0)) || (gCamera2Skipped && (screenId == 1)) ||
            (gCamera3Skipped && (screenId == 2)) || (gCamera4Skipped && (screenId == 3))) {
            // Skip
            // @recomp Tag the transform
            gEXMatrixGroupDecomposed(gDisplayListHead++,
                                     (TAG_OBJECT(player) << 16) & 0xFFFF0000 | playerId << 8 | screenId, G_EX_PUSH,
                                     G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_AUTO,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);
        } else {
            gEXMatrixGroupDecomposed(gDisplayListHead++,
                                     (TAG_OBJECT(player) << 16) & 0xFFFF0000 | playerId << 8 | screenId, G_EX_PUSH,
                                     G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);
        }
        render_ghost(player, playerId, screenId, var_v1);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
    osRecvMesg(&gDmaMesgQueue, (OSMesg*) &sp34, OS_MESG_BLOCK);
    if ((temp_t1 == (player->unk_002 & temp_t1)) && (player->surfaceType == ICE) && ((player->unk_0CA & 1) != 1) &&
        (player->collision.surfaceDistance[2] <= 30.0f)) {
        if ((gCamera1Skipped && (screenId == 0)) || (gCamera2Skipped && (screenId == 1)) ||
            (gCamera3Skipped && (screenId == 2)) || (gCamera4Skipped && (screenId == 3))) {
            // Skip
            // @recomp Tag the transform
            gEXMatrixGroupDecomposed(gDisplayListHead++,
                                     (TAG_OBJECT(player) << 16) & 0xFFFF0000 | playerId << 8 | screenId, G_EX_PUSH,
                                     G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_AUTO,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);
        } else {
            gEXMatrixGroupDecomposed(gDisplayListHead++,
                                     (TAG_OBJECT(player) << 16) & 0xFFFF0000 | playerId << 8 | screenId, G_EX_PUSH,
                                     G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);
        }
        render_player_ice_reflection(player, playerId, screenId, var_v1);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
    if (player->boostPower >= 2.0f) {
        if ((gCamera1Skipped && (screenId == 0)) || (gCamera2Skipped && (screenId == 1)) ||
            (gCamera3Skipped && (screenId == 2)) || (gCamera4Skipped && (screenId == 3))) {
            // Skip
            // @recomp Tag the transform
            gEXMatrixGroupDecomposed(gDisplayListHead++,
                                     (TAG_OBJECT(player) << 16) & 0xFFFF0000 | playerId << 8 | screenId, G_EX_PUSH,
                                     G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_AUTO,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);
        } else {
            gEXMatrixGroupDecomposed(gDisplayListHead++,
                                     (TAG_OBJECT(player) << 16) & 0xFFFF0000 | playerId << 8 | screenId, G_EX_PUSH,
                                     G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);
        }
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

// Ceremony balloons
#if 1
RECOMP_PATCH void balloon_update(CeremonyActor* actor) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gDisplayListHead++, (u32) actor, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    render_balloon(actor->pos, 1.0f, actor->unk2E, actor->unk2C);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

    actor->pos[1] += 0.8f;
    actor->unk2E = sins(actor->unk30) * actor->unk34;
    actor->unk30 += actor->unk32;
    actor->timer++;

    // Delete actor
    if (actor->timer > 800) {
        actor->isActive = 0;
    }
    if (D_802874B0[13] == 1) {
        actor->isActive = 0;
    }
}
#endif

// Ceremony trophy star particles
#if 1
RECOMP_PATCH void func_80054BE8(s32 cameraId) {
    s32 i;
    s32 temp_a0;
    Camera* camera;

    camera = &camera1[cameraId];
    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0D007AE0);
    load_texture_block_ia8_nomirror(D_8018D488, 0x00000020, 0x00000020);
    func_8004B35C(0x000000FF, 0x000000FF, 0, 0x000000FF);
    D_80183E80[0] = 0;
    for (i = 0; i < gObjectParticle3_SIZE; i++) {
        temp_a0 = gObjectParticle3[i];
        if ((temp_a0 != -1) && (gObjectList[temp_a0].state >= 2)) {
            // @recomp Tag the transform.
            gEXMatrixGroupDecomposedNormal(gDisplayListHead++, (u32) &gObjectList[temp_a0] << 16 | i, G_EX_PUSH,
                                           G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

            func_80054AFC(temp_a0, camera->pos);
            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
        }
    }
}
#endif

#if 1
RECOMP_PATCH void firework_update(Firework* actor) {
    s32 i;
    Vec3f pos;
    if (actor->unk44 < 30) {
        for (i = 0; i < 10; i++) {
            pos[0] = actor->pos[0];
            pos[1] = actor->pos[1] - i * 2;
            pos[2] = actor->pos[2];
            // @recomp Tag the transform.
            gEXMatrixGroupDecomposedNormal(gDisplayListHead++, (u32) actor + i, G_EX_PUSH, G_MTX_MODELVIEW,
                                           G_EX_EDIT_ALLOW);

            render_fireworks(pos, ((10 - i) / 10.0f) * 2, fireworkColour[actor->unk48],
                             (((((30 - actor->unk44) * 100)) / 30.0f)));

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
        }

    } else {
        if (actor->unk2C < 5) {
            actor->unk3C += actor->unk40 * 2;
            actor->unk34 += actor->unk38 * 2;
        } else {
            actor->unk3C += actor->unk40 / (1.0f + (((actor->unk2C * 7) - 0x23) / 10.0f));
            actor->unk34 += actor->unk38 / (1.0f + (((actor->unk2C * 7) - 0x23) / 10.0f));
            if (actor->unk3C < 0) {
                actor->unk3C = 0;
            }
        }
        actor->unk2C += 1;
        if (((actor->unk3C > 0) && ((actor->unk34 > 0.0f))) || (actor->unk2C < 30)) {
            // @recomp Tag the transform.
            gEXMatrixGroupDecomposedNormal(gDisplayListHead++, (u32) actor << 16, G_EX_PUSH, G_MTX_MODELVIEW,
                                           G_EX_EDIT_ALLOW);

            render_fireworks(actor->pos, actor->unk34, actor->unk30, (s16) actor->unk3C);

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
        } else {
            actor->isActive = 0;
        }
    }
    actor->unk44 += 1;
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
#if 0
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

// 0x21CA8
Vtx d_course_kalimari_desert_unknown_model1[] = {
    { { { 0, 12, 12 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 0, -12, 12 }, 0, { 0, 2048 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 0, -12, -12 }, 0, { 2048, 2048 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 0, 12, -12 }, 0, { 2048, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};
Vtx d_course_kalimari_desert_unknown_model2[] = {
    { { { 0, 6, 6 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 0, -6, 6 }, 0, { 0, 2048 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 0, -6, -6 }, 0, { 2048, 2048 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 0, 6, -6 }, 0, { 2048, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

// @recomp there's absolutely zero reason for the train wheels to be using decals
// I'm commenting them out since they cause performance issues in accurate renderers for nothing.
Gfx d_course_kalimari_desert_dl_22D70[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA),
    gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2),
    gsSPVertex(d_course_kalimari_desert_unknown_model1, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    // gsDPSetRenderMode(G_RM_AA_ZB_XLU_DECAL, G_RM_AA_ZB_XLU_DECAL2),
    // gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSPEndDisplayList(),
};

Gfx d_course_kalimari_desert_dl_22DB8[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA),
    gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2),
    gsSPVertex(d_course_kalimari_desert_unknown_model2, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    // gsDPSetRenderMode(G_RM_AA_ZB_XLU_DECAL, G_RM_AA_ZB_XLU_DECAL2),
    // gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSPEndDisplayList(),
};

#if 1
/**
 * @brief Renders the train engine actor.
 * Actor used in Kalimari Desert.
 * His update position are made in vehicle.
 *
 * @param camera
 * @param actor
 */
RECOMP_PATCH void render_actor_train_engine(Camera* camera, struct TrainCar* actor) {
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
                             G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

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
                             G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

    maxObjectsReached = render_set_position(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8 /* d_course_kalimari_desert_dl_22DB8 */);
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
                             G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

    maxObjectsReached = render_set_position(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8 /* d_course_kalimari_desert_dl_22DB8 */);
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
                             G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

    maxObjectsReached = render_set_position(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8 /* d_course_kalimari_desert_dl_22DB8 */);
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
                             G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

    maxObjectsReached = render_set_position(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8 /* d_course_kalimari_desert_dl_22DB8 */);
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
                             G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

    maxObjectsReached = render_set_position(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22D70 /* d_course_kalimari_desert_dl_22D70 */);
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
                             G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

    maxObjectsReached = render_set_position(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22D70 /* d_course_kalimari_desert_dl_22D70 */);
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
                             G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

    maxObjectsReached = render_set_position(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22D70 /* d_course_kalimari_desert_dl_22D70 */);
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
                             G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

    maxObjectsReached = render_set_position(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22D70 /* d_course_kalimari_desert_dl_22D70 */);
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
                             G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

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
    // if (1440000.0f < temp_f0) {
    //     return;
    // }

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
                             G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8 /* d_course_kalimari_desert_dl_22DB8 */);
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
                             G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

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
                             G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8 /* d_course_kalimari_desert_dl_22DB8 */);

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
                             G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8 /* d_course_kalimari_desert_dl_22DB8 */);
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
                             G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }
    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8 /* d_course_kalimari_desert_dl_22DB8 */);
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
                             G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

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

    // if (1440000.0f < temp_f0) {
    //     return;
    // }

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
                             G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8 /* d_course_kalimari_desert_dl_22DB8 */);
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
                             G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8 /* d_course_kalimari_desert_dl_22DB8 */);
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
                             G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8 /* d_course_kalimari_desert_dl_22DB8 */);
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
                             G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8 /* d_course_kalimari_desert_dl_22DB8 */);
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
                             G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8 /* d_course_kalimari_desert_dl_22DB8 */);
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
                             G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8 /* d_course_kalimari_desert_dl_22DB8 */);
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
                             G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8 /* d_course_kalimari_desert_dl_22DB8 */);
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
                             G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

    if (render_set_position(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8 /* d_course_kalimari_desert_dl_22DB8 */);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
}
#endif

// Trains smoke particles.
#if 1
RECOMP_PATCH void render_object_trains_smoke_particles(s32 cameraId) {
    Camera* camera;
    s32 i;

    camera = &camera1[cameraId];
    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0D007AE0 /* D_0D007AE0 */);
    load_texture_block_i8_nomirror((u8*) 0x0D029458 /*D_0D029458 */, 32, 32);
    func_8004B72C(255, 255, 255, 255, 255, 255, 255);
    D_80183E80[0] = 0;
    D_80183E80[2] = 0x8000;

    if ((gTrainList[0].someFlags != 0) &&
        (is_particle_on_screen(gTrainList[0].locomotive.position, camera, 0x4000U) != 0)) {

        for (i = 0; i < gObjectParticle2_SIZE; i++) {
            // @recomp Tag the transform
            gEXMatrixGroupDecomposed(gDisplayListHead++, (TAG_OBJECT(&gTrainList[0]) << 16) & 0xFFFF0000 | i, G_EX_PUSH,
                                     G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

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
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

            render_object_train_smoke_particle(gObjectParticle3[i], cameraId);

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
        }
    }
}
#endif

#if 1
RECOMP_PATCH void render_course_actors(struct UnkStruct_800DC5EC* arg0) {
    Camera* camera = arg0->camera;
    u16 pathCounter = arg0->pathCounter;
    UNUSED s32 pad[12];
    s32 i;

    struct Actor* actor;
    UNUSED Vec3f sp4C = { 0.0f, 5.0f, 10.0f };
    f32 sp48 = sins(camera->rot[1] - 0x8000); // unk26;
    f32 temp_f0 = coss(camera->rot[1] - 0x8000);

    D_801502C0[0][0] = temp_f0;
    D_801502C0[0][2] = -sp48;
    D_801502C0[2][2] = temp_f0;
    D_801502C0[1][0] = 0.0f;
    D_801502C0[0][1] = 0.0f;
    D_801502C0[2][1] = 0.0f;
    D_801502C0[1][2] = 0.0f;
    D_801502C0[0][3] = 0.0f;
    D_801502C0[1][3] = 0.0f;
    D_801502C0[2][3] = 0.0f; // 2c
    D_801502C0[2][0] = sp48;
    D_801502C0[1][1] = 1.0f;
    D_801502C0[3][3] = 1.0f; // unk3c

    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gSPSetLights1(gDisplayListHead++, D_800DC610[1]);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);

    if (gModeSelection != BATTLE) {
        func_80297340(camera);
    }
    D_8015F8E0 = 0;

    // recomp_printf("gCamera1Skipped: %d \n", gCamera1Skipped);
    // recomp_printf("camera->playerId: %d \n", camera->playerId);

    for (i = 0; i < ACTOR_LIST_SIZE; i++) {
        actor = &gActorList[i];

        if (actor->flags == 0) {
            continue;
        }

        if ((gCamera1Skipped && (camera->playerId == 0)) || (gCamera2Skipped && (camera->playerId == 1)) ||
            (gCamera3Skipped && (camera->playerId == 2)) || (gCamera4Skipped && (camera->playerId == 3))) {
            // Skip
            // @recomp Tag the transform
            gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT((u32) actor << 16) | 0x01000000, G_EX_PUSH,
                                     G_MTX_MODELVIEW, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP,
                                     G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP,
                                     G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

#if 0
            recomp_printf("render_course_actors: actor->type %d Camera %d skipped \n", actor->type, camera->playerId);

            if (gCamera1Skipped) {
                recomp_printf("gCamera1Skipped = %d \n", gCamera1Skipped);
            }
            if (gCamera2Skipped) {
                recomp_printf("gCamera2Skipped = %d \n", gCamera2Skipped);
            }
            if (gCamera3Skipped) {
                recomp_printf("gCamera3Skipped = %d \n", gCamera3Skipped);
            }
            if (gCamera4Skipped) {
                recomp_printf("gCamera4Skipped = %d \n", gCamera4Skipped);
            }
#endif
        } else {
            // @recomp Tag the transform.
            gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_OBJECT(actor), G_EX_PUSH, G_MTX_MODELVIEW,
                                           G_EX_EDIT_ALLOW);
        }

        switch (actor->type) {
            case ACTOR_TREE_MARIO_RACEWAY:
                render_actor_tree_mario_raceway(camera, D_801502C0, actor);
                break;
            case ACTOR_TREE_YOSHI_VALLEY:
                render_actor_tree_yoshi_valley(camera, D_801502C0, actor);
                break;
            case ACTOR_TREE_ROYAL_RACEWAY:
                render_actor_tree_royal_raceway(camera, D_801502C0, actor);
                break;
            case ACTOR_TREE_MOO_MOO_FARM:
                render_actor_tree_moo_moo_farm(camera, D_801502C0, actor);
                break;
            case ACTOR_UNKNOWN_0x1A:
                func_80299864(camera, D_801502C0, actor);
                break;
            case ACTOR_TREE_BOWSERS_CASTLE:
                render_actor_tree_bowser_castle(camera, D_801502C0, actor);
                break;
            case ACTOR_BUSH_BOWSERS_CASTLE:
                render_actor_bush_bowser_castle(camera, D_801502C0, actor);
                break;
            case ACTOR_TREE_FRAPPE_SNOWLAND:
                render_actor_tree_frappe_snowland(camera, D_801502C0, actor);
                break;
            case ACTOR_CACTUS1_KALAMARI_DESERT:
                render_actor_tree_cactus1_kalimari_desert(camera, D_801502C0, actor);
                break;
            case ACTOR_CACTUS2_KALAMARI_DESERT:
                render_actor_tree_cactus2_kalimari_desert(camera, D_801502C0, actor);
                break;
            case ACTOR_CACTUS3_KALAMARI_DESERT:
                render_actor_tree_cactus3_kalimari_desert(camera, D_801502C0, actor);
                break;
            case ACTOR_FALLING_ROCK:
                // @recomp Tag the transform
                gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT((u32) actor << 16) | 0x01000000, G_EX_PUSH,
                                         G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                         G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                         G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

                render_actor_falling_rock(camera, (struct FallingRock*) actor);

                // @recomp Pop the transform id.
                gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
                break;
            case ACTOR_KIWANO_FRUIT:
                render_actor_kiwano_fruit(camera, D_801502C0, actor);
                break;
            case ACTOR_BANANA:
                render_actor_banana(camera, D_801502C0, (struct BananaActor*) actor);
                break;
            case ACTOR_GREEN_SHELL:
                render_actor_green_shell(camera, D_801502C0, (struct ShellActor*) actor);
                break;
            case ACTOR_RED_SHELL:
                render_actor_red_shell(camera, D_801502C0, (struct ShellActor*) actor);
                break;
            case ACTOR_BLUE_SPINY_SHELL:
                render_actor_blue_shell(camera, D_801502C0, (struct ShellActor*) actor);
                break;
            case ACTOR_PIRANHA_PLANT:
                render_actor_piranha_plant(camera, D_801502C0, (struct PiranhaPlant*) actor);
                break;
            case ACTOR_TRAIN_ENGINE:
                render_actor_train_engine(camera, (struct TrainCar*) actor);
                break;
            case ACTOR_TRAIN_TENDER:
                render_actor_train_tender(camera, (struct TrainCar*) actor);
                break;
            case ACTOR_TRAIN_PASSENGER_CAR:
                render_actor_train_passenger_car(camera, (struct TrainCar*) actor);
                break;
            case ACTOR_COW:
                render_actor_cow(camera, D_801502C0, actor);
                break;
            case ACTOR_UNKNOWN_0x14:
                func_8029AC18(camera, D_801502C0, actor);
                break;
            case ACTOR_MARIO_SIGN:
                render_actor_mario_sign(camera, D_801502C0, actor);
                break;
            case ACTOR_WARIO_SIGN:
                render_actor_wario_sign(camera, actor);
                break;
            case ACTOR_PALM_TREE:
                render_actor_palm_tree(camera, D_801502C0, (struct PalmTree*) actor);
                break;
            case ACTOR_PADDLE_BOAT:
                render_actor_paddle_boat(camera, (struct PaddleWheelBoat*) actor, D_801502C0, pathCounter);
                break;
            case ACTOR_BOX_TRUCK:
                render_actor_box_truck(camera, actor);
                break;
            case ACTOR_SCHOOL_BUS:
                render_actor_school_bus(camera, actor);
                break;
            case ACTOR_TANKER_TRUCK:
                render_actor_tanker_truck(camera, actor);
                break;
            case ACTOR_CAR:
                render_actor_car(camera, actor);
                break;
            case ACTOR_RAILROAD_CROSSING:
                render_actor_railroad_crossing(camera, (struct RailroadCrossing*) actor);
                break;
            case ACTOR_YOSHI_EGG:
                render_actor_yoshi_egg(camera, D_801502C0, (struct YoshiValleyEgg*) actor, pathCounter);
                break;
        }
        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }

    if ((gCamera1Skipped && (camera->playerId == 1)) || (gCamera2Skipped && (camera->playerId == 2)) ||
        (gCamera3Skipped && (camera->playerId == 3)) || (gCamera4Skipped && (camera->playerId == 4))) {
        // Skip
        // @recomp Tag the transform
        gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT((u32) actor << 16) | 0x01000000, G_EX_PUSH,
                                 G_MTX_MODELVIEW, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP,
                                 G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP,
                                 G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);
        // recomp_printf("render_course_actors: Camera %d skipped \n", camera->playerId);
    } else {
        // @recomp Tag the transform
        gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT((u32) actor << 16) | 0x01000000, G_EX_PUSH,
                                 G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);
    }

    switch (gCurrentCourseId) {
        case COURSE_MOO_MOO_FARM:
            render_cows(camera, D_801502C0, actor);
            break;
        case COURSE_DK_JUNGLE:
            render_palm_trees(camera, D_801502C0, actor);
            break;
    }
    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
}
#endif

#if 1
RECOMP_PATCH void render_object_for_player(s32 cameraId) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_OBJECT_BY_CAM_ID(cameraId), G_EX_PUSH, G_MTX_MODELVIEW,
                                   G_EX_EDIT_ALLOW);

    switch (gCurrentCourseId) {
        case COURSE_MARIO_RACEWAY:
            break;
        case COURSE_CHOCO_MOUNTAIN:
            break;
        case COURSE_BOWSER_CASTLE:
            render_object_thwomps(cameraId);
            render_object_bowser_flame(cameraId);
            break;
        case COURSE_BANSHEE_BOARDWALK:
            if (gGamestate != CREDITS_SEQUENCE) {
                render_object_trash_bin(cameraId);
                render_object_bat(cameraId);
                func_8005217C(cameraId);
                render_object_boos(cameraId);
            }
            break;
        case COURSE_YOSHI_VALLEY:
            func_80055228(cameraId);
            if (gGamestate != CREDITS_SEQUENCE) {
                render_object_hedgehogs(cameraId);
            }
            break;
        case COURSE_FRAPPE_SNOWLAND:
            if (gGamestate != CREDITS_SEQUENCE) {
                render_object_snowmans(cameraId);
            }
            break;
        case COURSE_KOOPA_BEACH:
            if (gGamestate != CREDITS_SEQUENCE) {
                render_object_crabs(cameraId);
            }
            if (gGamestate != CREDITS_SEQUENCE) {
                if ((gPlayerCount == 1) || (gPlayerCount == 2)) {
                    render_object_seagulls(cameraId);
                }
            } else {
                render_object_seagulls(cameraId);
            }
            break;
        case COURSE_ROYAL_RACEWAY:
            break;
        case COURSE_LUIGI_RACEWAY:
            if (D_80165898 != 0) {
                render_object_hot_air_balloon(cameraId);
            }
            break;
        case COURSE_MOO_MOO_FARM:
            if (gGamestate != CREDITS_SEQUENCE) {
                render_object_moles(cameraId);
            }
            break;
        case COURSE_TOADS_TURNPIKE:
            break;
        case COURSE_KALAMARI_DESERT:
            render_object_trains_smoke_particles(cameraId);
            break;
        case COURSE_SHERBET_LAND:
            if (gGamestate != CREDITS_SEQUENCE) {
                func_80052E30(cameraId);
            }
            render_object_train_penguins(cameraId);
            break;
        case COURSE_RAINBOW_ROAD:
            if (gGamestate != CREDITS_SEQUENCE) {
                render_object_neon(cameraId);
                render_object_chain_chomps(cameraId);
            }
            break;
        case COURSE_WARIO_STADIUM:
            break;
        case COURSE_BLOCK_FORT:
            break;
        case COURSE_SKYSCRAPER:
            break;
        case COURSE_DOUBLE_DECK:
            break;
        case COURSE_DK_JUNGLE:
            if (gGamestate != CREDITS_SEQUENCE) {
                render_object_paddle_boat_smoke_particles(cameraId);
            }
            break;
    }

    render_object_smoke_particles(cameraId);
    render_object_leaf_particle(cameraId);

    if (D_80165730 != 0) {
        func_80053E6C(cameraId);
    }
    if (gModeSelection == BATTLE) {
        render_object_bomb_kart(cameraId);
    }
    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
}
#endif

#if 1
RECOMP_PATCH void render_object_chain_chomps(s32 cameraId) {
    s32 i;
    s32 objectIndex;

    for (i = 0; i < NUM_CHAIN_CHOMPS; i++) {
        objectIndex = indexObjectList2[i];
        // @recomp Tag the transform.
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_OBJECT((u32) &indexObjectList2[i] << 16) | cameraId,
                                       G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

        func_8008A1D0(objectIndex, cameraId, 0x000005DC, 0x000009C4);
        if (is_obj_flag_status_active(objectIndex, VISIBLE) != 0) {
            func_80055AB8(objectIndex, cameraId);
        }
        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
}
#endif

#if 1
RECOMP_PATCH void render_object_bomb_kart(s32 cameraId) {
    Player* temp_v0;
    s32 temp_s1;
    s32 temp_s0;
    s32 i;
    Object* object;

    for (i = 0; i < 4; i++) {
        temp_s0 = gIndexObjectBombKart[i];
        object = &gObjectList[temp_s0];
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gDisplayListHead++,
                                       TAG_OBJECT((u32) &gObjectList[temp_s0] << 16) | temp_s0 & 0xFF << 8 | cameraId,
                                       G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
        if (object->state != 0) {
            temp_s1 = object->primAlpha;
            temp_v0 = &gPlayerOne[i];
            object->pos[0] = temp_v0->pos[0];
            object->pos[1] = temp_v0->pos[1] - 2.0;
            object->pos[2] = temp_v0->pos[2];
            object->surfaceHeight = temp_v0->unk_074;
            func_800563DC(temp_s0, cameraId, temp_s1);
            func_8005669C(temp_s0, cameraId, temp_s1);
            func_800568A0(temp_s0, cameraId);
        }
        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
}
#endif

#if 1
RECOMP_PATCH void func_80053E6C(s32 arg0) __attribute__((optnone)) {
    s32 i;
    s32 objectIndex;

    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0D007E98);
    gDPLoadTLUT_pal256(gDisplayListHead++, (u8*) 0x800E52D0);
    func_8004B614(0, 0, 0, 0, 0, 0, 0);
    D_80183E80[0] = 0;
    D_80183E80[1] = 0x8000;
    rsp_load_texture(D_8018D4BC, 64, 32);

    for (i = 0; i < D_80165738; i++) {
        objectIndex = gObjectParticle3[i];
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_OBJECT(&gObjectParticle3[i]) | arg0, G_EX_PUSH,
                                       G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
        if ((objectIndex != NULL_OBJECT_ID) && (gObjectList[objectIndex].state >= 2)) {
            func_80053D74(objectIndex, arg0, 0);
        }
        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }

    rsp_load_texture(D_8018D4C0, 0x40, 0x20);

    for (i = 0; i < D_80165738; i++) {
        objectIndex = gObjectParticle3[i];
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_OBJECT(&gObjectParticle3[i]) | arg0, G_EX_PUSH,
                                       G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
        if ((objectIndex != NULL_OBJECT_ID) && (gObjectList[objectIndex].state >= 2)) {
            func_80053D74(objectIndex, arg0, 4);
        }
        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
}
#endif

#if 1
RECOMP_PATCH void render_object_leaf_particle(s32 cameraId) {
    s32 i;
    s32 leafIndex;
    Object* object;

    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0D0079C8);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BOTH);
    load_texture_block_rgba16_mirror((u8*) 0x0d028dd8 /* common_texture_particle_leaf */, 32, 16);
    for (i = 0; i < gLeafParticle_SIZE; i++) {
        leafIndex = gLeafParticle[i];
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_OBJECT(&gLeafParticle[i]) | cameraId, G_EX_PUSH,
                                       G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
        if (leafIndex != -1) {
            object = &gObjectList[leafIndex];
            if ((object->state >= 2) && (object->unk_0D5 == 7) && (gMatrixHudCount <= MTX_HUD_POOL_SIZE_MAX)) {
                rsp_set_matrix_gObjectList(leafIndex);
                gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0D0069C8);
            }
        }
        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}
#endif

#if 1
RECOMP_PATCH void render_object_smoke_particles(s32 cameraId) {
    Camera* sp54;
    s32 i;
    s32 objectIndex;
    Object* object;

    sp54 = &camera1[cameraId];
    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0D007AE0);
    u8(*common_texture_particle_smoke)[1024] = (u8(*)[1024]) 0x0d02bc58;
    load_texture_block_i8_nomirror(common_texture_particle_smoke[D_80165598], 32, 32);
    func_8004B72C(255, 255, 255, 255, 255, 255, 255);
    D_80183E80[0] = 0;
    D_80183E80[2] = 0x8000;
    for (i = 0; i < gObjectParticle4_SIZE; i++) {
        objectIndex = gObjectParticle4[i];
        // @recomp Tag the transform
        gEXMatrixGroupDecomposed(gDisplayListHead++, (TAG_OBJECT(gObjectParticle4[i]) << 16) & 0xFFFF0000 | i,
                                 G_EX_PUSH, G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                 G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,
                                 G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

        if (objectIndex != NULL_OBJECT_ID) {
            object = &gObjectList[objectIndex];
            if (object->state >= 2) {
                if (object->unk_0D8 == 3) {
                    func_8008A364(objectIndex, cameraId, 0x4000U, 0x00000514);
                } else {
                    func_8008A364(objectIndex, cameraId, 0x4000U, 0x000001F4);
                }
                if (is_obj_flag_status_active(objectIndex, VISIBLE) != 0) {
                    func_8005477C(objectIndex, object->unk_0D8, sp54->pos);
                }
            }
        }
        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
}

#endif

#if 1
RECOMP_PATCH void render_object_paddle_boat_smoke_particles(s32 cameraId) {
    UNUSED s32 pad[2];
    Camera* camera;
    s32 i;

    camera = &camera1[cameraId];
    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0D007AE0);

    load_texture_block_i8_nomirror((u8*) 0x0D029458, 32, 32);
    func_8004B72C(255, 255, 255, 255, 255, 255, 255);
    D_80183E80[0] = 0;
    D_80183E80[2] = 0x8000;
    if ((gPaddleBoats[0].someFlags != 0) && (is_particle_on_screen(gPaddleBoats[0].position, camera, 0x4000U) != 0)) {
        for (i = 0; i < gObjectParticle2_SIZE; i++) {
            // @recomp Tag the transform
            gEXMatrixGroupDecomposed(gDisplayListHead++, (TAG_OBJECT(gObjectParticle2[i]) << 16) & 0xFFFF0000 | i,
                                     G_EX_PUSH, G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                     G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,
                                     G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

            render_object_paddle_boat_smoke_particle(gObjectParticle2[i], cameraId);

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
        }
    }
    if ((gPaddleBoats[1].someFlags != 0) && (is_particle_on_screen(gPaddleBoats[1].position, camera, 0x4000U) != 0)) {
        for (i = 0; i < gObjectParticle3_SIZE; i++) {
            // @recomp Tag the transform
            gEXMatrixGroupDecomposed(gDisplayListHead++, (TAG_OBJECT(gObjectParticle3[i]) << 16) & 0xFFFF0000 | i,
                                     G_EX_PUSH, G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                     G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,
                                     G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

            render_object_paddle_boat_smoke_particle(gObjectParticle3[i], cameraId);

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
        }
    }
}
#endif

#if 1
RECOMP_PATCH void render_object_neon(s32 cameraId) {
    Camera* camera;
    s32 i;
    s32 objectIndex;
    Object* object;

    camera = &camera1[cameraId];
    for (i = 0; i < 10; i++) {
        objectIndex = indexObjectList1[i];

        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_OBJECT(&indexObjectList1[i]) | cameraId, G_EX_PUSH,
                                       G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

        if (D_8018E838[cameraId] == 0) {
            object = &gObjectList[objectIndex];
            if ((object->state >= 2) && (is_obj_index_flag_status_inactive(objectIndex, 0x00080000) != 0) &&
                (is_object_visible_on_camera(objectIndex, camera, 0x2AABU) != 0)) {
                object->orientation[1] = angle_between_object_camera(objectIndex, camera);
                draw_2d_texture_at(object->pos, object->orientation, object->sizeScaling, (u8*) object->activeTLUT,
                                   object->activeTexture, (Vtx*) 0x0d0060b0 /* common_vtx_hedgehog */, 64, 64, 64, 32);
            }
        }

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
}
#endif

#if 1
RECOMP_PATCH void render_object_train_penguins(s32 cameraId) {
    s32 i;
    s32 objectIndex;
    s32 temp_s1;
    s32 var_a3;
    u16 var_s1;
    u32 var_s3;

    if (gPlayerCountSelection1 == 1) {
        var_s3 = 0x0003D090;
    } else if (gPlayerCountSelection1 == 2) {
        var_s3 = 0x00027100;
    } else {
        var_s3 = 0x00015F90;
    }
    for (i = 0; i < NUM_PENGUINS; i++) {
        objectIndex = indexObjectList1[i];

        // @recomp Tag the transform.
        // gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_OBJECT(&indexObjectList1[i]) | cameraId, G_EX_PUSH,
        //                               G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
        // @recomp Tag the transform
        gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT(&indexObjectList1[i]) | cameraId, G_EX_PUSH,
                                 G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

        if (gObjectList[objectIndex].state >= 2) {
            if (gPlayerCountSelection1 == 1) {
                var_s1 = 0x4000;
                if (i == 0) {
                    var_a3 = 0x000005DC;
                } else if (func_80072320(objectIndex, 8) != 0) {
                    var_a3 = 0x00000320;
                } else {
                    var_a3 = 0x000003E8;
                }
            } else {
                if (func_80072320(objectIndex, 8) != 0) {
                    var_a3 = 0x000001F4;
                    var_s1 = 0x4000;
                } else {
                    var_a3 = 0x00000258;
                    var_s1 = 0x5555;
                }
            }
            temp_s1 = func_8008A364(objectIndex, cameraId, var_s1, var_a3);
            if (is_obj_flag_status_active(objectIndex, VISIBLE) != 0) {
                func_800557B4(objectIndex, (u32) temp_s1, var_s3);
            }
        }

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
}
#endif

#if 1
RECOMP_PATCH void render_object_moles(s32 cameraId) {
    s32 i;

    for (i = 0; i < NUM_GROUP1_MOLES; i++) {
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_OBJECT(&indexObjectList1[i]) | cameraId, G_EX_PUSH,
                                       G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
        func_80054D00(indexObjectList1[i], cameraId);
        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
    for (i = 0; i < NUM_GROUP2_MOLES; i++) {
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_OBJECT(&indexObjectList2[i]) | cameraId, G_EX_PUSH,
                                       G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
        func_80054D00(indexObjectList2[i], cameraId);
        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
    for (i = 0; i < NUM_GROUP3_MOLES; i++) {
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_OBJECT(&indexObjectList3[i]) | cameraId, G_EX_PUSH,
                                       G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
        func_80054D00(indexObjectList3[i], cameraId);
        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
    func_80054EB8(cameraId);
    func_80054F04(cameraId);
}

RECOMP_PATCH void func_80054EB8(s32 cameraId) {
    s32 i;

    for (i = 0; i < NUM_TOTAL_MOLES; i++) {
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_OBJECT(&gObjectParticle1[i]) | cameraId, G_EX_PUSH,
                                       G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
        func_80054E10(gObjectParticle1[i]);
        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
}

RECOMP_PATCH void func_80054F04(s32 cameraId) {
    s32 i;
    s32 objectIndex;
    Camera* sp44;
    Object* object;

    sp44 = &camera1[cameraId];
    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0D0079C8);
    load_texture_block_rgba16_mirror((u8*) 0x06013670 /* d_course_moo_moo_farm_mole_dirt */, 0x00000010, 0x00000010);
    for (i = 0; i < gObjectParticle2_SIZE; i++) {
        objectIndex = gObjectParticle2[i];
        object = &gObjectList[objectIndex];
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_OBJECT(object) | cameraId | (i & 0xFF) << 8, G_EX_PUSH,
                                       G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
        if (object->state > 0) {
            func_8008A364(objectIndex, cameraId, 0x2AABU, 0x000000C8);
            if ((is_obj_flag_status_active(objectIndex, VISIBLE) != 0) && (gMatrixHudCount <= MTX_HUD_POOL_SIZE_MAX)) {
                object->orientation[1] = func_800418AC(object->pos[0], object->pos[2], sp44->pos);
                rsp_set_matrix_gObjectList(objectIndex);
                gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0D006980);
            }
        }
        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}
#endif

#if 1
RECOMP_PATCH void render_object_seagulls(s32 arg0) {
    s32 i;
    s32 var_s1;

    for (i = 0; i < NUM_SEAGULLS; i++) {
        var_s1 = indexObjectList2[i];
        // @recomp Tag the transform.
        // gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_OBJECT(&indexObjectList1[i]) | arg0, G_EX_PUSH,
        //                                G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
        gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT(&indexObjectList1[i]) | arg0, G_EX_PUSH,
                                 G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

        if (func_8008A364(var_s1, arg0, 0x5555U, 0x000005DC) < 0x9C401) {
            D_80165908 = 1;
            func_800722A4(var_s1, 2);
        }
        if (is_obj_flag_status_active(var_s1, VISIBLE) != 0) {
            func_800552BC(var_s1);
        }
        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
}

#endif

#if 1
RECOMP_PATCH void render_object_crabs(s32 cameraId) {
    s32 i;
    s32 objId;

    for (i = 0; i < NUM_CRABS; i++) {
        objId = indexObjectList1[i];
        // @recomp Tag the transform.
        // gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_OBJECT(&indexObjectList1[i]) | cameraId, G_EX_PUSH,
        //                               G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
        gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_OBJECT(&indexObjectList1[i]) | cameraId, G_EX_PUSH,
                                 G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);
        func_8008A364(objId, cameraId, 0x2AABU, 800);
        if (1 /* is_obj_flag_status_active(objId, VISIBLE) != 0*/) {
            draw_crabs(objId, cameraId);
        }
        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
}
#endif

#if 1
RECOMP_PATCH void render_object_snowmans_list_1(s32 cameraId) {
    s32 i;
    s32 objectIndex;
    Camera* camera = &camera1[cameraId];

    for (i = 0; i < NUM_SNOWMEN; i++) {
        objectIndex = indexObjectList1[i];
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_OBJECT(&indexObjectList1[i]) | cameraId, G_EX_PUSH,
                                       G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
        if (gObjectList[objectIndex].state >= 2) {
            func_8008A364(objectIndex, cameraId, 0x2AABU, 600);
            if (is_obj_flag_status_active(objectIndex, VISIBLE) != 0) {
                D_80183E80[0] = (s16) gObjectList[objectIndex].orientation[0];
                D_80183E80[1] =
                    func_800418AC(gObjectList[objectIndex].pos[0], gObjectList[objectIndex].pos[2], camera->pos);
                D_80183E80[2] = (u16) gObjectList[objectIndex].orientation[2];
                if (is_obj_flag_status_active(objectIndex, 16) != 0) {
                    draw_2d_texture_at(gObjectList[objectIndex].pos, (u16*) D_80183E80,
                                       gObjectList[objectIndex].sizeScaling, (u8*) gObjectList[objectIndex].activeTLUT,
                                       gObjectList[objectIndex].activeTexture, gObjectList[objectIndex].vertex, 64, 64,
                                       64, 32);
                }
                objectIndex = indexObjectList2[i];
                D_80183E80[0] = (s16) gObjectList[objectIndex].orientation[0];
                D_80183E80[2] = (u16) gObjectList[objectIndex].orientation[2];
                draw_2d_texture_at(gObjectList[objectIndex].pos, (u16*) D_80183E80,
                                   gObjectList[objectIndex].sizeScaling, (u8*) gObjectList[objectIndex].activeTLUT,
                                   gObjectList[objectIndex].activeTexture, gObjectList[objectIndex].vertex, 64, 64, 64,
                                   32);
            }
        }
        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
}
#endif

#if 1
RECOMP_PATCH void render_object_snowmans_list_2(s32 cameraId) {
    Camera* sp44 = &camera1[cameraId];
    s32 i;
    s32 objectIndex;
    Object* object;

    load_texture_and_tlut((u8*) 0x06006d20 /* d_course_frappe_snowland_snow_tlut */,
                          (u8*) 0x06006f20 /* d_course_frappe_snowland_snow */, 32, 32);
    for (i = 0; i < gObjectParticle2_SIZE; i++) {
        objectIndex = gObjectParticle2[i];

        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_OBJECT(gObjectParticle2[i]) | cameraId, G_EX_PUSH,
                                       G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

        if (objectIndex != NULL_OBJECT_ID) {
            object = &gObjectList[objectIndex];
            if (object->state > 0) {
                func_8008A364(objectIndex, cameraId, 0x2AABU, 0x000001F4);
                if (is_obj_flag_status_active(objectIndex, VISIBLE) != 0) {
                    object->orientation[1] = func_800418AC(object->pos[0], object->pos[2], sp44->pos);
                    rsp_set_matrix_gObjectList(objectIndex);
                    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0D0069E0);
                }
            }
        }
        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}
#endif

#if 1
RECOMP_PATCH void func_80055228(s32 cameraId) {
    s32 i;
    s32 temp_s0;

    for (i = 0; i < 4; i++) {
        temp_s0 = indexObjectList1[i];
        func_8008A364(temp_s0, cameraId, 0x4000U, 0x000005DC);

        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_OBJECT(&indexObjectList1[i]) | cameraId, G_EX_PUSH,
                                       G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

        if (is_obj_flag_status_active(temp_s0, VISIBLE) != 0) {
            func_80055164(temp_s0);
        }

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
}
#endif

#if 1
RECOMP_PATCH void render_object_hedgehogs(s32 arg0) {
    s32 test;
    u32 var;
    s32 i;

    for (i = 0; i < NUM_HEDGEHOGS; i++) {
        test = indexObjectList2[i];
        var = func_8008A364(test, arg0, 0x4000U, 0x000003E8);

        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_OBJECT(&indexObjectList2[i]) | arg0, G_EX_PUSH,
                                       G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

        if (is_obj_flag_status_active(test, VISIBLE) != 0) {
            set_object_flag_status_true(test, 0x00200000);
            if (var < 0x2711U) {
                set_object_flag_status_true(test, 0x00000020);
            } else {
                set_object_flag_status_false(test, 0x00000020);
            }
            if (var < 0x57E41U) {
                set_object_flag_status_true(test, 0x00400000);
            }
            if (var < 0x52211U) {
                func_800555BC(test, arg0);
            }
        }
        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
}
#endif

#if 1
RECOMP_PATCH void render_object_boos(s32 arg0) {
    u32 temp_s2;
    s32 i;
    s32 objectIndex;

    for (i = 0; i < NUM_BOOS; i++) {
        objectIndex = indexObjectList3[i];

        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_OBJECT(&gObjectList[objectIndex]) | arg0, G_EX_PUSH,
                                       G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
        if (gObjectList[objectIndex].state >= 2) {
            temp_s2 = func_8008A364(objectIndex, arg0, 0x4000U, 0x00000320);
            if (is_obj_flag_status_active(objectIndex, VISIBLE) != 0) {
                func_800523B8(objectIndex, arg0, temp_s2);
            }
        }

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
}
#endif

#if 1 // Bats (Banshee Boardwalk) tb181
RECOMP_PATCH void render_object_bat(s32 cameraId) {
    s32 i;
    s32 objectIndex;
    Camera* temp_s7;

    objectIndex = indexObjectList1[0];
    temp_s7 = &camera1[cameraId];
    func_80046F60(gObjectList[objectIndex].activeTLUT, gObjectList[objectIndex].activeTexture, 0x00000020, 0x00000040,
                  5);

    D_80183E80[0] = gObjectList[objectIndex].orientation[0];
    D_80183E80[2] = gObjectList[objectIndex].orientation[2];

    if ((D_8018CFB0 != 0) || (D_8018CFC8 != 0)) {
        for (i = 0; i < 40; i++) {
            objectIndex = gObjectParticle2[i];
            if (objectIndex == -1)
                continue;

            if ((gObjectList[objectIndex].state >= 2) && (gMatrixHudCount < 0x2EF)) {
                D_80183E80[1] =
                    func_800418AC(gObjectList[objectIndex].pos[0], gObjectList[objectIndex].pos[2], temp_s7->pos);

                // Tagging the transform with unique object properties
                s32 transformTag = ((objectIndex & 0xFF) << 16) | ((i & 0xFF) << 8) | (cameraId & 0xFF);

                gEXMatrixGroupDecomposed(gDisplayListHead++, transformTag, G_EX_PUSH, G_MTX_MODELVIEW,
                                         G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                         G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                         G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

                func_800431B0(gObjectList[objectIndex].pos, D_80183E80, gObjectList[objectIndex].sizeScaling,
                              (Vtx*) 0x0D0062B0);

                // @recomp Pop the transform id.
                gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
            }
        }
    }

    if ((D_8018CFE8 != 0) || (D_8018D000 != 0)) {
        for (i = 0; i < 30; i++) {
            objectIndex = gObjectParticle3[i];
            if (objectIndex == -1)
                continue;

            if ((gObjectList[objectIndex].state >= 2) && (gMatrixHudCount < 0x2EF)) {
                D_80183E80[1] =
                    func_800418AC(gObjectList[objectIndex].pos[0], gObjectList[objectIndex].pos[2], temp_s7->pos);

                // Tagging the transform with unique object properties
                s32 transformTag = ((objectIndex & 0xFF) << 16) | ((i & 0xFF) << 8) | (cameraId & 0xFF);

                gEXMatrixGroupDecomposed(gDisplayListHead++, transformTag, G_EX_PUSH, G_MTX_MODELVIEW,
                                         G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                         G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                         G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

                func_800431B0(gObjectList[objectIndex].pos, D_80183E80, gObjectList[objectIndex].sizeScaling,
                              (Vtx*) 0x0D0062B0);

                // @recomp Pop the transform id.
                gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
            }
        }
    }

    gSPTexture(gDisplayListHead++, 0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF);
}
#endif

#if 1
RECOMP_PATCH void render_object_thwomps(s32 cameraId) {
    s32 objectIndex = 0;
    s32 i;
    UNUSED s32 stackPadding0;
    s16 minusone, plusone;
    Camera* camera;
    Object* object;

    camera = &camera1[cameraId];
    if (cameraId == PLAYER_ONE) {
        for (i = 0; i < gNumActiveThwomps; i++) {
            objectIndex = indexObjectList1[i];
            set_object_flag_status_false(objectIndex, 0x00070000);
            func_800722CC(objectIndex, 0x00000110);
        }
    }

    func_800534A4(objectIndex);
    for (i = 0; i < gNumActiveThwomps; i++) {
        objectIndex = indexObjectList1[i];
        minusone = gObjectList[objectIndex].unk_0DF - 1;
        plusone = gObjectList[objectIndex].unk_0DF + 1;

        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_OBJECT(&gObjectList[objectIndex]) | i, G_EX_PUSH,
                                       G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

        if (gGamestate != 9) {
            if ((D_8018CF68[cameraId] >= minusone) && (plusone >= D_8018CF68[cameraId]) &&
                (is_object_visible_on_camera(objectIndex, camera, 0x8000U) != 0)) {
                render_object_thwomps_model(objectIndex);
            }
        } else {
            render_object_thwomps_model(objectIndex);
        }

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0D0079C8);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gSPNumLights(gDisplayListHead++, 1);
    gSPLight(gDisplayListHead++, &D_800E4668.l[0], LIGHT_1);
    gSPLight(gDisplayListHead++, &D_800E4668.a, LIGHT_2);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BOTH);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_LIGHTING | G_SHADING_SMOOTH);
    load_texture_block_rgba16_mirror((u8*) 0x06006738 /* d_course_bowsers_castle_thwomp_side */, 0x00000020,
                                     0x00000020);
    for (i = 0; i < gObjectParticle3_SIZE; i++) {
        objectIndex = gObjectParticle3[i];
        if (objectIndex != NULL_OBJECT_ID) {
            object = &gObjectList[objectIndex];
            if ((object->state > 0) && (object->unk_0D5 == 3) && (gMatrixHudCount <= MTX_HUD_POOL_SIZE_MAX)) {
                // @recomp Tag the transform.
                gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_OBJECT(object) | i, G_EX_PUSH, G_MTX_MODELVIEW,
                                               G_EX_EDIT_ALLOW);

                rsp_set_matrix_transformation(object->pos, object->orientation, object->sizeScaling);
                gSPVertex(gDisplayListHead++, (Vtx*) 0x0D005C00, 3, 0);
                gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0D006930);

                // @recomp Pop the transform id.
                gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
            }
        }
    }
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gSPTexture(gDisplayListHead++, 0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF);
    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0D007AE0);
    load_texture_block_ia8_nomirror(D_8018D490, 0x00000020, 0x00000020);
    func_8004B3C8(0);
    D_80183E80[0] = 0;
    D_80183E80[2] = 0x8000;
    for (i = 0; i < gObjectParticle2_SIZE; i++) {
        objectIndex = gObjectParticle2[i];
        if (objectIndex != NULL_OBJECT_ID) {
            object = &gObjectList[objectIndex];
            if ((object->state >= 2) && (object->unk_0D5 == 2) && (gMatrixHudCount <= MTX_HUD_POOL_SIZE_MAX)) {
                // @recomp Tag the transform.
                gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_OBJECT(object) | i, G_EX_PUSH, G_MTX_MODELVIEW,
                                               G_EX_EDIT_ALLOW);

                func_8004B138(0x000000FF, 0x000000FF, 0x000000FF, (s32) object->primAlpha);
                D_80183E80[1] = func_800418AC(object->pos[0], object->pos[2], camera->pos);
                func_800431B0(object->pos, D_80183E80, object->sizeScaling, (Vtx*) 0x0D005AE0);

                // @recomp Pop the transform id.
                gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
            }
        }
    }
}
#endif

#if 1
RECOMP_PATCH void render_object_bowser_flame_particle(s32 objectIndex, s32 cameraId) {
    Camera* camera;
    Object* object;

    camera = &camera1[cameraId];
    if (gMatrixHudCount <= MTX_HUD_POOL_SIZE_MAX) {
        object = &gObjectList[objectIndex];

        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_OBJECT(object), G_EX_PUSH, G_MTX_MODELVIEW,
                                       G_EX_EDIT_ALLOW);

        if (object->unk_0D5 == 9) {
            func_8004B72C(0xFF, (s32) object->type, 0, (s32) object->unk_0A2, 0, 0, (s32) object->primAlpha);
        } else {
            func_8004B138(0xFF, (s32) object->type, 0, (s32) object->primAlpha);
        }
        D_80183E80[1] = func_800418AC(object->pos[0], object->pos[2], camera->pos);
        func_800431B0(object->pos, D_80183E80, object->sizeScaling, (Vtx*) 0x0D005AE0);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
}
#endif

#if 0
RECOMP_PATCH void render_cows(Camera* camera, Mat4 arg1, UNUSED struct Actor* actor) {
    u16 temp_s1;
    f32 temp_f0;
    struct ActorSpawnData* var_t1;
    struct ActorSpawnData* var_s1;
    struct ActorSpawnData* var_s5;
    Vec3f sp88;
    u32 soundThing = SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x4D);
    s32 segment = SEGMENT_NUMBER2(0x06014200 /* d_course_moo_moo_farm_cow_spawn */);
    s32 offset = SEGMENT_OFFSET(0x06014200 /* d_course_moo_moo_farm_cow_spawn */);

    var_t1 = (struct ActorSpawnData*) VIRTUAL_TO_PHYSICAL2(gSegmentTable[segment] + offset);
    D_8015F704 = 6.4e7f;
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    var_s5 = NULL;
    var_s1 = var_t1;
    while (var_s1->pos[0] != END_OF_SPAWN_DATA) {
        sp88[0] = var_s1->pos[0] * gCourseDirection;
        sp88[1] = var_s1->pos[1];
        sp88[2] = var_s1->pos[2];
        temp_f0 = is_within_render_distance(camera->pos, sp88, camera->rot[1], 0.0f, gCameraZoom[camera - camera1],
                                            4000000.0f);
        if (temp_f0 > 0.0f) {
            if (temp_f0 < D_8015F704) {
                D_8015F704 = temp_f0;
                var_s5 = var_s1;
            }
            arg1[3][0] = sp88[0];
            arg1[3][1] = sp88[1];
            arg1[3][2] = sp88[2];
            if ((gMatrixObjectCount < MTX_OBJECT_POOL_SIZE)) {
                // @recomp Tag the transform.
                gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_OBJECT((u32) actor << 16) | var_s1->someId,
                                               G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

                if (render_set_position(arg1, 0) != 0) {

                    switch (var_s1->someId) {
                        case 0:
                            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06013c00 /* d_course_moo_moo_farm_dl_cow1 */);
                            break;
                        case 1:
                            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06013ca0 /* d_course_moo_moo_farm_dl_cow2 */);
                            break;
                        case 2:
                            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06013d20 /* d_course_moo_moo_farm_dl_cow3 */);
                            break;
                        case 3:
                            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06013da0 /* d_course_moo_moo_farm_dl_cow4 */);
                            break;
                        case 4:
                            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06013e20 /* d_course_moo_moo_farm_dl_cow5 */);
                            break;
                    }
                }
                // @recomp Pop the transform id.
                gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
            } else {
                return;
            }
        }
        var_s1++;
    }

    if ((camera == camera1) && (var_s5 != NULL)) {
        if (D_8015F700 == 0) {
            temp_s1 = var_s5 - var_t1;
            if ((temp_s1 != D_8015F702) && (D_8015F704 < 160000.0f)) {
                func_800C99E0(D_8015F708, soundThing);
                D_8015F708[0] = var_s5->pos[0] * gCourseDirection;
                D_8015F708[1] = var_s5->pos[1];
                D_8015F708[2] = var_s5->pos[2];
                D_8015F702 = temp_s1;
                func_800C98B8(D_8015F708, D_802B91C8, soundThing);
                D_8015F700 = 0x00F0;
            }
        } else {
            D_8015F700 -= 1;
        }
    }
}
#endif

#if 1 // Nintendo Logo (Nintendo Rotating Blur Effect)
RECOMP_PATCH void func_800942D0(void) {
    Mtx* test;
    f32 var_f26;
    s32 var_s2;
    s32 thing;

    test = &gGfxPool->mtxObject[0];
    gSPMatrix(gDisplayListHead++, &gGfxPool->mtxScreen, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(gDisplayListHead++, &gGfxPool->mtxLookAt[0], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    // **Base Transformation (No Tagging for Static Logo)**
    guRotate(test, gIntroModelRotX, 1.0f, 0.0f, 0.0f);
    guRotate(test + 1, gIntroModelRotY, 0.0f, 1.0f, 0.0f);
    guScale(test + 2, 1.0f, 1.0f, gIntroModelScale);
    gSPMatrix(gDisplayListHead++, test++, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPMatrix(gDisplayListHead++, test++, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPMatrix(gDisplayListHead++, test++, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPSetEnvColor(gDisplayListHead++, 0x00, 0x00, 0x00, 0x00);
    gSPDisplayList(gDisplayListHead++, D_02007F60);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    if (sIntroModelMotionSpeed > 0) {
        var_f26 = sIntroModelSpeed;
        if (var_f26 > 10.0f) {
            var_f26 = 10.0f;
        }
        for (var_s2 = 0, thing = 0xC0; var_s2 < 0xC; var_s2++, thing -= 0x10) {

            // **Effect-Only Tagging for Motion (Keeps FPS Stable)**
            s32 motionTag = ((var_s2 & 0xF) << 8) | (((s32) sIntroModelMotionSpeed & 0xFF) << 16);
            gEXMatrixGroupDecomposed(gDisplayListHead++, motionTag, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO,
                                     G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                                     G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

            guRotate(test, 0.0f, 1.0f, 0.0f, 0.0f);
            guRotate(test + 1, (var_s2 + 1) * sIntroModelMotionSpeed * var_f26, 0.0f, 1.0f, 0.0f);
            guScale(test + 2, 1.0f, 1.0f, 2.0f);
            gSPMatrix(gDisplayListHead++, test++, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            gSPMatrix(gDisplayListHead++, test++, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            gSPMatrix(gDisplayListHead++, test++, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);
            gDPSetEnvColor(gDisplayListHead++, 0x00, 0x00, 0x00, thing);
            gSPDisplayList(gDisplayListHead++, 0x0000000006009410);

            // **Pop Effect Transform**
            gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
            gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
        }
    }
}
#endif

#if 1
RECOMP_PATCH void render_lakitu(s32 cameraId) {
    Camera* camera;
    f32 var_f0;
    f32 var_f2;
    s32 objectIndex;
    Object* object;

    objectIndex = gIndexLakituList[cameraId];
    camera = &camera1[cameraId];
    if (is_obj_flag_status_active(objectIndex, 0x00000010) != 0) {
        object = &gObjectList[objectIndex];
        object->orientation[0] = 0;
        object->orientation[1] = func_800418AC(object->pos[0], object->pos[2], camera->pos);
        object->orientation[2] = 0x8000;
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_OBJECT(&gIndexLakituList[cameraId]) | cameraId,
                                       G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

        if (func_80072354(objectIndex, 2) != 0) {
            draw_2d_texture_at(object->pos, object->orientation, object->sizeScaling, (u8*) object->activeTLUT,
                               object->activeTexture, object->vertex, (s32) object->textureWidth,
                               (s32) object->textureHeight, (s32) object->textureWidth,
                               (s32) object->textureHeight / 2);
        } else {
            func_800485C4(object->pos, object->orientation, object->sizeScaling, (s32) object->primAlpha,
                          (u8*) object->activeTLUT, object->activeTexture, object->vertex, (s32) object->textureWidth,
                          (s32) object->textureHeight, (s32) object->textureWidth, (s32) object->textureHeight / 2);
        }
        if (gScreenModeSelection == SCREEN_MODE_1P) {
            var_f0 = object->pos[0] - D_8018CF14->pos[0];
            var_f2 = object->pos[2] - D_8018CF14->pos[2];
            if (var_f0 < 0.0f) {
                var_f0 = -var_f0;
            }
            if (var_f2 < 0.0f) {
                var_f2 = -var_f2;
            }
            if ((var_f0 + var_f2) <= 200.0) {
                func_8004A630(&D_8018C0B0[cameraId], object->pos, 0.35f);
            }
        }
        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
}
#endif

#define ABS(x) ((x) >= 0 ? (x) : -(x))
#define ABSF(x) ((x) >= 0.0f ? (x) : -(x))

// Snowflakes in the background
#if 1
struct ObjectInterpData {
    s32 objectIndex;
    s16 x, y;
};
struct ObjectInterpData prevObject[OBJECT_LIST_SIZE] = { 0 };

RECOMP_PATCH void func_800518F8(s32 objectIndex, s16 x, s16 y) {
    // Search all recorded objects for the one we're drawing
    for (size_t i = 140; i < 190; i++) {
        if (objectIndex == prevObject[i].objectIndex) {
            // recomp_printf("Object coincidence: %d\n", objectIndex);
            //  Coincidence!
            //  Skip drawing the object this frame if it warped to the other side of the screen
            if ((ABS(x - prevObject[i].x) > SCREEN_WIDTH / 2) || (ABS(y - prevObject[i].y) > SCREEN_HEIGHT / 2)) {
                prevObject[objectIndex].x = x;
                prevObject[objectIndex].y = y;
                prevObject[objectIndex].objectIndex = objectIndex;
                return;
            }
        }
    }

    if (gObjectList[objectIndex].status & 0x10) {
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_OBJECT(&gObjectList[objectIndex]), G_EX_PUSH,
                                       G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

        if (D_8018D228 != gObjectList[objectIndex].unk_0D5) {
            D_8018D228 = gObjectList[objectIndex].unk_0D5;
            func_80044DA0(gObjectList[objectIndex].activeTexture, gObjectList[objectIndex].textureWidth,
                          gObjectList[objectIndex].textureHeight);
        }
        func_80042330(x, y, 0, gObjectList[objectIndex].sizeScaling);
        gSPVertex(gDisplayListHead++, gObjectList[objectIndex].vertex, 4, 0);
        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0d006940 /* common_rectangle_display */);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }

    // Save current cloud index and x position
    prevObject[objectIndex].x = x;
    prevObject[objectIndex].y = y;
    prevObject[objectIndex].objectIndex = objectIndex;
}
#endif

#if 1
struct ObjectInterpData2 {
    s32 objectIndex;
    f32 x, y;
};
struct ObjectInterpData2 prevObject2[OBJECT_LIST_SIZE] = { 0 };

s32 mtxf_set_matrix_gObjectList_snowflakes(s32 objectIndex, Mat4 transformMatrix) {
    f32 sinX;
    Object* object = &gObjectList[objectIndex];
    f32 sinY;
    f32 cosY;
    f32 sinZ;
    f32 cosZ;
    f32 cosX;

    sinX = sins(object->orientation[0]);
    cosX = coss(object->orientation[0]);
    sinY = sins(object->orientation[1]);
    cosY = coss(object->orientation[1]);
    sinZ = sins(object->orientation[2]);
    cosZ = coss(object->orientation[2]);

    transformMatrix[0][0] = object->sizeScaling * ((cosY * cosZ) + (sinX * sinY * sinZ));
    transformMatrix[1][0] = object->sizeScaling * ((-cosY * sinZ) + sinX * sinY * cosZ);
    transformMatrix[2][0] = object->sizeScaling * (cosX * sinY);
    transformMatrix[3][0] = object->pos[0];
    transformMatrix[0][1] = object->sizeScaling * (cosX * sinZ);
    transformMatrix[1][1] = object->sizeScaling * (cosX * cosZ);
    transformMatrix[2][1] = object->sizeScaling * -sinX;
    transformMatrix[3][1] = object->pos[1];
    transformMatrix[0][2] = object->sizeScaling * ((-sinY * cosZ) + (sinX * cosY * sinZ));
    transformMatrix[1][2] = object->sizeScaling * ((sinY * sinZ) + (sinX * cosY * cosZ));
    transformMatrix[2][2] = object->sizeScaling * (cosX * cosY);
    transformMatrix[3][2] = object->pos[2];
    transformMatrix[0][3] = 0.0f;
    transformMatrix[1][3] = 0.0f;
    transformMatrix[2][3] = 0.0f;
    transformMatrix[3][3] = 1.0f;

    // Search all recorded objects for the one we're drawing
    for (int i = 240; i < 290; i++) {
        if (objectIndex == prevObject2[i].objectIndex) {
            // recomp_printf("Object coincidence: %d\n", objectIndex);
            // Coincidence!
            // Skip drawing the object this frame if it warped to the other side of the screen
            if ((ABSF(object->pos[0] - prevObject2[i].x) > 10) || (ABSF(object->pos[1] - prevObject2[i].y) > 10)) {
                prevObject2[objectIndex].x = object->pos[0];
                prevObject2[objectIndex].y = object->pos[1];
                prevObject2[objectIndex].objectIndex = objectIndex;
                // recomp_printf("IDX: %d X: %f Y: %f Z: ", objectIndex, object->pos[0], object->pos[1],
                // object->pos[2]); recomp_printf("%f DX: %f DY %f \n", ABSF(object->pos[0] - prevObject2[i].x),
                //               ABSF(object->pos[1] - prevObject2[i].y));
                return 1;
            }
        }
    }
    prevObject2[objectIndex].x = object->pos[0];
    prevObject2[objectIndex].y = object->pos[1];
    prevObject2[objectIndex].objectIndex = objectIndex;

    return 0;
}
#endif

#if 1
void rsp_set_matrix_gObjectList_snowflakes(s32 transformIndex) {
    Mat4 matrix;

    if (mtxf_set_matrix_gObjectList_snowflakes(transformIndex, matrix))
        return;
    ;
    convert_to_fixed_point_matrix(&gGfxPool->mtxHud[gMatrixHudCount], matrix);

    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxHud[gMatrixHudCount++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}
#endif

// Snowflakes
#if 1
RECOMP_PATCH void render_object_snowflakes_particles(void) {
    s32 i;
    s32 snowflakeIndex;

    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0D007AE0);
    gDPSetCombineLERP(gDisplayListHead++, 1, 0, SHADE, 0, 0, 0, 0, TEXEL0, 1, 0, SHADE, 0, 0, 0, 0, TEXEL0);
    func_80044F34((u8*) 0x0D0293D8, 0x10, 0x10);
    for (i = 0; i < NUM_SNOWFLAKES; i++) {
        snowflakeIndex = gObjectParticle1[i];
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_OBJECT(&gObjectParticle1[i]), G_EX_PUSH, G_MTX_MODELVIEW,
                                       G_EX_EDIT_ALLOW);
        if (gObjectList[snowflakeIndex].state >= 2) {
            rsp_set_matrix_gObjectList_snowflakes(snowflakeIndex);
            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0D006980);
        }
        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}
#endif

#endif // GLOBAL