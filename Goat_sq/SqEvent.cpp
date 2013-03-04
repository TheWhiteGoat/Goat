#include "SqEvent.h"

SH_DECL_HOOK2(IGameEventManager2, FireEvent, SH_NOATTRIB, 0, bool, IGameEvent *, bool);

SQInteger Native_HookEvent(HSQUIRRELVM vm)
{

	return 0;
}

SqEvent::SqEvent()
{
	SH_ADD_HOOK_MEMFUNC(IGameEventManager2, FireEvent, gameevents, this, &SqEvent::OnFireEvent, false);
	SH_ADD_HOOK_MEMFUNC(IGameEventManager2, FireEvent, gameevents, this, &SqEvent::OnFireEvent_Post, true);
}


bool SqEvent::RegisterNatives(SqScript * pScript)
{
	pScript->RegisterFunction("HookEvent",Native_HookEvent,3,".sx|c");
}
