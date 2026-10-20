#ifndef __RECOMP_FUNCS_H__
#define __RECOMP_FUNCS_H__

#include "patch_helpers.h"

DECLARE_FUNC(void, recomp_load_overlays, u32 rom, void* ram, u32 size);
DECLARE_FUNC(void, recomp_puts, const char* data, u32 size);
DECLARE_FUNC(void, recomp_load_overlays, u32 rom, void* ram, u32 size);
DECLARE_FUNC(s32, osPiStartDma_recomp, OSIoMesg* mb, s32 priority, s32 direction, uintptr_t devAddr, void* vAddr, size_t nbytes,
                 OSMesgQueue* mq);
DECLARE_FUNC(float, recomp_get_aspect_ratio, float);
DECLARE_FUNC(s32, recomp_get_target_framerate, s32);
DECLARE_FUNC(float, recomp_get_resolution_scale);
DECLARE_FUNC(void, recomp_sleep_miliseconds, s32);
DECLARE_FUNC(void, recomp_set_singleplayer_mode, void);
DECLARE_FUNC(void, recomp_set_multiplayer_min_max_two_players, void);
DECLARE_FUNC(void, recomp_set_multiplayer_min_max_three_players, void);
DECLARE_FUNC(void, recomp_set_multiplayer_min_max_four_players, void);

#endif
