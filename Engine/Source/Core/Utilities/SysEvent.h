#ifndef SYSEVENT_H
#define SYSEVENT_H
#include "SysSyncObject.h"

// Event Can Only Be Created Via SysCreateEvent(bool,bool)
// Event Must Be Closed Via SysCloseEvent(SysEvent&)
class SysEvent : public SysSyncObject
{
public:
	SysEvent() {}
	~SysEvent() {}

	void Signal();
	void Reset();

public:

	friend SysEvent SysCreateEvent(bool bManuallyManaged = false, bool bInitialState = false);
	friend void SysCloseEvent(SysEvent& sysEvent);
};

#endif // SYSEVENT_H