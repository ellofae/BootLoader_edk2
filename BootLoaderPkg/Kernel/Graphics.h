#include <Uefi.h>
#include "../Video.h"

#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

typedef struct {
    UINT32 X;
    UINT32 Y;
    UINT32 Color;
} PIXEL;

typedef struct
{
    UINT32 X;
    UINT32 Y;
} POINT;

typedef struct
{
    POINT Start;
    POINT End;
    UINT32 Color;
} BLOCK;

UINT8
EFIAPI
VideoInitialize(
    IN BOOT_CONFIG *BootConfig
);

UINT8
EFIAPI
DrawBackground(
    IN BLOCK BackgroundBlock
);

UINT8
EFIAPI
DrawPixel(
    IN PIXEL Pixel
);

#endif