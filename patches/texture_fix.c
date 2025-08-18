#include "patches.h"

extern u8* D_802BA050;
extern u8* D_802BA054;
extern u8* D_802BA058;
extern u8 D_0B002A00[];

extern u8 gTextureGreenShell0[];
extern u8 gTextureGreenShell1[];
extern u8 gTextureGreenShell2[];
extern u8 gTextureGreenShell3[];
extern u8 gTextureGreenShell4[];
extern u8 gTextureGreenShell5[];
extern u8 gTextureGreenShell6[];
extern u8 gTextureGreenShell7[];
extern u8 gTextureBlueShell0[];
extern u8 gTextureBlueShell1[];
extern u8 gTextureBlueShell2[];
extern u8 gTextureBlueShell3[];
extern u8 gTextureBlueShell4[];
extern u8 gTextureBlueShell5[];
extern u8 gTextureBlueShell6[];
extern u8 gTextureBlueShell7[];
extern u8 gTextureFinishLineBanner1[];
extern u8 gTextureFinishLineBanner2[];
extern u8 gTextureFinishLineBanner3[];
extern u8 gTextureFinishLineBanner4[];
extern u8 gTextureFinishLineBanner5[];
extern u8 gTextureFinishLineBanner6[];
extern u8 gTextureFinishLineBanner7[];
extern u8 gTextureFinishLineBanner8[];
extern u8 gTexture671A88[];
extern u8 gTexture6774D8[];
extern u8 gTexturePiranhaPlant1[];
extern u8 gTexturePiranhaPlant2[];
extern u8 gTexturePiranhaPlant3[];
extern u8 gTexturePiranhaPlant4[];
extern u8 gTexturePiranhaPlant5[];
extern u8 gTexturePiranhaPlant6[];
extern u8 gTexturePiranhaPlant7[];
extern u8 gTexturePiranhaPlant8[];
extern u8 gTexturePiranhaPlant9[];
extern u8 gTextureShrub[];
extern u8 gTextureTrees1[];
extern u8 gTextureTrees2[];
extern u8 gTextureTrees3[];
extern u8 gTextureTrees4[];
extern u8 gTextureTrees5[];
extern u8 gTextureTrees6[];
extern u8 gTextureTrees7[];
extern u8 gTextureFrappeSnowlandTreeLeft[];
extern u8 gTextureFrappeSnowlandTreeRight[];
extern u8 gTextureTrees5Left[];
extern u8 gTextureTrees5Right[];
extern u8 gTextureTrees4Right[];
extern u8 gTextureCow01Left[];
extern u8 gTextureCow01Right[];
extern u8 gTextureCow02Left[];
extern u8 gTextureCow02Right[];
extern u8 gTextureCow03Left[];
extern u8 gTextureTrees4Left[];
extern u8 gTextureCow04Left[];

extern u8 gTextureCow03Right[];
extern u8 gTextureCow04Right[];
extern u8 gTextureCow05Left[];
extern u8 gTextureCow05Right[];
extern u8 gTextureCactus1Left[];
extern u8 gTextureCactus1Right[];
extern u8 gTextureCactus2Left[];
extern u8 gTextureCactus2Right[];
extern u8 gTextureCactus2Right[];
extern u8 gTextureCactus3[];
extern u8 gTextureDksJungleParkwayKiwanoFruit1[];
extern u8 gTextureDksJungleParkwayKiwanoFruit2[];
extern u8 gTextureDksJungleParkwayKiwanoFruit3[];

u8* dma_textures(u8*, u32, u32);
void spawn_course_vehicles(void);

#if 1
/**
 * @brief Loads actor textures, course specific actor textures.
 * Calls to spawn_course_vehicles and place_course_actors
 *
 */
RECOMP_PATCH void init_actors_and_load_textures(void) {
    set_segment_base_addr(3, (void*) gNextFreeMemoryAddress);
    // @recomp Fix the size of the compressed green shell assets to fix the infinite
    // hashes that could be generated.
    // D_802BA050 = dma_textures(gTextureGreenShell0, 0x00000257U, 0x00000400U);
    // dma_textures(gTextureGreenShell1, 0x00000242U, 0x00000400U);
    // dma_textures(gTextureGreenShell2, 0x00000259U, 0x00000400U);
    // dma_textures(gTextureGreenShell3, 0x00000256U, 0x00000400U);
    // dma_textures(gTextureGreenShell4, 0x00000246U, 0x00000400U);
    // dma_textures(gTextureGreenShell5, 0x0000025EU, 0x00000400U);
    // dma_textures(gTextureGreenShell6, 0x0000025CU, 0x00000400U);
    D_802BA050 = dma_textures(gTextureGreenShell0, 0x0000024EU, 0x00000400U);
    dma_textures(gTextureGreenShell1, 0x00000250U, 0x00000400U);
    dma_textures(gTextureGreenShell2, 0x00000258U, 0x00000400U);
    dma_textures(gTextureGreenShell3, 0x0000025EU, 0x00000400U);
    dma_textures(gTextureGreenShell4, 0x00000258U, 0x00000400U);
    dma_textures(gTextureGreenShell5, 0x00000269U, 0x00000400U);
    dma_textures(gTextureGreenShell6, 0x00000260U, 0x00000400U);
    dma_textures(gTextureGreenShell7, 0x00000254U, 0x00000400U);
    D_802BA054 = dma_textures(gTextureBlueShell0, 0x0000022AU, 0x00000400U);
    dma_textures(gTextureBlueShell1, 0x00000237U, 0x00000400U);
    dma_textures(gTextureBlueShell2, 0x0000023EU, 0x00000400U);
    dma_textures(gTextureBlueShell3, 0x00000243U, 0x00000400U);
    dma_textures(gTextureBlueShell4, 0x00000255U, 0x00000400U);
    dma_textures(gTextureBlueShell5, 0x00000259U, 0x00000400U);
    dma_textures(gTextureBlueShell6, 0x00000239U, 0x00000400U);
    dma_textures(gTextureBlueShell7, 0x00000236U, 0x00000400U);
    dma_textures(gTextureFinishLineBanner1, 0x0000028EU, 0x00000800U);
    dma_textures(gTextureFinishLineBanner2, 0x000002FBU, 0x00000800U);
    dma_textures(gTextureFinishLineBanner3, 0x00000302U, 0x00000800U);
    dma_textures(gTextureFinishLineBanner4, 0x000003B4U, 0x00000800U);
    dma_textures(gTextureFinishLineBanner5, 0x0000031EU, 0x00000800U);
    dma_textures(gTextureFinishLineBanner6, 0x0000036EU, 0x00000800U);
    dma_textures(gTextureFinishLineBanner7, 0x0000029CU, 0x00000800U);
    dma_textures(gTextureFinishLineBanner8, 0x0000025BU, 0x00000800U);
    dma_textures(gTexture671A88, 0x00000400U, 0x00000800U);
    dma_textures(gTexture6774D8, 0x00000400U, 0x00000800U);

    switch (gCurrentCourseId) {
        case COURSE_MARIO_RACEWAY:
            dma_textures(gTextureTrees1, 0x0000035BU, 0x00000800U);
            D_802BA058 = dma_textures(gTexturePiranhaPlant1, 0x000003E8U, 0x00000800U);
            dma_textures(gTexturePiranhaPlant2, 0x000003E8U, 0x00000800U);
            dma_textures(gTexturePiranhaPlant3, 0x000003E8U, 0x00000800U);
            dma_textures(gTexturePiranhaPlant4, 0x000003E8U, 0x00000800U);
            dma_textures(gTexturePiranhaPlant5, 0x000003E8U, 0x00000800U);
            dma_textures(gTexturePiranhaPlant6, 0x000003E8U, 0x00000800U);
            dma_textures(gTexturePiranhaPlant7, 0x000003E8U, 0x00000800U);
            dma_textures(gTexturePiranhaPlant8, 0x000003E8U, 0x00000800U);
            dma_textures(gTexturePiranhaPlant9, 0x000003E8U, 0x00000800U);
            break;
        case COURSE_BOWSER_CASTLE:
            dma_textures(gTextureShrub, 0x000003FFU, 0x00000800U);
            break;
        case COURSE_YOSHI_VALLEY:
            dma_textures(gTextureTrees2, 0x000003E8U, 0x00000800U);
            break;
        case COURSE_FRAPPE_SNOWLAND:
            dma_textures(gTextureFrappeSnowlandTreeLeft, 0x00000454U, 0x00000800U);
            dma_textures(gTextureFrappeSnowlandTreeRight, 0x00000432U, 0x00000800U);
            break;
        case COURSE_ROYAL_RACEWAY:
            dma_textures(gTextureTrees3, 0x000003E8U, 0x00000800U);
            dma_textures(gTextureTrees7, 0x000003E8U, 0x00000800U);
            D_802BA058 = dma_textures(gTexturePiranhaPlant1, 0x000003E8U, 0x00000800U);
            dma_textures(gTexturePiranhaPlant2, 0x000003E8U, 0x00000800U);
            dma_textures(gTexturePiranhaPlant3, 0x000003E8U, 0x00000800U);
            dma_textures(gTexturePiranhaPlant4, 0x000003E8U, 0x00000800U);
            dma_textures(gTexturePiranhaPlant5, 0x000003E8U, 0x00000800U);
            dma_textures(gTexturePiranhaPlant6, 0x000003E8U, 0x00000800U);
            dma_textures(gTexturePiranhaPlant7, 0x000003E8U, 0x00000800U);
            dma_textures(gTexturePiranhaPlant8, 0x000003E8U, 0x00000800U);
            dma_textures(gTexturePiranhaPlant9, 0x000003E8U, 0x00000800U);
            break;
        case COURSE_LUIGI_RACEWAY:
            dma_textures(gTextureTrees5Left, 0x000003E8U, 0x00000800U);
            dma_textures(gTextureTrees5Right, 0x000003E8U, 0x00000800U);
            break;
        case COURSE_MOO_MOO_FARM:
            dma_textures(gTextureTrees4Left, 0x000003E8U, 0x00000800U);
            dma_textures(gTextureTrees4Right, 0x000003E8U, 0x00000800U);
            dma_textures(gTextureCow01Left, 0x00000400U, 0x00000800U);
            dma_textures(gTextureCow01Right, 0x00000400U, 0x00000800U);
            dma_textures(gTextureCow02Left, 0x00000400U, 0x00000800U);
            dma_textures(gTextureCow02Right, 0x00000400U, 0x00000800U);
            dma_textures(gTextureCow03Left, 0x00000400U, 0x00000800U);
            dma_textures(gTextureCow03Right, 0x00000400U, 0x00000800U);
            dma_textures(gTextureCow04Left, 0x00000400U, 0x00000800U);
            dma_textures(gTextureCow04Right, 0x00000400U, 0x00000800U);
            dma_textures(gTextureCow05Left, 0x00000400U, 0x00000800U);
            dma_textures(gTextureCow05Right, 0x00000400U, 0x00000800U);
            break;
        case COURSE_KALAMARI_DESERT:
            dma_textures(gTextureCactus1Left, 0x0000033EU, 0x00000800U);
            dma_textures(gTextureCactus1Right, 0x000002FBU, 0x00000800U);
            dma_textures(gTextureCactus2Left, 0x000002A8U, 0x00000800U);
            dma_textures(gTextureCactus2Right, 0x00000374U, 0x00000800U);
            dma_textures(gTextureCactus3, 0x000003AFU, 0x00000800U);
            break;
        case COURSE_DK_JUNGLE:
            dma_textures(gTextureDksJungleParkwayKiwanoFruit1, 0x0000032FU, 0x00000400U);
            dma_textures(gTextureDksJungleParkwayKiwanoFruit2, 0x00000369U, 0x00000400U);
            dma_textures(gTextureDksJungleParkwayKiwanoFruit3, 0x00000364U, 0x00000400U);
            break;
    }

    init_red_shell_texture();
    destroy_all_actors();
    spawn_course_actors();
    spawn_course_vehicles();
}
#endif

#if 1
RECOMP_PATCH Gfx* func_80095E10(Gfx* displayListHead, s8 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6,
                                s32 arg7, s32 arg8, s32 arg9, u8* argA, u32 argB, u32 argC) {
    u32 var_a1_2 = arg4;
    u32 var_s3 = arg5;
    s32 sp7C;
    u32 var_s2;
    u32 var_s4;
    s32 var_t0 = 1;
    s32 temp_lo;
    s32 sp68 = 0;
    s32 sp64 = 0;
    s32 var_v0_2;

    while ((u32) var_t0 < argB) {
        var_t0 *= 2;
    }

    temp_lo = 0x400 / var_t0;

    while ((u32) (temp_lo / 2) > argC) {
        temp_lo /= 2;
    }

    var_v0_2 = var_t0;
    while (var_v0_2 > 1) {
        var_v0_2 /= 2;
        sp68 += 1;
    }
    var_v0_2 = temp_lo;

    while (var_v0_2 > 1) {
        var_v0_2 /= 2;
        sp64 += 1;
    }

    if (arg8 < 0) {
        arg4 -= arg8;
        arg8 = 0;
    } else if (((arg6 - arg4) + arg8) > SCREEN_WIDTH) {
        arg6 = (arg4 - arg8) + SCREEN_WIDTH;
    }

    if (arg9 < 0) {
        arg5 -= arg9;
        arg9 = 0;
    } else if (((arg7 - arg5) + arg9) > SCREEN_HEIGHT) {
        arg7 = (arg5 - arg9) + SCREEN_HEIGHT;
    }

    if (arg6 < arg4) {
        return displayListHead;
    }
    if (arg7 < arg5) {
        return displayListHead;
    }
    sp7C = arg8;
    for (var_s3 = arg5; var_s3 < (u32) arg7; var_s3 += temp_lo) {

        if ((u32) arg7 < temp_lo + var_s3) {
            var_s4 = arg7 - var_s3;
            if (!var_s4) {
                break;
            }
        } else {
            var_s4 = temp_lo;
        }

        for (var_a1_2 = arg4; var_a1_2 < (u32) arg6; var_a1_2 += var_t0) {

            if ((u32) arg6 < var_t0 + var_a1_2) {
                var_s2 = arg6 - var_a1_2;
                if (!var_s2) {
                    break;
                }
            } else {
                var_s2 = var_t0;
            }
            gDPLoadTextureTile(displayListHead++, argA, arg1, G_IM_SIZ_16b, argB, 0, var_a1_2, var_s3,
                               // var_a1_2 + var_s2, var_s3 + var_s4, 0, G_TX_NOMIRROR | G_TX_WRAP,
                               // G_TX_NOMIRROR | G_TX_WRAP, sp68, sp64, G_TX_NOLOD, G_TX_NOLOD);
                               // @recomp Fix textures loading more pixels than they should by adjusting the
                               // tile size and the mask.
                               var_a1_2 + var_s2 - 1, var_s3 + var_s4 - 1, 0, G_TX_NOMIRROR | G_TX_WRAP,
                               G_TX_NOMIRROR | G_TX_WRAP, 0, 0, G_TX_NOLOD, G_TX_NOLOD);
            gSPTextureRectangle(displayListHead++, arg8 * 4, arg9 * 4, (arg8 + var_s2) * 4, (arg9 + var_s4) * 4, 0,
                                (var_a1_2 * 32) & 0xFFFF, (var_s3 * 32) & 0xFFFF, arg2, arg3);

            arg8 += var_t0;
        }

        arg8 = sp7C;
        arg9 += temp_lo;
    }
    return displayListHead;
}
#endif

#if 1
RECOMP_PATCH Gfx* func_800963F0(Gfx* displayListHead, s8 arg1, s32 arg2, s32 arg3, f32 arg4, f32 arg5, s32 arg6,
                                s32 arg7, s32 arg8, s32 arg9, s32 argA, s32 argB, u8* argC, u32 argD, u32 argE) {
    u32 var_a1_2 = arg6;
    u32 var_s3 = arg7;
    s32 sp7C;
    u32 var_s2;
    u32 var_s4;
    u32 a;
    u32 b;
    s32 var_t0 = 1;
    s32 temp_lo;
    s32 sp68 = 0;
    s32 sp64 = 0;
    s32 var_v0_2;

    while ((u32) var_t0 < argD) {
        var_t0 *= 2;
    }

    temp_lo = 0x400 / var_t0;

    while ((u32) (temp_lo / 2) > argE) {
        temp_lo /= 2;
    }

    var_v0_2 = var_t0;
    while (var_v0_2 > 1) {
        var_v0_2 /= 2;
        sp68 += 1;
    }
    var_v0_2 = temp_lo;

    while (var_v0_2 > 1) {
        var_v0_2 /= 2;
        sp64 += 1;
    }

    if (argA < 0) {
        arg6 -= argA;
        argA = 0;
    } else if ((argA + (arg8 - arg6) * arg4) > SCREEN_WIDTH) {
        arg8 -= argA + (arg8 - arg6) * arg4 - SCREEN_WIDTH;
    }

    if (argB < 0) {
        arg7 -= argB;
        argB = 0;
    } else if ((argB + (arg9 - arg7) * arg5) > SCREEN_HEIGHT) {
        arg9 -= argB + (arg9 - arg7) * arg5 - SCREEN_HEIGHT;
    }

    if (arg8 < arg6) {
        return displayListHead;
    }
    if (arg9 < arg7) {
        return displayListHead;
    }
    arg2 /= arg4;
    arg3 /= arg5;

    sp7C = argA;
    for (var_s3 = arg7; var_s3 < (u32) arg9; var_s3 += temp_lo) {

        if ((u32) arg9 < temp_lo + var_s3) {
            var_s4 = arg9 - var_s3;
            if (!var_s4) {
                break;
            }
        } else {
            var_s4 = temp_lo;
        }
        b = var_s4 * arg5;
        for (var_a1_2 = arg6; var_a1_2 < (u32) arg8; var_a1_2 += var_t0) {

            if ((u32) arg8 < (var_t0 + var_a1_2)) {
                var_s2 = arg8 - var_a1_2;
                if (!var_s2) {
                    break;
                }
            } else {
                var_s2 = var_t0;
            }
            a = var_s2 * arg4;

            gDPLoadTextureTile(displayListHead++, argC, arg1, G_IM_SIZ_16b, argD, argE, var_a1_2, var_s3,
                               // var_a1_2 + var_s2, var_s3 + var_s4, 0, G_TX_NOMIRROR | G_TX_WRAP,
                               // G_TX_NOMIRROR | G_TX_WRAP, sp68, sp64, G_TX_NOLOD, G_TX_NOLOD);
                               // @recomp Fix textures loading more pixels than they should by adjusting the
                               // tile size and the mask.
                               var_a1_2 + var_s2 - 1, var_s3 + var_s4 - 1, 0, G_TX_NOMIRROR | G_TX_WRAP,
                               G_TX_NOMIRROR | G_TX_WRAP, 0, 0, G_TX_NOLOD, G_TX_NOLOD);
            gSPTextureRectangle(displayListHead++, argA * 4, argB * 4, (argA + a) * 4, (argB + b) * 4, 0,
                                (var_a1_2 * 32) & 0xFFFF, (var_s3 * 32) & 0xFFFF, arg2, arg3);

            argA += var_t0 * arg4;
        }

        argA = sp7C;
        argB += temp_lo * arg5;
    }
    return displayListHead;
}
#endif

#if 1
RECOMP_PATCH Gfx* func_80097274(Gfx* displayListHead, s8 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6,
                                s32 arg7, s32 arg8, s32 arg9, UNUSED u16* argA, u32 argB, u32 argC, UNUSED s32 argD) {
    u32 var_a1_2 = arg4;
    u32 var_s3 = arg5;
    s32 sp7C;
    u32 var_s2;
    u32 var_s4;
    s32 var_t0 = 1;
    s32 temp_lo;
    s32 sp68 = 0;
    s32 sp64 = 0;
    s32 var_v0_2;

    gDPPipeSync(displayListHead++);
    gDPSetCycleType(displayListHead++, G_CYC_2CYCLE);
    gDPSetTextureLOD(displayListHead++, G_TL_TILE);
    gDPSetPrimColor(displayListHead++, 0, 0, 0, 0, 0, gGlobalTimer % 256);
    gDPSetCombineLERP(displayListHead++, TEXEL1, TEXEL0, PRIMITIVE_ALPHA, TEXEL0, TEXEL1, TEXEL0, PRIMITIVE, TEXEL0, 0,
                      0, 0, COMBINED, 0, 0, 0, COMBINED);

    while ((u32) var_t0 < argB) {
        var_t0 *= 2;
    }
    temp_lo = 0x400 / var_t0;
    while ((u32) (temp_lo / 2) > argC) {
        temp_lo /= 2;
    }
    var_v0_2 = var_t0;
    while (var_v0_2 > 1) {
        var_v0_2 /= 2;
        sp68 += 1;
    }
    var_v0_2 = temp_lo;
    while (var_v0_2 > 1) {
        var_v0_2 /= 2;
        sp64 += 1;
    }
    if (arg8 < 0) {
        arg4 -= arg8;
        arg8 = 0;
    } else if (((arg6 - arg4) + arg8) > 320) {
        arg6 = (arg4 - arg8) + 320;
    }
    if (arg9 < 0) {
        arg5 -= arg9;
        arg9 = 0;
    } else if (((arg7 - arg5) + arg9) > 240) {
        arg7 = (arg5 - arg9) + 240;
    }
    if (arg6 < arg4) {
        return displayListHead;
    }
    if (arg7 < arg5) {
        return displayListHead;
    }
    sp7C = arg8;
    for (var_s3 = arg5; var_s3 < (u32) arg7; var_s3 += temp_lo) {
        if ((u32) arg7 < temp_lo + var_s3) {
            var_s4 = arg7 - var_s3;
            if (!var_s4) {
                break;
            }
        } else {
            var_s4 = temp_lo;
        }
        for (var_a1_2 = arg4; var_a1_2 < (u32) arg6; var_a1_2 += var_t0) {
            if ((u32) arg6 < var_t0 + var_a1_2) {
                var_s2 = arg6 - var_a1_2;
                if (!var_s2) {
                    break;
                }
            } else {
                var_s2 = var_t0;
            }

            u8* D_0B002A00_MissingSym = (u8*) 0x0B002A00;

            gDPLoadMultiTile(displayListHead++, argA, 0, G_TX_RENDERTILE, arg1, G_IM_SIZ_16b, argB, argC, var_a1_2,
                             // var_s3, var_a1_2 + var_s2, var_s3 + var_s4, 0, G_TX_WRAP, G_TX_WRAP, sp68, sp64,
                             // @recomp Fix textures loading more pixels than they should by adjusting the
                             // tile size and the mask.
                             var_s3, var_a1_2 + var_s2 - 1, var_s3 + var_s4 - 1, 0, G_TX_WRAP, G_TX_WRAP, 0, 0,
                             G_TX_NOLOD, G_TX_NOLOD);
            gDPLoadMultiTile(displayListHead++, *D_0B002A00_MissingSym + random_int(128) * 2, 256, G_TX_RENDERTILE + 1,
                             arg1,
                             // G_IM_SIZ_16b, argB, argC, var_a1_2, var_s3, var_a1_2 + var_s2, var_s3 + var_s4, 0,
                             // G_TX_WRAP, G_TX_WRAP, sp68, sp64, G_TX_NOLOD, G_TX_NOLOD);
                             // @recomp Fix textures loading more pixels than they should by adjusting the
                             // tile size and the mask.
                             G_IM_SIZ_16b, argB, argC, var_a1_2, var_s3, var_a1_2 + var_s2 - 1, var_s3 + var_s4 - 1, 0,
                             G_TX_WRAP, G_TX_WRAP, 0, 0, G_TX_NOLOD, G_TX_NOLOD);
            gSPTextureRectangle(displayListHead++, arg8 * 4, arg9 * 4, (arg8 + var_s2) * 4, (arg9 + var_s4) * 4, 0,
                                (var_a1_2 * 32) & 0xFFFF, (var_s3 * 32) & 0xFFFF, arg2, arg3);
            arg8 += var_t0;
        }
        arg8 = sp7C;
        arg9 += temp_lo;
    }
    gDPPipeSync(displayListHead++);
    gDPSetCycleType(displayListHead++, G_CYC_1CYCLE);
    return displayListHead;
}
#endif

#if 1
RECOMP_PATCH Gfx* func_80097E58(Gfx* displayListHead, s8 fmt, UNUSED u32 arg2, u32 arg3, UNUSED u32 arg4, u32 arg5,
                                s32 arg6, s32 arg7, u8* someTexture, u32 arg9, UNUSED u32 argA, s32 width) {
    u32 ult;
    u32 temp;
    s32 arg6Copy;
    s32 temp_v1;
    s32 var_s2;
    s32 lrs;
    s32 spDC;
    s32 temp2 = 32;

    if (width >= 32) {
        return displayListHead;
    }

    arg6Copy = arg6;

    lrs = arg9 / 2;
    spDC = arg9 - lrs;
    for (ult = arg3; ult < arg5; ult += 32) {
        temp = 0;
        if ((ult + temp2) > arg5) {
            var_s2 = arg5 - ult;
            if (!var_s2) {
                break;
            }
        } else {
            var_s2 = temp2;
        }
        temp_v1 = ((32 * lrs) << 10) / (lrs * (32 - width));

        // gDPLoadTextureTile(displayListHead++, someTexture, fmt, G_IM_SIZ_16b, arg9, argA, temp, ult, temp + lrs,
        //                    ult + var_s2, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD,
        // @recomp Fix textures loading more pixels than they should by adjusting the
        // tile size and the mask.
        gDPLoadTextureTile(displayListHead++, someTexture, fmt, G_IM_SIZ_16b, arg9, argA, temp, ult, temp + lrs - 1,
                           ult + var_s2 - 1, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 0, 0, G_TX_NOLOD,
                           G_TX_NOLOD);
        gSPTextureRectangle(displayListHead++, (arg6 + lrs * width / 32) << 2, arg7 << 2, (arg6 + lrs) << 2,
                            (arg7 + var_s2) << 2, 0, 0, (ult << 5) & 0xFFFF, temp_v1, 1024);
        arg6 += lrs;
        temp_v1 = ((32 * spDC) << 10) / (spDC * (32 - width));
        // gDPLoadTextureTile(displayListHead++, someTexture, fmt, G_IM_SIZ_16b, arg9, argA, temp + lrs, ult, temp +
        // arg9,
        //                    ult + var_s2, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD,
        // @recomp Fix textures loading more pixels than they should by adjusting the
        // tile size and the mask.
        gDPLoadTextureTile(displayListHead++, someTexture, fmt, G_IM_SIZ_16b, arg9, argA, temp + lrs, ult,
                           temp + arg9 - 1, ult + var_s2 - 1, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP,
                           0, 0, G_TX_NOLOD, G_TX_NOLOD);
        gSPTextureRectangle(displayListHead++, arg6 << 2, arg7 << 2, (arg6 + spDC * (32 - width) / 32) << 2,
                            (arg7 + var_s2) << 2, 0, (lrs << 5) & 0xFFFF, (ult << 5) & 0xFFFF, temp_v1, 1024);
        arg6 = arg6Copy;
        arg7 += temp2;
    }
    return displayListHead;
}
#endif

Gfx* drawBackground(Gfx* displayListHead, s8 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, u8* arg8,
                    u32 arg9, u32 argA) {
    gDPPipeSync(displayListHead++);
    gDPSetCycleType(displayListHead++, G_CYC_COPY);
    displayListHead = drawBackground2(displayListHead, arg1, 0x00001000, 0x00000400, arg2, arg3, arg4, arg5, arg6, arg7,
                                      arg8, arg9, argA);
    gDPPipeSync(displayListHead++);
    gDPSetCycleType(displayListHead++, G_CYC_1CYCLE);
    return displayListHead;
}

Gfx* RenderMenuTextures(Gfx* arg0, MenuTexture* arg1, s32 column, s32 row) {
    MenuTexture* temp_v0;
    u8* temp_v0_3;
    s8 var_s4;

    temp_v0 = segmented_to_virtual_dupe(arg1);
    while (temp_v0->textureData != NULL) {
        var_s4 = 0;
        switch (temp_v0->type) {
            case 0:
                gSPDisplayList(arg0++, 0x02007708);
                break;
            case 1:
                gSPDisplayList(arg0++, 0x02007728);
                break;
            case 2:
                gSPDisplayList(arg0++, 0x02007748);
                break;
            case 3:
                gSPDisplayList(arg0++, 0x02007768);
                var_s4 = 3;
                break;
            case 4:
                gSPDisplayList(arg0++, 0x02007788);
                break;
            default:
                gSPDisplayList(arg0++, 0x02007728);
                break;
        }
        temp_v0_3 = (u8*) func_8009B8C4(temp_v0->textureData);
        if (temp_v0_3 != 0) {
            if (D_8018E7AC[4] != 4) {
                arg0 = RenderBackground(arg0, var_s4, 0x00000400, 0x00000400, 0, 0, temp_v0->width, temp_v0->height,
                                        temp_v0->dX + column, temp_v0->dY + row, temp_v0_3, temp_v0->width,
                                        temp_v0->height);
            } else {
                arg0 = func_800987D0(arg0, 0U, 0U, temp_v0->width, temp_v0->height, temp_v0->dX + column,
                                     temp_v0->dY + row, temp_v0_3, temp_v0->width, temp_v0->height);
            }
        }
        temp_v0++;
    }
    return arg0;
}

Gfx* RenderBackground(Gfx* displayListHead, s8 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7,
                      s32 arg8, s32 arg9, u8* argA, u32 argB, u32 argC) {
    u32 var_a1_2 = arg4;
    u32 var_s3 = arg5;
    s32 sp7C;
    u32 var_s2;
    u32 var_s4;
    s32 var_t0 = 1;
    s32 temp_lo;
    s32 sp68 = 0;
    s32 sp64 = 0;
    s32 var_v0_2;

    while ((u32) var_t0 < argB) {
        var_t0 *= 2;
    }

    temp_lo = 0x400 / var_t0;

    while ((u32) (temp_lo / 2) > argC) {
        temp_lo /= 2;
    }

    var_v0_2 = var_t0;
    while (var_v0_2 > 1) {
        var_v0_2 /= 2;
        sp68 += 1;
    }
    var_v0_2 = temp_lo;

    while (var_v0_2 > 1) {
        var_v0_2 /= 2;
        sp64 += 1;
    }

    if (arg8 < 0) {
        arg4 -= arg8;
        arg8 = 0;
    } else if (((arg6 - arg4) + arg8) > SCREEN_WIDTH) {
        arg6 = (arg4 - arg8) + SCREEN_WIDTH;
    }

    if (arg9 < 0) {
        arg5 -= arg9;
        arg9 = 0;
    } else if (((arg7 - arg5) + arg9) > SCREEN_HEIGHT) {
        arg7 = (arg5 - arg9) + SCREEN_HEIGHT;
    }

    if (arg6 < arg4) {
        return displayListHead;
    }
    if (arg7 < arg5) {
        return displayListHead;
    }
    sp7C = arg8;
    for (var_s3 = arg5; var_s3 < (u32) arg7; var_s3 += temp_lo) {

        if ((u32) arg7 < temp_lo + var_s3) {
            var_s4 = arg7 - var_s3;
            if (!var_s4) {
                break;
            }
        } else {
            var_s4 = temp_lo;
        }

        for (var_a1_2 = arg4; var_a1_2 < (u32) arg6; var_a1_2 += var_t0) {

            if ((u32) arg6 < var_t0 + var_a1_2) {
                var_s2 = arg6 - var_a1_2;
                if (!var_s2) {
                    break;
                }
            } else {
                var_s2 = var_t0;
            }
            gDPLoadTextureTile(displayListHead++, argA, arg1, G_IM_SIZ_16b, argB, 0, var_a1_2, var_s3,
                               // var_a1_2 + var_s2, var_s3 + var_s4, 0, G_TX_NOMIRROR | G_TX_WRAP,
                               // G_TX_NOMIRROR | G_TX_WRAP, sp68, sp64, G_TX_NOLOD, G_TX_NOLOD);
                               // @recomp Fix textures loading more pixels than they should by adjusting the
                               // tile size and the mask.
                               var_a1_2 + var_s2 - 1, var_s3 + var_s4 - 1, 0, G_TX_NOMIRROR | G_TX_WRAP,
                               G_TX_NOMIRROR | G_TX_WRAP, 0, 0, G_TX_NOLOD, G_TX_NOLOD);
            gEXTextureRectangle(displayListHead++, G_EX_ORIGIN_CENTER, G_EX_ORIGIN_CENTER, arg8 * 4 - SCREEN_WIDTH * 2, arg9 * 4,
                                (arg8 + var_s2) * 4 - SCREEN_WIDTH * 2, (arg9 + var_s4) * 4, 0, (var_a1_2 * 32) & 0xFFFF,
                                (var_s3 * 32) & 0xFFFF, arg2, arg3);

            arg8 += var_t0;
        }

        arg8 = sp7C;
        arg9 += temp_lo;
    }
    return displayListHead;
}

#if 1
Gfx* drawBackground2(Gfx* displayListHead, s8 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7,
                     s32 arg8, s32 arg9, u8* argA, u32 argB, u32 argC) {
    u32 var_a1_2 = arg4;
    u32 var_s3 = arg5;
    s32 sp7C;
    u32 var_s2;
    u32 var_s4;
    s32 var_t0 = 1;
    s32 temp_lo;
    s32 sp68 = 0;
    s32 sp64 = 0;
    s32 var_v0_2;

    while ((u32) var_t0 < argB) {
        var_t0 *= 2;
    }

    temp_lo = 0x400 / var_t0;

    while ((u32) (temp_lo / 2) > argC) {
        temp_lo /= 2;
    }

    var_v0_2 = var_t0;
    while (var_v0_2 > 1) {
        var_v0_2 /= 2;
        sp68 += 1;
    }
    var_v0_2 = temp_lo;

    while (var_v0_2 > 1) {
        var_v0_2 /= 2;
        sp64 += 1;
    }

    if (arg8 < 0) {
        arg4 -= arg8;
        arg8 = 0;
    } else if (((arg6 - arg4) + arg8) > SCREEN_WIDTH) {
        arg6 = (arg4 - arg8) + SCREEN_WIDTH;
    }

    if (arg9 < 0) {
        arg5 -= arg9;
        arg9 = 0;
    } else if (((arg7 - arg5) + arg9) > SCREEN_HEIGHT) {
        arg7 = (arg5 - arg9) + SCREEN_HEIGHT;
    }

    if (arg6 < arg4) {
        return displayListHead;
    }
    if (arg7 < arg5) {
        return displayListHead;
    }
    sp7C = arg8;
    for (var_s3 = arg5; var_s3 < (u32) arg7; var_s3 += temp_lo) {

        if ((u32) arg7 < temp_lo + var_s3) {
            var_s4 = arg7 - var_s3;
            if (!var_s4) {
                break;
            }
        } else {
            var_s4 = temp_lo;
        }

        for (var_a1_2 = arg4; var_a1_2 < (u32) arg6; var_a1_2 += var_t0) {

            if ((u32) arg6 < var_t0 + var_a1_2) {
                var_s2 = arg6 - var_a1_2;
                if (!var_s2) {
                    break;
                }
            } else {
                var_s2 = var_t0;
            }
            gDPLoadTextureTile(displayListHead++, argA, arg1, G_IM_SIZ_16b, argB, 0, var_a1_2, var_s3,
                               // var_a1_2 + var_s2, var_s3 + var_s4, 0, G_TX_NOMIRROR | G_TX_WRAP,
                               // G_TX_NOMIRROR | G_TX_WRAP, sp68, sp64, G_TX_NOLOD, G_TX_NOLOD);
                               // @recomp Fix textures loading more pixels than they should by adjusting the
                               // tile size and the mask.
                               var_a1_2 + var_s2 - 1, var_s3 + var_s4 - 1, 0, G_TX_NOMIRROR | G_TX_WRAP,
                               G_TX_NOMIRROR | G_TX_WRAP, 0, 0, G_TX_NOLOD, G_TX_NOLOD);
            gEXTextureRectangle(displayListHead++, G_EX_ORIGIN_CENTER, G_EX_ORIGIN_CENTER, arg8 * 4 - SCREEN_WIDTH * 2, arg9 * 4,
                                (arg8 + var_s2) * 4 - SCREEN_WIDTH * 2, (arg9 + var_s4) * 4, 0, (var_a1_2 * 32) & 0xFFFF,
                                (var_s3 * 32) & 0xFFFF, arg2, arg3);

            arg8 += var_t0;
        }

        arg8 = sp7C;
        arg9 += temp_lo;
    }
    return displayListHead;
}
#endif
