#include "patches.h"

// Fixes a bug with the recompiler.
int dummy_val = 1;

// @recomp: Change gControllerFive to gControllers[0] to avoid bug where something
RECOMP_PATCH void func_8028FC34(void) {
    if (D_802BA038 < 0) {
        D_802BA038 = 1920;
        return;
    }
    D_802BA038--;

    // is pressing other controllers in memory.
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