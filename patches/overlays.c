#include "patches.h"

#define SEG_RACING_ (uintptr_t) 0x8028DF00
#define SEG_RACING_ROM_START_ (u8*) 0xf7510
#define SEG_RACING_SIZE_ (size_t) 0x2C470
#define SEG_RACING_ROM_SIZE_ (size_t) ALIGN16(0x0000000000123640 - 0x00000000000f7510)

#if 1
RECOMP_PATCH void dma_copy(u8* dest, u8* romAddr, size_t size) {
    recomp_load_overlays((u32) romAddr, dest, size);
    // recomp_printf("%08X %08X %08X\n", (u32) romAddr, (u32) dest, (u32) size);

    // osInvalDCache(dest, size);
    while (size > 0x100) {
        osPiStartDma(&gDmaIoMesg, 0, 0, (uintptr_t) romAddr, dest, 0x100, &gDmaMesgQueue);
        osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, 1);
        size -= 0x100;
        romAddr += 0x100;
        dest += 0x100;
    }
    if (size != 0) {
        osPiStartDma(&gDmaIoMesg, 0, 0, (uintptr_t) romAddr, dest, size, &gDmaMesgQueue);
        osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, 1);
    }
}
#endif

#if 0

RECOMP_PATCH void init_segment_racing(void) {
    recomp_load_overlays((u32) SEG_RACING_ROM_START_, (u32*) SEG_RACING_, SEG_RACING_ROM_SIZE_);
    bzero((void*) SEG_RACING_, SEG_RACING_SIZE_);
    // osWritebackDCacheAll();
    dma_copy((u8*) SEG_RACING_, (u8*) SEG_RACING_ROM_START_, SEG_RACING_ROM_SIZE_);
    // osInvalICache((void*) SEG_RACING_, SEG_RACING_SIZE_);
    // osInvalDCache((void*) SEG_RACING_, SEG_RACISEG_RACING_SIZE_NG_SIZE);
}
#endif

#if 0
#define SEG_ENDING_ (uintptr_t) 0x80280000
#define SEG_ENDING_ROM_START_ (uintptr_t) 0x0000000000123640
/**
 * Ending segment original size is 0xDF00 but much of that remains unused.
 * This auto fits the segment to its proper size.
 */
#define SEG_ENDING_SIZE_ (size_t) 0xDF00
#define SEG_ENDING_ROM_SIZE_ (size_t) ALIGN16((ptrdiff_t) (0x13aae0 - 0x123640))

RECOMP_PATCH void init_segment_ending_sequences(void) {
    recomp_load_overlays((u32) SEG_ENDING_ROM_START_, (u32*) SEG_ENDING_, SEG_ENDING_ROM_SIZE_);
    bzero((void*) SEG_ENDING_, SEG_ENDING_SIZE_);
    //  osWritebackDCacheAll();
    dma_copy((u8*) SEG_ENDING_, (u8*) SEG_ENDING_ROM_START_, SEG_ENDING_ROM_SIZE_);
    // osInvalICache((void*) SEG_ENDING_, SEG_ENDING_SIZE_);
    // osInvalDCache((void*) SEG_ENDING_, SEG_ENDING_SIZE_);
}
#endif
