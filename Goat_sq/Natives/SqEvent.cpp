/*
*
*	This file is licensed under the GNU GPLv3
*	All the licenses are located at the root folder
*
*/

#include "SqEvent.h"

SqEvent * g_event = NULL;

//Instance , String , Closure , String , Closure ...
//String , Closure
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
	else if(top == 3)
	{
		if(sq_gettype(vm,-2) == OT_STRING && sq_gettype(vm,-1) == OT_CLOSURE)
		{
			sq_getstring(vm,-2,&key);
			HSQOBJECT obj;
			sq_resetobject(&obj);
			sq_getstackobj(vm,-1,&obj);
			sq_addref(vm,&obj);
			g_event->AddToTrie(key,inst,false,obj,vm);
		}
		sq_pop(vm,2);
	}
	sq_settop(vm,1);
	top = sq_gettop(vm);
	return 0;
}

//String , Closure
SQInteger Native_UnHookEvent(HSQUIRRELVM vm)
{
	if(g_event == NULL)
	{
		sq_settop(vm,1);
		return 0;
	}
	int top = sq_gettop(vm);
	const SQChar * key = NULL;
	HSQOBJECT obj;
	sq_resetobject(&obj);
	if(SQ_SUCCEEDED(sq_getstring(vm,-2,&key)))
	{
		sq_getstackobj(vm,-1,&obj);
		g_event->FindAndRemoveClosure(vm,key,obj);
	}
	sq_pop(vm,2);
	return 0;
}

bool SqEvent::RegisterNatives(SqScript * pScript)
{
	pScript->RegisterFunction("HookEvent",Native_HookEvent,-3,".x|ss|c");
	return pScript->RegisterFunction("UnHookEvent",Native_UnHookEvent,-2,".sc");
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

bool SqEvent::OnScriptUnloaded(HSQUIRRELVM vm)
{
	
	return true;
}

bool SqEvent::OnScriptLoaded(HSQUIRRELVM vm)
{
	
	return true;
}

bool SqEvent::OnFireEvent(IGameEvent *pEvent, bool bDontBroadcast)
{
	const RegisteredEvents * eventcloslist = FindFromTrie(pEvent->GetName());
	if(eventcloslist == NULL)
		return true;
	
	HSQUIRRELVM vm;
	List<Closures*>::iterator it;
	int top = 1;
	
	if(eventcloslist->closures.size() > 0)
	{
		for(it=eventcloslist->closures.begin(); it!=eventcloslist->closures.end(); it++)
		{
			vm = (*it)->vm;
			if((*it)->UseInstance)
			{
				top = sq_gettop(vm);
				sq_pushobject(vm,(*it)->closure);
				sq_pushobject(vm,(*it)->inst);
				sq_call(vm,1,SQFalse,SQTrue);
				sq_settop(vm,top);
			}
			else
			{
				sq_pushobject(vm,(*it)->closure);
				sq_call(vm,0,SQFalse,SQTrue);
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
	Closures * clos = new Closures();
	clos->UseInstance = UseInstance;
	clos->inst = inst;
	clos->closure = closure;
	clos->vm = vm;

	RegisteredEvents * events = m_RegEvents.retrieve(key);
    //RegisteredEvents statevent = new RegisteredEvents;
	
	//statevent.
	if(events == NULL)
		events = new RegisteredEvents();

	events->closures.push_back(clos);
	if(!m_RegEvents.insert(key,*events))
	{
		if(events->closures.size() < 2)
			delete events;

		return false;
	}
	return true;
}

const RegisteredEvents * SqEvent::FindFromTrie(const char * key)
{
	return m_RegEvents.retrieve(key);
}

bool SqEvent::RemoveFromTrie(const char * key)
{
	return m_RegEvents.remove(key);
}

bool SqEvent::FindAndRemoveClosure(HSQUIRRELVM vm, const char * key, HSQOBJECT closure)
{
	RegisteredEvents * events = m_RegEvents.retrieve(key);
	if(events == NULL)
		return false;

	List<Closures*>::iterator it;
	for(it=events->closures.begin(); it!=events->closures.end(); it++)
	{
		if((*it)->vm == vm && closure._type == (*it)->closure._type && closure._unVal.pClosure == (*it)->closure._unVal.pClosure)
		{
			if((*it)->UseInstance)
				sq_release(vm,&((*it)->inst));

			sq_release(vm,&((*it)->closure));
			events->closures.remove((*it));
			
			if(events->closures.size() < 1)
			{
				m_RegEvents.remove(key);
				return true;
			}
			delete (*it);
		}
	}
	return true;
}

