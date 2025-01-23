#include "patches.h"
/*
gEXEnable(gDisplayListHead++); // @recomp
    gEXSetRefreshRate(gDisplayListHead++, 60 / 2);
*/
RECOMP_PATCH void config_gfx_pool(void) {
    gGfxPool = &gGfxPools[gGlobalTimer & 1];
    set_segment_base_addr(1, gGfxPool);
    gDisplayListHead = gGfxPool->gfxPool;
    gEXEnable(gDisplayListHead++); // @recomp
    gEXSetRefreshRate(gDisplayListHead++, 60 / 2);
    gGfxSPTask = &gGfxPool->spTask;
}