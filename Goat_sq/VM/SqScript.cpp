#include "SqScript.h"
#include <stdarg.h>
#include <cstddef>
#include <stdio.h>

SqScript::SqScript(HSQUIRRELVM vm, HSQOBJECT handle, bool Notify)
{
    m_vm = vm;
    m_handle = handle;
	m_bReleasHandle = true;
	if(Notify)
		g_ScriptManager.RegisterScript(vm,m_handle);
}

SqScript::SqScript(HSQUIRRELVM vm, bool Notify)
{
    m_vm = vm;
	m_bReleasHandle = false;
	if(Notify)
		g_ScriptManager.RegisterScript(vm);
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

	bool success = false;
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

	SqRefFunctionEx * funcs = classname->funcs;
	for(int i = 0; funcs[i].name; i++)
		RegisterFunction(funcs[i].name,funcs[i].f,funcs[i].nparamscheck,funcs[i].typemask,funcs[i].bstatic);

	success = SQ_SUCCEEDED(sq_newslot(m_vm,-3,SQFalse));
	sq_settop(m_vm,top);
	return success;
}

bool SqScript::GetObject(SQChar * name, HSQOBJECT *obj, int tableidx, int idx)
{
	sq_pushstring(m_vm,name,-1);
	if(SQ_FAILED(sq_get(m_vm,tableidx)))
		return false;

	sq_resetobject(obj);
	sq_getstackobj(m_vm,idx,obj);
	sq_pop(m_vm,1);
	return true;
}

bool SqScript::RegisterFunction(const SQChar *name,SQFUNCTION func,int nParams,const char * sPparamsTemplate,SQBool bstatic)
{
    sq_pushstring(m_vm,name,-1);
    sq_newclosure(m_vm,func,0);

	if(sPparamsTemplate && sPparamsTemplate[0] != '\0')
		sq_setparamscheck(m_vm,nParams,sPparamsTemplate);

    return SQ_SUCCEEDED(sq_newslot(m_vm,-3,bstatic));
}

bool SqScript::RegisterConst(const SQChar *name,HSQOBJECT arg)
{
    sq_pushstring(m_vm,name,-1);
    sq_pushobject(m_vm,arg);
	return SQ_SUCCEEDED(sq_newslot(m_vm,-3,SQTrue));
}

template <typename T>
T SqScript::GetProperty(const SQChar * key)
{
	sq_pushregistrytable(m_vm);
	sq_pushstring(vm,key,-1);
	T userdata = NULL;
	if(SQ_SUCCEEDED(sq_get(m_vm,-2)))
	{
		sq_getuserdata(vm,-1,&userdata,NULL);
		sq_pop(m_vm,2);
		return userdata;
	}
	sq_pop(vm,1);
	return NULL;
}

template <typename T>
bool SqScript::SetProperty(const SQChar * key, T prop)
{
	T userdata = NULL;
	sq_pushregistrytable(m_vm);
	sq_pushuserpointer(m_vm,(SQUserPointer)prop);
	sq_pushstring(m_vm,key,-1);
	if(SQ_SUCCEEDED(sq_newslot(m_vm,-3)))
	{
		sq_pop(m_vm,1);
		return true;
	}
	sq_pop(m_vm,1);
	return false;
}

void SqScript::Uninitialize()
{
	if(m_bReleasHandle)
	{
		g_ScriptManager.NotifyScriptUnloaded(m_vm,m_handle);
		sq_release(m_vm, &m_handle);
	}
	else
		g_ScriptManager.NotifyScriptUnloaded(m_vm);
}

SqScript::~SqScript()
{

}
