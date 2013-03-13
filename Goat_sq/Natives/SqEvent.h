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
#include "SqGroups.h"

#include <sm_trie_tpl.h>

using namespace SourceHook;

struct Closures
{
	bool Pre;
	bool Post;
	bool UseInstance;
	HSQOBJECT inst;
	HSQOBJECT closure;
	HSQUIRRELVM vm;

	bool IsPreHook(){return Pre;}
	bool IsPostHook(){return Post;}
	bool IsMemberClosure(){return UseInstance;}
	void Set(bool Pre_a, bool Post_a, bool UseInstance_a, HSQOBJECT inst_a,HSQOBJECT closure_a,HSQUIRRELVM vm_a)
	{Pre = Pre_a; Post = Post_a; UseInstance = UseInstance_a; inst = inst_a; closure = closure_a; vm = vm_a;}
};

struct RegisteredEvents
{
	List<Closures*> closures;
};

struct OnFireArgs
{
	bool isset;
	IGameEvent *gamevent;
	bool bDontBroadcast;

	void Set(bool IsSet_a, IGameEvent *gamevent_a, bool bDontBroadcast_a)
	{isset = IsSet_a; gamevent = gamevent_a; bDontBroadcast = bDontBroadcast_a;}

	void Get(IGameEvent **gamevent_a, bool *bDontBroadcast_a)
	{*gamevent_a = gamevent; *bDontBroadcast_a = bDontBroadcast;}
	bool IsSet(){ return isset;}

};

class SqEvent :
	public IScriptListener,
	public IEventListener
{
private:
	KTrie<RegisteredEvents> m_RegEvents;
	List<RegisteredEvents*> m_Closures;
	OnFireArgs m_Args;
public:
	virtual bool OnScriptUnloaded(HSQUIRRELVM vm,HSQOBJECT thread ,bool IsThread);
	virtual bool OnScriptLoaded(HSQUIRRELVM vm,HSQOBJECT thread ,bool IsThread);
public:
	virtual bool OnFireEvent(IGameEvent *pEvent, bool bDontBroadcast);
	virtual bool OnFireEvent_Post(IGameEvent *pEvent, bool bDontBroadcast);
public:
	bool AddToTrie(const char * key, HSQOBJECT inst, bool UseInstance, HSQOBJECT closure, HSQUIRRELVM vm, bool Post, bool Pre);
	const RegisteredEvents * FindFromTrie(const char * key);
	bool RemoveFromTrie(const char * key);
	bool FindAndRemoveClosure(HSQUIRRELVM vm, const char * key, HSQOBJECT closure);
	static bool RegisterNatives(SqGroups * pGroups);
    KTrie<RegisteredEvents> * GetTrie(){ return &m_RegEvents;}
	SqEvent(void);
	~SqEvent(void);
};

