#include "patches.h"

#if 0
RECOMP_PATCH void func_802A3730(struct UnkStruct_800DC5EC* arg0) {
    s32 ulx;
    s32 uly;
    s32 lrx;
    s32 lry;
    s32 screenWidth = arg0->screenWidth * 2;
    s32 screenHeight = arg0->screenHeight * 2;
    s32 screenStartX = arg0->screenStartX * 4;
    s32 screenStartY = arg0->screenStartY * 4;

    arg0->viewport.vp.vscale[0] = screenWidth;
    arg0->viewport.vp.vscale[1] = screenHeight;
    arg0->viewport.vp.vscale[2] = 511;
    arg0->viewport.vp.vscale[3] = 0;

    arg0->viewport.vp.vtrans[0] = screenStartX;
    arg0->viewport.vp.vtrans[1] = screenStartY;
    arg0->viewport.vp.vtrans[2] = 511;
    arg0->viewport.vp.vtrans[3] = 0;

    gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&arg0->viewport));

    screenWidth /= 4;
    screenHeight /= 4;

    screenStartX /= 4;
    screenStartY /= 4;

    lrx = screenStartX + screenWidth;
    if (lrx > SCREEN_WIDTH) {
        lrx = SCREEN_WIDTH;
    }

    lry = screenStartY + screenHeight;
    if (lry > SCREEN_HEIGHT) {
        lry = SCREEN_HEIGHT;
    }
    ulx = 0;
    uly = 0;

    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, ulx, uly, lrx, lry);
}
#endif

#if 1 // Fix Debug on issue ( need to figure out actaul issue. 
void func_800CA330(u8 arg0);

RECOMP_PATCH void func_8028F4E8(void) {
    if (gEnableDebugMode) {
        if (((gControllerOne->button & R_TRIG) != 0) && ((gControllerOne->button & L_TRIG) != 0) &&
            ((gControllerOne->button & A_BUTTON) != 0) && ((gControllerOne->button & B_BUTTON) != 0)) {
    
            func_800CA330(0x19);
            func_800CA388(0x19);
            gGotoMode = MAIN_MENU_FROM_QUIT;
            D_800DC510 = 6;
            D_800DC5B4 = 1;
            D_800DC5B0 = 1;
            D_800DC5B8 = 0;
            D_802BA038 = 5;
        }
    }
}
#endif 