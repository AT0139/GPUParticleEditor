#pragma once

//todo : 整理
namespace extensions
{
	using namespace DirectX;

	struct XMFLOAT3 : DirectX::XMFLOAT3
	{
		using DirectX::XMFLOAT3::XMFLOAT3;
		//XMVECTORからの引数コンストラクタ
		XMFLOAT3(const XMVECTOR& vec);
		//XMVECTORへの変換
		operator XMVECTOR()const;

		XMFLOAT3 operator= (const XMVECTOR& other);
		XMFLOAT3 operator+ (const XMFLOAT3 val)const;
		XMFLOAT3 operator+= (const XMFLOAT3 val);
		XMFLOAT3 operator- (const XMFLOAT3 val)const;
		XMFLOAT3 operator-= (const XMFLOAT3 val);
		XMFLOAT3 operator* (float val)const;
		XMFLOAT3 operator*= (float val);
		XMFLOAT3 operator/ (float val)const;
		XMFLOAT3 operator/= (float val);
		bool operator != (const XMFLOAT3 vec);
	};

	struct XMFLOAT4 : DirectX::XMFLOAT4
	{
		using DirectX::XMFLOAT4::XMFLOAT4;

		XMFLOAT4(const XMVECTOR& vec);

		operator XMVECTOR()const;
		XMFLOAT4 operator = (const XMVECTOR& other);
		XMFLOAT4 operator + (const XMFLOAT4 val)const;
		XMFLOAT4 operator += (const XMFLOAT4 val);
		XMFLOAT4 operator * (float val)const;
		XMFLOAT4 operator * (XMFLOAT4 val)const;
		XMFLOAT4 operator *= (float val);
		XMFLOAT4 operator *= (XMFLOAT4 val);
		bool operator != (const XMFLOAT4 vec);
	};

	struct XMFLOAT4X4 : public DirectX::XMFLOAT4X4
	{
		using DirectX::XMFLOAT4X4::XMFLOAT4X4;

		XMFLOAT4X4(const XMMATRIX& vec);

		operator XMMATRIX()const;
	
		XMFLOAT4X4 operator * (XMFLOAT4X4 val)const;
		//スケールを1に
		XMFLOAT4X4& ScaleIdentity();
		void Division(XMFLOAT3& scale, XMFLOAT4& quat, XMFLOAT3& pos)const;
		XMFLOAT4X4& AffineTransformation(XMFLOAT3& scale, XMFLOAT3 pivot, XMFLOAT4& quat, XMFLOAT3& pos);
	};
}

using Vector2 = XMFLOAT2;
using Vector3 = extensions::XMFLOAT3;
using Vector4 = extensions::XMFLOAT4;
using Quaternion = extensions::XMFLOAT4;
using Matrix = extensions::XMFLOAT4X4;
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

struct VERTEX_3D
{
	Vector3 Position;
	Vector3 Normal;
	Vector4 Diffuse;
	Vector2 TexCoord;
};

inline float VecLength(const Vector3& vec)
{
	return ((Vector3)XMVector3Length(vec)).x;
}

namespace DirextX = extensions;
