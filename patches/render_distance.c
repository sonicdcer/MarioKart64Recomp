#include "patches.h"

#if 1 // Draw Distance Improved@tb181
RECOMP_PATCH f32 is_within_render_distance(Vec3f cameraPos, Vec3f objectPos, u16 orientationY, f32 minDistance, f32 fov,
                                           f32 maxDistance) {
    u16 angleObject;
    u16 temp_v0;
    f32 distanceX;
    f32 distance;
    f32 distanceY;
    f32 scaleFov;
    s32 plus_fov_angle;
    s32 minus_fov_angle;
    u16 temp;
    s32 count = 0;

    maxDistance *= 6.5f;
    scaleFov = 1.25;

    f32 extended_fov = ((f32) fov * 300 * scaleFov); // Sets the Culling for objects on the left and right

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
            if (0) {                    // if (gCurrentCourseId == COURSE_KALAMARI_DESERT) {
                return distance / 6.5f; // set for better DD settings in Desert
            } else {
                return distance / 10.0f; // Items
            }
        }
        return -1.0f;
    }

    if (is_visible_between_angle((u16) plus_fov_angle, (u16) minus_fov_angle, angleObject) == 1) {
        if (0) { // if (gCurrentCourseId == COURSE_KALAMARI_DESERT) {
            return distance / 2.0f;
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

#if 1 // Increase DD for effects, snowmen, ect... @tb181
RECOMP_PATCH u32 get_horizontal_distance_to_camera(s32 objectIndex, Camera* camera) {
    s32 x;
    s32 y;

    x = gObjectList[objectIndex].pos[0] - camera->pos[0];
    y = gObjectList[objectIndex].pos[2] - camera->pos[2];
    return (x * x) + (y * y) / 5; // Increase DD
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

    // Multiplier to scale the draw distance
    const f32 distanceMultiplier = 3.0f;

    // Apply the multiplier to arg2 and arg3
    arg2 *= distanceMultiplier;
    arg3 *= distanceMultiplier / 5;

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

// Fix Culling for gObjectList items
#if 1
RECOMP_PATCH bool is_object_visible_on_camera(s32 objectIndex, Camera* camera, u16 angle) {
    u16 temp_t2;
    s32 var_t0;

    var_t0 = false;

    // Update angle dynamically if needed
    angle = 0x6000; // New angle for wider FOV, equivalent to ~120°

    temp_t2 = (get_angle_between_xy(camera->pos[0], gObjectList[objectIndex].pos[0], camera->pos[2],
                                    gObjectList[objectIndex].pos[2]) +
               ((s32) angle / 2)) -
              camera->rot[1];
    if ((temp_t2 >= 0) && (angle >= temp_t2)) {
        var_t0 = true;
    }
    return var_t0;
}
#endif

RECOMP_PATCH void render_moo_moo_farm(struct UnkStruct_800DC5EC* arg0) {
    UNUSED s32 pad[13];
    s16 temp_s0 = arg0->pathCounter;
    s16 playerDirection = arg0->playerDirection;

    set_course_lighting(D_800DC610, D_802B87D4, 0, 1);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEI, G_CC_MODULATEI);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    // d_course_moo_moo_farm_packed_dl_4DF8
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07004DF8));
    // d_course_moo_moo_farm_packed_dl_5640
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07005640));
    gSPFogPosition(gDisplayListHead++, D_802B87B0, D_802B87B4);

    render_course_segments((uintptr_t) moo_moo_farm_dls, arg0);

    if ((temp_s0 < 14) && (temp_s0 > 10)) {
        if ((playerDirection == 2) || (playerDirection == 3) || (playerDirection == 1))
            //
            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x6013ff8);

    } else if (temp_s0 < 16) {
        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x6013ff8);
    } else if (temp_s0 < 19) {
        if (playerDirection != 2)
            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x6013ff8);

    } else if (temp_s0 < 20) {
        if (playerDirection == 0)
            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x6013ff8);
    }
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEI, G_CC_MODULATEI);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);

// @recomp Always render the barn regardless of section or camera direction angle
#if 0
    if ((temp_s0 >= 16) && (temp_s0 < 24)) {
        if ((playerDirection == 2) || (playerDirection == 3))
            // d_course_moo_moo_farm_packed_dl_5410
            gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07005410));

    } else if (temp_s0 < 9) {
        if (playerDirection == 2)
            // d_course_moo_moo_farm_packed_dl_5410
            gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07005410));
    }
#else
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07005410));
#endif

    if (temp_s0 < 4) {
        if (playerDirection != 0)
            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x6014060);

    } else if (temp_s0 < 8) {
        if (playerDirection == 2)
            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x6014060);

    } else if (temp_s0 >= 22) {
        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x6014060);
    } else if (temp_s0 >= 18) {
        if ((playerDirection == 0) || (playerDirection == 3))
            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x6014060);
    }
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    // d_course_moo_moo_farm_packed_dl_10C0
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x070010C0));
}

// High/Low poly Pipe in Mario Raceway
RECOMP_PATCH void func_802911C4(void) {
// @recomp: Always render high poly pipe regardless of mode
#if 1
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x070008E8));
#else
    if (gScreenModeSelection == SCREEN_MODE_1P) {
        // d_course_mario_raceway_packed_dl_8E8
        gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x070008E8));
    } else {
        // d_course_mario_raceway_packed_dl_2D68
        gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07002D68));
    }
#endif
}

RECOMP_PATCH void render_actor_box_truck(Camera* arg0, struct Actor* arg1) {
    UNUSED s32 pad[6];
    Mat4 spD8;
    UNUSED s32 pad2[32];

    f32 temp_f0 =
        is_within_render_distance(arg0->pos, arg1->pos, arg0->rot[1], 2500.0f, gCameraZoom[arg0 - camera1], 9000000.0f);
    if (temp_f0 < 0.0f) {
        // return; // @recomp: Disable culling
    }

    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    mtxf_pos_rotation_xyz(spD8, arg1->pos, arg1->rot);
    if (render_set_position(spD8, 0) != 0) {

        switch (arg1->state) {
            case 0:
                gSPDisplayList(gDisplayListHead++, (Gfx*) 0x6023858 /* d_course_toads_turnpike_dl_23858 */);
                break;
            case 1:
                gSPDisplayList(gDisplayListHead++, (Gfx*) 0x60238A0 /* d_course_toads_turnpike_dl_238A0 */);
                break;
            case 2:
                gSPDisplayList(gDisplayListHead++, (Gfx*) 0x60238E8 /* d_course_toads_turnpike_dl_238E8*/);
                break;
        }
// @recomp always draw a high poly version regardless of mode
#if 1
        gSPDisplayList(gDisplayListHead++, toads_turnpike_dl_0);
#else
        if (gActiveScreenMode == SCREEN_MODE_1P) {
            if (temp_f0 < 160000.0f) {
                gSPDisplayList(gDisplayListHead++, toads_turnpike_dl_0);
            } else if (temp_f0 < 640000.0f) {
                gSPDisplayList(gDisplayListHead++, toads_turnpike_dl_1);
            } else {
                gSPDisplayList(gDisplayListHead++, toads_turnpike_dl_2);
            }
        } else if (temp_f0 < 160000.0f) {
            gSPDisplayList(gDisplayListHead++, toads_turnpike_dl_1);
        } else {
            gSPDisplayList(gDisplayListHead++, toads_turnpike_dl_2);
        }
#endif
    }
}

RECOMP_PATCH void render_actor_car(Camera* arg0, struct Actor* arg1) {
    UNUSED s32 pad[6];
    Mat4 spC8;
    UNUSED s32 pad2[32];
    f32 temp_f0 =
        is_within_render_distance(arg0->pos, arg1->pos, arg0->rot[1], 2500.0f, gCameraZoom[arg0 - camera1], 9000000.0f);

    if (temp_f0 < 0.0f) {
        // return; // @recomp: Disable culling
    }

    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    mtxf_pos_rotation_xyz(spC8, arg1->pos, arg1->rot);
    mtxf_scale(spC8, 0.1f);
    if (render_set_position(spC8, 0) != 0) {
// @recomp always draw a high poly version regardless of mode
#if 1
        gSPDisplayList(gDisplayListHead++, toads_turnpike_dl_9);
#else
        if (gActiveScreenMode == SCREEN_MODE_1P) {
            if (temp_f0 < 160000.0f) {
                gSPDisplayList(gDisplayListHead++, toads_turnpike_dl_9);
            } else if (temp_f0 < 640000.0f) {
                gSPDisplayList(gDisplayListHead++, toads_turnpike_dl_10);
            } else {
                gSPDisplayList(gDisplayListHead++, toads_turnpike_dl_11);
            }
        } else if (temp_f0 < 160000.0f) {
            gSPDisplayList(gDisplayListHead++, toads_turnpike_dl_10);
        } else {
            gSPDisplayList(gDisplayListHead++, toads_turnpike_dl_11);
        }
#endif
    }
}

RECOMP_PATCH void render_actor_piranha_plant(Camera* arg0, Mat4 arg1, struct PiranhaPlant* arg2) {
    UNUSED s32 pad;
    u8* addr;
    s16 temp_lo = arg0 - camera1;
    s16 animationFrame; // unconfirmed
    s16 temp = arg2->flags;
    f32 temp_f0;
    s32 maxObjectsReached;

    if (temp & 0x800) {
        return;
    }

    temp_f0 = is_within_render_distance(arg0->pos, arg2->pos, arg0->rot[1], 0, gCameraZoom[arg0 - camera1], 1000000.0f);

    if (temp_f0 < 0.0f) {

        switch (temp_lo) {
            case 0:
                arg2->visibilityStates[0] = -1;
                break;
            case 1:
                arg2->visibilityStates[1] = -1;
                break;
            case 2:
                arg2->visibilityStates[2] = -1;
                break;
            case 3:
                arg2->visibilityStates[3] = -1;
                break;
        }
        return;
    }
    arg1[3][0] = arg2->pos[0];
    arg1[3][1] = arg2->pos[1];
    arg1[3][2] = arg2->pos[2];
    maxObjectsReached = render_set_position(arg1, 0) == 0;
    if (maxObjectsReached) {
        return;
    }

    // @recomp: Disable LOD
    temp_f0 = 0.0f;

    if (90000.0f < temp_f0) {

        switch (temp_lo) {
            case 0:
                arg2->visibilityStates[0] = 0;
                break;
            case 1:
                arg2->visibilityStates[1] = 0;
                break;
            case 2:
                arg2->visibilityStates[2] = 0;
                break;
            case 3:
                arg2->visibilityStates[3] = 0;
                break;
        }
        animationFrame = 0;

    } else {
        switch (temp_lo) {
            case 0:
                arg2->visibilityStates[0] = 1;
                break;
            case 1:
                arg2->visibilityStates[1] = 1;
                break;
            case 2:
                arg2->visibilityStates[2] = 1;
                break;
            case 3:
                arg2->visibilityStates[3] = 1;
                break;
        }

        switch (temp_lo) {
            case 0:
                animationFrame = arg2->timers[0];
                break;
            case 1:
                animationFrame = arg2->timers[1];
                break;
            case 2:
                animationFrame = arg2->timers[2];
                break;
            case 3:
                animationFrame = arg2->timers[3];
                break;
        }
    }
    animationFrame /= 6;

    if (animationFrame > 8) {
        animationFrame = 8;
    }
    addr = D_802BA058 + (animationFrame << 0xB);
    gDPLoadTextureBlock(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(addr), G_IM_FMT_CI, G_IM_SIZ_8b, 32, 64, 0,
                        G_TX_MIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);

    if (gCurrentCourseId == COURSE_MARIO_RACEWAY) {
        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x6006990 /*d_course_mario_raceway_dl_piranha_plant*/);
    } else {
        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0600d850 /* d_course_royal_raceway_dl_piranha_plant*/);
    }
}

RECOMP_PATCH void render_actor_tanker_truck(Camera* camera, struct Actor* arg1) {
    UNUSED s32 pad[6];
    Mat4 spC8;
    UNUSED s32 pad2[32];
    f32 temp_f0 = is_within_render_distance(camera->pos, arg1->pos, camera->rot[1], 2500.0f,
                                            gCameraZoom[camera - camera1], 9000000.0f);

    if (temp_f0 < 0.0f) {
        // return; @recomp: Disable culling
    }

    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    mtxf_pos_rotation_xyz(spC8, arg1->pos, arg1->rot);
    if (render_set_position(spC8, 0) != 0) {
// @recomp always draw a high poly version regardless of mode
#if 1
        gSPDisplayList(gDisplayListHead++, toads_turnpike_dl_6);

#else
        if (gActiveScreenMode == SCREEN_MODE_1P) {
            if (temp_f0 < 160000.0f) {

                gSPDisplayList(gDisplayListHead++, toads_turnpike_dl_6);
            } else if (temp_f0 < 640000.0f) {
                gSPDisplayList(gDisplayListHead++, toads_turnpike_dl_7);
            } else {
                gSPDisplayList(gDisplayListHead++, toads_turnpike_dl_8);
            }
        } else if (temp_f0 < 160000.0f) {
            gSPDisplayList(gDisplayListHead++, toads_turnpike_dl_7);
        } else {
            gSPDisplayList(gDisplayListHead++, toads_turnpike_dl_8);
        }
#endif
    }
}

RECOMP_PATCH void render_actor_school_bus(Camera* arg0, struct Actor* arg1) {
    UNUSED s32 pad[6];
    Mat4 spC8;
    UNUSED s32 pad2[32];
    f32 temp_f0;

    temp_f0 =
        is_within_render_distance(arg0->pos, arg1->pos, arg0->rot[1], 2500.0f, gCameraZoom[arg0 - camera1], 9000000.0f);
    if (temp_f0 < 0.0f) {
        // return; //@recomp: Disable culling
    }

    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    mtxf_pos_rotation_xyz(spC8, arg1->pos, arg1->rot);
    if (render_set_position(spC8, 0) != 0) {
// @recomp always draw a high poly version regardless of mode
#if 1
        gSPDisplayList(gDisplayListHead++, toads_turnpike_dl_3);
#else
        if (gActiveScreenMode == SCREEN_MODE_1P) {
            if (temp_f0 < 160000.0f) {
                gSPDisplayList(gDisplayListHead++, toads_turnpike_dl_3);
            } else if (temp_f0 < 640000.0f) {
                gSPDisplayList(gDisplayListHead++, toads_turnpike_dl_4);
            } else {
                gSPDisplayList(gDisplayListHead++, toads_turnpike_dl_5);
            }
        } else if (temp_f0 < 160000.0f) {
            gSPDisplayList(gDisplayListHead++, toads_turnpike_dl_4);
        } else {
            gSPDisplayList(gDisplayListHead++, toads_turnpike_dl_5);
        }
#endif
    }
}

RECOMP_PATCH void render_actor_yoshi_egg(Camera* arg0, Mat4 arg1, struct YoshiValleyEgg* egg, u16 arg3) {
    Mat4 sp60;
    Vec3s sp5C;
    Vec3f sp54;
    f32 temp_f0;

    if (gGamestate != CREDITS_SEQUENCE) {
        temp_f0 = is_within_render_distance(arg0->pos, egg->pos, arg0->rot[1], 200.0f, gCameraZoom[arg0 - camera1],
                                            16000000.0f);
        if (temp_f0 < 0.0f) {
            // return; // @recomp: Disable culling
        }
    } else {
        arg3 = 15;
        temp_f0 = 0.0f;
    }

    // @recomp always draw a high poly version regardless of mode
    arg3 = 15;
    temp_f0 = 0.0f;

    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    if ((arg3 > 12) && (arg3 < 20)) {
        if (temp_f0 < 640000.0f) {
            sp54[0] = egg->pos[0];
            sp54[1] = 3.0f;
            sp54[2] = egg->pos[2];
            func_802976D8(sp5C);
            func_8029794C(sp54, sp5C, 10.0f);
        }
        sp5C[0] = 0;
        sp5C[1] = egg->eggRot;
        sp5C[2] = 0;
        mtxf_pos_rotation_xyz(sp60, egg->pos, sp5C);
        if (render_set_position(sp60, 0) == 0) {
            return;
        }

        gSPSetGeometryMode(gDisplayListHead++, G_LIGHTING);
        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x6016d70 /* d_course_yoshi_valley_dl_16D70*/ );
    } else {
        arg1[3][0] = egg->pos[0];
        arg1[3][1] = egg->pos[1];
        arg1[3][2] = egg->pos[2];

        if (render_set_position(arg1, 0) != 0) {
            gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x6017fa8 /* d_course_yoshi_valley_dl_egg_lod0 */);
        }
    }
}