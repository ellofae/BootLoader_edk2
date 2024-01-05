#include "Motion.h"

EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;

EFI_STATUS
EFIAPI
VideoInitialization(
    IN EFI_HANDLE ImageHandle, OUT VIDEO_CONFIG *VideoConfig
)
{
    EFI_STATUS Status = EFI_SUCCESS;
    
    Status = GetGopHandle(ImageHandle, &Gop);
    if(EFI_ERROR(Status))
    {
        Print(L"Error while function call: Unable to aquire EFI_GRAPHICS_OUTPUT_PROTOCOL interface.\n");
        return Status;
    } 
    Print(L"EFI_GRAPHICS_OUTPUT_PROTOCOL interface is acquired.\n");

    Status = SetVideoMode(Gop);
    if(EFI_ERROR(Status))
    {
        Print(L"Error while function call: Unable to set video mode.\n");
        return Status;
    }
    Print(L"Video Mode is set.\n");

    VideoConfig->FrameBufferBase = Gop->Mode->FrameBufferBase;
    VideoConfig->FrameBufferSize = Gop->Mode->FrameBufferSize;
    VideoConfig->HorizontalResolution = Gop->Mode->Info->HorizontalResolution;
    VideoConfig->VerticalResolution = Gop->Mode->Info->VerticalResolution;
    VideoConfig->PixelsPerScanLine = Gop->Mode->Info->PixelsPerScanLine;

    return Status;
}

EFI_STATUS
EFIAPI
DrawLogo(
    IN EFI_HANDLE ImageHandle,
    IN VIDEO_CONFIG *VideoConfig
)
{
    EFI_STATUS Status = EFI_SUCCESS;

    CHAR16 *FileName = L"\\Logo4.BMP"; 
    UINTN Hor = VideoConfig->HorizontalResolution;
    UINTN Ver = VideoConfig->VerticalResolution;

    EFI_FILE_PROTOCOL *Logo;
    Status = GetFileHandle(ImageHandle, FileName, &Logo);
    if(EFI_ERROR(Status))
    {
        Print(L"Error while function call: Unable to get file handler.\n");
        return Status;
    }

    EFI_PHYSICAL_ADDRESS LogoAddress;
    Status = ReadFile(Logo, &LogoAddress);
    if(EFI_ERROR(Status))
    {
        Print(L"Error while function call: Unable to read the file.\n");
        return Status;
    }
    Print(L"The file is read.\n");

    BMP_CONFIG BmpConfig;
    Status = BmpTransform(LogoAddress, &BmpConfig);
    if(EFI_ERROR(Status))
    {
        Print(L"Error while function call: Unable to transform BMP.\n");
        return Status;
    }
    Print(L"BMP is transformed.\n");

    INTN X = (Hor - BmpConfig.Width) / 2;
    UINTN Y = (Ver - BmpConfig.Height) / 2;

    Status = DrawBmp(Gop, BmpConfig, X, Y);
    if(EFI_ERROR(Status))
    {
        Print(L"Error while function call: Unable to display BMP image.\n");
        return Status;
    }

    for(UINTN i = 0; i < 10; i++)
    {
        Status = DrawStep(i);
        if(EFI_ERROR(Status))
        {
            Print(L"Error while function call: Unable to dispaly loading elements.\n");
            return Status;
        }
    }

    return Status;
}

EFI_STATUS
EFIAPI
DrawStep(
    IN UINTN Step
)
{
    EFI_STATUS Status = EFI_SUCCESS;

    UINTN BlockWidth = Gop->Mode->Info->HorizontalResolution >> 6;
    UINTN BlockHeight = Gop->Mode->Info->VerticalResolution >> 6;
    UINTN StartX = (Gop->Mode->Info->HorizontalResolution - (BlockWidth + GAP) * 10 - GAP) / 2;
    UINTN StartY = (Gop->Mode->Info->VerticalResolution * 3) >> 2;

    EFI_GRAPHICS_OUTPUT_BLT_PIXEL WhitePixel = { 255, 255, 255, 0};
    
    Status = Gop->Blt(
        Gop, 
        &WhitePixel, 
        EfiBltVideoFill, 
        0, 0, 
        StartX + (BlockWidth + GAP) * Step, StartY, 
        BlockWidth, BlockHeight, 
        0
    );
    if(EFI_ERROR(Status))
    {
        Print(L"Error: Unable to dispaly loading element on step %d.\n", Step);
        return Status;
    }


    return Status;
}