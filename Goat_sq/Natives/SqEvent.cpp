#include "SqEvent.h"

SqEvent  *g_SqEvent = NULL;

SqEvent::SqEvent(void)
{
	if(g_EventManager == NULL)
		g_EventManager = new EventManager();

	m_LastUnhhok.IsSet = false;
	g_EventManager->AddEventListener(this);
}

bool SqEvent::OnFireEvent_Internal(IGameEvent *pEvent, bool bDontBroadcast, bool Post)
{
	bool res = true;
	ReturnEventHook flags = CONTINUE;
	if((Post && m_RegistredEventsList.size() > 0) || 
		(!Post && m_RegistredEventsListPre.size() > 0))
	{
		RegisteredEventList *list = NULL;
		if(Post)
			list = m_RegistredEventsList.retrieve(pEvent->GetName());
		else
			list = m_RegistredEventsListPre.retrieve(pEvent->GetName());

		if(!list)
			return true;

		
		if(list->size() > 0)
		{
			SqGameEvent ge;
			ge.SetGameEvent(pEvent);

			RegisteredEventList::iterator it;
			for(it=list->begin(); it!=list->end(); it++)
			{
				res = (*it)->Evaluate<bool,SqGameEvent*,bool>(&ge, bDontBroadcast);
				if(!Post && ge.IsDontBroadcastSet())
					g_EventManager->SetDontBroadcast(ge.GetDontBroadcast());

				if(m_LastUnhhok.IsSet && m_LastUnhhok.eventname == pEvent->GetName())
				{
					if(m_LastUnhhok.IsListRemoved)
						break;
					
					it = m_LastUnhhok.fixedit;
				}
				m_LastUnhhok.IsSet = false;
			}
		}
		else
			m_RegistredEventsList.remove(pEvent->GetName());
	}
	return res;
}

bool SqEvent::OnFireEvent(IGameEvent *pEvent, bool bDontBroadcast)
{
	return OnFireEvent_Internal(pEvent,bDontBroadcast,false);
}

bool SqEvent::OnFireEvent_Post(IGameEvent *pEvent, bool bDontBroadcast)
{
	OnFireEvent_Internal(pEvent,bDontBroadcast,true);
	return true;
}

void SqEvent::AddToTrie(const char *key, Sqrat::Function *function, bool PostEvent)
{
	if(PostEvent)
	{
		RegisteredEventList *list = m_RegistredEventsList.retrieve(key);
		if(list)
			list->push_back(function);
		else
		{
			RegisteredEventList lst;
			lst.push_back(function);
			m_RegistredEventsList.insert(key,lst);
		}
	}
	else
	{
		RegisteredEventList *list = m_RegistredEventsListPre.retrieve(key);
		if(list)
			list->push_back(function);
		else
		{
			RegisteredEventList lst;
			lst.push_back(function);
			m_RegistredEventsListPre.insert(key,lst);
		}
	}
}

void SqEvent::HookEvent(Sqrat::Object instance, Sqrat::string eventname, Sqrat::Function function)
{
	if(instance.GetType() == OT_INSTANCE)
	{
		g_SqEvent->AddToTrie(eventname.data(), new Function(function.GetVM(),instance,function.GetFunc()),false);
	}
}

void SqEvent::HookEventStatic(Sqrat::string eventname, Sqrat::Function function)
{
	g_SqEvent->AddToTrie(eventname.data(), new Function(function),false);
}

void SqEvent::HookEventPost(Sqrat::Object instance, Sqrat::string eventname, Sqrat::Function function)
{
	if(instance.GetType() == OT_INSTANCE)
	{
		g_SqEvent->AddToTrie(eventname.data(), new Function(function.GetVM(),instance,function.GetFunc()),true);
	}
}

void SqEvent::HookEventPostStatic(Sqrat::string eventname, Sqrat::Function function)
{
	g_SqEvent->AddToTrie(eventname.data(), new Function(function),true);
}

void SqEvent::UnHookEvent_internal(Sqrat::string eventname, Sqrat::Function function, bool Post)
{
	RegisteredEventKTrie *trie = NULL;
	if(Post)
		trie = g_SqEvent->GetRegisteredEventsList();
	else
		trie = g_SqEvent->GetRegisteredEventsListPre();

	if(trie)
	{
		RegisteredEventList *list = trie->retrieve(eventname.data());
		RegisteredEventList::iterator it;
		if(list && list->size() > 0)
		{
			for(it=list->begin(); it!=list->end(); it++)
			{
				if((*it)->GetFunc()._unVal.pClosure == function.GetFunc()._unVal.pClosure &&
					function.GetVM() == (*it)->GetVM()) //thats nice...
				{
					UnHookSelf *self = g_SqEvent->GetLastUnhhokStrcut();
					self->eventname = eventname;
					it = list->erase(it);
					self->fixedit = it;
					self->IsSet = true;
					if(list->size() == 0)
					{
						if(trie->remove(eventname.data()))
							self->IsListRemoved = true;

						break;
					}
				}
			}
		}
	}
}

void SqEvent::UnHookEvent(Sqrat::string eventname, Sqrat::Function function)
{
	g_SqEvent->UnHookEvent_internal(eventname,function,true);
}

void SqEvent::UnHookEventPre(Sqrat::string eventname, Sqrat::Function function)
{
	g_SqEvent->UnHookEvent_internal(eventname,function,false);
}

SqGameEvent *SqEvent::CreateEvent(Sqrat::string eventname,bool bForce)
{
	IGameEvent *gevent = gameevents->CreateEvent(eventname.data(),bForce);
	if(gevent)
	{
		SqGameEvent *sge = new SqGameEvent();
		sge->SetGameEvent(gevent);
		return sge;
	}
	return NULL;
}

bool SqEvent::FireEvent(SqGameEvent *sqevent, bool bDontBroadcast)
{
	if(sqevent)
	{
		bool res = gameevents->FireEvent(sqevent->GetGameEvent(),bDontBroadcast);
		delete sqevent;
		return res;
	}
	return false;
}

void SqEvent::FreeEvent(SqGameEvent *sqevent)
{
	if(sqevent)
	{
		gameevents->FreeEvent(sqevent->GetGameEvent());
		delete sqevent;
	}
}

SqGameEvent *SqEvent::DuplicateEvent(SqGameEvent *sqevent)
{
	SqGameEvent *sge = new SqGameEvent();
	IGameEvent *gevent = gameevents->DuplicateEvent(sqevent->GetGameEvent());
	if(gevent == NULL)
	{
		delete sge;
		return NULL;
	}

	sge->SetGameEvent(gevent);
	return sge;
}

void SqEvent::SetDontBroadcast(SqGameEvent *sqevent, bool bDontBroadcast)
{
	sqevent->SetDontBroadcast(bDontBroadcast);
}

void SqEvent::RegisterInVm(Script *vm)
{
	if(g_SqEvent == NULL)
		g_SqEvent = new SqEvent();

	DefaultVM().Set(vm->GetVM());

	RootTable()
		.Overload(_SC("HookEvent"),&SqEvent::HookEventPost)
		.Overload(_SC("HookEvent"),&SqEvent::HookEventPostStatic)

		.Overload(_SC("HookEventPre"),&SqEvent::HookEvent)
		.Overload(_SC("HookEventPre"),&SqEvent::HookEventStatic)

		.Func(_SC("UnHookEvent"),&SqEvent::UnHookEvent)
		.Func(_SC("UnHookEventPre"),&SqEvent::UnHookEventPre)

		.Func(_SC("CreateEvent"),&SqEvent::CreateEvent)
		.Func(_SC("FireEvent"),&SqEvent::FireEvent)
		.Func(_SC("FreeEvent"),&SqEvent::FreeEvent)
		.Func(_SC("DuplicateEvent"),&SqEvent::DuplicateEvent)
		;

	ConstTable().Enum(_SC("EventHook"),Enumeration()
		.Const(_SC("Continue"),ReturnEventHook::CONTINUE)
		.Const(_SC("Stop"),ReturnEventHook::STOP));

	RootTable().Bind(_SC("GameEvent"),
		Class<SqGameEvent,NoCopy<SqGameEvent>>()
		.Func(_SC("GetName"),&SqGameEvent::SqGetName)

		.Func(_SC("IsReliable"),&SqGameEvent::SqIsReliable)
		.Func(_SC("IsLocal"),&SqGameEvent::SqIsLocal)
		.Func(_SC("IsEmpty"),&SqGameEvent::SqIsEmpty)
		//Getters
		.Func(_SC("GetBool"),&SqGameEvent::SqGetBool)
		.Func(_SC("GetInt"),&SqGameEvent::SqGetInt)
		.Func(_SC("GetFloat"),&SqGameEvent::SqGetFloat)
		.Func(_SC("GetString"),&SqGameEvent::SqGetString)
		//Setters
		.Func(_SC("SetBool"),&SqGameEvent::SqSetBool)
		.Func(_SC("SetInt"),&SqGameEvent::SqSetInt)
		.Func(_SC("SetFloat"),&SqGameEvent::SqSetFloat)
		.Func(_SC("SetString"),&SqGameEvent::SqSetString)
		);
}

SqEvent::~SqEvent(void)
{
	if(g_EventManager)
		g_EventManager->RemoveEventListener(this);
}
