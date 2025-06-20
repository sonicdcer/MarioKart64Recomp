#include "patches.h"

RECOMP_PATCH void config_gfx_pool(void) {
    gGfxPool = &gGfxPools[gGlobalTimer & 1];
    set_segment_base_addr(1, gGfxPool);
    gDisplayListHead = gGfxPool->gfxPool;
    gEXEnable(gDisplayListHead++); // @recomp
    gEXSetRefreshRate(gDisplayListHead++, 60 / 2);
    gGfxSPTask = &gGfxPool->spTask;
    gEXSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, 0, 0, 0,
                  SCREEN_HEIGHT); // @recomp
}