#include "SqEnvironment.h"
#include <stdio.h>
#include <stdarg.h>
#include <iostream>

using namespace std;

SqEnvironment::SqEnvironment()
{
	m_bHandleSet = false;
	m_vm = NULL;
}

SqEnvironment::SqEnvironment(HSQUIRRELVM vm, HSQOBJECT handle)
{
	m_vm = vm;
	m_handle = handle;
	m_bHandleSet = true;
}

bool SqEnvironment::Initialize(int stack)
{
    m_vm = sq_open(stack);
    if(!m_vm)
        return false;

    sq_setprintfunc(m_vm,PrintFunc,ErrorFunc);
    sq_setcompilererrorhandler(m_vm,CompileErrorFunc);
    sq_pushroottable(m_vm);
	sq_newclosure(m_vm,RuntimeErrorFunc,0);
	sq_seterrorhandler(m_vm);
    return true;
}


SqEnvironment *SqEnvironment::CreateFriendVM(int stack)
{
	HSQUIRRELVM vm = sq_newthread(m_vm, stack);
	if (vm)
    {
		HSQOBJECT handle;
        sq_resetobject(&handle);
        sq_getstackobj(m_vm, -1, &handle);
        sq_addref(m_vm, &handle);
        sq_pop(m_vm, 1);
		//sq_newtable(vm);
		return new SqEnvironment(vm,handle);
	}
	return NULL;
}

SqScript *SqEnvironment::CreateScript(const char *name)
{
    HSQUIRRELVM vm = sq_newthread(m_vm, 1024);
    //HSQUIRRELVM vm = m_vm;
    if (vm)
    {
        sq_newtable(vm);
        if (LoadFile(vm, name))
        {
            sq_push(vm, -2);
            if (SQ_SUCCEEDED(sq_call(vm, 1, SQFalse, SQTrue)))
            {
                sq_pop(vm, 1);

                HSQOBJECT handle;
                sq_resetobject(&handle);
                sq_getstackobj(m_vm, -1, &handle);
                sq_addref(m_vm, &handle);
                sq_pop(m_vm, 1);

				#ifdef SMLOG
				g_pSM->LogMessage(myself,"The script %s was loaded",name);
				#endif
				//SqScript * tmp = new SqScript(vm, handle);
				//m_scripts.push_front(tmp);
				//return tmp;
				return new SqScript(vm, handle);
            }
        }

        sq_pop(m_vm, 1);
    }

    return 0;
}

bool SqEnvironment::LoadLibrary(const char *filename)
{
    bool success = false;
    int top = sq_gettop(m_vm);
    //sq_pushroottable(m_vm);

    if (LoadFile(m_vm, filename))
    {
		sq_push(m_vm, -2);
		success = SQ_SUCCEEDED(sq_call(m_vm, 1, SQFalse, SQTrue));
		/*if(!cacheclosure)
		{
			sq_push(m_vm, -2);
			success = SQ_SUCCEEDED(sq_call(m_vm, 1, SQFalse, SQTrue));
		}
		else
		{
			HSQOBJECT * obj = new HSQOBJECT;
			sq_resetobject(obj);
			if((success = SQ_SUCCEEDED(sq_getstackobj(m_vm,-1,obj))))
			{
				sq_addref(m_vm,obj);//so the garbage collector won't cleanup the object
				m_ClosuresRunList.push_front(obj);
			}
		}*/
		#ifdef SMLOG
		g_pSM->LogMessage(myself,"LoadLIbrary: %s",filename);
		#endif
    }

    sq_settop(m_vm, top);
    return success;
}

/*bool SqEnvironment::ExecuteCachedClosures()
{
	int top = sq_gettop(m_vm);
	SQChar * type = NULL;
	if(m_ClosuresRunList.size() > 0)
	{
		for(list<HSQOBJECT*>::iterator it = m_ClosuresRunList.begin(); it != m_ClosuresRunList.end(); it++)
		{
			//bool success = false;
			//g_pSM->LogMessage(myself,"Executing main of group");
			sq_pushobject(m_vm,*(*it));
			sq_push(m_vm, 1);
			if(SQ_FAILED(sq_call(m_vm, 1, SQFalse, SQTrue)))
				g_pSM->LogMessage(myself,"Executing main failed!");
			sq_release(m_vm,(*it));
			delete (*it);
			sq_settop(m_vm, top);
		}
	}
	else
	{
		g_pSM->LogMessage(myself,"CLOSURES EMPTY");
		return false;
	}

	m_ClosuresRunList.clear();
	return true;
}*/

SQInteger SqEnvironment::file_lexfeedASCII(SQUserPointer file)
{
    int ret;
    char c;
    if( ( ret=fread(&c,sizeof(c),1,(FILE *)file )>0) )
        return c;
    return 0;
}

bool SqEnvironment::LoadFile(HSQUIRRELVM vm, const char *filename)
{
    FILE *f = fopen(filename,"rb");
    bool success = false;
    if(f)
    {
        success = SQ_SUCCEEDED(sq_compile(vm,file_lexfeedASCII,f,(SQChar*)filename,1));
        fclose(f);
        return success;
    }
    return false;
}

void SqEnvironment::PrintFunc(HSQUIRRELVM vm, const SQChar *s, ...)
{
	//g_pSM->LogMessage(myself,"PRINT CALLED");
    char formatted_string[1024];
	va_list argptr;
	va_start(argptr,s);
	vsnprintf(formatted_string,sizeof(formatted_string),s,argptr);
	va_end(argptr);
	rootconsole->ConsolePrint(formatted_string);
}

SQInteger SqEnvironment::RuntimeErrorFunc(HSQUIRRELVM vm)
{
	SQChar * error = NULL;
	sq_getstring(vm,-1,(const SQChar**)&error);
	if(error)
	{
		SQStackInfos info;
		if(SQ_SUCCEEDED(sq_stackinfos(vm,1,&info)))
		{
			g_pSM->LogError(myself,"Runtime error has occourd at:%s %d:%s\n%s",info.source,info.line,info.funcname,error);
		}
			//cout<<"Runtime error: "<<info.line<<":"<<info.funcname<<" at source: "<<info.source<<endl<<error<<endl;
	}
	return 0;
}

void SqEnvironment::ErrorFunc(HSQUIRRELVM vm, const SQChar *s, ...)
{
    char formatted_string[1024];
	va_list argptr;
	va_start(argptr,s);
	vsnprintf(formatted_string,sizeof(formatted_string),s,argptr);
	va_end(argptr);
	g_pSM->LogError(myself,formatted_string);
}

void SqEnvironment::CompileErrorFunc(HSQUIRRELVM v,const SQChar * desc,const SQChar *
                                     source,SQInteger line,SQInteger column)
{
	g_pSM->LogError(myself,"Compiler error has occourd at:%s %d\n%s",source,line,desc);
}

void SqEnvironment::DestroyScript(SqScript *pScript)
{
	//m_scripts.remove(pScript);
    delete pScript;
}

void SqEnvironment::Uninitialize()
{
    sq_close(m_vm);
}

SqEnvironment::~SqEnvironment()
{
	/*if(m_scripts.size() > 0)
		for(list<SqScript*>::iterator it = m_scripts.begin(); it != m_scripts.end(); it++)
			delete *it;*/

    //Uninitialize();
}
