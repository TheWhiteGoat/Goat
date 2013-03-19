/*
*
*	This file is licensed under the GNU GPLv3
*	All the licenses are located at the root folder
*
*/

#ifndef _INCLUDE_SQLOADER_H
#define _INCLUDE_SQLOADER_H

#include <sqrat.h>
#include <string>
#include <list>
#include "SqLoader.h"

class SQLoader
{
	typedef void (*RegisterInVmTD)(Sqrat::Script*);
	typedef std::list<RegisterInVmTD> RegisteringList;
protected:
	bool RecursiveFolderLoading(const char * path, bool rootfolder, char * group);
private:
	Sqrat::SqratVM *m_pVm;
	RegisteringList m_ListRegisteringFuncs;
private:
	void SetHandlers(Sqrat::SqratVM *vm);
private:
	static void PrintFunc(HSQUIRRELVM vm, const SQChar *s, ...);
	static SQInteger RuntimeErrorFunc(HSQUIRRELVM vm);
	static void CompileErrorFunc(HSQUIRRELVM v,const SQChar * desc,const SQChar *
                                     source,SQInteger line,SQInteger column);
	static void ErrorFunc(HSQUIRRELVM vm, const SQChar *s, ...);
public:
	SQLoader(void);
	bool Init();
	bool LoadAddons();
	bool LoadScripts();
	~SQLoader(void);
};

#endif