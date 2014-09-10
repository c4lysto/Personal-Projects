#ifndef COMPONENTCONTAINER_H
#define COMPONENTCONTAINER_H

#include "Component.h"

class ComponentContainer : public IComponent
{
private:


public:
	ComponentContainer(CEntity* pParent);
	virtual ~ComponentContainer();

};

#endif // COMPONENTCONTAINER_H