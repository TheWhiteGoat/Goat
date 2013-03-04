#include "EventManager.h"

SH_DECL_HOOK2(IGameEventManager2, FireEvent, SH_NOATTRIB, 0, bool, IGameEvent *, bool);

EventManager::EventManager(void)
{
	SH_ADD_HOOK_MEMFUNC(IGameEventManager2, FireEvent, gameevents, this, &EventManager::OnFireEvent, false);
	SH_ADD_HOOK_MEMFUNC(IGameEventManager2, FireEvent, gameevents, this, &EventManager::OnFireEvent_Post, true);
}

bool EventManager::OnFireEvent(IGameEvent *pEvent, bool bDontBroadcast)
{

}
	
bool EventManager::OnFireEvent_Post(IGameEvent *pEvent, bool bDontBroadcast)
{

}

EventManager::~EventManager(void)
{
}