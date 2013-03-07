/*
*
*	This file is licensed under the GNU GPLv3
*	All the licenses are located at the root folder
*
*/

#pragma once
#include "SqEnvironment.h"
#include "EventManager.h"
#include "ScriptManager.h"

#include <sm_trie_tpl.h>

using namespace SourceHook;

struct Closures
{
	bool UseInstance;
	HSQOBJECT inst;
	HSQOBJECT closure;
	HSQUIRRELVM vm;
};

struct RegisteredEvents
{
	List<Closures*> closures;
};

class SqEvent :
	public IScriptListener,
	public IEventListener
{
private:
	KTrie<RegisteredEvents> m_RegEvents;
public:
	virtual bool OnScriptUnloaded(HSQUIRRELVM vm);
	virtual bool OnScriptLoaded(HSQUIRRELVM vm);
public:
	virtual bool OnFireEvent(IGameEvent *pEvent, bool bDontBroadcast);
	virtual bool OnFireEvent_Post(IGameEvent *pEvent, bool bDontBroadcast);
public:
	bool AddToTrie(const char * key,HSQOBJECT inst,bool UseInstance,HSQOBJECT closure,HSQUIRRELVM vm);
	const RegisteredEvents * FindFromTrie(const char * key);
	bool RemoveFromTrie(const char * key);
	bool FindAndRemoveClosure(HSQUIRRELVM vm, const char * key, HSQOBJECT closure);
	static bool RegisterNatives(SqScript * pScript);
    KTrie<RegisteredEvents> * GetTrie(){ return &m_RegEvents;}
	SqEvent(void);
	~SqEvent(void);
};

