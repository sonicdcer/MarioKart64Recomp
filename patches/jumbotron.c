#include "patches.h"

#if 1

s16 currentScreenSection = 0;

RECOMP_PATCH void render_luigi_raceway(struct UnkStruct_800DC5EC* arg0) {
    u16 sp22 = (u16) arg0->pathCounter;
    s16 prevFrame;

    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    if (func_80290C20(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_luigi_raceway_packed_dl_9EC0
        gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07009EC0));
    }

    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);

    render_course_segments((uintptr_t) luigi_raceway_dls, arg0);

    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    // d_course_luigi_raceway_packed_dl_E0
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x070000E0));
    // d_course_luigi_raceway_packed_dl_68
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07000068));

    D_800DC5DC = 88;
    D_800DC5E0 = 72;

    // Render only the first player camera onto the television billboard. Screen agnostic screens of other players).
    if ((gActiveScreenMode == SCREEN_MODE_1P) && (sp22 >= 10) && (sp22 < 17)) {

        prevFrame = (s16) sRenderedFramebuffer - 1;

        if (prevFrame < 0) {
            prevFrame = 2;
        } else if (prevFrame >= 3) {
            prevFrame = 0;
        }

        // Loop through all six sections and render them in the same frame
        for (currentScreenSection = 0; currentScreenSection < 6; currentScreenSection++) {
            switch (currentScreenSection) {
                case 0:
                    copy_framebuffer(D_800DC5DC, D_800DC5E0, 64, 32,
                                     (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[prevFrame]),
                                     (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0xF800));
                    break;
                case 1:
                    copy_framebuffer(D_800DC5DC + 64, D_800DC5E0, 64, 32,
                                     (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[prevFrame]),
                                     (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0x10800));
                    break;
                case 2:
                    copy_framebuffer(D_800DC5DC, D_800DC5E0 + 32, 64, 32,
                                     (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[prevFrame]),
                                     (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0x11800));
                    break;
                case 3:
                    copy_framebuffer(D_800DC5DC + 64, D_800DC5E0 + 32, 64, 32,
                                     (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[prevFrame]),
                                     (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0x12800));
                    break;
                case 4:
                    copy_framebuffer(D_800DC5DC, D_800DC5E0 + 64, 64, 32,
                                     (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[prevFrame]),
                                     (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0x13800));
                    break;
                case 5:
                    copy_framebuffer(D_800DC5DC + 64, D_800DC5E0 + 64, 64, 32,
                                     (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[prevFrame]),
                                     (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0x14800));
                    break;
            }
        }
    }
}

#endif

#if 1
RECOMP_PATCH void render_wario_stadium(struct UnkStruct_800DC5EC* arg0) {
    s16 prevFrame;

    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    if (func_80290C20(arg0->camera) == 1) {

        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_wario_stadium_packed_dl_A0C8
        gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x0700A0C8));
    }
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATERGBA, G_CC_MODULATERGBA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);

    render_course_segments((uintptr_t) wario_stadium_dls, arg0);

    // d_course_wario_stadium_packed_dl_A228
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x0700A228));
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    // d_course_wario_stadium_packed_dl_A88
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07000A88));
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);

    D_800DC5DC = 88;
    D_800DC5E0 = 72;
    if (gActiveScreenMode == SCREEN_MODE_1P) {
        prevFrame = (s16) sRenderedFramebuffer - 1;
        if (prevFrame < 0) {
            prevFrame = 2;
        } else if (prevFrame >= 3) {
            prevFrame = 0;
        }

        // Loop through all six sections and render them in the same frame
        for (currentScreenSection = 0; currentScreenSection < 6; currentScreenSection++) {
            switch (currentScreenSection) {
                case 0:
                    copy_framebuffer(D_800DC5DC, D_800DC5E0, 64, 32,
                                     (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[prevFrame]),
                                     (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0x8800));
                    break;
                case 1:
                    copy_framebuffer(D_800DC5DC + 64, D_800DC5E0, 64, 32,
                                     (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[prevFrame]),
                                     (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0x9800));
                    break;
                case 2:
                    copy_framebuffer(D_800DC5DC, D_800DC5E0 + 32, 64, 32,
                                     (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[prevFrame]),
                                     (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0xA800));
                    break;
                case 3:
                    copy_framebuffer(D_800DC5DC + 64, D_800DC5E0 + 32, 64, 32,
                                     (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[prevFrame]),
                                     (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0xB800));
                    break;
                case 4:
                    copy_framebuffer(D_800DC5DC, D_800DC5E0 + 64, 64, 32,
                                     (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[prevFrame]),
                                     (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0xC800));
                    break;
                case 5:
                    copy_framebuffer(D_800DC5DC + 64, D_800DC5E0 + 64, 64, 32,
                                     (u16*) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[prevFrame]),
                                     (u16*) PHYSICAL_TO_VIRTUAL(gSegmentTable[5] + 0xD800));
                    break;
            }
        }
    }
}
#endif