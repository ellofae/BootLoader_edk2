#include <Library/BaseMemoryLib.h>
#include <Protocol/GraphicsOutput.h>

#ifndef _VIDEO_H_
#define _VIDEO_H_

#include "File.h"

// typedef struct
// {
//     EFI_PHYSICAL_ADDRESS       FrameBufferBase;
//     UINT64                     FrameBufferSize;
// } BOOT_CONFIG;

typedef struct
{
    EFI_PHYSICAL_ADDRESS       FrameBufferBase;
    UINTN                      FrameBufferSize;
    UINT32                     HorizontalResolution;
    UINT32                     VerticalResolution;
    UINT32                     PixelsPerScanLine;
} VIDEO_CONFIG;

typedef struct
{
    VIDEO_CONFIG VideoConfig;
} BOOT_CONFIG;

typedef struct
{
    UINTN Size;
    UINTN PageSize;
    UINTN Width;
    UINTN Height;
    UINTN Offset;
    UINT64 PixelStart;
} BMP_CONFIG;

// BMP Header Structure
//
// struct bmp_header {
//   UINT8 sig[2];
//   UINT32 file_size;
//   UINT16 reserved1;
//   UINT16 reserved2;
//   UINT32 header_size;
//   UINT32 info_header_size;
//   UINT32 width;
//   UINT32 height;
//   UINT16 plane_num;
//   UINT16 color_bit;
//   UINT32 compression_type;
//   UINT32 compression_size;
//   UINT32 horizontal_pixel;
//   UINT32 vertical_pixel;
//   UINT32 color_num;
//   UINT32 essentail_num;
// };

EFI_STATUS 
EFIAPI
GetGopHandle(
    IN EFI_HANDLE ImageHandle,
    OUT EFI_GRAPHICS_OUTPUT_PROTOCOL **Gop
);

EFI_STATUS
EFIAPI
SetVideoMode(
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop
);

UINTN
EFIAPI
GetValue(
    IN EFI_PHYSICAL_ADDRESS StartAddress,
    IN UINTN Offset,
    IN UINTN Size
);

EFI_STATUS
EFIAPI
BmpTransform(
    IN EFI_PHYSICAL_ADDRESS BmpBase,
    OUT BMP_CONFIG *BmpConfig
);

EFI_STATUS
EFIAPI
DrawBmp(
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
    IN BMP_CONFIG BmpConfig,
    IN UINTN X,
    IN UINTN Y
);

// int DrawPixel(
//     UINT32 *VideoHandle, 
//     UINT32 Color, 
//     UINTN X, 
//     UINTN Y
// );

// UINT64 
// EFIAPI
// DrawBackground(
//     BOOT_CONFIG *BootConfig
// );

#endif