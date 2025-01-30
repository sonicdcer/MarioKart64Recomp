#ifndef __RECOMP_FUNCS_H__
#define __RECOMP_FUNCS_H__

#include "patch_helpers.h"

DECLARE_FUNC(void, recomp_load_overlays, u32 rom, void* ram, u32 size);
DECLARE_FUNC(void, recomp_puts, const char* data, u32 size);
DECLARE_FUNC(void, recomp_exit);
DECLARE_FUNC(void, recomp_handle_quicksave_actions, OSMesgQueue* enter_mq, OSMesgQueue* exit_mq);
DECLARE_FUNC(void, recomp_handle_quicksave_actions_main, OSMesgQueue* enter_mq, OSMesgQueue* exit_mq);
DECLARE_FUNC(u16, recomp_get_pending_warp);
DECLARE_FUNC(u32, recomp_get_pending_set_time);
DECLARE_FUNC(s32, recomp_autosave_enabled);
DECLARE_FUNC(void, recomp_load_overlays, u32 rom, void* ram, u32 size);
DECLARE_FUNC(s32, osPiStartDma_recomp, OSIoMesg* mb, s32 priority, s32 direction, uintptr_t devAddr, void* vAddr, size_t nbytes,
                 OSMesgQueue* mq);
DECLARE_FUNC(float, recomp_get_aspect_ratio, float);
DECLARE_FUNC(s32, recomp_get_target_framerate, s32);
DECLARE_FUNC(s32, recomp_high_precision_fb_enabled);
DECLARE_FUNC(float, recomp_get_resolution_scale);
DECLARE_FUNC(void, recomp_sleep_miliseconds, s32);

#endif
