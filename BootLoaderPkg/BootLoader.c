#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include "Motion.h"

EFI_STATUS
EFIAPI
ByeBootServices(
    IN EFI_HANDLE ImageHandle
);

EFI_STATUS
EFIAPI
UefiMain(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
)
{
    EFI_STATUS Status = EFI_SUCCESS;

    VIDEO_CONFIG VideoConfig;
    Status = VideoInitialization(ImageHandle, &VideoConfig);
    if(EFI_ERROR(Status))
    {
        Print(L"Error Info: Status code - %d.\n", Status);
        return Status;
    }

    Status = DrawLogo(ImageHandle, &VideoConfig);
    if(EFI_ERROR(Status))
    {
        Print(L"Error Info: Status code - %d.\n", Status);
        return Status;
    }

    EFI_PHYSICAL_ADDRESS KernelEntryPoint;
    Status = Relocate(
        ImageHandle, 
        L"\\Kernel.elf", 
        &KernelEntryPoint
    );
    if(EFI_ERROR(Status))
    {
        Print(L"Error Info: Status code - %d.\n", Status);
        return Status;
    }

    BOOT_CONFIG BootConfig;
    BootConfig.FrameBufferBase = VideoConfig.FrameBufferBase;
    BootConfig.FrameBufferSize = VideoConfig.FrameBufferSize;

    gBS->Stall(3000000);
    DrawBackground(&BootConfig);

    return Status;
}