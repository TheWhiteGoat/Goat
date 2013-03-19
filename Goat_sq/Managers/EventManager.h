/*
*
*	This file is licensed under the GNU GPLv3
*	All the licenses are located at the root folder
*
*/

#ifndef _INCLUDE_EVENTMANAGER_H
#define _INCLUDE_EVENTMANAGER_H

#include "smsdk_ext.h"
#include <igameevents.h>
#include <sourcehook/sourcehook.h>
#include <sourcehook/sh_list.h>

using namespace SourceHook;

enum ReturnEventHook
{
	CONTINUE = 0,
	STOP = 1, 
};

class IEventListener
{
public:
	virtual bool OnFireEvent(IGameEvent *pEvent, bool bDontBroadcast){return ReturnEventHook::CONTINUE; }
	virtual bool OnFireEvent_Post(IGameEvent *pEvent, bool bDontBroadcast){return true; }
};

class EventManager
{
private:
	struct Override
	{
		bool IsSet;
		bool DontBroadcastOverride;
	};
	Override m_bOverrideDontBroadcast;
private:
	typedef List<IEventListener*> EventListenerList;
	EventListenerList m_EventListenerList;
	IGameEvent *m_pCachedGameEvent;
	bool m_bCachedDontBroadcast;
private:
	bool OnFireEvent(IGameEvent *pEvent, bool bDontBroadcast);
	bool OnFireEvent_Post(IGameEvent *pEvent, bool bDontBroadcast);
public:
	bool AddEventListener(IEventListener *listener);
	bool RemoveEventListener(IEventListener *listener);
	void SetDontBroadcast(bool bDontBroadcast){ m_bOverrideDontBroadcast.DontBroadcastOverride = bDontBroadcast; 
	m_bOverrideDontBroadcast.IsSet = true;}

	EventManager(void);
	~EventManager(void);
};

extern EventManager *g_EventManager;

#endif