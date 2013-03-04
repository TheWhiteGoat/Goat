#ifndef SQSCRIPT_H
#define SQSCRIPT_H


#include <squirrel.h>
#include <sqhelper.h>
#include "smsdk_ext.h"

struct SQClassName{
	SQChar * classname;
	SQChar * baseclassname;
	SQRegFunction * funcs;
};

class SqScript
{
    private:
        HSQUIRRELVM m_vm;
        HSQOBJECT m_handle;
		bool m_bReleasHandle;
    public:
        SqScript(HSQUIRRELVM vm, HSQOBJECT handle);
		SqScript::SqScript(HSQUIRRELVM vm);
		~SqScript(void);

		HSQUIRRELVM GetVM(){return m_vm;}

		bool RegisterConst(const SQChar *name,HSQOBJECT arg);
		bool RegisterClass(SQClassName * classes);
        bool Run(const SQChar *pFunc,bool bReturn, HSQOBJECT* phReturnObj ,const char *pFormat, ...);
		bool RegisterFunction(const SQChar *name,SQFUNCTION func,int nParams,const char * sPparamsTemplate);
		bool GetObject(SQChar * name,HSQOBJECT *obj);
};

#endif // SQSCRIPT_H
