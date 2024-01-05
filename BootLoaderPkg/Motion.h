// #include "Video.h"
// #include "File.h"
#include "Elf.h"

#define GAP 1

EFI_STATUS
EFIAPI
VideoInitialization(
    IN EFI_HANDLE ImageHandle, 
    OUT VIDEO_CONFIG *VideoConfig
);

EFI_STATUS
EFIAPI
DrawLogo(
    IN EFI_HANDLE ImageHandle,
    IN VIDEO_CONFIG *VideoConfig
);

EFI_STATUS
EFIAPI
DrawStep(
    IN UINTN Step
);

UINT64 
EFIAPI
DrawBackground(
    BOOT_CONFIG *BootConfig
);