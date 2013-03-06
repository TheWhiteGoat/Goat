/*
*
*	This file is licensed under the GNU GPLv3
*	All the licenses are located at the root folder
*
*/

#include "SqEvent.h"

SqEvent * g_event = NULL;

SQInteger Native_HookEvent(HSQUIRRELVM vm)
{
	if(g_event == NULL)
		g_event = new SqEvent();

	int top = sq_gettop(vm);
	const SQChar * key = NULL;
	HSQOBJECT inst;
	sq_resetobject(&inst);
	if(top >= 4)
	{
		if(sq_gettype(vm,2) == OT_INSTANCE)
		{
			sq_getstackobj(vm,2,&inst);
			sq_addref(vm,&inst);
			for(int i=2; i<top; i+=2)
			{
				if(sq_gettype(vm,-1) == OT_CLOSURE)
					if(SQ_SUCCEEDED(sq_getstring(vm,-2,&key)))
					{
						HSQOBJECT obj;
						sq_resetobject(&obj);
						sq_getstackobj(vm,-1,&obj);
						sq_addref(vm,&obj);
						g_event->AddToTrie(key,inst,true,obj,vm);
					}			
				sq_pop(vm,2);
			}
		}
	}
	sq_settop(vm,1);
	return 0;
}

SqEvent::SqEvent()
{
	if(g_EventManager == NULL)
		g_EventManager = new EventManager();

	if(!g_EventManager->AddEventListener(this))
		g_pSM->LogError(myself,"Failed adding event listener");

}

SqEvent::~SqEvent()
{
	g_EventManager->RemoveEventListener(this);
}

bool SqEvent::OnFireEvent(IGameEvent *pEvent, bool bDontBroadcast)
{
	const RegisteredEvents * eventcloslist = FindFromTrie(pEvent->GetName());
	if(eventcloslist == NULL)
		return true;
	
	HSQUIRRELVM vm;
	List<Closures>::iterator it;
	int top = 1;
	
	if(eventcloslist->closures.size() > 0)
	{
		for(it=eventcloslist->closures.begin(); it!=eventcloslist->closures.end(); it++)
		{
			vm = (*it).vm;
			if((*it).UseInstance)
			{
				top = sq_gettop(vm);
				sq_pushobject(vm,(*it).closure);
				sq_pushobject(vm,(*it).inst);
				sq_call(vm,1,SQFalse,SQTrue);
				sq_settop(vm,top);
			}
		}
	}
	return true;
}

bool SqEvent::OnFireEvent_Post(IGameEvent *pEvent, bool bDontBroadcast)
{
	rootconsole->ConsolePrint("Inside SqEvent OnFireEvent_Post");
	return true;
}


bool SqEvent::AddToTrie(const char * key, HSQOBJECT inst, bool UseInstance, HSQOBJECT closure, HSQUIRRELVM vm)
{
	const Closures clos = { UseInstance, inst, closure,vm};
	RegisteredEvents * events = m_RegEvents.retrieve(key);
    //RegisteredEvents statevent = new RegisteredEvents;
	
	//statevent.
	if(events == NULL)
		events = new RegisteredEvents();

	events->closures.push_back(clos);
	if(!m_RegEvents.insert(key,*events))
	{
		delete & clos;
		return false;
	}
	return true;
}

const RegisteredEvents * SqEvent::FindFromTrie(const char * key)
{
	return m_RegEvents.retrieve(key);
}

bool SqEvent::RegisterNatives(SqScript * pScript)
{
	return pScript->RegisterFunction("HookEvent",Native_HookEvent,-2,".x|cs");
}
