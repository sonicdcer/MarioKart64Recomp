#include "patches.h"

void dummy_function(void) {}

#if 1
RECOMP_PATCH void dma_copy(u8* dest, u8* romAddr, size_t size) {
    recomp_load_overlays((u32) romAddr, dest, size);

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