#include "Kernel.h"

UINT8
EFIAPI
KernelStart(
    IN BOOT_CONFIG *BootConfig
)
{
    VideoInitialize(BootConfig);
    ShellInitialize();
    
    return 0;    
}