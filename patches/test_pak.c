#if 0
#include "patches.h"

extern OSPfs gControllerPak1FileHandle;
extern u16 gCompanyCode;
extern u32 gGameCode;
extern const u8 gGameName[];
extern const u8 gExtCode[];
static u32 ghost_pak_emu[0x3C00] = { 0 };

void testPak(void);
s32 osPfsReadWriteFile_recomp(OSPfs* pfs, s32 file_no, u8 flag, int offset, int size_in_bytes, u8* data_buffer);
s32 osPfsFindFile_recomp(OSPfs* pfs, u16 company_code, u32 game_code, u8* game_name, u8* ext_name, s32* file_no);
void move_segment_table_to_dmem(void);
void init_rdp(void);
void set_viewport(void);
void select_framebuffer(void);
void init_z_buffer(void);
s32 func_800B6088(s32 arg0);
s32 func_800051C4(void);

void* memcpy2(void* dest, const void* src, size_t size) {
    unsigned char* su1 = (unsigned char*) dest;
    const unsigned char* su2 = (const unsigned char*) src;

    while (size > 0) {
        *su1 = *su2;
        su1++;
        su2++;
        size--;
    }

    return dest;
}

RECOMP_PATCH void init_rcp(void) {

    move_segment_table_to_dmem();
    init_rdp();
    set_viewport();
    select_framebuffer();
    init_z_buffer();
}

void testPak(void) {
    int success = 0;
    u8 buffer[256];
    u8 buffer2[256];
    recomp_printf("PAKTEST START\n");

    osPfsFindFile_recomp(&gControllerPak1FileHandle, gCompanyCode, gGameCode, (u8*) gGameName, (u8*) gExtCode,
                         &gControllerPak1FileNote);
    osPfsFindFile_recomp(&gControllerPak1FileHandle, gCompanyCode, gGameCode, (u8*) gGameName, (u8*) gExtCode,
                         &gControllerPak1FileNote);

    for (size_t i = 0; i < 256; i++) {
        recomp_printf("Preparing buffer: %d\n", i);
        buffer[i] = i;
    }

    osPfsReadWriteFile_recomp(&gControllerPak1FileHandle, 0, PFS_WRITE, 0, ARRAY_COUNT(buffer), (u8*) buffer);

    osPfsFindFile_recomp(&gControllerPak1FileHandle, gCompanyCode, gGameCode, (u8*) gGameName, (u8*) gExtCode,
                         &gControllerPak1FileNote);

    for (size_t i = 0; i < 256; i++) {
        buffer2[i] = -1;
    }

    osPfsReadWriteFile_recomp(&gControllerPak1FileHandle, 0, PFS_READ, 0, ARRAY_COUNT(buffer2), (u8*) buffer2);

    for (size_t i = 0; i < 256; i++) {
        recomp_printf("ReadBack: %d\n", buffer2[i]);
    }
}

extern u32* D_800DC714;
extern struct_8018EE10_entry D_8018EE10[2];
extern u16 bPlayerGhostDisabled;
extern s32 D_80162DE0;
extern s32 D_80162DFC;
void func_8000522C(void);
extern u8 func_800B60E8(s32 page);

#if 0
RECOMP_PATCH s32 func_800B64EC(s32 arg0) {
    s32 temp_s0;
    s32 readSuccess;
    u8* phi_s1;

    if ((arg0 != 0) && (arg0 != 1)) {
        return -1;
    }
    
    recomp_printf("osPfsReadWriteFile\n");
    recomp_printf("PAGE: %d\n", arg0);
   // readSuccess = osPfsReadWriteFile_recomp(&gControllerPak1FileHandle, gControllerPak1FileNote, PFS_READ, (arg0 * 0x3C00) + 0x100,
   //                              0x3C00, (u8*) D_800DC714);
   readSuccess = 0;
   memcpy2(D_800DC714, ghost_pak_emu, 0x3C00);

       // u8* ptr = (u8*) (D_800DC714 );
       // for (size_t i = 0; i < 0x3C00; i++) {
       //     recomp_printf("%x ", ptr[i]);
       // }

        recomp_printf("\n");
    if (readSuccess == 0) {
        recomp_printf("readSuccess pass\n");
        phi_s1 = (u8 *) &D_8018EE10[arg0]; temp_s0 = 0; 

        while (1) {
            if (phi_s1[7] != func_800B60E8(temp_s0)) {
                recomp_printf("phi_s1[7] != func_800B60E8(temp_s0)\n");
                D_8018EE10[arg0].ghostDataSaved = 0;
                return -2;
            }

            ++phi_s1;
            if ((++temp_s0) == 60) {
                recomp_printf("(++temp_s0) == 60\n");
                func_8000522C();
                u16* bPlayerGhostDisabled_ = (u16*) 0x80162f44;
                *bPlayerGhostDisabled_ = 0;
                D_80162DE0 = (s32) D_8018EE10[arg0].characterId;
                D_80162DFC = D_8018EE10[arg0].unk_00;
                break;
            }
        }
    }
    recomp_printf("return readSuccess\n");
    return readSuccess;
}
#endif

#if 0
RECOMP_PATCH s32 func_800B6178(s32 arg0) {
    s32 var_v0;
    s32 var_s0;
    struct_8018EE10_entry* temp_s3;

    switch (arg0) {
        case 0:
        case 1:
            break;
        default:
            return -1;
    }
    if (gGamestate == RACING) {
        func_800051C4();
    }
    temp_s3 = &D_8018EE10[arg0];
    temp_s3->ghostDataSaved = 0;
    var_v0 = func_800B6088(arg0);
    if (var_v0 != 0) {
        temp_s3->ghostDataSaved = 0;
        for (var_s0 = 0; var_s0 < 0x3C; var_s0++) {
            temp_s3->unk_07[var_s0] = var_s0;
        }
    } else {
        var_v0 = osPfsReadWriteFile_recomp(&gControllerPak1FileHandle, gControllerPak1FileNote, 1U, (arg0 * 0x3C00) + 0x100,
                                    0x00003C00, (u8*) D_800DC714);
        
        memcpy2(ghost_pak_emu, D_800DC714, 0x3C00);

        if (var_v0 == 0) {
            temp_s3->ghostDataSaved = 1;
            if (gGamestate == RACING) {
                temp_s3->courseIndex = (gCupSelection * 4) + gCourseIndexInCup;
            }
            temp_s3->unk_00 = D_80162DFC;
            temp_s3->characterId = (u8) D_80162DE0;
            for (var_s0 = 0; var_s0 < 0x3C; var_s0++) {
                temp_s3->unk_07[var_s0] = func_800B60E8(var_s0);
            }
            var_v0 = func_800B6088(arg0);
        }
        if (var_v0 != 0) {
            temp_s3->ghostDataSaved = 0;
            for (var_s0 = 0; var_s0 < 0x3C; var_s0++) {
                temp_s3->unk_07[var_s0] = var_s0;
            }
        }
    }
    return var_v0;
}
#endif

#endif