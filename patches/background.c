#include "patches.h"

#if 1

Mtx D_0D008E98 = {
    toFixedPointMatrix(6.0, 0.0, 0.0, 0.0,                                             // Make the skybox 5x wider
                       0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1000.0, 0.0, 0.0, 1.0) // Shift it farther left

};

// Background Extended for Widescreen
RECOMP_PATCH void func_802A4A0C(Vtx* vtx, struct UnkStruct_800DC5EC* arg1, UNUSED s32 arg2, UNUSED s32 arg3,
                                UNUSED f32* arg4) {
    Camera* camera = arg1->camera;
    s16 temp_t5;
    f32 temp_f0;
    u16 sp128;
    Mat4 matrix1, matrix2, matrix3;
    Vec3f sp5C = { 0.0f, 0.0f, 30000.0f };
    f32 sp58;

    course_set_skybox_colours(vtx);
    func_802B5564(matrix1, &sp128, camera->unk_B4, gScreenAspect, gCourseNearPersp, gCourseFarPersp, 1.0f);
    func_802B5794(matrix2, camera->pos, camera->lookAt);
    mtxf_multiplication(matrix3, matrix1, matrix2);

    sp58 = ((matrix3[0][3] * sp5C[0]) + (matrix3[1][3] * sp5C[1]) + (matrix3[2][3] * sp5C[2])) + matrix3[3][3];
    mtxf_translate_vec3f_mat4(sp5C, matrix3);

    temp_f0 = (1.0f / sp58);
    sp5C[0] *= temp_f0 * 160.0f;
    sp5C[1] *= temp_f0 * 120.0f;

    temp_t5 = 120 - (s16) sp5C[1];
    arg1->cameraHeight = temp_t5;

    vtx[1].v.ob[1] = vtx[2].v.ob[1] = vtx[4].v.ob[1] = vtx[7].v.ob[1] = temp_t5;

    init_rdp();
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, (SCREEN_WIDTH - 320) / 2, 0, (SCREEN_WIDTH + 320) / 2, 240);
    gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gSPClearGeometryMode(gDisplayListHead++, G_ZBUFFER | G_LIGHTING);
    guOrtho(&gGfxPool->mtxScreen, 0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT, 0.0f, 5.0f, 1.0f);
    gSPPerspNormalize(gDisplayListHead++, 0xFFFF);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxScreen),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&D_0D008E98), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    // Draw the top half
    gSPVertex(gDisplayListHead++, &vtx[0], 4, 0);
    gSP2Triangles(gDisplayListHead++, 0, 3, 1, 0, 1, 3, 2, 0);

    // Draw the bottom half
    gSPVertex(gDisplayListHead++, &vtx[4], 4, 0);
    gSP2Triangles(gDisplayListHead++, 0, 3, 1, 0, 1, 3, 2, 0);
}
#endif

#if 0
RECOMP_PATCH void func_80042330(s32 x, s32 y, u16 angle, f32 scale) {
    Mat4 matrix;
    mtxf_translation_x_y_rotate_z_scale_x_y(matrix, x, y, angle, scale);
    convert_to_fixed_point_matrix(&gGfxPool->mtxHud[gMatrixHudCount], matrix);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxHud[gMatrixHudCount++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}
#endif