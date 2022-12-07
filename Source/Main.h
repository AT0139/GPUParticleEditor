#pragma once

#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "d3d11.lib")
//#pragma comment (lib, "d3dx9.lib")
//#pragma comment (lib, "d3dx11.lib")

#define SCREEN_WIDTH	(960)
#define SCREEN_HEIGHT	(540)
#define FPS (60)

HWND GetWindow();
POINTS GetMousePos();