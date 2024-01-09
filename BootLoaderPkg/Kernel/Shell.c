#include "Shell.h"

BASE ShellBase;

UINT8
EFIAPI
ShellInitialize(
)
{
    ShellBase.ShellConfig.Height = 860;
    ShellBase.ShellConfig.Width = 1400;
    ShellBase.ShellConfig.X = 20;
    ShellBase.ShellConfig.Y = 20;
    ShellBase.ShellConfig.OutLines = 1;

    BLOCK ShellBackground;
    ShellBackground.Color = 0xFF000000;
    ShellBackground.Start.X =  20;
    ShellBackground.Start.Y = 20;
    ShellBackground.End.X = 1420;
    ShellBackground.End.Y = 880;

    DrawBackground(ShellBackground);

    if(ShellBase.ShellConfig.OutLines)
    {    
        // Draw OutLine Up
        BLOCK OutLine;
        OutLine.Color = 0xFFFFFFFF;
        OutLine.Start.X =  20;
        OutLine.Start.Y = 20;
        OutLine.End.X = 1420;
        OutLine.End.Y = 22;
        DrawBackground(OutLine);
        
        // Draw OutLine Middle
        OutLine.Start.X =  22;
        OutLine.Start.Y = 68;
        OutLine.End.X = 1418;
        OutLine.End.Y = 70;
        DrawBackground(OutLine);

        // Draw OutLine Down
        OutLine.Start.X =  20;
        OutLine.Start.Y = 878;
        OutLine.End.X = 1420;
        OutLine.End.Y = 880;
        DrawBackground(OutLine);
    
        // Draw OutLine Left
        OutLine.Start.X =  20;
        OutLine.Start.Y = 22;
        OutLine.End.X = 22;
        OutLine.End.Y = 878;
        DrawBackground(OutLine);

        // Draw OutLine right
        OutLine.Start.X =  1418;
        OutLine.Start.Y = 22;
        OutLine.End.X = 1420;
        OutLine.End.Y = 878;
        DrawBackground(OutLine);
    }

    return 0;
}