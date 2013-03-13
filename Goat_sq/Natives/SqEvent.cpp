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
						g_event->AddToTrie(key,inst,true,obj,vm,true,false);
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
			g_event->AddToTrie(key,inst,false,obj,vm,true,false);
		}
		sq_pop(vm,2);
	}
	sq_settop(vm,1);
	return 0;
}

SQInteger Native_HookEventPre(HSQUIRRELVM vm)
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
						g_event->AddToTrie(key,inst,true,obj,vm,false,true);
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
			g_event->AddToTrie(key,inst,false,obj,vm,false,true);
		}
		sq_pop(vm,2);
	}
	sq_settop(vm,1);
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
	if(SQ_SUCCEEDED(sq_getstring(vm,-2,&key)) && sq_gettype(vm,-1) == OT_CLOSURE)
	{
		sq_getstackobj(vm,-1,&obj);
		g_event->FindAndRemoveClosure(vm,key,obj);
	}
	sq_settop(vm,1);
	return 0;
}

SQ_CLASS_FUNC_DEF(GameEvent,GetName)
{
	IGameEvent *gameevent = sq_getinstance<IGameEvent*>(vm);
	if(gameevent)
	{
		sq_pushstring(vm,gameevent->GetName(),-1);
		return 1;
	}
	return 0;
}

SQ_CLASS_FUNC_DEF(GameEvent,IsReliable)
{
	IGameEvent *gameevent = sq_getinstance<IGameEvent*>(vm);
	if(gameevent)
	{
		sq_pushbool(vm,gameevent->IsReliable());
		return 1;
	}
	return 0;
}

SQ_CLASS_FUNC_DEF(GameEvent,IsLocal)
{
	IGameEvent *gameevent = sq_getinstance<IGameEvent*>(vm);
	if(gameevent)
	{
		sq_pushbool(vm,gameevent->IsLocal());
		return 1;
	}
	return 0;
}

SQ_CLASS_FUNC_DEF(GameEvent,IsEmpty)
{
	IGameEvent *gameevent = sq_getinstance<IGameEvent*>(vm);
	if(gameevent)
	{
		sq_pushbool(vm,gameevent->IsEmpty());
		return 1;
	}
	return 0;
}

SQ_CLASS_FUNC_DEF(GameEvent,GetBool)
{
	IGameEvent *gameevent = sq_getinstance<IGameEvent*>(vm);
	if(gameevent)
	{
		const SQChar * keyname = NULL;
		SQBool defaultret;
		if(SQ_SUCCEEDED(sq_getstring(vm,-2,&keyname)))
		{
			sq_getbool(vm,-1,&defaultret);
			sq_pop(vm,2);
			sq_pushbool(vm,(SQBool)gameevent->GetBool(keyname,defaultret));
			return 1;
		}
	}
	sq_pop(vm,2);
	return 0;
}

SQ_CLASS_FUNC_DEF(GameEvent,GetInt)
{
	IGameEvent *gameevent = sq_getinstance<IGameEvent*>(vm);
	if(gameevent)
	{
		const SQChar * keyname = NULL;
		SQInteger defaultret;
		if(SQ_SUCCEEDED(sq_getstring(vm,-2,&keyname)))
		{
			sq_getinteger(vm,-1,&defaultret);
			sq_pop(vm,2);
			sq_pushinteger(vm,gameevent->GetInt(keyname,defaultret));
			return 1;
		}
	}
	sq_pop(vm,2);
	return 0;
}

SQ_CLASS_FUNC_DEF(GameEvent,GetFloat)
{
	IGameEvent *gameevent = sq_getinstance<IGameEvent*>(vm);
	if(gameevent)
	{
		const SQChar * keyname = NULL;
		SQFloat defaultret;
		if(SQ_SUCCEEDED(sq_getstring(vm,-2,&keyname)))
		{
			sq_getfloat(vm,-1,&defaultret);
			sq_pop(vm,2);
			sq_pushfloat(vm,gameevent->GetFloat(keyname,defaultret));
			return 1;
		}
	}
	sq_pop(vm,2);
	return 0;
}

SQ_CLASS_FUNC_DEF(GameEvent,GetString)
{
	IGameEvent *gameevent = sq_getinstance<IGameEvent*>(vm);
	if(gameevent)
	{
		const SQChar * keyname = NULL;
		const SQChar * defaultret;
		if(SQ_SUCCEEDED(sq_getstring(vm,-2,&keyname)))
		{
			sq_getstring(vm,-1,&defaultret);
			sq_pop(vm,2);
			sq_pushstring(vm,gameevent->GetString(keyname,defaultret),-1);
			return 1;
		}
	}
	sq_pop(vm,2);
	return 0;
}

SQ_CLASS_FUNC_DEF(GameEvent,SetBool)
{
	IGameEvent *gameevent = sq_getinstance<IGameEvent*>(vm);
	if(gameevent)
	{
		const SQChar * keyname = NULL;
		SQBool setval;
		if(SQ_SUCCEEDED(sq_getstring(vm,-2,&keyname)))
		{
			sq_getbool(vm,-1,&setval);
			gameevent->SetBool(keyname,(bool)setval);
		}
	}
	sq_pop(vm,2);
	return 0;
}

SQ_CLASS_FUNC_DEF(GameEvent,SetInt)
{
	IGameEvent *gameevent = sq_getinstance<IGameEvent*>(vm);
	if(gameevent)
	{
		const SQChar * keyname = NULL;
		SQInteger setval;
		if(SQ_SUCCEEDED(sq_getstring(vm,-2,&keyname)))
		{
			sq_getinteger(vm,-1,&setval);
			gameevent->SetInt(keyname,setval);
		}
	}
	sq_pop(vm,2);
	return 0;
}

SQ_CLASS_FUNC_DEF(GameEvent,SetFloat)
{
	IGameEvent *gameevent = sq_getinstance<IGameEvent*>(vm);
	if(gameevent)
	{
		const SQChar * keyname = NULL;
		SQFloat setval;
		if(SQ_SUCCEEDED(sq_getstring(vm,-2,&keyname)))
		{
			sq_getfloat(vm,-1,&setval);
			gameevent->SetFloat(keyname,setval);
		}
	}
	sq_pop(vm,2);
	return 0;
}

SQ_CLASS_FUNC_DEF(GameEvent,SetString)
{
	IGameEvent *gameevent = sq_getinstance<IGameEvent*>(vm);
	if(gameevent)
	{
		const SQChar * keyname = NULL;
		const SQChar * setval;
		if(SQ_SUCCEEDED(sq_getstring(vm,-2,&keyname)))
		{
			sq_getstring(vm,-1,&setval);
			gameevent->SetString(keyname,setval);
		}
	}
	sq_pop(vm,2);
	return 0;
}

SQ_CLASS_BUILD_START(GameEvent)
//SQ_CLASS_FUNC(GameEvent,constructor,2,".i")

SQ_CLASS_FUNC(GameEvent,GetName,1,".")

SQ_CLASS_FUNC(GameEvent,IsReliable,1,".")
SQ_CLASS_FUNC(GameEvent,IsLocal,1,".")
SQ_CLASS_FUNC(GameEvent,IsEmpty,2,".s")

SQ_CLASS_FUNC(GameEvent,GetBool,3,".sb")
SQ_CLASS_FUNC(GameEvent,GetInt,3,".si")
SQ_CLASS_FUNC(GameEvent,GetFloat,3,".sf")
SQ_CLASS_FUNC(GameEvent,GetString,3,".ss")

SQ_CLASS_FUNC(GameEvent,SetBool,3,".sb")
SQ_CLASS_FUNC(GameEvent,SetInt,3,".si")
SQ_CLASS_FUNC(GameEvent,SetFloat,3,".sf")
SQ_CLASS_FUNC(GameEvent,SetString,3,".ss")
SQ_CLASS_BUILD_END(GameEvent)

bool SqEvent::RegisterNatives(SqGroups * pGroups)
{
	pGroups->RegisterFunction("HookEvent",Native_HookEvent,-3,".x|ss|c");
	pGroups->RegisterFunction("HookEventPre",Native_HookEventPre,-3,".x|ss|c");
	pGroups->RegisterFunction("UnHookEvent",Native_UnHookEvent,-3,".sc");
	pGroups->RegisterClass(&SQ_CLASS_GET(GameEvent));
	return true;
}

SqEvent::SqEvent()
{
	if(g_EventManager == NULL)
		g_EventManager = new EventManager();

	if(!g_EventManager->AddEventListener(this))
		g_pSM->LogError(myself,"Failed adding event listener");

	m_Args.isset = false;
}

SqEvent::~SqEvent()
{
	g_EventManager->RemoveEventListener(this);
}

bool SqEvent::OnScriptUnloaded(HSQUIRRELVM vm,HSQOBJECT thread ,bool IsThread)
{
	if(m_RegEvents.size() > 0)
	{
		List<RegisteredEvents*>::iterator it;
		for(it=m_Closures.begin(); it!=m_Closures.end(); it++)
		{
			List<Closures*>::iterator itb;
			List<Closures*> *closures = &(*it)->closures;
			if(closures->size() > 0)
			{
				for(itb=closures->begin(); itb!=closures->end(); itb++)
				{
					if((*itb)->vm == vm)
					{
						(*it)->closures.remove((*itb));
						delete (*itb);
					}
				}
			}
		}
	}
	return true;
}

bool SqEvent::OnScriptLoaded(HSQUIRRELVM vm,HSQOBJECT thread ,bool IsThread)
{
	
	return true;
}

bool SqEvent::OnFireEvent_Post(IGameEvent *pEvent_wrong, bool bDontBroadcast_wrong)//Since it is a post hook the arguments are wrong
{
	if(!m_Args.IsSet())
		return true;

	IGameEvent *pEvent;
	bool bDontBroadcast;
	m_Args.Get(&pEvent,&bDontBroadcast);
	
	const RegisteredEvents * eventcloslist = FindFromTrie(pEvent->GetName());
	if(eventcloslist == NULL)
		return true;
	
	HSQUIRRELVM vm;
	HSQOBJECT eventinst;
	sq_resetobject(&eventinst);
	List<Closures*>::iterator it;
	int top = 1;
	
	if(eventcloslist->closures.size() > 0)
	{
		for(it=eventcloslist->closures.begin(); it!=eventcloslist->closures.end(); it++)
		{
			if(!(*it)->IsPostHook())
				continue;

			vm = (*it)->vm;
			sq_pushroottable(vm);
			sq_pushstring(vm,_SC("GameEvent"),-1);
			if(SQ_SUCCEEDED(sq_get(vm,-2)))
			{
				sq_createinstance(vm,-1);
				sq_setinstance<IGameEvent *>(vm,pEvent,-1);
				sq_getstackobj(vm,-1,&eventinst);
				sq_addref(vm,&eventinst);
				sq_pop(vm,3);
				
				if((*it)->IsMemberClosure())
				{
					sq_pushobject(vm,(*it)->closure);
					sq_pushobject(vm,(*it)->inst);
					sq_pushobject(vm,eventinst);
					sq_call(vm,2,SQFalse,SQTrue);
				}
				else
				{
					sq_pushobject(vm,(*it)->closure);
					sq_pushobject(vm,eventinst);
					sq_call(vm,1,SQFalse,SQTrue);
				}
				sq_settop(vm,1);
				sq_release(vm,&eventinst);
			}
		}
	}
	gameevents->FreeEvent(pEvent);
	return true;
}

bool SqEvent::OnFireEvent(IGameEvent *pEvent, bool bDontBroadcast)
{
	m_Args.Set(true,gameevents->DuplicateEvent(pEvent),bDontBroadcast);

	const RegisteredEvents * eventcloslist = FindFromTrie(pEvent->GetName());
	if(eventcloslist == NULL)
		return true;
	
	HSQUIRRELVM vm;
	HSQOBJECT eventinst;
	sq_resetobject(&eventinst);
	List<Closures*>::iterator it;
	int top = 1;
	
	if(eventcloslist->closures.size() > 0)
	{
		for(it=eventcloslist->closures.begin(); it!=eventcloslist->closures.end(); it++)
		{
			if(!(*it)->IsPreHook())
				continue;

			vm = (*it)->vm;
			sq_pushroottable(vm);
			sq_pushstring(vm,_SC("GameEvent"),-1);
			if(SQ_SUCCEEDED(sq_get(vm,-2)))
			{
				sq_createinstance(vm,-1);
				sq_setinstance<IGameEvent *>(vm,pEvent,-1);
				sq_getstackobj(vm,-1,&eventinst);
				sq_addref(vm,&eventinst);
				sq_pop(vm,3);
				
				if((*it)->IsMemberClosure())
				{
					sq_pushobject(vm,(*it)->closure);
					sq_pushobject(vm,(*it)->inst);
					sq_pushobject(vm,eventinst);
					sq_call(vm,2,SQFalse,SQTrue);
				}
				else
				{
					sq_pushobject(vm,(*it)->closure);
					sq_pushobject(vm,eventinst);
					sq_call(vm,1,SQFalse,SQTrue);
					
				}
				sq_settop(vm,1);
				sq_release(vm,&eventinst);
			}
		}
	}
	return true;
}


bool SqEvent::AddToTrie(const char * key, HSQOBJECT inst, bool UseInstance, HSQOBJECT closure, HSQUIRRELVM vm, bool Post, bool Pre)
{
	Closures * clos = new Closures();
	clos->Set(Pre,Post,UseInstance,inst,closure,vm);

	RegisteredEvents * events = m_RegEvents.retrieve(key);
	if(events)
	{
		events->closures.push_back(clos);
		return true;
	}

	events = new RegisteredEvents();
	events->closures.push_back(clos);
	if(!m_RegEvents.insert(key,*events))
	{
		delete clos;

		if(events->closures.size() < 2)
			delete events;

		return false;
	}
	else
		m_Closures.push_back(events);
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
				m_Closures.remove(events);
				return true;
			}
			delete (*it);
		}
	}
	return true;
}

