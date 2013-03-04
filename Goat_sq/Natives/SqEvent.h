/*
*
*	This file is licensed under the GNU GPLv3
*	All the licenses are located at the root folder
*
*/

#pragma once
#include "SqEnvironment.h"
#include <igameevents.h>

using namespace SourceHook;

class SqEvent
{
public:
	static bool RegisterNatives(SqScript * pScript);
	//bool OnFireEvent(IGameEvent *pEvent, bool bDontBroadcast);
	//bool OnFireEvent_Post(IGameEvent *pEvent, bool bDontBroadcast);
	SqEvent(void);
	~SqEvent(void);
};

