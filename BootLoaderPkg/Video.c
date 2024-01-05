#include "Video.h"

EFI_STATUS 
EFIAPI
GetGopHandle(
    IN EFI_HANDLE ImageHandle,
    OUT EFI_GRAPHICS_OUTPUT_PROTOCOL **Gop
)
{
    EFI_STATUS Status = EFI_SUCCESS;

    UINTN NoHandles = 0;
    EFI_HANDLE *Buffer;
    Status = gBS->LocateHandleBuffer(
        ByProtocol,
        &gEfiGraphicsOutputProtocolGuid,
        NULL,
        &NoHandles,
        &Buffer
    );
    if(EFI_ERROR(Status))
    {
        Print(L"Error: Unable to LocateHandleBuffer with GraphicsOutputProtocol.\n");
        return Status;
    }

    Status = gBS->OpenProtocol(
        Buffer[0],
        &gEfiGraphicsOutputProtocolGuid,
        (VOID **)Gop,
        ImageHandle,
        NULL,
        EFI_OPEN_PROTOCOL_GET_PROTOCOL
    );
    if(EFI_ERROR(Status))
    {
        Print(L"Error: Unable to OpenProtocol with GraphicsOutputProtocol.\n");
        return Status;
    }

    return Status;

}

EFI_STATUS
EFIAPI
SetVideoMode(
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop
)
{
    EFI_STATUS Status = EFI_SUCCESS;

    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *ModeInfo;
    UINT32 H = 0;
    UINT32 V = 0;
    UINTN ModeIndex = 0;

    for(UINTN i = 0; i < Gop->Mode->MaxMode; i++)
    {
        Status = Gop->QueryMode(
            Gop,
            i,
            &Gop->Mode->SizeOfInfo,
            &ModeInfo
        );
        if(EFI_ERROR(Status))
        {
            Print(L"Error: Unable to QueryMode with GraphicsOutputProtocol.\n");
            return Status;
        }
        
        H = ModeInfo->HorizontalResolution;
        V = ModeInfo->VerticalResolution;
        if(((H == 1024) && (V == 768)) || ((H == 1440) && (V == 900)))
        {    
            ModeIndex = i;            
        }
    }

    Status = Gop->SetMode(
        Gop,
        ModeIndex
    );
    if(EFI_ERROR(Status))
    {
        Print(L"Error: Unable to SetMode with GraphicsOutputProtocol.\n");
        return Status;
    }

    return Status;
}

UINTN
EFIAPI
GetValue(
    IN EFI_PHYSICAL_ADDRESS StartAddress,
    IN UINTN Offset,
    IN UINTN Size
)
{
    UINT8 *ByteStart = (UINT8 *)StartAddress + Offset;
    UINTN Result = 0;
    for(UINTN i = 0; i < Size; i++)
    {
        Result += *(ByteStart + i) << i * 8;
    }
    return Result;
}

EFI_STATUS
EFIAPI
BmpTransform(
    IN EFI_PHYSICAL_ADDRESS BmpBase,
    OUT BMP_CONFIG *BmpConfig
)
{
    EFI_STATUS Status = EFI_SUCCESS;

    BmpConfig->Size = GetValue(BmpBase, 0x02, 4);
    BmpConfig->PageSize = (BmpConfig->Size >> 12) + 1;
    BmpConfig->Offset = GetValue(BmpBase, 0x0A, 4); // header size
    BmpConfig->Width = GetValue(BmpBase, 0x12, 4);
    BmpConfig->Height = GetValue(BmpBase, 0x16, 4);

    EFI_PHYSICAL_ADDRESS PixelStart;
    Status = gBS->AllocatePages(
        AllocateAnyPages,
        EfiLoaderData,
        BmpConfig->PageSize,
        &PixelStart
    );
    if(EFI_ERROR(Status))
    {
        Print(L"Error: Unable to AllocatePages for PixelStart.\n");
        return Status;
    }

    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *PixelFromFile = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)(BmpBase + BmpConfig->Offset + BmpConfig->Width * BmpConfig->Height * 4); // 4 bytes each pixel
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *PixelToBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)PixelStart;

    for(UINTN i = 0; i < BmpConfig->Height; i++)
    {
        PixelFromFile -= BmpConfig->Width;
        for(UINTN j = 0; j < BmpConfig->Width; j++)
        {
            *PixelToBuffer = *PixelFromFile;
            PixelToBuffer++;
            PixelFromFile++;
        }
        PixelFromFile -= BmpConfig->Width;
    }

    BmpConfig->PixelStart = PixelStart;

    return Status;
}

EFI_STATUS
EFIAPI
DrawBmp(
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
    IN BMP_CONFIG BmpConfig,
    IN UINTN X,
    IN UINTN Y
)
{
    EFI_STATUS Status = EFI_SUCCESS;

    Status = Gop->Blt(
        Gop,
        (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)BmpConfig.PixelStart,
        EfiBltBufferToVideo,
        0,0,
        X,Y,
        BmpConfig.Width,BmpConfig.Height,
        0
    );
    if(EFI_ERROR(Status))
    {
        Print(L"Error: Unable to Draw BMP.\n");
        return Status;
    }


    return Status;
}

int DrawPixel(
    UINT32 *VideoHandle, 
    UINT32 Color, 
    UINTN X, 
    UINTN Y
)
{
    UINT32 *Pixel = VideoHandle;
    Pixel += Y * 1440 + X;
    *Pixel = Color;

    return 0;
}

UINT64 
EFIAPI
DrawBackground(
    BOOT_CONFIG *BootConfig
) {
    UINT64 PassBack = 0;
    UINT32 DarkBlue = 0xFF18679A;
    UINT32 *VideoHandle = (UINT32 *)(BootConfig->FrameBufferBase);

    for(UINTN i = 0; i < 900; i++)
    {
        for(UINTN j = 0; j < 1440; j++)
        {
            DrawPixel(VideoHandle, DarkBlue, j, i);
        }
    }
    PassBack = BootConfig->FrameBufferBase;
    return PassBack;
}