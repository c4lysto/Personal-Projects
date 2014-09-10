#include "Entity.h"
#include "Component.h"

CEntity::CEntity()
{
	m_Components.clear();
}

CEntity::~CEntity()
{
	m_Components.clear();
}

template<typename ComponentClass>
void CEntity::AddComponent(ComponentClass* pComponent)
{
	Assert(pComponent, "Attempting To Add An Invalid Component To The Entity", pComponent->GetID());

	if(pComponent)
	{
		Assert(pComponent->GetID() != INVALID_COMPONENT_ID, "Attempting To Add A Component With Invalid ID");

		if(pComponent->GetID() != INVALID_COMPONENT_ID && m_Components.find(pComponent->GetID()) == m_Components.end())
		{
			m_Components.insert(pComponent);
			pComponent->PostAdd();
		}
	}
}

template<typename ComponentClass>
void CEntity::RemoveComponent(ComponentClass* pComponent)
{
	Assert(pComponent, "Attempting To Remove An Invalid Component (%i) To The Entity", pComponent->GetID());

	if(pComponent)
	{
		Assert(pComponent->GetID() != INVALID_COMPONENT_ID, "Attempting To Remove A Component With Invalid ID");

		if(pComponent->GetID() != INVALID_COMPONENT_ID && m_Components.find(pComponent->GetID()) == m_Components.end())
		{
			m_Components.erase(pComponent);
			pComponent->PostRemove();
		}
	}
}

template<typename ComponentClass>
IComponent* CEntity::GetComponent()
{
	if(pComponent->GetID() != INVALID_COMPONENT_ID)
	{
		EntityComponentContainer::iterator iter = m_Components.find(pComponent->GetID());

		if(iter != m_Component.end())
		{
			return (*iter);
		}
	}

	return nullptr;
}