#include <Uefi.h>
#include <Guid/FileInfo.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/SimpleFileSystem.h>

EFI_STATUS
EFIAPI
GetFileHandle(
    IN EFI_HANDLE ImageHandle,
    IN CHAR16 * FileName,
    OUT EFI_FILE_PROTOCOL **FileHandle
);

EFI_STATUS
EFIAPI
ReadFile(
    IN EFI_FILE_PROTOCOL *File,
    OUT EFI_PHYSICAL_ADDRESS *FileBase
);