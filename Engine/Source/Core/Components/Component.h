#ifndef COMPONENT_H
#define COMPONENT_H

#define INVALID_COMPONENT_ID (-1)

class CEntity;

class IComponent
{
private:
	CEntity* m_pParent;
	int m_ID;

#if DEBUG
	char* m_szComponentName;
#endif // DEBUG

	void SetUniqueID(int nID) { if(Verify(m_ID == INVALID_COMPONENT_ID, "Component ID Already Set!")) m_ID = nID; }

public:
	IComponent() : m_pParent(nullptr), m_ID(INVALID_COMPONENT_ID) {}
	IComponent(CEntity* pParent) : m_pParent(pParent), m_ID(INVALID_COMPONENT_ID) DEBUG_ONLY(, m_szComponentName())
	{
		Assert(m_pParent, "IComponent - Invalid Parent Entity");
	}

	virtual ~IComponent() = 0 {}

	int GetID() {return m_ID;}

	virtual void PostAdd() {}
	virtual void PostRemove() {}

	bool operator<(const IComponent& rhs)
	{
		return m_ID < rhs.m_ID;
	}
};

#endif // COMPONENT_H