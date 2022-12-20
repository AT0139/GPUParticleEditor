#include "Utility.h"
bool Utility::MouseOver(Vector2 mousePos, Vector2 overPos, Vector2 overSize, bool isOverCenter)
{
	//マウスオーバー相手が中心原点だったら
	if (isOverCenter)
	{
		if (mousePos.x > overPos.x - overSize.x * 0.5f && mousePos.x < overPos.x + overSize.x * 0.5f)
		{
			if (mousePos.y > overPos.y - overSize.y * 0.5f && mousePos.y < overPos.y + overSize.y * 0.5f)
			{
				return true;
			}
		}
	}
	else
	{
		if (mousePos.x > overPos.x && mousePos.x < overPos.x + overSize.x)
		{
			if (mousePos.y > overPos.y && mousePos.y < overPos.y + overSize.y)
			{
				return true;
			}
		}
	}

	return false;
}

Vector3* Utility::CalcScreenToXZ(Vector3* pout, float screenX, float screenY, Matrix* view, Matrix* prj)
{
	Vector3 nearpos;
	Vector3 farpos;
	Vector3 ray;
	CalcScreenToWorld(nearpos, screenX, screenY, 0, view, prj);
	CalcScreenToWorld(farpos, screenX, screenY, 1, view, prj);

	ray = farpos - nearpos;
	ray.Normalize();

	// 床との交差が起きている場合は交点を
	// 起きていない場合は遠くの壁との交点を出力
	if (ray.y <= 0)
	{
		// 床交点
		float Lray = ray.Dot(Vector3(0, 1, 0));
		float LP0 = (-nearpos).Dot(Vector3(0, 1, 0));
		*pout = nearpos + (LP0 / Lray) * ray;
	}
	else
	{
		*pout = farpos;
	}

	return pout;
}

Vector3 Utility::CalcScreenToWorld(Vector3& pout, float screenX, float screenY, int farZ, Matrix* view, Matrix* prj)
{
	// 各行列の逆行列を算出
	Matrix InvView, InvPrj, VP, InvViewport;
	InvView = view->Invert();
	InvPrj = prj->Invert();
	VP = Matrix::Identity;
	VP._11 = SCREEN_WIDTH / 2.0f; VP._22 = -SCREEN_HEIGHT / 2.0f;
	VP._41 = SCREEN_WIDTH / 2.0f; VP._42 = SCREEN_HEIGHT / 2.0f;
	InvViewport = VP.Invert();
	
	// 逆変換
	Matrix tmp = InvViewport * InvPrj * InvView;
	auto p = Vector3::Transform(Vector3(screenX, screenY, (float)farZ), tmp);
	pout = p;
	
	return pout;
}

Ray Utility::ScreenPosToRay(float screenX, float screenY, Matrix* view, Matrix* prj)
{
	Vector3 nearpos;
	Vector3 farpos;
	CalcScreenToWorld(nearpos, screenX, screenY, 0, view, prj);
	CalcScreenToWorld(farpos, screenX, screenY, 1, view, prj);

	Ray ray;
	ray.position = nearpos;
	ray.direction = farpos - nearpos;
	ray.direction.Normalize();

	return ray;
}
