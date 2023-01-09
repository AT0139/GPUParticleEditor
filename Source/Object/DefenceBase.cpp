#include "DefenceBase.h"
#include "DrawAnimationModel.h"
#include "Manager.h"
#include "Result.h"

static const int INITIAL_HP = 30;

namespace MainGame
{
	DefenceBase::DefenceBase()
		: m_hp(INITIAL_HP)
	{
		auto trans = GetComponent<Transform>();
		trans->SetScale(Vector3(0.005f, 0.005f, 0.005f));
		trans->SetQuaternion(Quaternion::CreateFromAxisAngle(Vector3::Forward, 4.7f));

		m_model = AddComponent<DrawAnimationModel>(this);
		m_model->Load("Asset\\Model\\Base.fbx");

		auto modelBase = AddComponent<DrawModel>(this);
		modelBase->Load("Asset\\Model\\Base.obj");

		auto col = AddComponent<SphereCollision>();
		col->SetScale(5.0f);
		col->SetCenterPosition(Vector3(0.0f, 1.5f, 0.0f));
		col->SetIsStaticObject(true);
		col->SetHitAction(HitAction::None);

		auto rigid = AddComponent<Rigidbody>();
		rigid->SetIsKinematic(true);
	}

	DefenceBase::~DefenceBase()
	{
	}

	void DefenceBase::Update()
	{
	}

	void DefenceBase::Draw()
	{
	}

	void DefenceBase::OnTrigger(GameObject* collision)
	{
		if (collision->GetTag() == TAG::BULLET)
		{
			collision->SetDestroy();
			m_hp--;
		}
	}
}