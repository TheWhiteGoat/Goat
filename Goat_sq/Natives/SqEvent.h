/*
*
*	This file is licensed under the GNU GPLv3
*	All the licenses are located at the root folder
*
*/

#pragma once
#include "SqEnvironment.h"
#include "EventManager.h"
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
	List<Closures> closures;
};

class SqEvent : public IEventListener
{
private:
	KTrie<RegisteredEvents > m_RegEvents;
public:
	virtual bool OnFireEvent(IGameEvent *pEvent, bool bDontBroadcast);
	virtual bool OnFireEvent_Post(IGameEvent *pEvent, bool bDontBroadcast);
public:
	bool AddToTrie(const char * key,HSQOBJECT inst,bool UseInstance,HSQOBJECT closure,HSQUIRRELVM vm);
	const RegisteredEvents * FindFromTrie(const char * key);
	static bool RegisterNatives(SqScript * pScript);
    //KTrie<RegisteredEvents> * GetTrie(){ return m_RegEvetns;}
	SqEvent(void);
	~SqEvent(void);
};

