#pragma once
#include "SqEnvironment.h"

struct SqGroup
{
	SqEnvironment * env;
	char * groupname;
};

class SqGroups
{
private:
	list<SqGroup *> m_pEnviromentList;
public:
	SqGroups(void);
	//bool Init(int stack = 1024);
	bool AddGroup(const char * name,SqEnvironment * env);
	list<SqGroup *> * GetRawList(){return &m_pEnviromentList;}
	~SqGroups(void);
};

