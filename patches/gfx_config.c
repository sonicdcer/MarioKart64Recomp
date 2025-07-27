#include "patches.h"
#include "sound.h"

#define GFX_POOL_EXTENDED_SIZE (GFX_POOL_SIZE * 0x10)

Gfx gfxPoolExtended[2][GFX_POOL_EXTENDED_SIZE];

RECOMP_PATCH void config_gfx_pool(void) {
    gGfxPool = &gGfxPools[gGlobalTimer & 1];
    set_segment_base_addr(1, gGfxPool);
    // gDisplayListHead = gGfxPool->gfxPool;
    gDisplayListHead = gfxPoolExtended[gGlobalTimer & 1];
    gEXEnable(gDisplayListHead++);              // @recomp
    gEXSetRDRAMExtended(gDisplayListHead++, 1); // @recomp
    gEXSetRefreshRate(gDisplayListHead++, 60 / 2);
    gGfxSPTask = &gGfxPool->spTask;
    gEXSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, 0, 0, 0,
                  SCREEN_HEIGHT); // @recomp

    // TODO: move this to a more appropriate place
    audio_set_volume();
}

#if 1
/**
 * Initializes the Fast3D OSTask structure.
 * Loads F3DEX or F3DLX based on the number of players
 **/
RECOMP_PATCH void create_gfx_task_structure(void) {
    gGfxSPTask->msgqueue = &gGfxVblankQueue;
    gGfxSPTask->msg = (OSMesg) 2;
    gGfxSPTask->task.t.type = M_GFXTASK;
    gGfxSPTask->task.t.flags = OS_TASK_DP_WAIT;
    gGfxSPTask->task.t.ucode_boot = rspF3DBootStart;
    gGfxSPTask->task.t.ucode_boot_size = ((u8*) rspF3DBootEnd - (u8*) rspF3DBootStart);
    // The split-screen multiplayer racing state uses F3DLX which has a simple subpixel calculation.
    // Singleplayer race mode and all other game states use F3DEX.
    // http://n64devkit.square7.ch/n64man/ucode/gspF3DEX.htm
    if (gGamestate != RACING || gPlayerCountSelection1 == 1) {
        gGfxSPTask->task.t.ucode = gspF3DEXTextStart;
        gGfxSPTask->task.t.ucode_data = gspF3DEXDataStart;
    } else {
        gGfxSPTask->task.t.ucode = gspF3DLXTextStart;
        gGfxSPTask->task.t.ucode_data = gspF3DLXDataStart;
    }
    gGfxSPTask->task.t.flags = 0;
    gGfxSPTask->task.t.flags = OS_TASK_DP_WAIT;
    gGfxSPTask->task.t.ucode_size = SP_UCODE_SIZE;
    gGfxSPTask->task.t.ucode_data_size = SP_UCODE_DATA_SIZE;
    gGfxSPTask->task.t.dram_stack = (u64*) &gGfxSPTaskStack;
    gGfxSPTask->task.t.dram_stack_size = SP_DRAM_STACK_SIZE8;
    gGfxSPTask->task.t.output_buff = (u64*) &gGfxSPTaskOutputBuffer;
    gGfxSPTask->task.t.output_buff_size = (u64*) ((u8*) gGfxSPTaskOutputBuffer + sizeof(gGfxSPTaskOutputBuffer));
    // gGfxSPTask->task.t.data_ptr = (u64*) gGfxPool->gfxPool;
    gGfxSPTask->task.t.data_ptr = (u64*) gfxPoolExtended[gGlobalTimer & 1];
    // gGfxSPTask->task.t.data_size = (gDisplayListHead - gGfxPool->gfxPool) * sizeof(Gfx);
    gGfxSPTask->task.t.data_size = (gDisplayListHead - gfxPoolExtended[gGlobalTimer & 1]) * sizeof(Gfx);
    func_8008C214();
    gGfxSPTask->task.t.yield_data_ptr = (u64*) &gGfxSPTaskYieldBuffer;
    gGfxSPTask->task.t.yield_data_size = OS_YIELD_DATA_SIZE;

    u32 bytesLeft = (GFX_POOL_EXTENDED_SIZE * 8 - gGfxSPTask->task.t.data_size);
    // recomp_printf("bytesLeft: %d\n\n", bytesLeft);
    if (bytesLeft < 0) {
        // CRASH
        recomp_printf("GFXPOOL is full!\n bytesLeft: %x\n\n", bytesLeft);
        *(volatile int*) 0 = 0;
    }
}
#endif

#if 0
RECOMP_PATCH void rendering_init(void) {
    gGfxPool = &gGfxPools[0];
    set_segment_base_addr(1, gGfxPool);
    gGfxSPTask = &gGfxPool->spTask;
    gDisplayListHead = gGfxPool->gfxPool;
    init_rcp();
    clear_framebuffer(0);
    end_master_display_list();
    exec_display_list(&gGfxPool->spTask);
    sRenderingFramebuffer++;
    gGlobalTimer++;
}
#endif

/**
 * Inserts matrix into the rsp. Position, rotation and mode of where to render the next object and check number of
 * object already render Note that gMatrixObjectCount gets reset at the beginning of the game loop. So no cleanup needs
 * to be performed.
 */
#define MTX_OBJECT_POOL_SIZE_EXTENDED (MTX_OBJECT_POOL_SIZE * 10)

Mtx mtxObjectExtended[2][MTX_OBJECT_POOL_SIZE_EXTENDED];

RECOMP_PATCH s32 render_set_position(Mat4 mtx, s32 mode) {
    int frameIndex = (int) (gGfxPool - gGfxPools);

    if (gMatrixObjectCount >= MTX_OBJECT_POOL_SIZE * 10) {
        recomp_printf("gMatrixObjectCount : %d\n", gMatrixObjectCount);
        return 0;
    }

    mtxf_to_mtx(&mtxObjectExtended[frameIndex][gMatrixObjectCount], mtx);
    switch (mode) {
        case 0:
            gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&mtxObjectExtended[frameIndex][gMatrixObjectCount++]),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            break;
        case 1:
            gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&mtxObjectExtended[frameIndex][gMatrixObjectCount++]),
                      G_MTX_PUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            break;
        case 3:
            gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&mtxObjectExtended[frameIndex][gMatrixObjectCount++]),
                      G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            break;
        case 2:
            gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&mtxObjectExtended[frameIndex][gMatrixObjectCount++]),
                      G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            break;
    }
    return 1;
}
