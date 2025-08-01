#ifndef __ZELDA_SOUND_H__
#define __ZELDA_SOUND_H__

namespace zelda64 {
    void reset_sound_settings();
    void set_main_volume(int volume);
    int get_main_volume();
    void set_bgm_volume(int volume);
    void set_sfx_volume(int volume);
    void set_env_volume(int volume);
    int get_bgm_volume();
    int get_sfx_volume();
    int get_env_volume();
    void set_low_health_beeps_enabled(bool enabled);
    bool get_low_health_beeps_enabled();
}

#endif
