#include "SqScript.h"
#include <stdarg.h>
#include <cstddef>
#include <stdio.h>

SqScript::SqScript(HSQUIRRELVM vm, HSQOBJECT handle)
{
    m_vm = vm;
    m_handle = handle;
	m_bReleasHandle = true;
}

SqScript::SqScript(HSQUIRRELVM vm)
{
    m_vm = vm;
	m_bReleasHandle = false;
}

bool SqScript::Run(const SQChar *pFunc,bool bReturn, HSQOBJECT* phReturnObj,const char *pFormat, ...)
{
    bool success = false;
    SQInteger top = sq_gettop(m_vm);
    sq_pushstring(m_vm, pFunc, -1);
    if (SQ_SUCCEEDED(sq_get(m_vm, -2)))
    {
        sq_push(m_vm, 1);   // Function table as 'this' argument
        SQInteger args = 1;
        va_list argList;

        va_start(argList, pFormat);

        while (*pFormat != NULL)
        {
            switch (*pFormat)
            {
            case 'd':
                ++args;
                sq_pushinteger(m_vm, (SQInteger)(va_arg(argList, int)));
                break;
            case 'f':
                ++args;
                sq_pushfloat(m_vm, (SQFloat)(va_arg(argList, double)));
                break;
            case 's':
                ++args;
                sq_pushstring(m_vm, va_arg(argList, const SQChar *), -1);
                break;
            case '%':
                break;
            }

            ++pFormat;
        }

        va_end(argList);
        bool success = SQ_SUCCEEDED(sq_call(m_vm, args, bReturn, SQTrue));
    }

	if(success && bReturn)
		success = SQ_SUCCEEDED(sq_getstackobj(m_vm,-1,phReturnObj));

    sq_settop(m_vm, top);
    return success;
}

bool SqScript::RegisterClass(SQClassName * classname)
{
	if(!classname)
		return false;

	int top = sq_gettop(m_vm);
	sq_pushstring(m_vm, classname->classname, -1);
	if(classname->baseclassname)
	{
		// Push the base class name onto the stack
		sq_pushstring(m_vm, classname->baseclassname, -1);

		// Attempt to get the base class
		if(SQ_FAILED(sq_get(m_vm, -3)))
		{
			// Failed to get the base class
			sq_settop(m_vm, top);
			return false;
		}
	}

	if(SQ_FAILED(sq_newclass(m_vm, classname->baseclassname ? 1 : 0)))
	{
		// Failed to create the class, Restore the stack top
		sq_settop(m_vm, top);
		return false;
	}

	SQRegFunction * funcs = classname->funcs;
	for(int i = 0; funcs[i].name; i++)
		RegisterFunction(funcs[i].name,funcs[i].f,funcs[i].nparamscheck,funcs[i].typemask);

	sq_newslot(m_vm,-3,SQFalse);
	sq_settop(m_vm,top);
	return true;
}

bool SqScript::GetObject(SQChar * name,HSQOBJECT *obj)
{
	int top = sq_gettop(m_vm);
	sq_pushstring(m_vm,name,-1);
	if(SQ_FAILED(sq_get(m_vm,1)))
	{
		sq_settop(m_vm,top);
		return false;
	}
	sq_resetobject(obj);
	sq_getstackobj(m_vm,-1,obj);
	sq_settop(m_vm,top);
	return true;
}

bool SqScript::RegisterFunction(const SQChar *name,SQFUNCTION func,int nParams,const char * sPparamsTemplate)
{
    sq_pushstring(m_vm,name,-1);
    sq_newclosure(m_vm,func,0);

	if(sPparamsTemplate && sPparamsTemplate[0] != '\0')
		sq_setparamscheck(m_vm,nParams,sPparamsTemplate);

    return SQ_SUCCEEDED(sq_newslot(m_vm,-3,SQFalse));
}

bool SqScript::RegisterConst(const SQChar *name,HSQOBJECT arg)
{
    sq_pushstring(m_vm,name,-1);
    sq_pushobject(m_vm,arg);
	return SQ_SUCCEEDED(sq_newslot(m_vm,-3,SQTrue));
}

SqScript::~SqScript()
{
	if(m_bReleasHandle)
		sq_release(m_vm, &m_handle);
}
