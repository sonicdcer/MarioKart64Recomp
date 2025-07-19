#include "patches.h"
#include "sound.h"

void testPak(void);
void audio_set_volume(void);
s32 osPfsFindFile_recomp(OSPfs* pfs, u16 company_code, u32 game_code, u8* game_name, u8* ext_name, s32* file_no);
RECOMP_PATCH void config_gfx_pool(void) {
    gGfxPool = &gGfxPools[gGlobalTimer & 1];
    set_segment_base_addr(1, gGfxPool);
    gDisplayListHead = gGfxPool->gfxPool;
    gEXEnable(gDisplayListHead++); // @recomp
    gEXSetRDRAMExtended(gDisplayListHead++, 1); // @recomp
    gEXSetRefreshRate(gDisplayListHead++, 60 / 2);
    gGfxSPTask = &gGfxPool->spTask;
    gEXSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, 0, 0, 0,
                  SCREEN_HEIGHT); // @recomp

    // TODO: move this to a more appropriate place
    audio_set_volume();
    // osPfsFindFile_recomp((OSPfs*)0, 1,2,(u8*)3, (u8*)4, (s32*)5);

      static int once = 0;
    if (!once) {
        once = 1;
        //testPak();
    }
}


