//#pragma once
//
//#include "Structures.h"
//
//namespace extensions
//{
//	XMFLOAT3::XMFLOAT3(const XMVECTOR& vec)
//	{
//		XMStoreFloat3((XMFLOAT3*)this, vec);
//	}
//
//	XMFLOAT3::operator XMVECTOR() const
//	{
//		DirectX::XMFLOAT3 my = *this;
//		XMVECTOR vec = XMLoadFloat3(&my);
//		return vec;
//	}
//
//	XMFLOAT3 XMFLOAT3::operator=(const XMVECTOR& other)
//	{
//		XMStoreFloat3((DirectX::XMFLOAT3*)this, other);
//		return *this;
//	}
//
//	//XMFLOAT3 XMFLOAT3::operator+(const XMFLOAT3 val) const
//	//{
//	//	return XMVectorAdd(*this, val);
//	//}
//	XMFLOAT3 XMFLOAT3::operator+=(const XMFLOAT3 val)
//	{
//		*this = *this + val;
//		return *this;
//	}
//	XMFLOAT3 XMFLOAT3::operator-(const XMFLOAT3 val) const
//	{
//		return XMVectorSubtract(*this, val);
//	}
//	XMFLOAT3 XMFLOAT3::operator-=(const XMFLOAT3 val)
//	{
//		*this = *this - val;
//		return *this;
//	}
//	XMFLOAT3 XMFLOAT3::operator*(float val) const
//	{
//		XMFLOAT3 temp(val, val, val);
//		return XMVectorMultiply(*this, temp);
//	}
//	XMFLOAT3 XMFLOAT3::operator*=(float val)
//	{
//		*this = *this * val;
//		return *this;
//	}
//
//	XMFLOAT3 XMFLOAT3::operator* (const XMFLOAT4X4 mat)const
//	{
//		return(Vector3)XMVector3Transform(*this, mat);
//	}
//
//	XMFLOAT3 XMFLOAT3::operator*=(const XMFLOAT4X4 mat)
//	{
//		*this = *this * mat;
//		return *this;
//	}
//	XMFLOAT3 XMFLOAT3::operator/(float val) const
//	{
//		XMFLOAT3 temp(XMVectorReciprocal(XMFLOAT3(val, val, val)));
//		return XMVectorMultiply(*this, temp);
//	}
//	XMFLOAT3 XMFLOAT3::operator/=(float val)
//	{
//		*this = *this / val;
//		return *this;
//	}
//	bool XMFLOAT3::operator!=(const XMFLOAT3 vec)
//	{
//		return !XMVector3Equal(*this, vec);
//	}
//	XMFLOAT4::XMFLOAT4(const XMVECTOR& vec)
//	{
//		XMStoreFloat4((XMFLOAT4*)this, vec);
//	}
//	XMFLOAT4::operator XMVECTOR() const
//	{
//		DirectX::XMFLOAT4 my = *this;
//		XMVECTOR vec = XMLoadFloat4(&my);
//		return vec;
//	}
//	XMFLOAT4 XMFLOAT4::operator=(const XMVECTOR& other)
//	{
//		XMStoreFloat4((DirectX::XMFLOAT4*)this, other);
//		return *this;
//	}
//	XMFLOAT4 XMFLOAT4::operator+(const XMFLOAT4 val) const
//	{
//		return XMVectorAdd(*this, val);
//	}
//	XMFLOAT4 XMFLOAT4::operator+=(const XMFLOAT4 val)
//	{
//		*this = *this + val;
//		return *this;
//	}
//
//	XMFLOAT4 XMFLOAT4::operator*(float val) const
//	{
//		XMFLOAT4 temp(val, val, val, val);
//		return XMVectorMultiply(*this, temp);
//	}
//
//	XMFLOAT4 XMFLOAT4::operator*(XMFLOAT4 val) const
//	{
//		XMFLOAT4 temp(val);
//		return XMVectorMultiply(*this, temp);
//	}
//	XMFLOAT4 XMFLOAT4::operator*=(float val)
//	{
//		*this = *this * val;
//		return *this;
//	}
//	XMFLOAT4 XMFLOAT4::operator*=(XMFLOAT4 val)
//	{
//		*this = *this * val;
//		return *this;
//	}
//	bool XMFLOAT4::operator!=(const XMFLOAT4 vec)
//	{
//		return !XMVector4Equal(*this, vec);
//	}
//	XMFLOAT4X4::XMFLOAT4X4(const XMMATRIX& vec)
//	{
//		XMStoreFloat4x4((XMFLOAT4X4*)this, vec);
//	}
//	XMFLOAT4X4::XMFLOAT4X4(const XMFLOAT4& quat)
//	{
//		*this = XMMatrixRotationQuaternion(quat);
//	}
//	XMFLOAT4X4::operator XMMATRIX() const
//	{
//		DirectX::XMFLOAT4X4 my = *this;
//		XMMATRIX vec = XMLoadFloat4x4(&my);
//		return vec;
//	}
//	XMFLOAT4X4 XMFLOAT4X4::operator*(XMFLOAT4X4 val) const
//	{
//		XMFLOAT4X4 temp(val);
//		return XMMatrixMultiply(*this, temp);
//	}
//	XMFLOAT3 XMFLOAT4X4::TransInMatrix() const
//	{
//		XMFLOAT3 pos;
//		pos.x = _41;
//		pos.y = _42;
//		pos.z = _43;
//		return pos;
//	}
//	XMFLOAT4 XMFLOAT4X4::QuaternionInMatrix() const
//	{
//		XMFLOAT3 scale, pos;
//		XMFLOAT4 quat;
//		Division(scale, quat, pos);
//		return quat;
//	}
//	XMFLOAT4X4& XMFLOAT4X4::ScaleIdentity()
//	{
//		Vector3 scale, position;
//		Quaternion quat;
//		Division(scale, quat, position);
//		scale = Vector3(1.0f, 1.0f, 1.0f);
//		return AffineTransformation(scale, Vector3(0.0f, 0.0f, 0.0f), quat, position);
//	}
//	void XMFLOAT4X4::Division(XMFLOAT3& scale, XMFLOAT4& quat, XMFLOAT3& pos) const
//	{
//		XMVECTOR sc, qt, trans;
//		if (XMMatrixDecompose(&sc, &qt, &trans, *this))
//		{
//			scale = sc;
//			quat = qt;
//			pos = trans;
//		}
//		else
//		{
//			auto xLen = VecLength(Vector3(_11, _12, _13));
//			auto yLen = VecLength(Vector3(_21, _22, _23));
//			auto zLen = VecLength(Vector3(_31, _32, _33));
//
//			if (xLen == 0.0f)
//				xLen = 0.0001f;
//			if (yLen == 0.0f)
//				yLen = 0.0001f;
//			if (zLen == 0.0f)
//				zLen = 0.0001f;
//
//			scale = Vector3(xLen, yLen, zLen);
//			pos = Vector3(_41, _42, _43);
//
//			Vector3 x = Vector3(_11, _12, _13) / xLen;
//			Vector3 y = Vector3(_21, _22, _23) / xLen;
//			Vector3 z = Vector3(_31, _32, _33) / xLen;
//
//			Matrix ret;
//			ret._11 = x.x;
//			ret._12 = x.y;
//			ret._13 = x.z;
//
//			ret._21 = y.x;
//			ret._22 = y.y;
//			ret._23 = y.z;
//
//			ret._31 = z.x;
//			ret._32 = z.y;
//			ret._33 = z.z;
//
//			quat = (Quaternion)XMQuaternionRotationMatrix(ret);
//		}
//	}
//	XMFLOAT4X4& XMFLOAT4X4::AffineTransformation(XMFLOAT3& scale, XMFLOAT3 pivot, XMFLOAT4& quat, XMFLOAT3& pos)
//	{
//		*this = (XMFLOAT4X4)XMMatrixAffineTransformation(scale, pivot, quat, pos);
//		return *this;
//	}
//}