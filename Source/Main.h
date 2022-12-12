#pragma once

#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "d3d11.lib")

#ifdef _DEBUG
#pragma comment(lib, "DirectXTK_Debug.lib")
#else
#pragma comment(lib, "DirectXTK.lib")
#endif


#define SCREEN_WIDTH	(960)
#define SCREEN_HEIGHT	(540)
#define FPS (60)

HWND GetWindow();
POINTS GetMousePos();