#include <Uefi.h>
#include "Graphics.h"

typedef struct {
    UINT32 Width;
    UINT32 Height;
    UINT32 X;
    UINT32 Y;
    UINT32 OutLines;
} SETUP;

typedef struct {
    SETUP ShellConfig;
} BASE;

UINT8
EFIAPI
ShellInitialize();