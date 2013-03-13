#pragma once
#include "SqEnvironment.h"

struct SqGroup
{
	SqEnvironment * env;
	const char * groupname;
};

struct SqRegisteredConsts
{

	const SQChar *name;
	HSQOBJECT val;	
	SqRegisteredConsts(const SQChar *name_a,HSQOBJECT val_a)
	{
		name = name_a;
		val = val_a;
	}
};

class SqGroups
{
private:
	list<SqGroup *> m_pEnviromentList;
	list<SQClassName *> m_RegisteredClasses;
	list<SQRegFunction*> m_RegisteredFunctions;
	list<SqRegisteredConsts*> m_RegisteredConsts;

	void LoadConsts(SqEnvironment * env);
	void LoadClasses(SqEnvironment * env);
	void LoadFunctions(SqEnvironment * env);

public:
	SqGroups(void);
	//bool Init(int stack = 1024);
	bool AddGroup(const char * name,SqEnvironment * env);
	list<SqGroup *> * GetRawList(){return &m_pEnviromentList;}
	void RegisterConst(const SQChar *name,HSQOBJECT arg);
	void RegisterClass(SQClassName * classes);
	void RegisterFunction(const SQChar *name,SQFUNCTION func,int nParams,const char * sPparamsTemplate);

	~SqGroups(void);
};

