#ifndef _INCLUDE_SCRIPTMANAGER_H
#define _INCLUDE_SCRIPTMANAGER_H

#include <sqrat.h>
#include <sourcehook/sh_list.h>


class IScriptListener
{
public:
	virtual bool OnScriptUnloaded(HSQUIRRELVM vm, HSQOBJECT thread ,bool IsThreadUnloading){return true; }
	virtual bool OnScriptLoaded(HSQUIRRELVM vm,HSQOBJECT thread ,bool IsThread){return true; }
};

using namespace SourceHook;

class ScriptManager
{
private:
	//List<HSQUIRRELVM> m_RegisteredVMs;
	List<IScriptListener*> m_ListenersList;
public:
	ScriptManager(void);
	bool NotifyScriptUnloaded(HSQUIRRELVM vm);
	bool NotifyScriptUnloaded(HSQUIRRELVM vm, HSQOBJECT thread);
	bool RegisterScript(HSQUIRRELVM vm);
	bool RegisterScript(HSQUIRRELVM vm, HSQOBJECT thread);
	bool AddListener(IScriptListener *listener);
	bool RemoveListener(IScriptListener *listener);
	~ScriptManager(void);
};

extern ScriptManager g_ScriptManager;

#endif