#pragma once
#include "SqEnvironment.h"
#include <igameevents.h>

using namespace SourceHook;

class SqEvent
{
public:
	static bool RegisterNatives(SqScript * pScript);
	bool OnFireEvent(IGameEvent *pEvent, bool bDontBroadcast);
	bool OnFireEvent_Post(IGameEvent *pEvent, bool bDontBroadcast);
	SqEvent(void);
	~SqEvent(void);
};

