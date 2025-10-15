#include "patches.h"

f32 func_802AAB4C(Player*);

#if 1 // GLOBAL

// Draws the map
#if 0
extern Gfx* sherbet_land_dls_2[];

RECOMP_PATCH void func_8029122C(struct UnkStruct_800DC5EC* arg0, s32 playerId) {
    UNUSED s32 pad;
    Player* player = arg0->player;
    Mat4 matrix;
    Vec3f vector;
    u16 pathCounter;
    u16 cameraRot;
    s16 playerDirection;
return;
    init_rdp();
    pathCounter = (u16) arg0->pathCounter;
    cameraRot = (u16) arg0->camera->rot[1];
    playerDirection = arg0->playerDirection;

    gEXMatrixGroupSimple(gDisplayListHead++, 0xFFFFAAAA, G_EX_NOPUSH, G_MTX_PROJECTION,
            G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_NONE);

    switch (playerId) {
        case PLAYER_ONE:
            gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxPersp[PLAYER_ONE]),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
            gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[PLAYER_ONE]),
                      G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
            break;
        case PLAYER_TWO:
            gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxPersp[PLAYER_TWO]),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
            gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[PLAYER_TWO]),
                      G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
            break;
        case PLAYER_THREE:
            gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxPersp[PLAYER_THREE]),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
            gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[PLAYER_THREE]),
                      G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
            break;
        case PLAYER_FOUR:
            gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxPersp[PLAYER_FOUR]),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
            gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[PLAYER_FOUR]),
                      G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
            break;
    }
    mtxf_identity(matrix);
    render_set_position(matrix, 0);
    switch (gCurrentCourseId) {
        case COURSE_BOWSER_CASTLE:
            if (gActiveScreenMode != SCREEN_MODE_1P) {
                return;
            }
            if (pathCounter < 6) {
                return;
            }
            if (pathCounter > 9) {
                return;
            }
            if (pathCounter == 9) {
                if (cameraRot < 0xA000) {
                    return;
                }
                if (cameraRot > 0xE000) {
                    return;
                }
            }
            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x06009228 /* d_course_bowsers_castle_dl_9228*/);
            break;
        case COURSE_BANSHEE_BOARDWALK:
            gDPPipeSync(gDisplayListHead++);
            gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
            gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
            gDPSetBlendMask(gDisplayListHead++, 0xFF);
            gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
            // d_course_banshee_boardwalk_packed_dl_878
            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07000878);
            gDPSetAlphaCompare(gDisplayListHead++, G_AC_NONE);
            gDPPipeSync(gDisplayListHead++);
            break;
        case COURSE_KOOPA_BEACH:

            gDPPipeSync(gDisplayListHead++);
            gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
            gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
            gDPSetBlendMask(gDisplayListHead++, 0xFF);
            gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);

            switch (pathCounter) {
                case 22:
                case 23:
                case 29:
                case 30:
                case 31:
                case 37:
                    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
                    // d_course_koopa_troopa_beach_packed_dl_9E70
                    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07009E70);
                    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
                    break;
            }
            vector[0] = 0.0f;
            vector[1] = D_8015F8E4;
            vector[2] = 0.0f;
            mtxf_translate(matrix, vector);
            render_set_position(matrix, 0);

            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
            gDPSetBlendMask(gDisplayListHead++, 0xFF);
            gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
            gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
            render_course_segments((u32) 0x06019578 /* d_course_koopa_troopa_beach_dl_list2*/, arg0);
            gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 1, 1, G_OFF);
            gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
            gDPSetAlphaCompare(gDisplayListHead++, G_AC_NONE);
            gDPPipeSync(gDisplayListHead++);
            break;
        case COURSE_SHERBET_LAND:

            gDPPipeSync(gDisplayListHead++);
            gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
            gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
            gDPSetBlendMask(gDisplayListHead++, 0xFF);
            gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
            gDPSetTextureFilter(gDisplayListHead++, G_TF_BILERP);
            gDPSetTexturePersp(gDisplayListHead++, G_TP_PERSP);

            mtxf_identity(matrix);
            render_set_position(matrix, 0);
            render_course_segments((uintptr_t) sherbet_land_dls_2, arg0);

            gDPSetAlphaCompare(gDisplayListHead++, G_AC_NONE);
            if ((func_80290C20(arg0->camera) == 1) && (func_802AAB4C(player) < player->pos[1])) {
                gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER);
                gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
                gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
                // d_course_sherbet_land_packed_dl_2B48
                gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07002B48);
            }
            gDPPipeSync(gDisplayListHead++);
            break;
        case COURSE_RAINBOW_ROAD:
            gDPPipeSync(gDisplayListHead++);
            mtxf_identity(matrix);
            render_set_position(matrix, 0);
            gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
            render_course_segments((uintptr_t) 0x060164b8, arg0);
            gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
            gDPSetAlphaCompare(gDisplayListHead++, G_AC_NONE);
            gDPPipeSync(gDisplayListHead++);
            break;
        case COURSE_WARIO_STADIUM:
            gDPPipeSync(gDisplayListHead++);
            gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
            gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
            gDPSetBlendMask(gDisplayListHead++, 0xFF);
            gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
            gDPSetTextureFilter(gDisplayListHead++, G_TF_BILERP);
            gDPSetTexturePersp(gDisplayListHead++, G_TP_PERSP);

            mtxf_identity(matrix);
            render_set_position(matrix, 0);

            gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
            gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);
            gDPSetPrimColor(gDisplayListHead++, 0, 0, 0xFF, 0xFF, 0x00, 0xFF);
            // d_course_wario_stadium_packed_dl_EC0
            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07000EC0);
            gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 1, 1, G_OFF);
            gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
            gDPSetAlphaCompare(gDisplayListHead++, G_AC_NONE);
            gDPPipeSync(gDisplayListHead++);
            break;
        case COURSE_DK_JUNGLE:
            gDPPipeSync(gDisplayListHead++);
            gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
            gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
            gDPSetBlendMask(gDisplayListHead++, 0xFF);
            gDPSetTextureFilter(gDisplayListHead++, G_TF_BILERP);
            gDPSetTexturePersp(gDisplayListHead++, G_TP_PERSP);

            mtxf_identity(matrix);
            render_set_position(matrix, 0);

            gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);

            if (pathCounter < 17) {
                gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
                // d_course_dks_jungle_parkway_packed_dl_3E40
                gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07003E40);
                // d_course_dks_jungle_parkway_packed_dl_3EB0
                gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07003EB0);
                if ((pathCounter >= 6) && (pathCounter < 13)) {
                    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
                    // d_course_dks_jungle_parkway_packed_dl_3DD0
                    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07003DD0);
                }
            } else if ((pathCounter == 21) || (pathCounter == 22)) {
                if (playerDirection == 3) {
                    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
                    // d_course_dks_jungle_parkway_packed_dl_36A8
                    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x070036A8);
                }
                if ((playerDirection == 1) || (playerDirection == 0)) {
                    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
                    // d_course_dks_jungle_parkway_packed_dl_36A8
                    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x070036A8);
                } else {
                    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
                    // d_course_dks_jungle_parkway_packed_dl_3F30
                    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07003F30);
                    // d_course_dks_jungle_parkway_packed_dl_36A8
                    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x070036A8);
                }
            } else if (pathCounter == 24) {
                if ((playerDirection == 0) || (playerDirection == 3)) {
                    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
                    // d_course_dks_jungle_parkway_packed_dl_36A8
                    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x070036A8);
                }
            } else if (pathCounter == 23) {
                if (playerDirection == 3) {
                    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
                    // d_course_dks_jungle_parkway_packed_dl_36A8
                    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x070036A8);
                } else if (playerDirection == 0) {
                    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
                    // d_course_dks_jungle_parkway_packed_dl_36A8
                    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x070036A8);
                }
            }
            gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
            switch (pathCounter) {
                case 5:
                    if (playerDirection != 3) {
                        // d_course_dks_jungle_parkway_packed_dl_3DD0
                        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07003DD0);
                    }
                    break;
                case 17:
                    switch (playerDirection) {
                        case 0:
                            // d_course_dks_jungle_parkway_packed_dl_3E40
                            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07003E40);
                            // d_course_dks_jungle_parkway_packed_dl_3EB0
                            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07003EB0);
                            break;
                        case 1:
                            // d_course_dks_jungle_parkway_packed_dl_3DD0
                            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07003DD0);
                            // d_course_dks_jungle_parkway_packed_dl_3E40
                            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07003E40);
                            // d_course_dks_jungle_parkway_packed_dl_3EB0
                            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07003EB0);
                            break;
                        case 2:
                            // d_course_dks_jungle_parkway_packed_dl_
                            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07003E40);
                            // d_course_dks_jungle_parkway_packed_dl_3EB0
                            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07003EB0);
                            // d_course_dks_jungle_parkway_packed_dl_3F30
                            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07003F30);
                            break;
                        case 3:
                            // d_course_dks_jungle_parkway_packed_dl_3EB0
                            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07003EB0);
                            // d_course_dks_jungle_parkway_packed_dl_3F30
                            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07003F30);
                            break;
                    }
                    break;
                case 18:
                    switch (playerDirection) {
                        case 0:
                            // d_course_dks_jungle_parkway_packed_dl_3E40
                            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07003E40);
                            // d_course_dks_jungle_parkway_packed_dl_3EB0
                            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07003EB0);
                            break;
                        case 1:
                            // d_course_dks_jungle_parkway_packed_dl_3DD0
                            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07003DD0);
                            // d_course_dks_jungle_parkway_packed_dl_3E40
                            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07003E40);
                            // d_course_dks_jungle_parkway_packed_dl_3EB0
                            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07003EB0);
                            break;
                        case 2:
                            // d_course_dks_jungle_parkway_packed_dl_3E40
                            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07003E40);
                            // d_course_dks_jungle_parkway_packed_dl_3EB0
                            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07003EB0);
                            break;
                    }
                    break;
                case 21:
                    if ((playerDirection == 0) || (playerDirection == 1)) {
                        // d_course_dks_jungle_parkway_packed_dl_3E40
                        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07003E40);
                        // d_course_dks_jungle_parkway_packed_dl_3EB0
                        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07003EB0);
                        // d_course_dks_jungle_parkway_packed_dl_3F30
                        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07003F30);
                    } else {
                        // d_course_dks_jungle_parkway_packed_dl_3EB0
                        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07003EB0);
                    }
                    break;
                case 22:
                    if (playerDirection == 0) {
                        // d_course_dks_jungle_parkway_packed_dl_3F30
                        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07003F30);
                    }
                    break;
                case 23:
                    if (playerDirection != 1) {
                        // d_course_dks_jungle_parkway_packed_dl_3F30
                        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x07003F30);
                    }
                    break;
            }
            gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 1, 1, G_OFF);
            gDPSetAlphaCompare(gDisplayListHead++, G_AC_NONE);
            gDPPipeSync(gDisplayListHead++);
            break;
    }
}
#endif

void* memcpy2(void* dest, const void* src, size_t size) {
    unsigned char* su1 = (unsigned char*) dest;
    const unsigned char* su2 = (const unsigned char*) src;

    while (size > 0) {
        *su1 = *su2;
        su1++;
        su2++;
        size--;
    }

    return dest;
}

void Math_Vec3f_Sum(Vec3f l, Vec3f r, Vec3f dest) {
    dest[0] = l[0] + r[0];
    dest[1] = l[1] + r[1];
    dest[2] = l[2] + r[2];
}

void Math_Vec3f_Diff(Vec3f l, Vec3f r, Vec3f dest) {
    dest[0] = l[0] - r[0];
    dest[1] = l[1] - r[1];
    dest[2] = l[2] - r[2];
}

#define SQXYZ(vec) ((vec[0]) * (vec[0]) + (vec[1]) * (vec[1]) + (vec[2]) * (vec[2]))

f32 Math_Vec3f_DistXYZ(Vec3f a, Vec3f b) {
    Vec3f diff;
    Math_Vec3f_Diff(b, a, diff);
    return sqrtf(SQXYZ(diff));
}

typedef struct CameraProps {
    Vec3f prev_eye;
    Vec3f prev_at;
    Vec3f eye_velocity;
    Vec3f at_velocity;
} CameraProps;

static CameraProps gMultiCamera[4];

bool should_interpolate_perspective(Vec3f eye, Vec3f at, int cameraId) {
    (void) gMultiCamera[cameraId].prev_eye;
    (void) gMultiCamera[cameraId].prev_at;
    (void) gMultiCamera[cameraId].eye_velocity;
    (void) gMultiCamera[cameraId].at_velocity;

    Vec3f predicted_eye;
    Vec3f predicted_at;
    // Predict the new eye and at positions based on the previous velocity and positions.
    Math_Vec3f_Sum(gMultiCamera[cameraId].prev_eye, gMultiCamera[cameraId].eye_velocity, predicted_eye);
    Math_Vec3f_Sum(gMultiCamera[cameraId].prev_at, gMultiCamera[cameraId].at_velocity, predicted_at);

    // Calculate the current velocities from the previous and current positions.
    Math_Vec3f_Diff(eye, gMultiCamera[cameraId].prev_eye, gMultiCamera[cameraId].eye_velocity);
    Math_Vec3f_Diff(at, gMultiCamera[cameraId].prev_at, gMultiCamera[cameraId].at_velocity);

    // Compare the predicted positions to the real positions.
    float eye_dist = Math_Vec3f_DistXYZ(predicted_eye, eye);
    float at_dist = Math_Vec3f_DistXYZ(predicted_at, at);

    // Compare the velocities of the eye and at positions.
    float velocity_diff = Math_Vec3f_DistXYZ(gMultiCamera[cameraId].eye_velocity, gMultiCamera[cameraId].at_velocity);

    // Update the tracking for the previous positions with the new ones.
    memcpy2(gMultiCamera[cameraId].prev_eye, eye, sizeof(Vec3f));
    memcpy2(gMultiCamera[cameraId].prev_at, at, sizeof(Vec3f));

    // These numbers are all picked via testing.

    // If the velocity of both positions was the same, then they're moving together and should interpolate.
    if (velocity_diff <= 3.0f && eye_dist <= 100.0f && at_dist <= 100.0f) {
        // recomp_printf("if (velocity_diff <= 3.0f && eye_dist <= 100.0f && at_dist <= 100.0f) {\n");
        return true;
    }

    // If the focus or position are basically the same across frames and the eye didn't move too far then it should
    // probably be interpolated.
    // if (at_dist <= 10.0f && eye_dist <= 300.0f) {
    //     return true;
    // }
    // if (eye_dist <= 10.0f && at_dist <= 300.0f) {
    //     return true;
    // }

    if (velocity_diff > 50.0f || at_dist > 50.0f || eye_dist > 300.0f) {
        gMultiCamera[cameraId].eye_velocity[0] = 0.0f;
        gMultiCamera[cameraId].eye_velocity[1] = 0.0f;
        gMultiCamera[cameraId].eye_velocity[2] = 0.0f;
        gMultiCamera[cameraId].at_velocity[0] = 0.0f;
        gMultiCamera[cameraId].at_velocity[1] = 0.0f;
        gMultiCamera[cameraId].at_velocity[2] = 0.0f;
        return false;
    }
    // recomp_printf("last return true\n");
    return true;
}

s32 gCamera1Skipped = 0;
s32 gCamera2Skipped = 0;
s32 gCamera3Skipped = 0;
s32 gCamera4Skipped = 0;

#if 1
RECOMP_PATCH void render_player_one_1p_screen(void) {
    Camera* camera = &cameras[0];
    UNUSED s32 pad[4];
    u16 perspNorm;
    UNUSED s32 pad2[2];
#ifdef VERSION_EU
    f32 sp9C;
#endif
    UNUSED s32 pad3;
    Mat4 matrix;

    // recomp_printf("CAMERA POS X: %f\n", camera->pos[0]);
    // recomp_printf("CAMERA POS Y: %f\n", camera->pos[1]);
    // recomp_printf("CAMERA POS Z: %f\n\n", camera->pos[2]);
    // recomp_printf("CAMERA LOOKAT X: %f\n", camera->pos[0]);
    // recomp_printf("CAMERA LOOKAT Y: %f\n", camera->pos[1]);
    // recomp_printf("CAMERA LOOKAT Z: %f\n\n", camera->pos[2]);

    bool bigJump = !should_interpolate_perspective(camera->pos, camera->lookAt, 0);

    if (bigJump) {
        gEXMatrixGroupSimple(gDisplayListHead++, 0xFFFFAAAA, G_EX_NOPUSH, G_MTX_PROJECTION, G_EX_COMPONENT_SKIP,
                             G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_NONE);
        recomp_printf("CAMERA 1 SKIPED: %d\n", bigJump);
        gCamera1Skipped = true;
    } else {
        gEXMatrixGroupSimple(gDisplayListHead++, 0xFFFFAAAA, G_EX_NOPUSH, G_MTX_PROJECTION, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_NONE);
        gCamera1Skipped = false;
    }

    func_802A53A4();
    init_rdp();
    // func_802A3730(D_800DC5EC);
    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);

    guPerspective(&gGfxPool->mtxPersp[0], &perspNorm, gCameraZoom[0], gScreenAspect, gCourseNearPersp, gCourseFarPersp,
                  1.0f);

    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxPersp[0]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);

    guLookAt(&gGfxPool->mtxLookAt[0], camera->pos[0], camera->pos[1], camera->pos[2], camera->lookAt[0],
             camera->lookAt[1], camera->lookAt[2], camera->up[0], camera->up[1], camera->up[2]);
    if (D_800DC5C8 == 0) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    render_course(D_800DC5EC);
    if (D_800DC5C8 == 1) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    }
    render_course_actors(D_800DC5EC);
    render_object(RENDER_SCREEN_MODE_1P_PLAYER_ONE);
    render_players_on_screen_one();
    func_8029122C(D_800DC5EC, PLAYER_ONE);
    func_80021B0C();
    render_item_boxes(D_800DC5EC);
    render_player_snow_effect(RENDER_SCREEN_MODE_1P_PLAYER_ONE);
    func_80058BF4();
    if (D_800DC5B8 != 0) {
        func_80058C20(RENDER_SCREEN_MODE_1P_PLAYER_ONE);
    }
    func_80093A5C(RENDER_SCREEN_MODE_1P_PLAYER_ONE);
    if (D_800DC5B8 != 0) {
        render_hud(RENDER_SCREEN_MODE_1P_PLAYER_ONE);
    }
}
#endif

#if 1
RECOMP_PATCH void render_player_one_2p_screen_vertical(void) {
    Camera* camera = &cameras[0];
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 matrix;

    bool bigJump = !should_interpolate_perspective(camera->pos, camera->lookAt, 0);

    if (bigJump) {
        gEXMatrixGroupSimple(gDisplayListHead++, 0xFFFFAAAB, G_EX_NOPUSH, G_MTX_PROJECTION, G_EX_COMPONENT_SKIP,
                             G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_NONE);
        recomp_printf("CAMERA 1 SKIPED: %d\n", bigJump);
        gCamera1Skipped = true;
    } else {
        gEXMatrixGroupSimple(gDisplayListHead++, 0xFFFFAAAB, G_EX_NOPUSH, G_MTX_PROJECTION, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_NONE);
        gCamera1Skipped = false;
    }

    func_802A50EC();

    init_rdp();
    func_802A3730(D_800DC5EC);
    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);

    guPerspective(&gGfxPool->mtxPersp[0], &perspNorm, gCameraZoom[0], gScreenAspect, gCourseNearPersp, gCourseFarPersp,
                  1.0f);
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxPersp[0]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&gGfxPool->mtxLookAt[0], camera->pos[0], camera->pos[1], camera->pos[2], camera->lookAt[0],
             camera->lookAt[1], camera->lookAt[2], camera->up[0], camera->up[1], camera->up[2]);

    if (D_800DC5C8 == 0) {

        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    render_course(D_800DC5EC);
    if (D_800DC5C8 == 1) {

        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);

        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    }
    render_course_actors(D_800DC5EC);
    render_object(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE);
    render_players_on_screen_one();
    func_8029122C(D_800DC5EC, PLAYER_ONE);
    func_80021B0C();
    render_item_boxes(D_800DC5EC);
    render_player_snow_effect(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE);
    func_80058BF4();
    if (D_800DC5B8 != 0) {
        func_80058C20(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE);
    }
    func_80093A5C(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE);
    if (D_800DC5B8 != 0) {
        render_hud(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE);
    }
    D_8015F788 += 1;
}
#endif

#if 1
RECOMP_PATCH void render_player_two_2p_screen_vertical(void) {
    Camera* camera = &cameras[1];
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 matrix;

    bool bigJump = !should_interpolate_perspective(camera->pos, camera->lookAt, 1);

    if (bigJump) {
        gEXMatrixGroupSimple(gDisplayListHead++, 0xFFFFAAAC, G_EX_NOPUSH, G_MTX_PROJECTION, G_EX_COMPONENT_SKIP,
                             G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_NONE);
        recomp_printf("CAMERA 2 SKIPED: %d\n", bigJump);
        gCamera2Skipped = true;
    } else {
        gEXMatrixGroupSimple(gDisplayListHead++, 0xFFFFAAAC, G_EX_NOPUSH, G_MTX_PROJECTION, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_NONE);
        gCamera2Skipped = false;
    }

    func_802A5004();
    init_rdp();
    func_802A3730(D_800DC5F0);

    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);

    guPerspective(&gGfxPool->mtxPersp[1], &perspNorm, gCameraZoom[1], gScreenAspect, gCourseNearPersp, gCourseFarPersp,
                  1.0f);

    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxPersp[1]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&gGfxPool->mtxLookAt[1], camera->pos[0], camera->pos[1], camera->pos[2], camera->lookAt[0],
             camera->lookAt[1], camera->lookAt[2], camera->up[0], camera->up[1], camera->up[2]);

    if (D_800DC5C8 == 0) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    render_course(D_800DC5F0);
    if (D_800DC5C8 == 1) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    }
    render_course_actors(D_800DC5F0);
    render_object(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO);
    render_players_on_screen_two();
    func_8029122C(D_800DC5F0, PLAYER_TWO);
    func_80021C78();
    render_item_boxes(D_800DC5F0);
    func_80058BF4();
    render_player_snow_effect(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO);
    if (D_800DC5B8 != 0) {
        func_80058C20(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO);
    }
    func_80093A5C(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO);
    if (D_800DC5B8 != 0) {
        render_hud(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO);
    }
    D_8015F788 += 1;
}
#endif

#if 1
RECOMP_PATCH void render_player_one_2p_screen_horizontal(void) {
    Camera* camera = &cameras[0];
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 matrix;
#ifdef VERSION_EU
    f32 sp9C;
#endif

    bool bigJump = !should_interpolate_perspective(camera->pos, camera->lookAt, 0);

    if (bigJump) {
        gEXMatrixGroupSimple(gDisplayListHead++, 0xFFFFAAAD, G_EX_NOPUSH, G_MTX_PROJECTION, G_EX_COMPONENT_SKIP,
                             G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_NONE);
        recomp_printf("CAMERA 1 SKIPED: %d\n", bigJump);
        gCamera1Skipped = true;
    } else {
        gEXMatrixGroupSimple(gDisplayListHead++, 0xFFFFAAAD, G_EX_NOPUSH, G_MTX_PROJECTION, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_NONE);
        gCamera1Skipped = false;
    }

    func_802A51D4();
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH);
    init_rdp();
    func_802A3730(D_800DC5EC);
#ifdef VERSION_EU
    sp9C = gScreenAspect * 1.2f;
#endif
    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
#ifdef VERSION_EU
    guPerspective(&gGfxPool->mtxPersp[0], &perspNorm, gCameraZoom[0], sp9C, gCourseNearPersp, gCourseFarPersp, 1.0f);
#else
    guPerspective(&gGfxPool->mtxPersp[0], &perspNorm, gCameraZoom[0], gScreenAspect, gCourseNearPersp, gCourseFarPersp,
                  1.0f);
#endif
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxPersp[0]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&gGfxPool->mtxLookAt[0], camera->pos[0], camera->pos[1], camera->pos[2], camera->lookAt[0],
             camera->lookAt[1], camera->lookAt[2], camera->up[0], camera->up[1], camera->up[2]);

    if (D_800DC5C8 == 0) {

        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    render_course(D_800DC5EC);
    if (D_800DC5C8 == 1) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    }
    render_course_actors(D_800DC5EC);
    render_object(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE);
    render_players_on_screen_one();
    func_8029122C(D_800DC5EC, PLAYER_ONE);
    func_80021B0C();
    render_item_boxes(D_800DC5EC);
    render_player_snow_effect(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE);
    func_80058BF4();
    if (D_800DC5B8 != 0) {
        func_80058C20(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE);
    }
    func_80093A5C(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE);
    if (D_800DC5B8 != 0) {
        render_hud(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE);
    }
    D_8015F788 += 1;
}
#endif

#if 1
RECOMP_PATCH void render_player_two_2p_screen_horizontal(void) {
    Camera* camera = &cameras[1];
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 matrix;
#ifdef VERSION_EU
    f32 sp9C;
#endif

    bool bigJump = !should_interpolate_perspective(camera->pos, camera->lookAt, 1);

    if (bigJump) {
        gEXMatrixGroupSimple(gDisplayListHead++, 0xFFFFAAAE, G_EX_NOPUSH, G_MTX_PROJECTION, G_EX_COMPONENT_SKIP,
                             G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_NONE);
        recomp_printf("CAMERA 2 SKIPED: %d\n", bigJump);
        gCamera2Skipped = true;
    } else {
        gEXMatrixGroupSimple(gDisplayListHead++, 0xFFFFAAAE, G_EX_NOPUSH, G_MTX_PROJECTION, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_NONE);
        gCamera2Skipped = false;
    }

    func_802A52BC();
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH);
    init_rdp();
    func_802A3730(D_800DC5F0);
#ifdef VERSION_EU
    sp9C = gScreenAspect * 1.2f;
#endif
    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
#ifdef VERSION_EU
    guPerspective(&gGfxPool->mtxPersp[1], &perspNorm, gCameraZoom[1], sp9C, gCourseNearPersp, gCourseFarPersp, 1.0f);
#else
    guPerspective(&gGfxPool->mtxPersp[1], &perspNorm, gCameraZoom[1], gScreenAspect, gCourseNearPersp, gCourseFarPersp,
                  1.0f);
#endif
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxPersp[1]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&gGfxPool->mtxLookAt[1], camera->pos[0], camera->pos[1], camera->pos[2], camera->lookAt[0],
             camera->lookAt[1], camera->lookAt[2], camera->up[0], camera->up[1], camera->up[2]);

    if (D_800DC5C8 == 0) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    render_course(D_800DC5F0);
    if (D_800DC5C8 == 1) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    }
    render_course_actors(D_800DC5F0);
    render_object(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO);
    render_players_on_screen_two();
    func_8029122C(D_800DC5F0, PLAYER_TWO);
    func_80021C78();
    render_item_boxes(D_800DC5F0);
    render_player_snow_effect(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO);
    func_80058BF4();
    if (D_800DC5B8 != 0) {
        func_80058C20(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO);
    }
    func_80093A5C(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO);
    if (D_800DC5B8 != 0) {
        render_hud(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO);
    }
    D_8015F788 += 1;
}
#endif

#if 1
RECOMP_PATCH void render_player_one_3p_4p_screen(void) {
    Camera* camera = camera1;
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 matrix;
#ifdef VERSION_EU
    f32 sp9C;
    sp9C = gScreenAspect * 1.2f;
#endif

    bool bigJump = !should_interpolate_perspective(camera->pos, camera->lookAt, 0);

    if (bigJump) {
        gEXMatrixGroupSimple(gDisplayListHead++, 0xFFFFAAAF, G_EX_NOPUSH, G_MTX_PROJECTION, G_EX_COMPONENT_SKIP,
                             G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_NONE);
        recomp_printf("CAMERA 1 SKIPED: %d\n", bigJump);
        gCamera1Skipped = true;
    } else {
        gEXMatrixGroupSimple(gDisplayListHead++, 0xFFFFAAAF, G_EX_NOPUSH, G_MTX_PROJECTION, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_NONE);
        gCamera1Skipped = false;
    }

    func_802A54A8();
    init_rdp();
    func_802A3730(D_800DC5EC);
    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
#ifdef VERSION_EU
    guPerspective(&gGfxPool->mtxPersp[0], &perspNorm, gCameraZoom[0], sp9C, gCourseNearPersp, gCourseFarPersp, 1.0f);
#else
    guPerspective(&gGfxPool->mtxPersp[0], &perspNorm, gCameraZoom[0], gScreenAspect, gCourseNearPersp, gCourseFarPersp,
                  1.0f);
#endif
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxPersp[0]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&gGfxPool->mtxLookAt[0], camera->pos[0], camera->pos[1], camera->pos[2], camera->lookAt[0],
             camera->lookAt[1], camera->lookAt[2], camera->up[0], camera->up[1], camera->up[2]);

    if (D_800DC5C8 == 0) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    render_course(D_800DC5EC);
    if (D_800DC5C8 == 1) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    }
    render_course_actors(D_800DC5EC);
    render_object(RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE);
    render_players_on_screen_one();
    func_8029122C(D_800DC5EC, PLAYER_ONE);
    func_80021B0C();
    render_item_boxes(D_800DC5EC);
    render_player_snow_effect(RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE);
    func_80058BF4();
    if (D_800DC5B8 != 0) {
        func_80058C20(RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE);
    }
    func_80093A5C(RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE);
    if (D_800DC5B8 != 0) {
        render_hud(RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE);
    }
    D_8015F788 += 1;
}
#endif

#if 1
RECOMP_PATCH void render_player_two_3p_4p_screen(void) {
    Camera* camera = camera2;
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 matrix;
#ifdef VERSION_EU
    f32 sp9C;
    sp9C = gScreenAspect * 1.2f;
#endif

    bool bigJump = !should_interpolate_perspective(camera->pos, camera->lookAt, 1);

    if (bigJump) {
        gEXMatrixGroupSimple(gDisplayListHead++, 0xFFFFAAB0, G_EX_NOPUSH, G_MTX_PROJECTION, G_EX_COMPONENT_SKIP,
                             G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_NONE);
        recomp_printf("CAMERA 2 SKIPED: %d\n", bigJump);
        gCamera2Skipped = true;
    } else {
        gEXMatrixGroupSimple(gDisplayListHead++, 0xFFFFAAB0, G_EX_NOPUSH, G_MTX_PROJECTION, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_NONE);
        gCamera2Skipped = false;
    }

    func_802A5590();
    init_rdp();
    func_802A3730(D_800DC5F0);
    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
#ifdef VERSION_EU
    guPerspective(&gGfxPool->mtxPersp[1], &perspNorm, gCameraZoom[1], sp9C, gCourseNearPersp, gCourseFarPersp, 1.0f);
#else
    guPerspective(&gGfxPool->mtxPersp[1], &perspNorm, gCameraZoom[1], gScreenAspect, gCourseNearPersp, gCourseFarPersp,
                  1.0f);
#endif
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxPersp[1]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);

    guLookAt(&gGfxPool->mtxLookAt[1], camera->pos[0], camera->pos[1], camera->pos[2], camera->lookAt[0],
             camera->lookAt[1], camera->lookAt[2], camera->up[0], camera->up[1], camera->up[2]);
    if (D_800DC5C8 == 0) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    render_course(D_800DC5F0);
    if (D_800DC5C8 == 1) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    }
    render_course_actors(D_800DC5F0);
    render_object(RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO);
    render_players_on_screen_two();
    func_8029122C(D_800DC5F0, PLAYER_TWO);
    func_80021C78();
    render_item_boxes(D_800DC5F0);
    render_player_snow_effect(RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO);
    func_80058BF4();
    if (D_800DC5B8 != 0) {
        func_80058C20(RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO);
    }
    func_80093A5C(RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO);
    if (D_800DC5B8 != 0) {
        render_hud(RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO);
    }
    D_8015F788 += 1;
}
#endif

#if 1
RECOMP_PATCH void render_player_three_3p_4p_screen(void) {
    Camera* camera = camera3;
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 matrix;

    bool bigJump = !should_interpolate_perspective(camera->pos, camera->lookAt, 2);

    if (bigJump) {
        gEXMatrixGroupSimple(gDisplayListHead++, 0xFFFFAAB1, G_EX_NOPUSH, G_MTX_PROJECTION, G_EX_COMPONENT_SKIP,
                             G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_NONE);
        recomp_printf("CAMERA 3 SKIPED: %d\n", bigJump);
        gCamera3Skipped = true;
    } else {
        gEXMatrixGroupSimple(gDisplayListHead++, 0xFFFFAAB1, G_EX_NOPUSH, G_MTX_PROJECTION, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_NONE);
        gCamera3Skipped = false;
    }

    func_802A5678();
    init_rdp();
    func_802A3730(D_800DC5F4);

    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);

    guPerspective(&gGfxPool->mtxPersp[2], &perspNorm, gCameraZoom[2], gScreenAspect, gCourseNearPersp, gCourseFarPersp,
                  1.0f);

    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxPersp[2]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&gGfxPool->mtxLookAt[2], camera->pos[0], camera->pos[1], camera->pos[2], camera->lookAt[0],
             camera->lookAt[1], camera->lookAt[2], camera->up[0], camera->up[1], camera->up[2]);
    if (D_800DC5C8 == 0) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[2]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);

        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[2]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    render_course(D_800DC5F4);
    if (D_800DC5C8 == 1) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[2]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    }
    render_course_actors(D_800DC5F4);
    render_object(RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE);
    render_players_on_screen_three();
    func_8029122C(D_800DC5F4, PLAYER_THREE);
    func_80021D40();
    render_item_boxes(D_800DC5F4);
    render_player_snow_effect(RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE);
    func_80058BF4();
    if (D_800DC5B8 != 0) {
        func_80058C20(RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE);
    }
    func_80093A5C(RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE);
    if (D_800DC5B8 != 0) {
        render_hud(RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE);
    }
    D_8015F788 += 1;
}
#endif

#if 1
RECOMP_PATCH void render_player_four_3p_4p_screen(void) {
    Camera* camera = camera4;
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 matrix;

    bool bigJump = !should_interpolate_perspective(camera->pos, camera->lookAt, 3);

    if (bigJump) {
        gEXMatrixGroupSimple(gDisplayListHead++, 0xFFFFAAB2, G_EX_NOPUSH, G_MTX_PROJECTION, G_EX_COMPONENT_SKIP,
                             G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_NONE);
        recomp_printf("CAMERA 4 SKIPED: %d\n", bigJump);
        gCamera4Skipped = true;
    } else {
        gEXMatrixGroupSimple(gDisplayListHead++, 0xFFFFAAB2, G_EX_NOPUSH, G_MTX_PROJECTION, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_NONE);
        gCamera4Skipped = false;
    }

    func_802A5760();
    if (gPlayerCountSelection1 == 3) {
        func_80093A5C(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR);
        if (D_800DC5B8 != 0) {
            render_hud(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR);
        }
        D_8015F788 += 1;
        return;
    }

    init_rdp();
    func_802A3730(D_800DC5F8);

    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);

    guPerspective(&gGfxPool->mtxPersp[3], &perspNorm, gCameraZoom[3], gScreenAspect, gCourseNearPersp, gCourseFarPersp,
                  1.0f);
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxPersp[3]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&gGfxPool->mtxLookAt[3], camera->pos[0], camera->pos[1], camera->pos[2], camera->lookAt[0],
             camera->lookAt[1], camera->lookAt[2], camera->up[0], camera->up[1], camera->up[2]);
    if (D_800DC5C8 == 0) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[3]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[3]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    render_course(D_800DC5F8);
    if (D_800DC5C8 == 1) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxLookAt[3]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        mtxf_identity(matrix);
        render_set_position(matrix, 0);
    }
    render_course_actors(D_800DC5F8);
    render_object(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR);
    render_players_on_screen_four();
    func_8029122C(D_800DC5F8, PLAYER_FOUR);
    func_80021DA8();
    render_item_boxes(D_800DC5F8);
    render_player_snow_effect(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR);
    func_80058BF4();
    if (D_800DC5B8 != 0) {
        func_80058C20(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR);
    }
    func_80093A5C(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR);
    if (D_800DC5B8 != 0) {
        render_hud(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR);
    }
    D_8015F788 += 1;
}
#endif

#endif // GLOBAL