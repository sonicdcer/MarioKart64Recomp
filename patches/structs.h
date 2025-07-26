#ifndef RECOMP_STRUCTS
#define RECOMP_STRUCTS

#include "patches.h"

enum SeqId {
    SEQ_SOUND_PLAYER,                           // 0x00 (00)
    SEQ_MENU_TITLE_SCREEN,                      // 0x01 (01)
    SEQ_MENU_MAIN_MENU,                         // 0x02 (02)
    SEQ_TRACK_RACEWAY,                          // 0x03 (03)
    SEQ_TRACK_FARM,                             // 0x04 (04)
    SEQ_TRACK_MOUNTAIN,                         // 0x05 (05)
    SEQ_TRACK_BEACH,                            // 0x06 (06)
    SEQ_TRACK_SCARY,                            // 0x07 (07)
    SEQ_TRACK_SNOW,                             // 0x08 (08)
    SEQ_TRACK_CASTLE,                           // 0x09 (09)
    SEQ_TRACK_DESERT,                           // 0x0A (10)
    SEQ_EVENT_RACE_STARTING,                    // 0x0B (11)
    SEQ_EVENT_RACE_FINAL_LAP,                   // 0x0C (12)
    SEQ_EVENT_RACE_FINISH_FIRST,                // 0x0D (13)
    SEQ_EVENT_RACE_FINISH_OTHER,                // 0x0E (14)
    SEQ_EVENT_RACE_FINISH_LOSE,                 // 0x0F (15)
    SEQ_MENU_RESULTS_SCREEN_WIN,                // 0x10 (16)
    SEQ_EVENT_RACE_POWERUP_STAR,                // 0x11 (17)
    SEQ_TRACK_RAINBOW,                          // 0x12 (18)
    SEQ_TRACK_JUNGLE,                           // 0x13 (19)
    SEQ_EVENT_CEREMONY_TROPHY_LOSE,             // 0x14 (20)
    SEQ_TRACK_TURNPIKE,                         // 0x15 (21)
    SEQ_EVENT_RACE_STARTING_VS,                 // 0x16 (22)
    SEQ_MENU_RESULTS_SCREEN_WIN_VS,             // 0x17 (23)
    SEQ_MENU_RESULTS_SCREEN_LOSE,               // 0x18 (24)
    SEQ_TRACK_BATTLE,                           // 0x19 (25)
    SEQ_EVENT_CEREMONY_PRESENTATION_PART1,      // 0x1A (26)
    SEQ_EVENT_CEREMONY_PRESENTATION_PART2_WIN,  // 0x1B (27)
    SEQ_EVENT_CEREMONY_TROPHY_CREDITS,          // 0x1C (28)
    SEQ_EVENT_CEREMONY_PRESENTATION_PART2_LOSE, // 0x1D (29)
	SEQ_COUNT
};

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

#define CHANNELS_MAX 16
#define SEQUENCE_PLAYERS 4
#define SEQ_PLAYER_LEVEL 0 // Level background music
#define SEQ_PLAYER_ENV 1   // Misc music like the puzzle jingle
#define SEQ_PLAYER_SFX 2   // Sound effects

struct NotePool;

struct AudioListItem {
    // A node in a circularly linked list. Each node is either a head or an item:
    // - Items can be either detached (prev = NULL), or attached to a list.
    //   'value' points to something of interest.
    // - List heads are always attached; if a list is empty, its head points
    //   to itself. 'count' contains the size of the list.
    // If the list holds notes, 'pool' points back to the pool where it lives.
    // Otherwise, that member is NULL.
    struct AudioListItem* prev;
    struct AudioListItem* next;
    union {
        void* value; // either Note* or SequenceChannelLayer*
        s32 count;
    } u;
    struct NotePool* pool;
}; // size = 0x10

struct NotePool {
    struct AudioListItem disabled;
    struct AudioListItem decaying;
    struct AudioListItem releasing;
    struct AudioListItem active;
};

struct M64ScriptState {
    u8* pc;
    u8* stack[4];
    u8 remLoopIters[4];
    u8 depth;
}; // size = 0x1C

// Also known as a Group, according to debug strings.
struct SequencePlayer {
    /*US/JP, EU,    SH   */
    /*0x000, 0x000, 0x000*/ u8 enabled : 1;
    /*0x000, 0x000*/ u8 finished : 1; // never read
    /*0x000, 0x000*/ u8 muted : 1;
    /*0x000, 0x000*/ u8 seqDmaInProgress : 1;
    /*0x000, 0x000*/ u8 bankDmaInProgress : 1;
    /*       0x000*/ u8 recalculateVolume : 1;
    /*0x002, 0x001, 0x001*/ u8 state;
    /*0x003, 0x002*/ u8 noteAllocPolicy;
    /*0x004, 0x003*/ u8 muteBehavior;
    /*0x005, 0x004*/ u8 seqId;
    /*0x006, 0x005*/ u8 defaultBank[1]; // must be an array to get a comparison
                                        // to match; other u8's might also be part of that array
    /*0x007, 0x006*/ u8 loadingBankId;
    /*     , 0x007, 0x007*/ s8 seqVariationEu[1];
    /*0x00A, 0x008*/ u16 tempo; // beats per minute in JP, tatums per minute in US/EU
    /*0x00C, 0x00A*/ u16 tempoAcc;
    /*0x010, 0x00C, 0x00E*/ s16 transposition;
    /*0x012, 0x00E, 0x010*/ u16 delay;
    /*0x00E, 0x010, 0x012*/ u16 fadeRemainingFrames;
    /*     , 0x012, 0x014*/ u16 fadeTimerUnkEu;
    /*0x014, 0x014*/ u8* seqData;          // buffer of some sort
    /*0x018, 0x018, 0x1C*/ f32 fadeVolume; // set to 1.0f
    /*0x01C, 0x01C*/ f32 fadeVelocity;     // set to 0.0f
    /*0x020, 0x020, 0x024*/ f32 volume;    // set to 0.0f
    /*0x024, 0x024*/ f32 muteVolumeScale;  // set to 0.5f
    /*     , 0x028, 0x02C*/ f32 fadeVolumeScale;
    /*     , 0x02C*/ f32 appliedFadeVolume;
    /*0x02C, 0x030, 0x034*/ struct SequenceChannel* channels[CHANNELS_MAX];
    /*0x06C, 0x070*/ struct M64ScriptState scriptState;
    /*0x088, 0x08C*/ u8* shortNoteVelocityTable;
    /*0x08C, 0x090*/ u8* shortNoteDurationTable;
    /*0x090, 0x094*/ struct NotePool notePool;
    /*0x0D0, 0x0D4*/ OSMesgQueue seqDmaMesgQueue;
    /*0x0E8, 0x0EC*/ OSMesg seqDmaMesg;
    /*0x0EC, 0x0F0*/ OSIoMesg seqDmaIoMesg;
    /*0x100, 0x108*/ OSMesgQueue bankDmaMesgQueue;
    /*0x118, 0x120*/ OSMesg bankDmaMesg;
    /*0x11C, 0x124*/ OSIoMesg bankDmaIoMesg;
    /*0x130, 0x13C*/ u8* bankDmaCurrMemAddr;
    /*0x138, 0x140*/ uintptr_t bankDmaCurrDevAddr;
    /*0x13C, 0x144*/ ssize_t bankDmaRemaining;
}; // size = 0x140, 0x148 on EU, 0x14C on SH

typedef struct {
    s16 type;
    Vec3s unk2;
    s8 unk8;
    s8 unk9;
    s8 unkA;
    s8 unkB;
} ActorInitParams;

typedef struct {
    /* 0x00 */ s32 isActive;
    /* 0x04 */ s16 type;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s8 unk8;
    /* 0x09 */ s8 unk9;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ Vec3f pos;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ ActorInitParams* initParams;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ s32 timer; // Used to delete the actor after some time.
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
} CeremonyActor;

typedef struct {
    /* 0x00 */ s32 isActive;
    /* 0x04 */ s16 type;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s8 unk8;
    /* 0x09 */ s8 unk9;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ Vec3f pos;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ ActorInitParams* initParams;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30; // s32 instead of s16
    /* 0x34 */ f32 unk34; // f32 instead of s16
    /* 0x38 */ f32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
} Firework;

#define RENDER_SCREEN_MODE_1P_PLAYER_ONE PLAYER_ONE + SCREEN_MODE_1P
#define RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE PLAYER_ONE + SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL
#define RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO PLAYER_TWO + SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL
#define RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE PLAYER_ONE + SCREEN_MODE_2P_SPLITSCREEN_VERTICAL + 1
#define RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO PLAYER_TWO + SCREEN_MODE_2P_SPLITSCREEN_VERTICAL + 1
#define RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE PLAYER_ONE + SCREEN_MODE_3P_4P_SPLITSCREEN + 5
#define RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO PLAYER_TWO + SCREEN_MODE_3P_4P_SPLITSCREEN + 5
#define RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE PLAYER_THREE + SCREEN_MODE_3P_4P_SPLITSCREEN + 5
#define RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR PLAYER_FOUR + SCREEN_MODE_3P_4P_SPLITSCREEN + 5

#define VIRTUAL_TO_PHYSICAL_RECOMP(addr) ((u32) (addr) & 0x1FFFFFFF)
#define END_OF_SPAWN_DATA -0x8000

#endif