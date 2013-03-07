#include "SqGroups.h"


SqGroups::SqGroups(void)
{
}

bool SqGroups::AddGroup(const char * name,SqEnvironment * env)
{
	if(!name || name [0] == '\0')
		return false;

	SqGroup * group = new SqGroup;
	char * nm = new char[strlen(name)];
	group->groupname = (char *)nm;
	group->env = env;
	return true;
}

SqGroups::~SqGroups(void)
{
	if(m_pEnviromentList.size () > 0)
		for(list<SqGroup *>::iterator it = m_pEnviromentList.begin(); it!=m_pEnviromentList.end();it++)
		{
			delete (*it)->groupname;
			(*it)->env->Uninitialize();
			delete (*it)->env;
		}
}
