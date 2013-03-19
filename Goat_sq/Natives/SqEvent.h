#pragma once
#include "EventManager.h"
#include <Sqrat.h>
#include <sm_trie_tpl.h>
#include <sourcehook/sh_list.h>

using namespace Sqrat;
class SqGameEvent;

class SqEvent : public IEventListener
{
private:	
	typedef SourceHook::List<Function*> RegisteredEventList;
	typedef KTrie<RegisteredEventList> RegisteredEventKTrie;
	/*struct FunctionAndInst
	{
		Object inst;
		Function f;
	};*/

	struct UnHookSelf
	{
		bool IsSet;
		bool IsListRemoved;
		Sqrat::string eventname;
		RegisteredEventList::iterator fixedit;
	};
private:
	UnHookSelf m_LastUnhhok;
	RegisteredEventKTrie m_RegistredEventsList;
	RegisteredEventKTrie m_RegistredEventsListPre;
public:
	SqEvent(void);
	~SqEvent(void);

	bool OnFireEvent_Internal(IGameEvent *pEvent, bool bDontBroadcast, bool Post);
	virtual bool OnFireEvent(IGameEvent *pEvent, bool bDontBroadcast);
	virtual bool OnFireEvent_Post(IGameEvent *pEvent, bool bDontBroadcast);

	RegisteredEventKTrie *GetRegisteredEventsList(){return &m_RegistredEventsList; }
	RegisteredEventKTrie *GetRegisteredEventsListPre(){return &m_RegistredEventsListPre; }
	UnHookSelf *GetLastUnhhokStrcut(){return &m_LastUnhhok;}

	void AddToTrie(const char *key, Sqrat::Function *function, bool PostEvent);
	bool RemoveFromTrie(const char *key){return m_RegistredEventsList.remove(key); }

	static void RegisterInVm(Script *vm);

	static void HookEvent(Sqrat::Object instance, Sqrat::string eventname, Sqrat::Function function);
	static void HookEventStatic(Sqrat::string eventname, Sqrat::Function function);

	static void HookEventPost(Sqrat::Object instance, Sqrat::string eventname, Sqrat::Function function);
	static void HookEventPostStatic(Sqrat::string eventname, Sqrat::Function function);

	void UnHookEvent_internal(Sqrat::string eventname, Sqrat::Function function, bool Post);
	static void UnHookEvent(Sqrat::string eventname, Sqrat::Function function);
	static void UnHookEventPre(Sqrat::string eventname, Sqrat::Function function);

	static SqGameEvent *CreateEvent(Sqrat::string eventname, bool bForce);
	static bool FireEvent(SqGameEvent *sqevent, bool bDontBroadcast);
	static void FreeEvent(SqGameEvent *sqevent);
	static SqGameEvent *DuplicateEvent(SqGameEvent *sqevent);
	static void SetDontBroadcast(SqGameEvent *sqevent, bool bDontBroadcast);
};

//wrapper for the virtuals
class SqGameEvent
{
private:
	IGameEvent *m_pGameEvent;
	bool m_bDontBroadcast;
	bool m_bOverrideSet;
public:
	void SetDontBroadcast(bool bDontBroadcast){ m_bDontBroadcast = bDontBroadcast; m_bOverrideSet = true;}
	bool GetDontBroadcast(){return m_bDontBroadcast; }
	bool IsDontBroadcastSet(){return m_bOverrideSet; }
	void SetGameEvent(IGameEvent *pEvent){m_pGameEvent = pEvent; } //used only in C++
	IGameEvent *GetGameEvent(){return m_pGameEvent; }

	SqGameEvent(){m_pGameEvent = NULL; m_bOverrideSet = false; }
	~SqGameEvent(){}

	Sqrat::string SqGetName(){return m_pGameEvent->GetName(); }	// get event name

	bool  SqIsReliable(){return m_pGameEvent->IsReliable(); } // if event handled reliable
	bool  SqIsLocal(){return m_pGameEvent->IsLocal(); } // if event is never networked
	bool  SqIsEmpty(Sqrat::string keyName = NULL){return m_pGameEvent->IsEmpty(); } // check if data field exists
	
	// Data access
	bool  SqGetBool( Sqrat::string keyName = NULL, bool defaultValue = false ){return m_pGameEvent->GetBool(keyName.data(),defaultValue); }
	int   SqGetInt( Sqrat::string keyName = NULL, int defaultValue = 0 ){return m_pGameEvent->GetInt(keyName.data(),defaultValue); }
	float SqGetFloat( Sqrat::string keyName = NULL, float defaultValue = 0.0f ){return m_pGameEvent->GetFloat(keyName.data(),defaultValue); }
	Sqrat::string SqGetString( Sqrat::string keyName = NULL, const char *defaultValue = "" ){return m_pGameEvent->GetString(keyName.data(),defaultValue); }

	void SqSetBool( Sqrat::string keyName, bool value ){return m_pGameEvent->SetBool(keyName.data(),value); }
	void SqSetInt( Sqrat::string keyName, int value ){return m_pGameEvent->SetInt(keyName.data(),value); }
	void SqSetFloat( Sqrat::string keyName, float value ){return m_pGameEvent->SetFloat(keyName.data(),value); }
	void SqSetString( Sqrat::string keyName, Sqrat::string value ){return m_pGameEvent->SetString(keyName.data(),value.data()); }
};