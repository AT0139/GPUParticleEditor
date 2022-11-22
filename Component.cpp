#include "Component.h"

Component::Component(const std::shared_ptr<GameObject> pGameObject)
	: m_pGameObject(pGameObject)

{}

std::shared_ptr<GameObject> Component::GetGameObject() const
{
	//weak_ptrなので参照先チェック
	assert(m_pGameObject.expired());

	return m_pGameObject.lock();
}
