#include "patches.h"

// Fix Culling for gObjectList items
#if 1

s32 get_angle_between_xy(f32 x1, f32 x2, f32 y1, f32 y2);

RECOMP_PATCH bool is_object_visible_on_camera(s32 objectIndex, Camera* camera, u16 angle) {
    u16 temp_t2;
    s32 var_t0;

    var_t0 = false;

    // Update angle dynamically if needed
    angle = 0x6000; // New angle for wider FOV, equivalent to ~120°

    temp_t2 = (get_angle_between_xy(camera->pos[0], gObjectList[objectIndex].pos[0], camera->pos[2],
                                    gObjectList[objectIndex].pos[2]) +
               ((s32) angle / 2)) -
              camera->rot[1];
    if ((temp_t2 >= 0) && (angle >= temp_t2)) {
        var_t0 = true;
    }
    return var_t0;
}


#endif