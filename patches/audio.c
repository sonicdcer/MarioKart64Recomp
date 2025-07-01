#include "patches.h"
#include "sound.h"

void func_800C36C4(u8 arg0, u8 arg1, u8 arg2, u8 arg3);
void func_800C8F44(u8);

#if 1
RECOMP_PATCH void func_800029B0(void) {
    switch (D_800DC5A8) {
        case 0:
            // func_800C8F44(127);
            recomp_set_bgm_volume_100();
            break;
        case 1:
            // func_800C8F44(75);
            recomp_set_bgm_volume_59();
            break;
        case 2:
            // func_800C8F44(0);
            recomp_set_bgm_volume_0();
            break;
    }
}
#endif

void audio_set_volume(void) {
    func_800C36C4(SEQ_PLAYER_LEVEL, 0, recomp_get_bgm_volume() * 127, 1);
    func_800C36C4(SEQ_PLAYER_ENV, 0, recomp_get_env_volume() * 127, 1);
    func_800C36C4(SEQ_PLAYER_SFX, 0, recomp_get_sfx_volume() * 127, 1);
}
