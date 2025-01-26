#include "patches.h"

// Background Extended for Widescreen
#if 1

Mtx D_0D008E98;

RECOMP_PATCH void func_802A4A0C(Vtx* vtx, struct UnkStruct_800DC5EC* arg1, UNUSED s32 arg2, UNUSED s32 arg3,
                                UNUSED f32* arg4) {
    float aspect = recomp_get_aspect_ratio(0.0f);
    u32 margin = 0;

    if (aspect < (4.0f / 3.0f)) {
        aspect = (4.0f / 3.0f);
    }

    {
        // 4:3 baseline
        float baseAspect = 1.33133334f;
        float scaleX = aspect / baseAspect;
        float translateX = (1.0f - scaleX) * 160.0f;

        // Build your matrix at runtime
        D_0D008E98 = (Mtx){ toFixedPointMatrix(scaleX, 0.0f, 0.0f, 0.0f,    // X scale
                                               0.0f, 1.0f, 0.0f, 0.0f,      // Y scale = 1.0
                                               0.0f, 0.0f, 1.0f, 0.0f,      // Z scale = 1.0
                                               translateX, 0.0f, 0.0f, 1.0f // Shift left/right
                                               ) };
    }

    {
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

        sp58 = (matrix3[0][3] * sp5C[0] + matrix3[1][3] * sp5C[1] + matrix3[2][3] * sp5C[2]) + matrix3[3][3];
        mtxf_translate_vec3f_mat4(sp5C, matrix3);

        temp_f0 = 1.0f / sp58;
        sp5C[0] *= temp_f0 * 160.0f;
        sp5C[1] *= temp_f0 * 120.0f;

        temp_t5 = 120 - (s16) sp5C[1];
        arg1->cameraHeight = temp_t5;

        vtx[1].v.ob[1] = vtx[2].v.ob[1] = vtx[4].v.ob[1] = vtx[7].v.ob[1] = temp_t5;

        init_rdp();
        gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
        gSPClearGeometryMode(gDisplayListHead++, G_ZBUFFER | G_LIGHTING);

        // gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE,
        //               0,   // left
        //               0,  // top
        //               SCREEN_WIDTH, // right 320
        //               SCREEN_HEIGHT  // bottom 240
        //);

        gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0 + margin, 0 + margin, SCREEN_WIDTH - margin,
                      SCREEN_HEIGHT - margin);
        guOrtho(&gGfxPool->mtxScreen, 0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT, 0.0f, 5.0f, 1.0f);
        gSPPerspNormalize(gDisplayListHead++, 0xFFFF);

        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxScreen),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&D_0D008E98), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        // Top half
        gSPVertex(gDisplayListHead++, &vtx[0], 4, 0);
        gSP2Triangles(gDisplayListHead++, 0, 3, 1, 0, 1, 3, 2, 0);

        // Bottom half
        gSPVertex(gDisplayListHead++, &vtx[4], 4, 0);
        gSP2Triangles(gDisplayListHead++, 0, 3, 1, 0, 1, 3, 2, 0);
    }
}
#endif

#include "patches.h"

// clouds
#if 1
RECOMP_PATCH void func_800788F8(s32 objectIndex, u16 rot, Camera* camera) {
    s16 cameraRot;
    // Adjustable culling factor
    const float cullingFactor = recomp_get_aspect_ratio(0.0f);

    // Calculate object's rotation relative to the camera
    cameraRot = camera->rot[1] + rot;

    // Adjust bounds based on the culling factor
    s16 adjustedLowerBound = (s16) (D_8018D210 * cullingFactor);
    s16 adjustedUpperBound = (s16) (D_8018D208 * cullingFactor);

    // Check if the object is within the adjusted bounds
    if ((cameraRot >= adjustedLowerBound) && (adjustedUpperBound >= cameraRot)) {
        // Calculate and update the object's position
        gObjectList[objectIndex].unk_09C = (D_8018D218 + (D_8018D1E8 * cameraRot));

        // Mark the object as visible
        set_object_flag_status_true(objectIndex, 0x10);
    } else {
        // If outside the bounds, mark the object as not visible
        set_object_flag_status_false(objectIndex, 0x10);
    }
}
#endif

