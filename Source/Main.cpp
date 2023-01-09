#include "main.h"
#include "manager.h"
#include "Renderer.h"

const char* CLASS_NAME = "AppClass";
const char* WINDOW_NAME = "DX11ゲーム";

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND g_Window;
POINTS g_mousePoint;
int g_zDelta;

HWND GetWindow()
{
	return g_Window;
}

POINTS MouseAcquisition()
{
	if (g_mousePoint.x >= SCREEN_WIDTH)
		g_mousePoint.x = SCREEN_WIDTH;
	if (g_mousePoint.x <= 0.0f)
		g_mousePoint.x = (SHORT)0.0f;

	if (g_mousePoint.y >= SCREEN_HEIGHT)
		g_mousePoint.y = SCREEN_HEIGHT;
	if (g_mousePoint.y <= 0.0f)
		g_mousePoint.y = (SHORT)0.0f;

	return g_mousePoint;
}

void SetCursorPosToClient(POINT pos)
{
	ClientToScreen(g_Window, &pos);
	SetCursorPos(pos.x, pos.y);
}

int GetMouseWheel()
{
	return g_zDelta / WHEEL_DELTA;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	srand((unsigned)time(NULL));

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};

	RegisterClassEx(&wcex);

	g_Window = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2),
		(SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION)),
		NULL,
		NULL,
		hInstance,
		NULL);

	Manager::GetInstance().Init();

	ShowWindow(g_Window, nCmdShow);
	UpdateWindow(g_Window);

	DWORD dwExecLastTime;
	DWORD dwCurrentTime;
	timeBeginPeriod(1);
	dwExecLastTime = timeGetTime();
	dwCurrentTime = 0;

	MSG msg;
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / FPS))
			{
				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();

				dwExecLastTime = dwCurrentTime;

				Manager::GetInstance().Update();
				g_zDelta = 0;
				Manager::GetInstance().Draw();
			}
		}
	}

	timeEndPeriod(1);

	UnregisterClass(CLASS_NAME, wcex.hInstance);

	Manager::GetInstance().Uninit();

	return (int)msg.wParam;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_MOUSEMOVE:
		g_mousePoint = MAKEPOINTS(lParam);
		break;

	case WM_MOUSEWHEEL:
		g_zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}