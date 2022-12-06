#pragma once

#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3dx11.lib")
#if _DEBUG
#pragma comment(lib, "x64/Debug/DirectXTex.lib")
#else
#pragma comment(lib, "x64/Release/DirectXTex.lib")
#endif

#define SCREEN_WIDTH	(960)
#define SCREEN_HEIGHT	(540)
#define FPS (60)

HWND GetWindow();
POINTS GetMousePos();