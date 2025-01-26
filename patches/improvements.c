#include "patches.h"

// Blur characters
#if 1
RECOMP_PATCH void render_kart(Player* player, s8 playerId, s8 arg2, s8 arg3) {
    UNUSED s32 pad;
    Mat4 sp1A4;
    UNUSED s32 pad2[17];
    Vec3f sp154;
    Vec3s sp14C;
    f32 sp148;
    f32 sp144;
    f32 sp140;
    s16 temp_v1;
    s16 thing;

    if (player->unk_044 & 0x2000) {
        sp14C[0] = 0;
        sp14C[1] = player->unk_048[arg2];
        sp14C[2] = 0;
        func_80062B18(&sp148, &sp144, &sp140, 0.0f, 1.5f, 0.0f, -player->unk_048[arg2], player->unk_050[arg2]);
        sp154[1] = (player->pos[1] - player->boundingBoxSize) + (sp144 - 2.0);
        sp154[0] = player->pos[0] + sp148;
        sp154[2] = player->pos[2] + sp140;
    } else {
        thing = (u16) (player->unk_048[arg2] + player->rotation[1] + player->unk_0C0);
        temp_v1 = player->unk_0CC[arg2] * sins(thing);
        if ((player->effects & 8) == 8) {
            sp14C[0] = cameras[arg2].rot[0] - 0x4000;
        } else {
            sp14C[0] = -temp_v1 * 0.8;
        }
        sp14C[1] = player->unk_048[arg2];
        sp14C[2] = player->unk_050[arg2];
        if (((s32) player->effects & HIT_EFFECT) == HIT_EFFECT) {
            func_80062B18(&sp148, &sp144, &sp140, 0.0f, 8.0f, 0.0f, -player->unk_048[arg2], player->unk_050[arg2]);
            sp154[1] = (player->pos[1] - player->boundingBoxSize) + player->unk_108;
            sp154[0] = player->pos[0] + sp148;
            sp154[2] = player->pos[2] + sp140;
        } else {
            func_80062B18(&sp148, &sp144, &sp140, 0.0f, 1.5f, 0.0f, -player->unk_048[arg2], player->unk_050[arg2]);
            sp154[1] = (player->pos[1] - player->boundingBoxSize) + player->unk_108 + (sp144 - 2.0);
            sp154[0] = player->pos[0] + sp148;
            sp154[2] = player->pos[2] + sp140;
        }
    }
#ifdef AVOID_UB
    gPlayerPalette = &gPlayerPalettesList[D_801651D0[arg2][playerId]][arg2][playerId];
#else
    gPlayerPalette = (struct_D_802F1F80*) &gPlayerPalettesList[D_801651D0[arg2][playerId]][arg2][playerId * 0x100];
#endif
    if ((arg2 == 0) || (arg2 == 1)) {
        sKartUpperTexture = &D_802BFB80.arraySize8[D_801651D0[arg2][playerId]][arg2][playerId].pixel_index_array[0];
        sKartLowerTexture = &D_802BFB80.arraySize8[D_801651D0[arg2][playerId]][arg2][playerId].pixel_index_array[0x7C0];
    } else {
        sKartUpperTexture =
            &D_802BFB80.arraySize8[D_801651D0[arg2][playerId]][arg2 - 1][playerId - 4].pixel_index_array[0];
        sKartLowerTexture =
            &D_802BFB80.arraySize8[D_801651D0[arg2][playerId]][arg2 - 1][playerId - 4].pixel_index_array[0x7C0];
    }
    mtxf_translate_rotate(sp1A4, sp154, sp14C);
    mtxf_scale2(sp1A4, gCharacterSize[player->characterId] * player->size);
    convert_to_fixed_point_matrix(&gGfxPool->mtxKart[playerId + (arg2 * 8)], sp1A4);

    if ((player->effects & BOO_EFFECT) == BOO_EFFECT) {
        if (arg2 == playerId) {
            gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxKart[playerId + (arg2 * 8)]),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(gDisplayListHead++, (Gfx*) 0xd008cd8);
            gDPLoadTLUT_pal256(gDisplayListHead++, gPlayerPalette);
            gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
            func_8004B614(gPlayerRedEffect[playerId], gPlayerGreenEffect[playerId], gPlayerBlueEffect[playerId],
                          gPlayerCyanEffect[playerId], gPlayerMagentaEffect[playerId], gPlayerYellowEffect[playerId],
                          (s32) player->unk_0C6);
            gDPSetRenderMode(gDisplayListHead++,
                             AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                                 GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA),
                             AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                                 GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA));
        } else {
            gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxKart[playerId + (arg2 * 8)]),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(gDisplayListHead++, (Gfx*) 0xd008cd8);
            gDPLoadTLUT_pal256(gDisplayListHead++, gPlayerPalette);
            gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
            func_8004B614(gPlayerRedEffect[playerId], gPlayerGreenEffect[playerId], gPlayerBlueEffect[playerId],
                          gPlayerCyanEffect[playerId], gPlayerMagentaEffect[playerId], gPlayerYellowEffect[playerId],
                          D_8018D970[playerId]);
            gDPSetRenderMode(gDisplayListHead++,
                             AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                                 GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA),
                             AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                                 GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA));
        }
    } else if (((player->unk_0CA & 4) == 4) || (player->soundEffects & 0x08000000) ||
               (player->soundEffects & 0x04000000)) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxKart[playerId + (arg2 * 8)]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gDisplayListHead++, (Gfx*) 0xd008cd8);
        gDPLoadTLUT_pal256(gDisplayListHead++, gPlayerPalette);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
        func_8004B614(gPlayerRedEffect[playerId], gPlayerGreenEffect[playerId], gPlayerBlueEffect[playerId],
                      gPlayerCyanEffect[playerId], gPlayerMagentaEffect[playerId], gPlayerYellowEffect[playerId],
                      (s32) player->unk_0C6);
        gDPSetAlphaCompare(gDisplayListHead++, G_AC_DITHER);
        gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_XLU_SURF, G_RM_ZB_XLU_SURF2);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gGfxPool->mtxKart[playerId + (arg2 * 8)]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gDisplayListHead++, (Gfx*) 0xd008cd8);
        gDPLoadTLUT_pal256(gDisplayListHead++, gPlayerPalette);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
        func_8004B614(gPlayerRedEffect[playerId], gPlayerGreenEffect[playerId], gPlayerBlueEffect[playerId],
                      gPlayerCyanEffect[playerId], gPlayerMagentaEffect[playerId], gPlayerYellowEffect[playerId],
                      (s32) player->unk_0C6);

        // @recomp: blured kart looks better.
        // gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
        gDPSetRenderMode(gDisplayListHead++,
                         AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                             GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA),
                         AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                             GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA));
    }

    gDPLoadTextureBlock(gDisplayListHead++, sKartUpperTexture, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(gDisplayListHead++, &D_800DDBB4[playerId][arg3], 4, 0);
    gSPDisplayList(gDisplayListHead++, (Gfx*) 0xd008c78);

    gDPLoadTextureBlock(gDisplayListHead++, sKartLowerTexture, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(gDisplayListHead++, &D_800DDBB4[playerId][arg3 + 4], 4, 0);
    gSPDisplayList(gDisplayListHead++, (Gfx*) 0xd008c78);
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
    gDPSetAlphaCompare(gDisplayListHead++, G_AC_NONE);
}
#endif