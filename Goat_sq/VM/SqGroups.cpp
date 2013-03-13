#include "SqGroups.h"


SqGroups::SqGroups(void)
{
}

void SqGroups::RegisterConst(const SQChar *name,HSQOBJECT arg)
{
	m_RegisteredConsts.push_back(new SqRegisteredConsts(name,arg));
}

void SqGroups::RegisterClass(SQClassName * classes)
{
	m_RegisteredClasses.push_back(classes);
}

void SqGroups::RegisterFunction(const SQChar *name,SQFUNCTION func,int nParams,const char * sPparamsTemplate)
{
	SQRegFunction * function = new SQRegFunction();
	function->f = func;
	function->name = name;
	function->nparamscheck = nParams;
	function->typemask = sPparamsTemplate;
	m_RegisteredFunctions.push_back(function);
}

void SqGroups::LoadConsts(SqEnvironment * env)
{
	if(m_RegisteredConsts.size() > 0)
	{
		list<SqRegisteredConsts*>::iterator it;
		for(it=m_RegisteredConsts.begin(); it!=m_RegisteredConsts.end(); it++)
			env->RegisterConst((*it)->name,(*it)->val);
	}
}

void SqGroups::LoadClasses(SqEnvironment * env)
{
	if(m_RegisteredClasses.size() > 0)
	{
		list<SQClassName*>::iterator it;
		for(it=m_RegisteredClasses.begin(); it!=m_RegisteredClasses.end(); it++)
			env->RegisterClass((*it));
	}

}

void SqGroups::LoadFunctions(SqEnvironment * env)
{
	if(m_RegisteredFunctions.size() > 0)
	{
		list<SQRegFunction*>::iterator it;
		for(it=m_RegisteredFunctions.begin(); it!=m_RegisteredFunctions.end(); it++)
			env->RegisterFunction((*it)->name,(*it)->f,(*it)->nparamscheck,(*it)->typemask);
	}

}

bool SqGroups::AddGroup(const char * name,SqEnvironment * env)
{
	if(!name || name [0] == '\0')
		return false;


	SqGroup * group = new SqGroup();
	char * nm = new char[strlen(name)];
	group->groupname = nm;
	group->env = env;
	m_pEnviromentList.push_back(group);

	LoadConsts(env);
	LoadFunctions(env);
	LoadClasses(env);
	return true;
}

SqGroups::~SqGroups(void)
{
	if(m_pEnviromentList.size () > 0)
		for(list<SqGroup *>::iterator it=m_pEnviromentList.begin(); it!=m_pEnviromentList.end(); it++)
		{
			delete (*it)->groupname;
			(*it)->env->Uninitialize();
			delete (*it)->env;
		}
}
