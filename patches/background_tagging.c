#if 1
#include "patches.h"

// There are two types of snowflakes, the ones on the background and the ones on the foreground
// Foreground snowflakes only render in single player.
// background snowflakes only render in 1P and 2P modes.

// =================== Code for the background snowflakes ===============

typedef enum ScreenID {
    /* 0 */ SCREEN_1P,
    /* 1 */ SCREEN_2P_TOP,
    /* 2 */ SCREEN_2P_BOTTOM,
    /* 3 */ SCREEN_3_4_P_TOP_LEFT,
    /* 4 */ SCREEN_3_4_P_TOP_RIGHT,
    /* 5 */ SCREEN_3_4_P_BOTTOM_LEFT,
    /* 6 */ SCREEN_3_4_P_BOTTOM_RIGHT,
} ScreenID;

#define ABS(x) ((x) >= 0 ? (x) : -(x))

struct ObjectInterpData {
    s32 objectIndex;
    s16 x, y;
};
struct ObjectInterpData prevObject[OBJECT_LIST_SIZE][4] = { 0 };

// Snowflakes in the background (draw func)
void func_800518F8_recomp(s32 objectIndex, s16 x, s16 y, ScreenID screen) {
    // Search all recorded objects for the one we're drawing
    for (size_t i = 140; i < 190; i++) {
        if (objectIndex == prevObject[i][screen].objectIndex) {
            // recomp_printf("Object coincidence: %d\n", objectIndex);
            //  Coincidence!
            //  Skip drawing the object this frame if it warped to the other side of the screen

            switch (screen) {
                case SCREEN_1P:
                    if ((ABS(x - prevObject[i][screen].x) > SCREEN_WIDTH / 2) || (ABS(y - prevObject[i][screen].y) > SCREEN_HEIGHT / 2)) {
                        prevObject[objectIndex][screen].x = x;
                        prevObject[objectIndex][screen].y = y;
                        prevObject[objectIndex][screen].objectIndex = objectIndex;
                        return;
                    }
                break;
                
                case SCREEN_2P_TOP:
                case SCREEN_2P_BOTTOM:
                    if ((ABS(x - prevObject[i][screen].x) > SCREEN_WIDTH / 2) || (ABS(y - prevObject[i][screen].y) > (SCREEN_HEIGHT / 2) / 2)) {
                        prevObject[objectIndex][screen].x = x;
                        prevObject[objectIndex][screen].y = y;
                        prevObject[objectIndex][screen].objectIndex = objectIndex;
                        return;
                    }
                break;

                // Unimplemented yet
                case SCREEN_3_4_P_TOP_LEFT:
                case SCREEN_3_4_P_TOP_RIGHT:
                case SCREEN_3_4_P_BOTTOM_LEFT:
                case SCREEN_3_4_P_BOTTOM_RIGHT:
                    if ((ABS(x - prevObject[i][screen].x) > (SCREEN_WIDTH / 2) / 2) || (ABS(y - prevObject[i][screen].y) > (SCREEN_HEIGHT / 2) / 2)) {
                        prevObject[objectIndex][screen].x = x;
                        prevObject[objectIndex][screen].y = y;
                        prevObject[objectIndex][screen].objectIndex = objectIndex;
                        return;
                    }
                break;

                default:
                break;
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
    prevObject[objectIndex][screen].x = x;
    prevObject[objectIndex][screen].y = y;
    prevObject[objectIndex][screen].objectIndex = objectIndex;
}

// Background SnowFlakes in 1P
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

            // @recomp: use custom function func_800518F8_recomp so we can pass the screen index
            // func_800518F8(objectIndex, object->unk_09C, arg0 - object->unk_09E);
            func_800518F8_recomp(objectIndex, object->unk_09C, arg0 - object->unk_09E, SCREEN_1P);
        }
    }
}

// Background SnowFlakes in 2P
void func_80051C60_recomp(s16 arg0, s32 arg1, ScreenID screen) {
    s16 var_s5;
    s32 var_s0;
    s32 objectIndex;
    Object* object;

    if (D_801658FE == 0) {
        if (gCurrentCourseId == COURSE_KOOPA_BEACH) {
            var_s5 = arg0;
        } else if (gCurrentCourseId == COURSE_MOO_MOO_FARM) {
            var_s5 = arg0 - 0x10;
        } else if (gCurrentCourseId == COURSE_YOSHI_VALLEY) {
            var_s5 = arg0 - 0x10;
        } else {
            var_s5 = arg0 + 0x10;
        }
    } else if (gCurrentCourseId == COURSE_KOOPA_BEACH) {
        var_s5 = arg0 * 2;
    } else {
        var_s5 = arg0 + 0x20;
    }
    D_8018D228 = 0xFF;
    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0D007A60 /* D_0D007A60*/);
    if ((u8) D_8018D230 != 0) {
        func_8004B414(0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF);
        for (var_s0 = 0; var_s0 < D_8018D1F0; var_s0++) {
            objectIndex = D_8018CC80[arg1 + var_s0];
            object = &gObjectList[objectIndex];
            func_800519D4(objectIndex, object->unk_09C, (var_s5 - object->unk_09E) / 2);
        }
    } else {
        func_8004B6C4(0x000000FF, 0x000000FF, 0x000000FF);
        for (var_s0 = 0; var_s0 < D_8018D1F0; var_s0++) {
            objectIndex = D_8018CC80[arg1 + var_s0];
            object = &gObjectList[objectIndex];

            // @recomp: use custom function func_800518F8_recomp so we can pass the screen index
            // func_800518F8(objectIndex, object->unk_09C, (var_s5 - object->unk_09E) / 2);
            func_800518F8_recomp(objectIndex, object->unk_09C, (var_s5 - object->unk_09E) / 2, screen);
        }
    }
}

// Background SnowFlakes in 2P TOP SCREEN
RECOMP_PATCH void func_80052044(void) {
    func_80051C60_recomp(240 - D_800DC5EC->cameraHeight, 0, SCREEN_2P_TOP);
}

// Background SnowFlakes in 2P BOTTOM SCREEN
RECOMP_PATCH void func_80052080(void) {
    func_80051C60_recomp(240 - D_800DC5F0->cameraHeight, D_8018D1F0, SCREEN_2P_BOTTOM);
}

// =================== Code for the foreground snowflakes ===============

#define ABSF(x) ((x) >= 0.0f ? (x) : -(x))

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

// @return: 0 equals drawn
s32 rsp_set_matrix_gObjectList_snowflakes(s32 transformIndex) {
    Mat4 matrix;

    if (mtxf_set_matrix_gObjectList_snowflakes(transformIndex, matrix))
        return 1; // skipped

    convert_to_fixed_point_matrix(&gGfxPool->mtxHud[gMatrixHudCount], matrix);

    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxHud[gMatrixHudCount++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    return 0; // drawn
}

// Snowflakes
RECOMP_PATCH void render_object_snowflakes_particles(void) {
    s32 i;
    s32 snowflakeIndex;

    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0D007AE0);
    gDPSetCombineLERP(gDisplayListHead++, 1, 0, SHADE, 0, 0, 0, 0, TEXEL0, 1, 0, SHADE, 0, 0, 0, 0, TEXEL0);
    func_80044F34((u8*) 0x0D0293D8, 0x10, 0x10);
    for (i = 0; i < NUM_SNOWFLAKES; i++) {
        snowflakeIndex = gObjectParticle1[i];
        if (gObjectList[snowflakeIndex].state >= 2) {
            // @recomp Tag the transform.
            gEXMatrixGroupDecomposedNormal(gDisplayListHead++, TAG_OBJECT(&gObjectParticle1[i]), G_EX_PUSH, G_MTX_MODELVIEW,
                                           G_EX_EDIT_ALLOW);
            if (rsp_set_matrix_gObjectList_snowflakes(snowflakeIndex)) {
                // @recomp Pop the transform id.
                gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
                continue;
            }
            gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0D006980);

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
        }
    }
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}
#endif
