/*
*
*	This file is licensed under the GNU GPLv3
*	All the licenses are located at the root folder
*
*/

#include "smsdk_ext.h"
#include "SqLoader.h"

#include "SqEntity.h"
#include "SqPlayer.h"
#include "SqEvent.h"

using namespace std;
using namespace Sqrat;

SQLoader::SQLoader(void)
{
	//m_pEnviroment = NULL;
	//m_pGroups = NULL;
	m_pVm = NULL;
}

bool SQLoader::Init()
{
	m_pVm = new SqratVM(1024);
	DefaultVM::Set(m_pVm->getVM());
	SetHandlers(m_pVm);
	return true;
}

bool SQLoader::LoadScripts()
{
	char path[PLATFORM_MAX_PATH];
	if(!g_pSM->BuildPath(PathType::Path_SM,path,sizeof(path),"plugins/squirrel"))
		return false;
	g_pSM->LogMessage(myself,path);

	libsys->CreateFolder(path);
	return RecursiveFolderLoading(path,true,"root");
}


bool SQLoader::LoadAddons()
{
	m_ListRegisteringFuncs.push_back(&SqEntity::RegisterInVm);
	m_ListRegisteringFuncs.push_back(&SqPlayer::RegisterInVm);
	m_ListRegisteringFuncs.push_back(&SqEvent::RegisterInVm);
	/*if(!SqEvent::RegisterNatives(m_pGroups))
	{
		g_pSM->LogError(myself,"Failed adding event natives");
		return false;
	}
	if(!SqEntity::RegisterNatives(m_pGroups))
	{
		g_pSM->LogError(myself,"Failed adding event natives");
		return false;
	}
	if(!SqPlayer::RegisterNatives(m_pGroups))
	{
		g_pSM->LogError(myself,"Failed adding event natives");
		return false;
	}*/
	return true;
}


void SQLoader::PrintFunc(HSQUIRRELVM vm, const SQChar *s, ...)
{
	//g_pSM->LogMessage(myself,"PRINT CALLED");
    char formatted_string[1024];
	va_list argptr;
	va_start(argptr,s);
	vsnprintf(formatted_string,sizeof(formatted_string),s,argptr);
	va_end(argptr);
	rootconsole->ConsolePrint(formatted_string);
}

SQInteger SQLoader::RuntimeErrorFunc(HSQUIRRELVM vm)
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

void SQLoader::CompileErrorFunc(HSQUIRRELVM v,const SQChar * desc,const SQChar *
                                     source,SQInteger line,SQInteger column)
{
	g_pSM->LogError(myself,"Compiler error has occourd at:%s %d\n%s",source,line,desc);
}

void SQLoader::ErrorFunc(HSQUIRRELVM vm, const SQChar *s, ...)
{
    char formatted_string[1024];
	va_list argptr;
	va_start(argptr,s);
	vsnprintf(formatted_string,sizeof(formatted_string),s,argptr);
	va_end(argptr);
	g_pSM->LogError(myself,formatted_string);
}

void SQLoader::SetHandlers(SqratVM *vm)
{
	vm->setErrorHandler(RuntimeErrorFunc,CompileErrorFunc);
	vm->setPrintFunc(PrintFunc,ErrorFunc);
}

std::string GetFileExtension(const std::string& FileName)
{
    if(FileName.find_last_of(".") != std::string::npos)
        return FileName.substr(FileName.find_last_of(".")+1);
    return "";
}

bool isNut(const std::string& name)
{
	if(GetFileExtension(name).compare("nut") == 0)
		return true;
	return false;
}

bool SQLoader::RecursiveFolderLoading(const char * path,bool rootfolder,char * group)
{
	IDirectory * dir = NULL;
	if((dir = libsys->OpenDirectory(path)) == NULL)
		return false;

	const char * name = NULL;
	std::string completepath;
	//SqEnvironment * env = NULL;
	SqratVM *vm = NULL;
	while(dir->MoreFiles())
	{
		if(dir->IsEntryValid() && (name = dir->GetEntryName()) != NULL)
		{
			if(dir->IsEntryFile())
			{
				if(isNut(std::string(name)))
				{
					completepath=path+std::string("\\")+name;
					if(vm == NULL && !rootfolder)
					{
						vm = new SqratVM(1024);
						SetHandlers(vm);
					    Script scr = vm->getScript();
						scr.CompileFile(completepath);
						RegisteringList::iterator it;
						for(it=m_ListRegisteringFuncs.begin(); it!=m_ListRegisteringFuncs.end(); it++)
							(*it)(&scr);
						scr.Run();
					}

					else if(rootfolder)
					{
						
						//m_pRootEnviroment->CreateScript(completepath.data());
					}
				}
			}
			else if(dir->IsEntryDirectory() && strcmp(name,"disabled") != 0 && strcmp(name,".") != 0 && strcmp(name,"..") != 0)
			{
				completepath = path+std::string("\\")+name;
				RecursiveFolderLoading(completepath.data(),false,(char*)name);
			}
		}
		dir->NextEntry();
	}
	//if(env && !rootfolder)
		//m_pGroups->AddGroup(group,env);
	
	return true;
}

SQLoader::~SQLoader(void)
{
	//m_pEnviroment->Uninitialize();
	//delete m_pGroups;
}
