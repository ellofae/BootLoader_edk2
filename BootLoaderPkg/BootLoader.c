#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include "Elf.h"
#include "Motion.h"

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

    return Status;
}