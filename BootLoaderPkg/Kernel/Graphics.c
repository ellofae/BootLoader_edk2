#include "Graphics.h"

VIDEO_CONFIG *VideoConfig;
UINT32 *VideoStart;

UINT8
EFIAPI
VideoInitialize(
    IN BOOT_CONFIG *BootConfig
)
{
    VideoConfig = &BootConfig->VideoConfig;
    VideoStart = (UINT32 *) BootConfig->VideoConfig.FrameBufferBase;
    
    BLOCK BackgroundBlock;

    BackgroundBlock.Start.X = 0;
    BackgroundBlock.Start.Y = 0;
    BackgroundBlock.End.X = BootConfig->VideoConfig.HorizontalResolution - 1;
    BackgroundBlock.End.Y = BootConfig->VideoConfig.VerticalResolution - 1;
    BackgroundBlock.Color = 0xFF18679A; // 4 bytes

    DrawBackground(BackgroundBlock);

    return 0;
}

UINT8
EFIAPI
DrawPixel(
    IN PIXEL Pixel
)
{
    UINT32 *PixelPosition = VideoStart + (Pixel.Y - 1) * VideoConfig->PixelsPerScanLine + Pixel.X;
    *PixelPosition = Pixel.Color;

    return 0;
}

UINT8
EFIAPI
DrawBackground(
    IN BLOCK BackgroundBlock
)
{
    PIXEL Pixel;
    Pixel.Color = BackgroundBlock.Color;
    for(UINT32 i = BackgroundBlock.Start.Y; i < BackgroundBlock.End.Y; i++)
    {   
        for(UINT32 j = BackgroundBlock.Start.X; j < BackgroundBlock.End.X; j++)
        {
            Pixel.X = j;
            Pixel.Y = i;
            DrawPixel(Pixel);
        }
    }

    return 0;
}