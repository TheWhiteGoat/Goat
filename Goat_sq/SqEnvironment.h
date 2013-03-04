#ifndef SQENVIRONMENT_H
#define SQENVIRONMENT_H

#include <squirrel.h>
#include <list>
#include "SqScript.h"

#define SMLOG // to log some script actions

#define SQ_CLASS_BUILD_START(classname)\
	SQInteger _SQ_CLASSMEMBER_##classname##_typeof(SQVM * pVM) \
	{ \
		sq_pushstring(pVM, #classname, -1); \
		return 1; \
	} \
	SQRegFunction _SQ_CLASS_##classname## [] =\
	{\
		{ "_typeof", _SQ_CLASSMEMBER_##classname##_typeof,0,NULL },\

#define SQ_CLASS_FUNC(classname,funcname,nparams,typemask)\
	{ #funcname,_SQ_CLASSMEMBER_##classname##_##funcname ,nparams,typemask},\

#define SQ_CLASS_BUILD_END_BASE(classname,baseclassname)\
	{NULL,NULL,NULL,NULL}\
	};\
	SQClassName _SQ_CLASSNAME_##classname = \
	{ #classname, #baseclassname,_SQ_CLASS_##classname };\

#define SQ_CLASS_BUILD_END(classname)\
	{NULL,NULL,NULL,NULL}\
	};\
	SQClassName _SQ_CLASSNAME_##classname = \
	{ #classname, NULL,_SQ_CLASS_##classname };\

#define SQ_CLASS_FUNC_DEF(classname,funcname)\
	SQInteger _SQ_CLASSMEMBER_##classname##_##funcname(HSQUIRRELVM vm)\

#define SQ_CLASS_GET(classname)\
	_SQ_CLASSNAME_##classname\

#define SQ_CLASS_RELEASE_HOOK(classname) \
	sq_setreleasehook(vm, 1, _SQ_CLASSMEMBER_##classname##_releasehook);\


template <typename T>
static SQRESULT sq_setinstance(SQVM * pVM, T pInstance, int iIndex = 1)
{
	return sq_setinstanceup(pVM, iIndex, (SQUserPointer *)pInstance);
}

// Now a function
template <typename T>
static T sq_getinstance(SQVM * pVM, int iIndex = 1)
{
	SQUserPointer pInstance = NULL;

	if(SQ_FAILED(sq_getinstanceup(pVM, iIndex, &pInstance, NULL)))
		pInstance = NULL;

	return (T)pInstance;
}

using namespace std;

class SqEnvironment
{
    protected:
        static void PrintFunc(HSQUIRRELVM vm, const SQChar *str, ...);
		static SQInteger RuntimeErrorFunc(HSQUIRRELVM vm);
        static void ErrorFunc(HSQUIRRELVM vm, const SQChar *str, ...);
        static SQInteger file_lexfeedASCII(SQUserPointer file);
        static void CompileErrorFunc(HSQUIRRELVM v,const SQChar * desc,const SQChar *source,SQInteger line,SQInteger column);

		HSQOBJECT m_handle;
		bool m_bHandleSet;
		HSQUIRRELVM m_vm;

		//list<SqScript*> m_scripts;
		//SqScript* m_pScript;
		//list <HSQOBJECT*> m_ClosuresRunList; //used to run all main functions at the same time

        bool LoadFile(HSQUIRRELVM vm, const char *filename);
    public:
        SqEnvironment(void);
		SqEnvironment(HSQUIRRELVM vm, HSQOBJECT handle);
        ~SqEnvironment(void);
        bool Initialize(int stack = 1024);
        void Uninitialize(void);

        HSQUIRRELVM GetVM(){return m_vm;}
		SqScript * toScript(){return new SqScript(m_vm);}
		SqEnvironment * CreateFriendVM(int stack = 1024);

		SqScript* CreateScript(const char *name);
		void DestroyScript(SqScript *pScript);
        bool LoadLibrary(const char *filename);

		//bool ExecuteCachedClosures();
		//bool RegisterClass(SQClassName * classname);
        //bool RegisterFunction(const SQChar *name,SQFUNCTION func,int nParams = -1,const char * sPparamsTemplate = NULL);
       // bool RegisterConst(const SQChar *name,HSQOBJECT arg);
		
};

#endif
