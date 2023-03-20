#pragma once

#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "dxgi.lib")

HWND GetWindow();
POINTS MouseAcquisition();
void SetCursorPosToClient(POINT pos);
int GetMouseWheel();