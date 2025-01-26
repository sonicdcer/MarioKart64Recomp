#include "patches.h"

#if 0
RECOMP_PATCH void print_text_mode_1(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposed(gDisplayListHead++, (u32) text << 16 | column << 8 | row, G_EX_PUSH, G_MTX_MODELVIEW,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

    print_text0(column, row, text, tracking, scaleX, scaleY, 1);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
}
#endif

#if 1
RECOMP_PATCH void print_text0(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY, s32 mode) {
    s32 stringWidth = 0;
    s32 glyphIndex;

    gSPDisplayList(gDisplayListHead++, (u8*) 0x020077A8);
    if (*text != 0) {
        do {
            // @recomp Tag the transform.
            gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_LETTER(text), G_EX_PUSH, G_MTX_MODELVIEW,
                                     G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

            glyphIndex = char_to_glyph_index(text);
            if (glyphIndex >= 0) {
                load_menu_img((MenuTexture*) segmented_to_virtual_dupe((const void*) gGlyphTextureLUT[glyphIndex]));
                gDisplayListHead =
                    print_letter(gDisplayListHead,
                                 (MenuTexture*) segmented_to_virtual_dupe((const void*) gGlyphTextureLUT[glyphIndex]),
                                 column + (stringWidth * scaleX), row, mode, scaleX, scaleY);
                stringWidth += gGlyphDisplayWidth[glyphIndex] + tracking;
            } else if ((glyphIndex != -2) && (glyphIndex == -1)) {
                stringWidth += tracking + 7;
            } else {
                gSPDisplayList(gDisplayListHead++, (u8*) 0x020077D8);
                return;
            }
            if (glyphIndex >= 0x30) {
                text += 2;
            } else {
                text += 1;
            }

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

        } while (*text != 0);
    }
    gSPDisplayList(gDisplayListHead++, (u8*) 0x020077D8);
}

// "tracking" is a uniform spacing between all characters in a given word
RECOMP_PATCH void print_text1(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY, s32 arg6) {
    char* temp_string = text;
    s32 stringWidth = 0;
    s32 glyphIndex;
    s32 sp60;

    while (*temp_string != 0) {
        glyphIndex = char_to_glyph_index(temp_string);
        if (glyphIndex >= 0) {
            stringWidth += ((gGlyphDisplayWidth[glyphIndex] + tracking) * scaleX);
        } else if ((glyphIndex != -2) && (glyphIndex == -1)) {
            stringWidth += ((tracking + 7) * scaleX);
        } else {
            return;
        }
        if (glyphIndex >= 0x30) {
            temp_string += 2;
        } else {
            temp_string += 1;
        }
    }

    switch (arg6) {
        case LEFT_TEXT:
            // ???
            do {
            } while (0);
        case RIGHT_TEXT:
            column -= stringWidth;
            break;
        case CENTER_TEXT_MODE_1:
        case CENTER_TEXT_MODE_2:
            column -= stringWidth / 2;
            break;
        default:
            break;
    }

    if (arg6 < 3) {
        sp60 = 1;
    } else {
        sp60 = 2;
    }

    gSPDisplayList(gDisplayListHead++, (u8*) 0x020077A8);
    while (*text != 0) {
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_LETTER(text), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO,
                                 G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                                 G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

        glyphIndex = char_to_glyph_index(text);
        if (glyphIndex >= 0) {
            load_menu_img(segmented_to_virtual_dupe(gGlyphTextureLUT[glyphIndex]));
            gDisplayListHead = print_letter(gDisplayListHead, segmented_to_virtual_dupe(gGlyphTextureLUT[glyphIndex]),
                                            column, row, sp60, scaleX, scaleY);
            column = column + (s32) ((gGlyphDisplayWidth[glyphIndex] + tracking) * scaleX);
        } else if ((glyphIndex != -2) && (glyphIndex == -1)) {
            column = column + (s32) ((tracking + 7) * scaleX);
        } else {
            gSPDisplayList(gDisplayListHead++, (u8*) 0x020077D8);
            return;
        }
        if (glyphIndex >= 0x30) {
            text += 2;
        } else {
            text += 1;
        }

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);
    }
    gSPDisplayList(gDisplayListHead++, (u8*) 0x020077D8);
}

RECOMP_PATCH void print_text2(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY, s32 arg6) {
    MenuTexture* glyphTexture;
    s32 characterWidth;
    s32 glyphIndex;

    gSPDisplayList(gDisplayListHead++, (u8*) 0x020077A8);
    if (*text != 0) {
        do {
            // @recomp Tag the transform.
            gEXMatrixGroupDecomposed(gDisplayListHead++, TAG_LETTER(text), G_EX_PUSH, G_MTX_MODELVIEW,
                                     G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW);

            glyphIndex = char_to_glyph_index(text);
            if (glyphIndex >= 0) {
                glyphTexture = (MenuTexture*) segmented_to_virtual_dupe((const void*) gGlyphTextureLUT[glyphIndex]);
                load_menu_img(glyphTexture);
                gDisplayListHead =
                    print_letter(gDisplayListHead, glyphTexture, column - (gGlyphDisplayWidth[glyphIndex] / 2), row,
                                 arg6, scaleX, scaleY);
                if ((glyphIndex >= 213) && (glyphIndex < 224)) {
                    characterWidth = 32;
                } else {
                    characterWidth = 12;
                }
                column = column + (s32) ((characterWidth + tracking) * scaleX);
            } else if ((glyphIndex != -2) && (glyphIndex == -1)) {
                column = column + (s32) ((tracking + 7) * scaleX);
            } else {
                gSPDisplayList(gDisplayListHead++, (u8*) 0x020077D8);
                return;
            }
            if (glyphIndex >= 0x30) {
                text += 2;
            } else {
                text += 1;
            }

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gDisplayListHead++, G_MTX_MODELVIEW);

        } while (*text != 0);
    }

    gSPDisplayList(gDisplayListHead++, (u8*) 0x020077D8);
}
#endif
