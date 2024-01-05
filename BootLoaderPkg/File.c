#include "File.h"

EFI_STATUS
EFIAPI
GetFileHandle(
    IN EFI_HANDLE ImageHandle,
    IN CHAR16 *FileName,
    OUT EFI_FILE_PROTOCOL **FileHandle
)
{
    EFI_STATUS Status = EFI_SUCCESS;

    UINTN NoHandles = 0;
    EFI_HANDLE *Buffer;
    Status = gBS->LocateHandleBuffer(
        ByProtocol,
        &gEfiSimpleFileSystemProtocolGuid,
        NULL,
        &NoHandles,
        &Buffer
    );
    if(EFI_ERROR(Status))
    {
        Print(L"Error: Unable to LocateHandleBuffer with SimpleFileSystemProtocol.\n");
        return Status;
    } 

    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
    Status = gBS->OpenProtocol(
        Buffer[0],
        &gEfiSimpleFileSystemProtocolGuid,
        (VOID**)&FileSystem,
        ImageHandle,
        NULL,
        EFI_OPEN_PROTOCOL_GET_PROTOCOL
    );
    if(EFI_ERROR(Status))
    {
        Print(L"Error: Unable to OpenProtocol with SimpleFileSystemProtocol.\n");
        return Status;
    } 

    EFI_FILE_PROTOCOL *Root;
    Status = FileSystem->OpenVolume(
        FileSystem,
        &Root
    );
    if(EFI_ERROR(Status))
    {
        Print(L"Error: Unable to OpenVolume with SimpleFileSystemProtocol.\n");
        return Status;
    } 

    Status = Root->Open(
        Root,
        FileHandle,
        FileName,
        EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
        EFI_OPEN_PROTOCOL_GET_PROTOCOL
    );
    if(EFI_ERROR(Status))
    {
        Print(L"Error: Unable to Open the file with SimpleFileSystemProtocol.\n");
        return Status;
    } 
    
    return Status;
}

EFI_STATUS
EFIAPI
ReadFile(
    IN EFI_FILE_PROTOCOL *File,
    OUT EFI_PHYSICAL_ADDRESS *FileBase
)
{
    EFI_STATUS Status = EFI_SUCCESS;

    EFI_FILE_INFO *FileInfo;
    UINTN InfoSize = sizeof(EFI_FILE_INFO) + 128;
    Status = gBS->AllocatePool(
        EfiLoaderData,
        InfoSize,
        (VOID **)&FileInfo
    );
    if(EFI_ERROR(Status))
    {
        Print(L"Error: Unable to AllocatePool for EFI_FILE_INFO interface.\n");
        return Status;
    } 
    
    Status = File->GetInfo(
        File,
        &gEfiFileInfoGuid,
        &InfoSize,
        FileInfo
    );
    if(EFI_ERROR(Status))
    {
        Print(L"Error: Unable to GetInfo for EFI_FILE_INFO interface.\n");
        return Status;
    } 

    UINTN FilePageSize = (FileInfo->FileSize >> 12) + 1; // get the amount of 4096 bytes memory pages
    EFI_PHYSICAL_ADDRESS FileBufferAddress;
    Status = gBS->AllocatePages(
        AllocateAnyPages,
        EfiLoaderData,
        FilePageSize,
        &FileBufferAddress
    );
    if(EFI_ERROR(Status))
    {
        Print(L"Error: Unable to AllocatePages for FileBufferAddress.\n");
        return Status;
    } 
    Print(L"File size: %d\n", FileInfo->FileSize);
    Print(L"Memory pages (each 4096 bytes): %d\n", (FileInfo->FileSize >> 12) + 1);

    UINTN ReadSize = FileInfo->FileSize;
    Status = File->Read(
        File,
        &ReadSize,
        (VOID*)FileBufferAddress
    );
    if(EFI_ERROR(Status))
    {
        Print(L"Error: Unable to Read the file\n");
        return Status;
    } 

    gBS->FreePool(FileInfo);
    *FileBase = FileBufferAddress;

    return Status;
}