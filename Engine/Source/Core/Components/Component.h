#ifndef COMPONENT_H
#define COMPONENT_H

#define INVALID_COMPONENT_ID ((u32)-1)

template<typename ComponentClass>
__forceinline size_t GetComponentID() \
{ \
	return typeid(ComponentClass).hash_code(); \
}

#define COMPONENT_ID(className) (GetComponentID<(className)>())


class CEntity;

class IComponent
{
#if DEBUG
	char* m_szComponentName;
#endif // DEBUG

protected:
	CEntity* m_pEntity;

	//void SetUniqueID(u32 nID) { if(Verify(m_ID == INVALID_COMPONENT_ID, "Component ID Already Set!")) m_ID = nID; }

public:
	IComponent() : m_pEntity(nullptr)/*, m_ID(INVALID_COMPONENT_ID)*/ {}
	IComponent(CEntity* pParent) : m_pEntity(pParent)/*, m_ID(INVALID_COMPONENT_ID)*/ DEBUG_ONLY(, m_szComponentName())
	{
		Assert(m_pEntity, "IComponent - Invalid Parent Entity");
	}

	virtual ~IComponent() = 0 {}

	/*int GetID() {return m_ID;}*/

	virtual void PostAdd() {}
	virtual void PostRemove() {}

	/*bool operator<(const IComponent& rhs)
	{
		return m_ID < rhs.m_ID;
	}*/
};

#endif // COMPONENT_H