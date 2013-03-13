#ifndef SQSCRIPT_H
#define SQSCRIPT_H


#include <squirrel.h>
#include <sqhelper.h>
#include "smsdk_ext.h"
#include "ScriptManager.h"

struct SqRefFunctionEx
{
	const SQChar *name;
	SQFUNCTION f;
	SQInteger nparamscheck;
	const SQChar *typemask;
	SQBool bstatic;
};

struct SQClassName{
	SQChar * classname;
	SQChar * baseclassname;
	SqRefFunctionEx * funcs;
};

class SqScript
{
protected:
	HSQUIRRELVM m_vm;
	HSQOBJECT m_handle;
	bool m_bReleasHandle;
public:
	SqScript(HSQUIRRELVM vm, HSQOBJECT handle, bool Notify = true);
	SqScript(HSQUIRRELVM vm, bool Notify = true);
protected: 
	SqScript(){ }
public:
	~SqScript(void);

	HSQUIRRELVM GetVM(){return m_vm;}

	bool Run(const SQChar *pFunc,bool bReturn, HSQOBJECT* phReturnObj ,const char *pFormat, ...);
	bool RegisterConst(const SQChar *name,HSQOBJECT arg);
	bool RegisterClass(SQClassName * classes);
	bool RegisterFunction(const SQChar *name,SQFUNCTION func,int nParams,const char * sPparamsTemplate,SQBool bstatic = SQFalse);

	template <typename T>
    T GetProperty(const SQChar * key);
	template <typename T>
	bool SetProperty(const SQChar * key, T prop);

	bool GetObject(SQChar * name, HSQOBJECT *obj, int tableidx = 1, int idx = -1);
	void PushRootTable();


	void Uninitialize();
};

#endif // SQSCRIPT_H
