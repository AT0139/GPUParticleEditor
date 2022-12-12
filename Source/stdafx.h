#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define NOMINMAX
#include <windows.h>

#include <assert.h>

#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <memory>
#include <typeinfo>
#include <typeindex>

#pragma warning(push)
#pragma warning(disable:4005)

#include <d3d11.h>
//#include <d3dx9.h>
//#include <d3dx11.h>
#include <DirectXMath.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTK_Debug.lib")
#else
#pragma comment(lib, "DirectXTK.lib")
#endif

#include "SpriteBatch.h"
#include "WICTextureLoader.h"
#include "SimpleMath.h"

#pragma warning(pop)

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

using namespace DirectX;

#include "Utility.h"
#include "Structures.h"