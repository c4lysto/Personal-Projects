#ifndef ENTITY_H
#define ENTITY_H

#include "Utilities/UtilitiesInclude.h"

#include <map>
using std::map;

#include <string>
using std::string;

#define INVALID_ENTITY_ID ((u32)-1)

class IComponent;

typedef map<u32, IComponent*> EntityComponentContainer;

class CEntity
{
private:
	u32 m_ID;

#if DEBUG
	string m_szEntityName;
#endif

	EntityComponentContainer m_Components;

public:
	CEntity();
	~CEntity();

	u32 GetID() {return m_ID;}

	template<typename ComponentClass>
	void AddComponent(ComponentClass* pComponent);

	template<typename ComponentClass>
	void RemoveComponent(ComponentClass* pComponent);

	template<typename ComponentClass>
	IComponent* GetComponent();

};

#endif // ENTITY_H