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
#include <fstream>
#include <algorithm>

#pragma warning(push)
#pragma warning(disable:4005)

#include <d3d11.h>

#include "SpriteBatch.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"
#include "SimpleMath.h"
#include <GeometricPrimitive.h>


#pragma warning(pop)

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/list.hpp>


using namespace DirectX;
using namespace DirectX::SimpleMath;

#define SCREEN_WIDTH	(1920)
#define SCREEN_HEIGHT	(1080)
#define SCREEN_WIDTH_HALF	(SCREEN_WIDTH * 0.5f)
#define SCREEN_HEIGHT_HALF	(SCREEN_HEIGHT * 0.5f)
#define FPS (60)
#define GET_INPUT Input::GetInstance()

#define PI (3.141592)
#define TO_RADIAN(degree) (PI / 180 * degree)

#include "Utility.h"
#include "Structures.h"
#include "StaticDataTable.h"

