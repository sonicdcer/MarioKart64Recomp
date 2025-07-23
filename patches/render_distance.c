#include "patches.h"

#if 1 // Draw Distance Improved@tb181
RECOMP_PATCH f32 is_within_render_distance(Vec3f cameraPos, Vec3f objectPos, u16 orientationY, f32 minDistance, f32 fov,
                                           f32 maxDistance) {
    u16 angleObject;
    u16 temp_v0;
    f32 distanceX;
    f32 distance;
    f32 distanceY;
    f32 scaleFov;
    s32 plus_fov_angle;
    s32 minus_fov_angle;
    u16 temp;
    s32 count = 0;

    maxDistance *= 6.5f;
    scaleFov = 1.25;

    f32 extended_fov = ((f32) fov * 0xB6 * scaleFov); // Sets the Culling for objects on the left and right


    distanceX = objectPos[0] - cameraPos[0];
    distanceX = distanceX * distanceX;
    if (maxDistance < distanceX) {
        return -1.0f;
    }

    distanceY = objectPos[2] - cameraPos[2];
    distanceY = distanceY * distanceY;
    if (maxDistance < distanceY) {
        return -1.0f;
    }

    distance = distanceX + distanceY;
    if (distance < minDistance) {
        return distance;
    }

    if (distance > maxDistance) {
        return -1.0f;
    }

    angleObject = get_angle_between_two_vectors(cameraPos, objectPos);
    minus_fov_angle = (orientationY - extended_fov);
    plus_fov_angle = (orientationY + extended_fov);

    if (minDistance == 0.0f) {
        if (is_visible_between_angle((orientationY + extended_fov), (orientationY - extended_fov), angleObject) == 1) {
            if (0){//if (gCurrentCourseId == COURSE_KALAMARI_DESERT) {
                return distance / 6.5f; // set for better DD settings in Desert
            } else {
                return distance / 10.0f; // Items
            }
        }
        return -1.0f;
    }

    if (is_visible_between_angle((u16) plus_fov_angle, (u16) minus_fov_angle, angleObject) == 1) {
        if (0){//if (gCurrentCourseId == COURSE_KALAMARI_DESERT) {
            return distance / 2.0f;
        } else {
            return distance / 10.0f; // DD Vhicles
        }
    }

    temp_v0 = func_802B7CA8(minDistance / distance);
    temp = angleObject + temp_v0;

    if (is_visible_between_angle(plus_fov_angle, minus_fov_angle, temp) == 1) {
        return 0;
    }

    temp = angleObject - temp_v0;
    if (is_visible_between_angle(plus_fov_angle, minus_fov_angle, temp) == 1) {
        return distance;
    }
    return -1.0f;
}
#endif

#if 1 // Increase DD for effects, snowmen, ect... @tb181
RECOMP_PATCH u32 get_horizontal_distance_to_camera(s32 objectIndex, Camera* camera) {
    s32 x;
    s32 y;

    x = gObjectList[objectIndex].pos[0] - camera->pos[0];
    y = gObjectList[objectIndex].pos[2] - camera->pos[2];
    return (x * x) + (y * y) / 5; // Increase DD
}
#endif

// Kart Draw Distance
#if 1
RECOMP_PATCH u16 check_player_camera_collision(Player* player, Camera* camera, f32 arg2, f32 arg3) {
    UNUSED f32 pad[6];
    f32 sp64;
    f32 sp60;
    f32 sp5C;
    f32 sp58;
    f32 sp54;
    f32 sp50;
    f32 sp4C;
    f32 sp48;
    f32 sp44;
    s16 var_v0;
    u16 ret;

    // Define a multiplier to scale the draw distance
    const f32 distanceMultiplier = 3.0f; // Adjust this value as needed

    // Apply the multiplier to arg2 and arg3
    arg2 *= distanceMultiplier;
    arg3 *= distanceMultiplier / 5;

    ret = false;
    switch (gActiveScreenMode) {
        case SCREEN_MODE_1P:
            var_v0 = 0x293C;
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            var_v0 = 0x3058;
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            var_v0 = 0x1FFE;
            break;
        default:
            var_v0 = 0x1FFE;
            break;
    }
    sp4C = (arg2 * coss((camera->rot[1] - var_v0))) + camera->pos[2];
    sp58 = (arg2 * sins((camera->rot[1] - var_v0))) + camera->pos[0];
    sp48 = (arg2 * coss((camera->rot[1] + var_v0))) + camera->pos[2];
    sp54 = (arg2 * sins((camera->rot[1] + var_v0))) + camera->pos[0];
    sp44 = (arg3 * coss((camera->rot[1] + 0x5FFA))) + camera->pos[2];
    sp50 = (arg3 * sins((camera->rot[1] + 0x5FFA))) + camera->pos[0];

    sp64 = ((sp4C - player->pos[2]) * (sp54 - player->pos[0])) - ((sp48 - player->pos[2]) * (sp58 - player->pos[0]));
    sp60 = ((sp48 - player->pos[2]) * (sp50 - player->pos[0])) - ((sp44 - player->pos[2]) * (sp54 - player->pos[0]));
    sp5C = ((sp44 - player->pos[2]) * (sp58 - player->pos[0])) - ((sp4C - player->pos[2]) * (sp50 - player->pos[0]));

    if (((sp64 >= 0) && (sp60 >= 0) && (sp5C >= 0)) || (((sp64) <= 0) && (sp60 <= 0) && (sp5C <= 0))) {
        ret = true;
    }
    return ret;
}
#endif

// Fix Culling for gObjectList items
#if 1
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