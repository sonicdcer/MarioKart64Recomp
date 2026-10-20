#include <cmath>

#include "recomp.h"
#include "librecomp/overlays.hpp"
#include "zelda_config.h"
#include "recompinput/recompinput.h"
#include "recompui/recompui.h"
#include "recompui/renderer.h"
#include "zelda_sound.h"
#include "librecomp/helpers.hpp"
// #include "../patches/input.h"
// #include "../patches/graphics.h"
// #include "../patches/sound.h"
#include "ultramodern/ultramodern.hpp"
#include "ultramodern/config.hpp"

extern "C" void recomp_update_inputs(uint8_t* rdram, recomp_context* ctx) {
    recompinput::poll_inputs();
}

extern "C" void recomp_sleep_miliseconds(uint8_t* rdram, recomp_context* ctx) {
    int time = _arg<0, u32>(rdram, ctx);
    ultramodern::sleep_milliseconds(time);
}


extern "C" void rmonPrintf_recomp(uint8_t* rdram, recomp_context* ctx) {
    // Empty
}

extern "C" void __ll_rshift_recomp(uint8_t * rdram, recomp_context * ctx) {
    int64_t a = (ctx->r4 << 32) | ((ctx->r5 << 0) & 0xFFFFFFFFu);
    int64_t b = (ctx->r6 << 32) | ((ctx->r7 << 0) & 0xFFFFFFFFu);
    int64_t ret = a >> b;

    ctx->r2 = (int32_t)(ret >> 32);
    ctx->r3 = (int32_t)(ret >> 0);
}

extern "C" void recomp_puts(uint8_t* rdram, recomp_context* ctx) {
    PTR(char) cur_str = _arg<0, PTR(char)>(rdram, ctx);
    u32 length = _arg<1, u32>(rdram, ctx);

    for (u32 i = 0; i < length; i++) {
        fputc(MEM_B(i, (gpr)cur_str), stdout);
    }
}

extern "C" void recomp_exit(uint8_t* rdram, recomp_context* ctx) {
    ultramodern::quit();
}

extern "C" void recomp_get_gyro_deltas(uint8_t* rdram, recomp_context* ctx) {
    float* x_out = _arg<0, float*>(rdram, ctx);
    float* y_out = _arg<1, float*>(rdram, ctx);

    // TODO: use controller number
    recompinput::get_gyro_deltas(0, x_out, y_out);
}

extern "C" void recomp_get_mouse_deltas(uint8_t* rdram, recomp_context* ctx) {
    float* x_out = _arg<0, float*>(rdram, ctx);
    float* y_out = _arg<1, float*>(rdram, ctx);

    recompinput::get_mouse_deltas(x_out, y_out);
}

extern "C" void recomp_powf(uint8_t* rdram, recomp_context* ctx) {
    float a = _arg<0, float>(rdram, ctx);
    float b = ctx->f14.fl; //_arg<1, float>(rdram, ctx);

    _return(ctx, std::pow(a, b));
}

extern "C" void recomp_get_target_framerate(uint8_t* rdram, recomp_context* ctx) {
    int frame_divisor = _arg<0, u32>(rdram, ctx);

    _return(ctx, ultramodern::get_target_framerate(60 / frame_divisor));
}

extern "C" void recomp_get_window_resolution(uint8_t* rdram, recomp_context* ctx) {
    int width, height;
    recompui::get_window_size(width, height);

    gpr width_out = _arg<0, PTR(u32)>(rdram, ctx);
    gpr height_out = _arg<1, PTR(u32)>(rdram, ctx);

    MEM_W(0, width_out) = (u32) width;
    MEM_W(0, height_out) = (u32) height;
}

extern "C" void recomp_set_multiplayer_min_max_two_players(uint8_t* rdram, recomp_context* ctx) {
    recompinput::players::set_player_count_range(2, 2);
    recompinput::players::set_single_player_mode(false);
}

extern "C" void recomp_set_multiplayer_min_max_three_players(uint8_t* rdram, recomp_context* ctx) {
    recompinput::players::set_player_count_range(3, 3);
    recompinput::players::set_single_player_mode(false);
}

extern "C" void recomp_set_multiplayer_min_max_four_players(uint8_t* rdram, recomp_context* ctx) {
    recompinput::players::set_player_count_range(4, 4);
    recompinput::players::set_single_player_mode(false);
}

extern "C" void recomp_set_singleplayer_mode(uint8_t* rdram, recomp_context* ctx) {
    recompinput::players::set_single_player_mode(true);
}

extern "C" void recomp_get_aspect_ratio(uint8_t* rdram, recomp_context* ctx) {
    ultramodern::renderer::GraphicsConfig graphics_config = ultramodern::renderer::get_graphics_config();
    float original = _arg<0, float>(rdram, ctx);
    int width, height;
    recompui::get_window_size(width, height);

    switch (graphics_config.ar_option) {
        case ultramodern::renderer::AspectRatio::Original:
        default:
            _return(ctx, original);
            return;
        case ultramodern::renderer::AspectRatio::Expand:
            _return(ctx, std::max(static_cast<float>(width) / height, original));
            return;
    }
}

#if 0
extern "C" void recomp_set_bgm_volume_100(uint8_t* rdram, recomp_context* ctx) {
    zelda64::set_bgm_volume(100);
}

extern "C" void recomp_set_bgm_volume_59(uint8_t* rdram, recomp_context* ctx) {
    zelda64::set_bgm_volume(59);
}

extern "C" void recomp_set_bgm_volume_0(uint8_t* rdram, recomp_context* ctx) {
    zelda64::set_bgm_volume(0);
}
#endif

extern "C" void recomp_get_bgm_volume(uint8_t* rdram, recomp_context* ctx) {
    _return(ctx, zelda64::get_bgm_volume() / 100.0f);
}

extern "C" void recomp_get_sfx_volume(uint8_t* rdram, recomp_context* ctx) {
    _return(ctx, zelda64::get_sfx_volume() / 100.0f);
}

extern "C" void recomp_get_fanfare_volume(uint8_t* rdram, recomp_context* ctx) {
    _return(ctx, zelda64::get_fanfare_volume() / 100.0f);
}

extern "C" void recomp_time_us(uint8_t* rdram, recomp_context* ctx) {
    _return(ctx, static_cast<u32>(std::chrono::duration_cast<std::chrono::microseconds>(ultramodern::time_since_start()).count()));
}

extern "C" void recomp_load_overlays(uint8_t * rdram, recomp_context * ctx) {
    u32 rom = _arg<0, u32>(rdram, ctx);
    PTR(void) ram = _arg<1, PTR(void)>(rdram, ctx);
    u32 size = _arg<2, u32>(rdram, ctx);

    load_overlays(rom, ram, size);
}

#if 0
extern "C" void recomp_high_precision_fb_enabled(uint8_t * rdram, recomp_context * ctx) {
    _return(ctx, static_cast<s32>(recompui::renderer::RT64HighPrecisionFBEnabled()));
}
#endif

extern "C" void recomp_get_resolution_scale(uint8_t* rdram, recomp_context* ctx) {
    _return(ctx, ultramodern::get_resolution_scale());
}
