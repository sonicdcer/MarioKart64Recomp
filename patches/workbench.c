#include "patches.h"

#if 0
RECOMP_PATCH void func_802A3730(struct UnkStruct_800DC5EC* arg0) {
    s32 ulx;
    s32 uly;
    s32 lrx;
    s32 lry;
    s32 screenWidth = arg0->screenWidth * 2;
    s32 screenHeight = arg0->screenHeight * 2;
    s32 screenStartX = arg0->screenStartX * 4;
    s32 screenStartY = arg0->screenStartY * 4;

    arg0->viewport.vp.vscale[0] = screenWidth;
    arg0->viewport.vp.vscale[1] = screenHeight;
    arg0->viewport.vp.vscale[2] = 511;
    arg0->viewport.vp.vscale[3] = 0;

    arg0->viewport.vp.vtrans[0] = screenStartX;
    arg0->viewport.vp.vtrans[1] = screenStartY;
    arg0->viewport.vp.vtrans[2] = 511;
    arg0->viewport.vp.vtrans[3] = 0;

    gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&arg0->viewport));

    screenWidth /= 4;
    screenHeight /= 4;

    screenStartX /= 4;
    screenStartY /= 4;

    lrx = screenStartX + screenWidth;
    if (lrx > SCREEN_WIDTH) {
        lrx = SCREEN_WIDTH;
    }

    lry = screenStartY + screenHeight;
    if (lry > SCREEN_HEIGHT) {
        lry = SCREEN_HEIGHT;
    }
    ulx = 0;
    uly = 0;

    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, ulx, uly, lrx, lry);
}
#endif

#if 1 // Fix Debug on issue ( need to figure out actual issue.
void func_800CA330(u8 arg0);

RECOMP_PATCH void func_8028F4E8(void) {
    if (gEnableDebugMode) {
        if (((gControllerOne->button & R_TRIG) != 0) && ((gControllerOne->button & L_TRIG) != 0) &&
            ((gControllerOne->button & A_BUTTON) != 0) && ((gControllerOne->button & B_BUTTON) != 0)) {

            func_800CA330(0x19);
            func_800CA388(0x19);
            gGotoMode = MAIN_MENU_FROM_QUIT;
            D_800DC510 = 6;
            D_800DC5B4 = 1;
            D_800DC5B0 = 1;
            D_800DC5B8 = 0;
            D_802BA038 = 5;
        }
    }
}
#endif

#if 0
int counter = 0;

RECOMP_PATCH void func_80094A64(struct GfxPool* pool) {

    gMatrixHudCount = 0;
    gMatrixEffectCount = 0;
    gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(0x802B8880));
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    guOrtho(&pool->mtxScreen, 0.0f, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, 0.0f, -100.0f, 100.0f, 1.0f);
    gSPMatrix(gDisplayListHead++, &pool->mtxScreen, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x02007650);
    setup_menus();
    func_80092290(4, D_8018E850, D_8018E858);
    func_80092290(5, &D_8018E850[1], &D_8018E858[1]);
    func_80099A70();
    func_8009C918();
    switch (gMenuSelection) {
        case START_MENU:
            func_80095574();
            func_80093E40();
            break;
        case OPTIONS_MENU:
        case DATA_MENU:
        case COURSE_DATA_MENU:
        case LOGO_INTRO_MENU:
        case CONTROLLER_PAK_MENU:
        case MAIN_MENU:
        case CHARACTER_SELECT_MENU:
        case COURSE_SELECT_MENU:
            handle_menus_default();
            func_80099AEC();
            // recomp_sleep_miliseconds(60);
            break;
    }
    func_8009CA2C();
    gCycleFlashMenu += 1;
    gDPPipeSync(gDisplayListHead++);
    gSPDisplayList(gDisplayListHead++, (Gfx*) 0x020076B0);
}

RECOMP_PATCH void func_8009CA6C(s32 arg0) {
    s32 var_a1;

    if ((arg0 == 4) || ((find_menu_items(0x000000AA) == NULL) && (find_menu_items(0x000000AB) == NULL) &&
                        (find_menu_items(0x000000B9) == NULL) && (find_menu_items(0x000000BA) == NULL) &&
                        (find_menu_items(0x000000AC) == NULL) && (find_menu_items(0x000000B0) == NULL))) {
        var_a1 = 0;
        gSPDisplayList(gDisplayListHead++, (Gfx*) 0x0D0076F8);
        if ((arg0 != 4) && (gIsGamePaused != 0)) {
            var_a1 = 1;
        }
        switch (D_8018E7AC[arg0]) {
            case 1:
                func_8009CDDC(arg0, var_a1); // fade in
                return;
            case 2:
                func_8009D958(arg0, var_a1); // fade out
                return;
            case 3:
                func_8009DB8C();
                return;
            case 4:
                func_8009DAA8();
                return;
            case 5:
                func_8009D998(arg0);
                return;
            case 7:
                func_8009D978(arg0, var_a1);
                return;
            case 8:
                func_8009CDFC(arg0, var_a1);
                break;
            case 0:
            default:
                break;
        }
    }
}

RECOMP_PATCH void func_8009CBE4(s32 arg0, s32 arg1, s32 arg2) {
    RGBA16* color;
    s16 x, y, w, h;
    UNUSED s32 pad[3];
    struct UnkStruct_800DC5EC* unk;
    struct UnkStruct_8018E7E8 *size, *start;

    if ((gModeSelection == GRAND_PRIX) || (gModeSelection == TIME_TRIALS)) {
        start = &(D_8018E7E8[arg0]);
        size = &(D_8018E810[arg0]);
        x = start->x;
        y = start->y;
        w = size->x;
        h = size->y;
    } else if (arg0 >= 4) {
        start = &(D_8018E7E8[arg0]);
        size = &(D_8018E810[arg0]);
        x = start->x;
        y = start->y;
        w = size->x;
        h = size->y;
    } else {
        unk = &D_8015F480[arg0];
        x = unk->screenStartX;
        y = unk->screenStartY;
        w = unk->screenWidth;
        h = unk->screenHeight;
    }
    color = &D_800E7AE8[arg2];
    gDisplayListHead = draw_box(gDisplayListHead, x - (w / 2), y - (h / 2), (w / 2) + x, (h / 2) + y, color->red,
                                color->green, color->blue, 0xFF - (D_8018E7D0[arg0] * 0xFF / D_8018E7B8[arg0]));

    if ((arg1 == 0) && (D_8018E7D0[arg0] += 1, (D_8018E7D0[arg0] >= D_8018E7B8[arg0]))) {
        if (gGamestate == RACING) {

            D_8018E7AC[arg0] = 6;
            return;
        }
        D_8018E7AC[arg0] = 0;
        unref_8018EE0C = 0;
    }
}

RECOMP_PATCH void func_8009D77C(s32 arg0, s32 arg1, s32 arg2) {
    s16 var_ra;
    s16 var_t3;
    s16 var_t4;
    s32 temp_t8;
    s32 temp_v1;
    s32 var_t2;
    s32 someMath0;
    s32 someMath1;
    RGBA16* temp_v0_2;
    s32 sp44;
    UNUSED s32 stackPadding0;

    counter++;
    recomp_printf("counter: %d\n", counter);

    if ((gModeSelection == 0) || (gModeSelection == 1)) {
        var_t3 = D_8018E7E8[arg0].x;
        var_t4 = D_8018E7E8[arg0].y;
        var_ra = D_8018E810[arg0].x;
        sp44 = D_8018E810[arg0].y;
    } else if (arg0 >= 4) {
        var_t3 = D_8018E7E8[arg0].x;
        var_t4 = D_8018E7E8[arg0].y;
        var_ra = D_8018E810[arg0].x;
        sp44 = D_8018E810[arg0].y;
    } else {
        var_t3 = D_8015F480[arg0].screenStartX;
        var_t4 = D_8015F480[arg0].screenStartY;
        var_ra = D_8015F480[arg0].screenWidth;
        sp44 = D_8015F480[arg0].screenHeight;
    }
    var_t2 = (D_8018E7D0[arg0] * 0xFF) / D_8018E7B8[arg0];
    if (var_t2 >= 0x100) {
        var_t2 = 0x000000FF;
    }
    temp_v1 = var_ra / 2;
    temp_t8 = sp44 / 2;
    temp_v0_2 = &D_800E7AE8[arg2];
    // Why does it have to written like this to match?
    someMath0 = temp_v1;
    someMath0 += var_t3;
    someMath1 = temp_t8;
    someMath1 += var_t4;
    gDisplayListHead = draw_box(gDisplayListHead, var_t3 - temp_v1, var_t4 - temp_t8, someMath0, someMath1,
                                temp_v0_2->red, temp_v0_2->green, temp_v0_2->blue, var_t2);
    if (arg1 == 0) {
        D_8018E7D0[arg0]++;
        if ((D_8018E7B8[arg0] + 1) < D_8018E7D0[arg0]) {
            if (counter >= 300) {
                counter = 0;
                func_8009CE64(arg0);
            }
        }
    }
}
#endif

#if 0
RECOMP_PATCH void render_course(struct UnkStruct_800DC5EC* arg0) {

    set_course_lighting(D_800DC610, D_802B87D4, 0, 1);
    if (creditsRenderMode) {
        render_course_credits();
        return;
    }

    switch (gCurrentCourseId) {
        case COURSE_MARIO_RACEWAY:
            render_mario_raceway(arg0);
            break;
        case COURSE_CHOCO_MOUNTAIN:
            render_choco_mountain(arg0);
            break;
        case COURSE_BOWSER_CASTLE:
            render_bowsers_castle(arg0);
            break;
        case COURSE_BANSHEE_BOARDWALK:
            render_banshee_boardwalk(arg0);
            break;
        case COURSE_YOSHI_VALLEY:
            render_yoshi_valley(arg0);
            break;
        case COURSE_FRAPPE_SNOWLAND:
            render_frappe_snowland(arg0);
            break;
        case COURSE_KOOPA_BEACH:
            render_koopa_troopa_beach(arg0);
            break;
        case COURSE_ROYAL_RACEWAY:
            render_royal_raceway(arg0);
            break;
        case COURSE_LUIGI_RACEWAY:
            render_luigi_raceway(arg0);
            break;
        case COURSE_MOO_MOO_FARM:
            render_moo_moo_farm(arg0);
            break;
        case COURSE_TOADS_TURNPIKE:
            render_toads_turnpike(arg0);
            break;
        case COURSE_KALAMARI_DESERT:
            render_kalimari_desert(arg0);
            break;
        case COURSE_SHERBET_LAND:
            render_sherbet_land(arg0);
            break;
        case COURSE_RAINBOW_ROAD:
            render_rainbow_road(arg0);
            // render_course_credits();
            break;
        case COURSE_WARIO_STADIUM:
            render_wario_stadium(arg0);
            break;
        case COURSE_BLOCK_FORT:
            render_block_fort(arg0);
            break;
        case COURSE_SKYSCRAPER:
            render_skyscraper(arg0);
            break;
        case COURSE_DOUBLE_DECK:
            render_double_deck(arg0);
            break;
        case COURSE_DK_JUNGLE:
            render_dks_jungle_parkway(arg0);
            break;
        case COURSE_BIG_DONUT:
            render_big_donut(arg0);
            break;
    }
}
#endif