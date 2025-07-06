#include "patches.h"

extern u8* D_802BA050;
extern u8* D_802BA054;
extern u8* D_802BA058;

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

#if 0
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

#if 0
RECOMP_PATCH Gfx* func_80097E58(Gfx* displayListHead, s8 fmt, UNUSED u32 arg2, u32 arg3, UNUSED u32 arg4, u32 arg5, s32 arg6,
                   s32 arg7, u8* someTexture, u32 arg9, UNUSED u32 argA, s32 width) {
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

        //gDPLoadTextureTile(displayListHead++, someTexture, fmt, G_IM_SIZ_16b, arg9, argA, temp, ult, temp + lrs,
        //                   ult + var_s2, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD,
        // @recomp Fix textures loading more pixels than they should by adjusting the
        // tile size and the mask.
        gDPLoadTextureTile(displayListHead++, someTexture, fmt, G_IM_SIZ_16b, arg9, argA, temp, ult, temp + lrs - 1,
                           ult + var_s2 - 1, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 0, 0, G_TX_NOLOD,
                           G_TX_NOLOD);
        gSPTextureRectangle(displayListHead++, (arg6 + lrs * width / 32) << 2, arg7 << 2, (arg6 + lrs) << 2,
                            (arg7 + var_s2) << 2, 0, 0, (ult << 5) & 0xFFFF, temp_v1, 1024);
        arg6 += lrs;
        temp_v1 = ((32 * spDC) << 10) / (spDC * (32 - width));
        //gDPLoadTextureTile(displayListHead++, someTexture, fmt, G_IM_SIZ_16b, arg9, argA, temp + lrs, ult, temp + arg9,
        //                   ult + var_s2, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD,
        // @recomp Fix textures loading more pixels than they should by adjusting the
        // tile size and the mask.
        gDPLoadTextureTile(displayListHead++, someTexture, fmt, G_IM_SIZ_16b, arg9, argA, temp + lrs, ult, temp + arg9 - 1,
                           ult + var_s2 - 1, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 0, 0, G_TX_NOLOD,
                           G_TX_NOLOD);
        gSPTextureRectangle(displayListHead++, arg6 << 2, arg7 << 2, (arg6 + spDC * (32 - width) / 32) << 2,
                            (arg7 + var_s2) << 2, 0, (lrs << 5) & 0xFFFF, (ult << 5) & 0xFFFF, temp_v1, 1024);
        arg6 = arg6Copy;
        arg7 += temp2;
    }
    return displayListHead;
}
#endif
