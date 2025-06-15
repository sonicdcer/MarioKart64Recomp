#include "patches.h"

// Fixes a bug with the recompiler.
int dummy_val = 1;

RECOMP_PATCH void func_8028FC34(void) {
    if (D_802BA038 < 0) {
        D_802BA038 = 1920;
        return;
    }
    D_802BA038--;

// @recomp: Change gControllerFive to gControllers[0] to avoid bug where something
// is pressing other controllers in memory.

// if (gControllerFive->buttonPressed != 0) {
    if (gControllers[0].buttonPressed != 0) {
        func_8028FBD4();
        gMenuSelection = START_MENU;
        return;
    }
    if (D_802BA038 == 0) {
        func_8028FBD4();
        gMenuSelection = LOGO_INTRO_MENU;
    }
}

#if 0
RECOMP_PATCH void read_controllers(void) {
    OSMesg msg;

    // Start reading controller data
    osContStartReadData(&gSIEventMesgQueue);
    osRecvMesg(&gSIEventMesgQueue, &msg, OS_MESG_BLOCK);

    // Retrieve the controller data
    osContGetReadData(gControllerPads);

    // Update each controller individually
    update_controller(0);
    update_controller(1);
    update_controller(2);
    update_controller(3);

    // Optionally, clear or reset gControllerFive if it's no longer needed
    if (gControllerFive) {
        gControllerFive->button = 0;
        gControllerFive->buttonPressed = 0;
        gControllerFive->buttonDepressed = 0;
        gControllerFive->stickDirection = 0;
        gControllerFive->stickPressed = 0;
        gControllerFive->stickDepressed = 0;
    }
}
#endif