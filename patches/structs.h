#ifndef RECOMP_STRUCTS
#define RECOMP_STRUCTS

#include "patches.h"

typedef struct {
    f32 unk_0;
    s16 unk_4;
    s16 unk_6;
    s16 unk_8;
} UnkCameraInner;

typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ Vec3f lookAt;
    /* 0x18 */ Vec3f up;
    /* 0x24 */ Vec3s rot;
    /* 0x2A */ u16 someBitFlags;
    /* 0x2C */ s16 unk_2C;
    /* 0x2E */ s16 unk_2E;
    /* 0x30 */ Vec3f unk_30;
    /* 0x3C */ Vec3f unk_3C;
    /* 0x48 */ s32 unk_48;
    /* 0x4C */ s32 unk_4C;
    /* 0x50 */ s32 unk_50;
    /* 0x54 */ Collision collision;
    /* 0x94 */ UnkCameraInner unk_94;
    /* 0xA0 */ f32 unk_A0;
    /* 0xA4 */ s32 unk_A4;
    /* 0xA8 */ s32 unk_A8;
    /* 0xAC */ s16 unk_AC;
    /* 0xAE */ s16 playerId;
    /* 0xB0 */ s16 unk_B0;
    /* 0xB2 */ s16 unk_B2;
    /* 0xB4 */ f32 unk_B4;
} Camera; /* size = 0xB8 */

struct UnkStruct_800DC5EC {
    /* 0x00 */ struct Controller* controllers; // gControllers ptr 800F6910
    /* 0x04 */ Camera* camera;                 // Player camera ptr
    /* 0x08 */ Player* player;                 // Player ptr 800F6990
    /* 0x0C */ s32* unkC;                      // unk struct?
    /* 0x10 */ Vp viewport;
    /* 0x20 */ s32 pad[2];
    /* 0x28 */ s16 cameraHeight;
    /* 0x2A */ s16 unk;
    /* 0x2C */ s16 screenWidth;
    /* 0x2E */ s16 screenHeight;
    /* 0x30 */ s16 screenStartX;
    /* 0x32 */ s16 screenStartY;
    /* 0x34 */ s16 unk3C;
    /* 0x36 */ s16 playerDirection;
    /* 0x38 */ s16 pathCounter;
    /* 0x3A */ s16 unk42;
    /* 0x3C */ s32 pad2;
}; // size = 0x40

typedef struct {
    u16 red : 5;
    u16 green : 5;
    u16 blue : 5;
    u16 alpha : 1;
} RGBA5551;

typedef struct {
    /* 0x000 */ RGBA5551 kart_palette[0xC0];
    /* 0x180 */ RGBA5551 wheel_palette[0x40];
} struct_D_802F1F80; // size = 0x200

typedef struct {
    u8 pixel_index_array[0x1000];
} struct_D_802BFB80_8; // size = 0x1000

typedef struct {
    u8 pixel_index_array[0x2000];
} struct_D_802BFB80_4; // size = 0x1000

/*
 * In render_player, spawn_players, and staff_ghosts D_802BFB80 is the arraySize8 entry
 * But in menu_item its the arraySize4 entry
 * The only way to unify those 2 things is to use a union
 */
typedef union {
    struct_D_802BFB80_4 arraySize4[2][2][4];
    struct_D_802BFB80_8 arraySize8[2][2][8];
} union_D_802BFB80;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 actorTimer;
    /* 0x0C */ u8 unk[16]; // 10 bytes
    /* 0x1C */ s8 unk1C;
    /* 0x1D */ s8 unk1D;
    /* 0x1E */ s8 unk1E; // gCharacterSelections
} struct_D_802874D8;

#define RENDER_SCREEN_MODE_1P_PLAYER_ONE PLAYER_ONE + SCREEN_MODE_1P
#define RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE PLAYER_ONE + SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL
#define RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO PLAYER_TWO + SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL
#define RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE PLAYER_ONE + SCREEN_MODE_2P_SPLITSCREEN_VERTICAL + 1
#define RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO PLAYER_TWO + SCREEN_MODE_2P_SPLITSCREEN_VERTICAL + 1
#define RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE PLAYER_ONE + SCREEN_MODE_3P_4P_SPLITSCREEN + 5
#define RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO PLAYER_TWO + SCREEN_MODE_3P_4P_SPLITSCREEN + 5
#define RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE PLAYER_THREE + SCREEN_MODE_3P_4P_SPLITSCREEN + 5
#define RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR PLAYER_FOUR + SCREEN_MODE_3P_4P_SPLITSCREEN + 5

#endif