#include "DefenceBase.h"
#include "DrawAnimationModel.h"

DefenceBase::DefenceBase()
{
	auto trans = GetComponent<Transform>();
	trans->SetScale(Vector3(0.005f, 0.005f, 0.005f));
	trans->SetQuaternion(Quaternion::CreateFromAxisAngle(Vector3::Forward, 4.7f));

	m_model = AddComponent<DrawAnimationModel>(this);
	m_model->Load("Asset\\Model\\Base.fbx");

	auto modelBase = AddComponent<DrawModel>(this);
	modelBase->Load("Asset\\Model\\Base.obj");

	auto col = AddComponent<OBBCollision>();
	col->SetScale(Vector3(3.0f, 3.0f, 3.0f));
	col->SetIsStaticObject(false);
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
