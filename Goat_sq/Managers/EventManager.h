/*
*
*	This file is licensed under the GNU GPLv3
*	All the licenses are located at the root folder
*
*/

#pragma once

#include "SqEnvironment.h"
#include <igameevents.h>
#include <sourcehook/sourcehook.h>
#include <sourcehook/sh_list.h>

using namespace SourceHook;
class IEventListener
{
public:
	bool OnFireEvent(IGameEvent *pEvent, bool bDontBroadcast) {return true; }
	bool OnFireEvent_Post(IGameEvent *pEvent, bool bDontBroadcast){return true; }
};

class EventManager
{
private:
	typedef List<IEventListener*> EventListenerList;

	EventListenerList m_EventListenerList;
private:
	bool OnFireEvent(IGameEvent *pEvent, bool bDontBroadcast);
	bool OnFireEvent_Post(IGameEvent *pEvent, bool bDontBroadcast);
public:
	bool AddEventListener(IEventListener *listener);
	bool RemoveEventListener(IEventListener *listener);
	EventManager(void);
	~EventManager(void);
};

