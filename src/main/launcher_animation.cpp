#include "zelda_launcher.h"
#include "elements/ui_image.h"
#include "elements/ui_label.h"
#include "util/file.h"
#include <atomic>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <vector>

#include "../../lib/rt64/src/contrib/stb/stb_image.h"

namespace {
    constexpr const char* splash_asset_name = "splash_by_dingitydingus.jpg";
    constexpr const char* splash_src = "?/mk64_launcher/splash";

    std::vector<char> read_asset_bytes(const char* asset_name) {
        std::filesystem::path path = recompui::file::get_asset_path(asset_name);
        std::ifstream file{ path, std::ios::binary };
        if (!file) {
            return {};
        }

        return { std::istreambuf_iterator<char>{ file }, std::istreambuf_iterator<char>{} };
    }

    bool queue_splash_image() {
        std::vector<char> png_bytes = read_asset_bytes(splash_asset_name);
        if (png_bytes.empty()) {
            return false;
        }

        int width = 0;
        int height = 0;
        stbi_uc* rgba = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(png_bytes.data()),
                                              static_cast<int>(png_bytes.size()), &width, &height, nullptr, 4);

        bool queued = false;
        if (rgba != nullptr && width > 0 && height > 0) {
            std::vector<char> rgba_bytes{ reinterpret_cast<char*>(rgba),
                                          reinterpret_cast<char*>(rgba) + (width * height * 4) };
            recompui::release_image(splash_src);
            recompui::queue_image_from_bytes_rgba32(splash_src, rgba_bytes, width, height);
            queued = true;
        }

        if (rgba != nullptr) {
            stbi_image_free(rgba);
        }

        return queued;
    }

    recompui::Image *splash_image = nullptr;
    recompui::Label* title_label = nullptr;
    recompui::Label* subtitle_label = nullptr;
}

void zelda64::launcher_animation_setup(recompui::LauncherMenu *menu) {
    auto context = recompui::get_current_context();
    recompui::Element *background_container = menu->get_background_container();
    background_container->set_background_color({ 0x46, 0x46, 0x46, 0xFF });

    if (queue_splash_image()) {
        splash_image = context.create_element<recompui::Image>(background_container, splash_src);
        splash_image->set_position(recompui::Position::Absolute);
        splash_image->set_top(0);
        splash_image->set_left(0);
        splash_image->set_height(100.0f, recompui::Unit::Percent);
        splash_image->set_width_auto();
    }

    menu->remove_default_title();

    title_label = context.create_element<recompui::Label>(background_container, "MARIO KART 64",
                                                                           recompui::theme::Typography::Header1);
    title_label->set_position(recompui::Position::Absolute);
    title_label->set_top(200.0f);
    title_label->set_right(120.0f);
    title_label->set_font_family("Mayro Kart Retro");
    title_label->set_font_size(80.0f);
    title_label->set_color({ 0xFF, 0xFF, 0xFF, 0xFF });


    subtitle_label = context.create_element<recompui::Label>(background_container, "RECOMPILED",
                                                                           recompui::theme::Typography::Header2);
    subtitle_label->set_position(recompui::Position::Absolute);
    subtitle_label->set_top(320.0f);
    subtitle_label->set_right(60.0f);
    subtitle_label->set_font_family("Mayro Kart Retro");
    subtitle_label->set_font_size(72.0f);
    subtitle_label->set_color({ 0xFF, 0xFF, 0xFF, 0xFF });
}

enum class InterpolationMethod {
    Linear,
    Smootherstep
};

namespace {
    bool launcher_started = false;
    float launcher_seconds = 0.0f;
    std::chrono::steady_clock::time_point launcher_last_update_time;
    bool launcher_skip_animation_next = false;
    float launcher_animation_skip_time = 0.0f;

    float interpolate_value(float a, float b, float t, InterpolationMethod method) {
        switch (method) {
            case InterpolationMethod::Smootherstep:
                return a + (b - a) * (t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f));
            case InterpolationMethod::Linear:
            default:
                return a + (b - a) * t;
        }
    }

}

void zelda64::launcher_animation_update(recompui::LauncherMenu *menu) {
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    float delta_time = launcher_started ? std::chrono::duration_cast<std::chrono::milliseconds>(now - launcher_last_update_time).count() / 1000.0f : 0.0f;
    if (launcher_skip_animation_next) {
        delta_time = std::max(launcher_animation_skip_time - launcher_seconds, 0.0f);
        launcher_skip_animation_next = false;
    }

    launcher_seconds += delta_time;
    launcher_last_update_time = now;
    launcher_started = true;

    constexpr float flyin_distance = 1200.0f;
    constexpr float text_time_offset = 1.0f;
    constexpr float general_speed = 3.0f;
    splash_image->set_left(-flyin_distance + interpolate_value(0.0f, flyin_distance, std::max(0.0f, std::min(launcher_seconds * general_speed, 1.0f)), InterpolationMethod::Smootherstep));
    splash_image->set_opacity(interpolate_value(0.0f, 1.0f, std::max(0.0f, std::min(launcher_seconds * general_speed, 1.0f)), InterpolationMethod::Smootherstep));


    title_label->set_right(120.0f - flyin_distance + interpolate_value(0.0f, flyin_distance, std::max(0.0f, std::min(launcher_seconds * general_speed - text_time_offset, 1.0f)), InterpolationMethod::Smootherstep));
    subtitle_label->set_right(220.0f - flyin_distance + interpolate_value(0.0f, flyin_distance, std::max(0.0f, std::min(launcher_seconds * general_speed- text_time_offset, 1.0f)), InterpolationMethod::Smootherstep));

    title_label->set_opacity(interpolate_value(0.0f, 1.0f, std::max(0.0f, std::min(launcher_seconds * general_speed - text_time_offset, 1.0f)), InterpolationMethod::Smootherstep));
    subtitle_label->set_opacity(interpolate_value(0.0f, 1.0f, std::max(0.0f, std::min(launcher_seconds * general_speed - text_time_offset, 1.0f)), InterpolationMethod::Smootherstep));
}