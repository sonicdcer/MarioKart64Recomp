#include "patches.h"

#if 1
Mtx D_0D008E98 = {
    toFixedPointMatrix(1.0, 0.0, 0.0, 0.0, 
                       0.0, 1.0, 0.0, 0.0, 
                       0.0, 0.0, 1.0, 0.0, 
                       0.0, 0.0, 0.0, 1.0),
};

// Background
RECOMP_PATCH void func_802A4A0C(Vtx* vtx, struct UnkStruct_800DC5EC* arg1, UNUSED s32 arg2, UNUSED s32 arg3, UNUSED f32* arg4) {
    Camera* camera = arg1->camera;
    s16 temp_t5;
    f32 temp_f0;
    UNUSED s32 pad[2];
    UNUSED u16 pad2;
    u16 sp128;
    Mat4 matrix1;
    Mat4 matrix2;
    Mat4 matrix3;
    Vec3f sp5C;
    f32 sp58;

    course_set_skybox_colours(vtx);

    // Adjust vertices for 32:9 aspect ratio
    s32 extendedWidth = SCREEN_WIDTH * (32.0f / 9.0f); // Calculate the new 32:9 width
    s32 leftEdge = -extendedWidth / 2;                 // Start at the far left of the 32:9 screen
    s32 rightEdge = extendedWidth / 2;                // End at the far right of the 32:9 screen

    vtx[0].v.ob[0] = leftEdge;
    vtx[1].v.ob[0] = leftEdge;
    vtx[2].v.ob[0] = rightEdge;
    vtx[3].v.ob[0] = rightEdge;
    vtx[4].v.ob[0] = leftEdge;
    vtx[5].v.ob[0] = leftEdge;
    vtx[6].v.ob[0] = rightEdge;
    vtx[7].v.ob[0] = rightEdge;

    sp5C[0] = 0.0f;
    sp5C[1] = 0.0f;
    sp5C[2] = 30000.0f;
    func_802B5564(matrix1, &sp128, camera->unk_B4, gScreenAspect, gCourseNearPersp, gCourseFarPersp, 1.0f);
    func_802B5794(matrix2, camera->pos, camera->lookAt);
    mtxf_multiplication(matrix3, matrix1, matrix2);

    sp58 = ((matrix3[0][3] * sp5C[0]) + (matrix3[1][3] * sp5C[1]) + (matrix3[2][3] * sp5C[2])) + matrix3[3][3];

    mtxf_translate_vec3f_mat4(sp5C, matrix3);

    temp_f0 = (1.0 / sp58);

    sp5C[0] *= temp_f0;
    sp5C[1] *= temp_f0;

    sp5C[0] *= 160.0f;
    sp5C[1] *= 120.0f;

    temp_t5 = 120 - (s16) sp5C[1];
    arg1->cameraHeight = temp_t5;
    vtx[1].v.ob[1] = temp_t5;
    vtx[2].v.ob[1] = temp_t5;
    vtx[4].v.ob[1] = temp_t5;
    vtx[7].v.ob[1] = temp_t5;

    init_rdp();
    gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gSPClearGeometryMode(gDisplayListHead++, G_ZBUFFER | G_LIGHTING);
    guOrtho(&gGfxPool->mtxScreen, 0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT, 0.0f, 5.0f, 1.0f);
    gSPPerspNormalize(gDisplayListHead++, 0xFFFF);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxScreen),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&D_0D008E98), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPVertex(gDisplayListHead++, &vtx[0], 4, 0);
    gSP2Triangles(gDisplayListHead++, 0, 3, 1, 0, 1, 3, 2, 0);
    if (gCurrentCourseId == COURSE_RAINBOW_ROAD) {
        gSPVertex(gDisplayListHead++, &vtx[4], 4, 0);
        gSP2Triangles(gDisplayListHead++, 0, 3, 1, 0, 1, 3, 2, 0);
    }
}
#endif
