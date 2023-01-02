#pragma once

#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "d3d11.lib")


HWND GetWindow();
POINTS MouseAcquisition();
void SetCursorPosToClient(POINT pos);