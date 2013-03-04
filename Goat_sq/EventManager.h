#pragma once

#include "SqEnvironment.h"
#include "igameevents.h"
#include "sourcehook/sourcehook.h"
//#include "
using namespace SourceHook;

class EventManager
{
private:
	bool OnFireEvent(IGameEvent *pEvent, bool bDontBroadcast);
	bool OnFireEvent_Post(IGameEvent *pEvent, bool bDontBroadcast);
public:
	EventManager(void);
	~EventManager(void);
};

