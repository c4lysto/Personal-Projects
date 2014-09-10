#ifndef ENTITY_H
#define ENTITY_H

#include <map>
using std::map;

class IComponent;

typedef map<int, IComponent*> EntityComponentContainer;

class CEntity
{
private:
	EntityComponentContainer m_Components;

public:
	CEntity();
	~CEntity();

	template<typename ComponentClass>
	void AddComponent(ComponentClass* pComponent);

	template<typename ComponentClass>
	void RemoveComponent(ComponentClass* pComponent);

	template<typename ComponentClass>
	IComponent* GetComponent();

};

#endif // ENTITY_H