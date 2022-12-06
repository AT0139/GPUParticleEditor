#pragma once

namespace extensions
{
	using namespace DirectX;

	struct XMFLOAT3 : DirectX::XMFLOAT3
	{
		using DirectX::XMFLOAT3::XMFLOAT3;

		XMFLOAT3(const XMVECTOR& vec)
		{
			XMStoreFloat3((XMFLOAT3*)this, vec);
		}

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

		XMFLOAT3 operator + (const XMFLOAT3 val)const
		{
			return XMVectorAdd(*this, val);
		}

		XMFLOAT3 operator += (const XMFLOAT3 val)
		{
			*this = *this + val;
			return *this;
		}

		XMFLOAT3 operator - (const XMFLOAT3 val)const
		{
			return XMVectorSubtract(*this, val);
		}

		XMFLOAT3 operator -= (const XMFLOAT3 val)
		{
			*this = *this - val;
			return *this;
		}

		XMFLOAT3 operator * (float val)const
		{
			XMFLOAT3 temp(val, val, val);
			return XMVectorMultiply(*this, temp);
		}

		XMFLOAT3 operator *= (float val)
		{
			*this = *this * val;
			return *this;
		}

		XMFLOAT3 operator / (float val)const
		{
			XMFLOAT3 temp(XMVectorReciprocal(XMFLOAT3(val, val, val)));
			return XMVectorMultiply(*this, temp);
		}

		XMFLOAT3 operator /= (float val)
		{
			*this = *this / val;
			return *this;
		}

		bool operator != (const XMFLOAT3 vec)
		{
			return !XMVector3Equal(*this, vec);
		}
	};

	struct XMFLOAT4 : DirectX::XMFLOAT4
	{
		using DirectX::XMFLOAT4::XMFLOAT4;

		XMFLOAT4(const XMVECTOR& vec)
		{
			XMStoreFloat4((XMFLOAT4*)this, vec);
		}

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

		XMFLOAT4 operator + (const XMFLOAT4 val)const
		{
			return XMVectorAdd(*this, val);
		}

		XMFLOAT4 operator += (const XMFLOAT4 val)
		{
			*this = *this + val;
			return *this;
		}

		XMFLOAT4 operator * (float val)const
		{
			XMFLOAT4 temp(val, val, val,val);
			return XMVectorMultiply(*this, temp);
		}
		XMFLOAT4 operator * (XMFLOAT4 val)const
		{
			XMFLOAT4 temp(val);
			return XMVectorMultiply(*this, temp);
		}
		XMFLOAT4 operator *= (float val)
		{
			*this = *this * val;
			return *this;
		}
		XMFLOAT4 operator *= (XMFLOAT4 val)
		{
			*this = *this * val;
			return *this;
		}



		bool operator != (const XMFLOAT4 vec)
		{
			return !XMVector4Equal(*this, vec);
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

namespace DirextX = extensions;