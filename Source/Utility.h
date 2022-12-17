#pragma once

namespace Utility
{
	template<typename T>
	std::shared_ptr<T> GetCastThis()
	{
		auto Ptr = std::dynamic_pointer_cast<T>(this);
		if (Ptr)
		{
			return Ptr;
		}
		else
		{
			assert(Ptr);
		}
		return nullptr;
	}

	bool MouseOver(POINTS mousePos, Vector2 overPos, Vector2 overSize,bool isOverCenter = true);

	Vector3* CalcScreenToXZ(Vector3* pout, int screenX, int screenY, Matrix* view, Matrix* prj);
	Vector3 CalcScreenToWorld(Vector3& pout, int screenX, int screenY, int farZ,  Matrix* view, Matrix* prj);

	Ray ScreenPosToRay(int screenX, int screenY, Matrix* view, Matrix* prj);

	inline float VECtoFloat(XMVECTOR vec)
	{
		float tmp;
		XMStoreFloat(&tmp, vec);
		return tmp;
	}

	inline Matrix ScaleIdentity(Matrix& mat)
	{
		Vector3 scale, position;
		Quaternion quat;
		mat.Decompose(scale, quat, position);
		scale = Vector3(1.0f, 1.0f, 1.0f);
		auto matrix = XMMatrixAffineTransformation(scale, Vector3(0.0f, 0.0f, 0.0f), quat, position);
		return matrix;
	}

	inline Vector3 TransInMatrix(Matrix& mat)
	{
		XMFLOAT3 pos;
		pos.x = mat._41;
		pos.y = mat._42;
		pos.z = mat._43;
		return pos;
	}
}