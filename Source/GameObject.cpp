#include "GameObject.h"


shared_ptr<Component> GameObject::SearchComponent(std::type_index index)const
{
	auto it = m_componentMap.find(index);
	if (it != m_componentMap.end())
	{
		return it->second;
	}
	return nullptr;
}
