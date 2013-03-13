/*
*
*	This file is licensed under the GNU GPLv3
*	All the licenses are located at the root folder
*
*/

#include "EventManager.h"

EventManager * g_EventManager;

SH_DECL_HOOK2(IGameEventManager2, FireEvent, SH_NOATTRIB, 0, bool, IGameEvent *, bool);

EventManager::EventManager(void)
{
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
	//rootconsole->ConsolePrint("EVENT IS: %s",pEvent->GetName());
	if(m_EventListenerList.size() > 0)
	{
		EventListenerList::iterator it;
		for(it = m_EventListenerList.begin(); it!=m_EventListenerList.end(); it++)
			(*it)->OnFireEvent(pEvent,bDontBroadcast);
	}
	return true;
}

bool EventManager::OnFireEvent_Post(IGameEvent *pEvent, bool bDontBroadcast)
{
	if(m_EventListenerList.size() > 0)
	{
		EventListenerList::iterator it;
		for(it=m_EventListenerList.begin(); it!=m_EventListenerList.end(); it++)
			(*it)->OnFireEvent_Post(pEvent,bDontBroadcast);
	}
	return true;
}

EventManager::~EventManager(void)
{
	SH_REMOVE_HOOK_MEMFUNC(IGameEventManager2, FireEvent, gameevents, this, &EventManager::OnFireEvent, false);
	SH_REMOVE_HOOK_MEMFUNC(IGameEventManager2, FireEvent, gameevents, this, &EventManager::OnFireEvent_Post, true);
}