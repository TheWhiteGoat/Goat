#include "ScriptManager.h"

ScriptManager g_ScriptManager;

ScriptManager::ScriptManager(void)
{
}

bool ScriptManager::NotifyScriptUnloaded(HSQUIRRELVM vm)
{
	HSQOBJECT obj;
	sq_resetobject(&obj);
	if(m_ListenersList.size() > 0)
	{
		List<IScriptListener*>::iterator it;
		for(it=m_ListenersList.begin(); it!=m_ListenersList.end(); it++)
			(*it)->OnScriptUnloaded(vm,obj,false);
	}
	else
		return false;

	return true;
}

bool ScriptManager::NotifyScriptUnloaded(HSQUIRRELVM vm, HSQOBJECT thread)
{
	if(m_ListenersList.size() > 0)
	{
		List<IScriptListener*>::iterator it;
		for(it=m_ListenersList.begin(); it!=m_ListenersList.end(); it++)
			(*it)->OnScriptUnloaded(vm,thread,true);
	}
	else
		return false;

	return true;
}

bool ScriptManager::RegisterScript(HSQUIRRELVM vm)
{
	HSQOBJECT obj;
	sq_resetobject(&obj);
	if(m_ListenersList.size() > 0)
	{
		List<IScriptListener*>::iterator it;
		for(it=m_ListenersList.begin(); it!=m_ListenersList.end(); it++)
			(*it)->OnScriptLoaded(vm,obj,false);
	}
	else
		return false;

	return true;
}

bool ScriptManager::RegisterScript(HSQUIRRELVM vm, HSQOBJECT thread)
{
	if(m_ListenersList.size() > 0)
	{
		List<IScriptListener*>::iterator it;
		for(it=m_ListenersList.begin(); it!=m_ListenersList.end(); it++)
			(*it)->OnScriptLoaded(vm,thread,true);
	}
	else
		return false;

	return true;
}

bool ScriptManager::AddListener(IScriptListener *listener)
{
	m_ListenersList.push_back(listener);
	return true;
}

bool ScriptManager::RemoveListener(IScriptListener *listener)
{
	m_ListenersList.remove(listener);
	return true;
}

ScriptManager::~ScriptManager(void)
{
}
