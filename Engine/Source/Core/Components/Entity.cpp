#include "Entity.h"
#include "Component.h"

CEntity::CEntity() : m_ID(INVALID_ENTITY_ID)
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
	if(Verify(pComponent, "Attempting To Add An Invalid Component To The Entity", COMPONENT_ID(ComponentClass)))
	{
		if(/*Verify(COMPONENT_ID(ComponentClass) != INVALID_COMPONENT_ID, "Attempting To Add A Component With Invalid ID") &&*/ 
		   m_Components.find(COMPONENT_ID(ComponentClass)) == m_Components.end())
		{
			m_Components.insert(pComponent);
			pComponent->PostAdd();
		}
	}
}

template<typename ComponentClass>
void CEntity::RemoveComponent(ComponentClass* pComponent)
{
	if(Verify(pComponent, "Attempting To Remove An Invalid Component (%i) To The Entity", COMPONENT_ID(ComponentClass)))
	{
		if(/*Verify(COMPONENT_ID(ComponentClass) != INVALID_COMPONENT_ID, "Attempting To Remove A Component With Invalid ID") &&*/ 
			m_Components.find(COMPONENT_ID(ComponentClass)) == m_Components.end())
		{
			m_Components.erase(pComponent);
			pComponent->PostRemove();
		}
	}
}

template<typename ComponentClass>
IComponent* CEntity::GetComponent()
{
	//if(COMPONENT_ID(ComponentClass) != INVALID_COMPONENT_ID)
	{
		EntityComponentContainer::iterator iter = m_Components.find(COMPONENT_ID(ComponentClass));

		if(iter != m_Component.end())
		{
			return (*iter);
		}
	}

	return nullptr;
}