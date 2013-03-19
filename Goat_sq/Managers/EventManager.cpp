/*
*
*	This file is licensed under the GNU GPLv3
*	All the licenses are located at the root folder
*
*/

#include "EventManager.h"

EventManager *g_EventManager;

SH_DECL_HOOK2(IGameEventManager2, FireEvent, SH_NOATTRIB, 0, bool, IGameEvent *, bool);

EventManager::EventManager(void)
{
	m_pCachedGameEvent = NULL;
	m_bOverrideDontBroadcast.IsSet = false;
	SH_ADD_HOOK_MEMFUNC(IGameEventManager2, FireEvent, gameevents, this, &EventManager::OnFireEvent, false);
	SH_ADD_HOOK_MEMFUNC(IGameEventManager2, FireEvent, gameevents, this, &EventManager::OnFireEvent_Post, true);
}

bool EventManager::AddEventListener(IEventListener *listener)
{
	/*if(m_EventListenerList.find(listener))
		return false;*/

	m_EventListenerList.push_back(listener);
	return true;
}

bool EventManager::RemoveEventListener(IEventListener *listener)
{
	if(!m_EventListenerList.find(listener))
		return false;

	m_EventListenerList.remove(listener);
	return true;
}

bool EventManager::OnFireEvent(IGameEvent *pEvent, bool bDontBroadcast)
{
	bool bStop = false;
	m_bOverrideDontBroadcast.IsSet = false;
	if(m_EventListenerList.size() > 0)
	{
		if(pEvent)
		{
			m_bCachedDontBroadcast = bDontBroadcast;
			m_pCachedGameEvent = gameevents->DuplicateEvent(pEvent);
		
			EventListenerList::iterator it;
			for(it=m_EventListenerList.begin(); it!=m_EventListenerList.end(); it++)
			{
				if(!(*it)->OnFireEvent(pEvent,bDontBroadcast))
					bStop = true;
			}
		}
		if(m_bOverrideDontBroadcast.IsSet)
		{
			m_bOverrideDontBroadcast.IsSet = false;
			if(bStop)
			{
				gameevents->FreeEvent(pEvent); //the original event will not get called so we have to free it
				RETURN_META_VALUE_NEWPARAMS(MRES_SUPERCEDE,true,&IGameEventManager2::FireEvent,(pEvent,m_bOverrideDontBroadcast.DontBroadcastOverride));
			}
			else
				RETURN_META_VALUE_NEWPARAMS(MRES_IGNORED,true,&IGameEventManager2::FireEvent,(pEvent,m_bOverrideDontBroadcast.DontBroadcastOverride));
		}
	}
	if(bStop)
	{
		gameevents->FreeEvent(pEvent); //the original event will not get called so we have to free it
		RETURN_META_VALUE(MRES_SUPERCEDE,true);
	}	
	else
		RETURN_META_VALUE(MRES_IGNORED,true);
}

bool EventManager::OnFireEvent_Post(IGameEvent *pEvent, bool bDontBroadcast)
{
	if(m_EventListenerList.size() > 0)
	{	
		if(m_pCachedGameEvent)
		{
			EventListenerList::iterator it;
			for(it=m_EventListenerList.begin(); it!=m_EventListenerList.end(); it++)
				(*it)->OnFireEvent_Post(m_pCachedGameEvent,m_bCachedDontBroadcast);
			
			gameevents->FreeEvent(m_pCachedGameEvent);
			m_pCachedGameEvent = NULL;
		}
	}
	return true;
}

EventManager::~EventManager(void)
{
	SH_REMOVE_HOOK_MEMFUNC(IGameEventManager2, FireEvent, gameevents, this, &EventManager::OnFireEvent, false);
	SH_REMOVE_HOOK_MEMFUNC(IGameEventManager2, FireEvent, gameevents, this, &EventManager::OnFireEvent_Post, true);
}