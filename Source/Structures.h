#pragma once

namespace extensions
{
	using namespace DirectX;

	struct XMFLOAT3 : DirectX::XMFLOAT3
	{
		using DirectX::XMFLOAT3::XMFLOAT3;

		operator XMVECTOR()const
		{
			DirectX::XMFLOAT3 my = *this;
			XMVECTOR vec = XMLoadFloat3(&my);
			return vec;
		}

		XMFLOAT3 operator = (const XMVECTOR& other)
		{
			XMStoreFloat3((DirectX::XMFLOAT3*)this, other);
			return *this;
		}

	};

	struct XMFLOAT4 : DirectX::XMFLOAT4
	{
		using DirectX::XMFLOAT4::XMFLOAT4;

		operator XMVECTOR()const
		{
			DirectX::XMFLOAT4 my = *this;
			XMVECTOR vec = XMLoadFloat4(&my);
			return vec;
		}
		XMFLOAT4 operator = (const XMVECTOR& other)
		{
			XMStoreFloat4((DirectX::XMFLOAT4*)this, other);
			return *this;
		}
	};
}

using Vector2 = XMFLOAT2;
using Vector3 = extensions::XMFLOAT3;
using Vector4 = extensions::XMFLOAT4;
using Quaternion = extensions::XMFLOAT4;
using Matrix = XMMATRIX;
//using Color = XMFLOAT4;

struct Color
{
	float r;
	float g;
	float b;
	float a;

	Color() = default;

	Color(const Color&) = default;
	Color& operator=(const Color&) = default;

	Color(Color&&) = default;
	Color& operator=(Color&&) = default;

	constexpr Color(float _x, float _y, float _z, float _w) noexcept : r(_x), g(_y), b(_z), a(_w) {}
	explicit Color(_In_reads_(4) const float* pArray) noexcept : r(pArray[0]), g(pArray[1]), b(pArray[2]), a(pArray[3]) {}
};

