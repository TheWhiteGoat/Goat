/*
*
*	This file is licensed under the GNU GPLv3
*	All the licenses are located at the root folder
*
*/

#include "SQLoader.h"

SQLoader::SQLoader(void)
{
	//m_pEnviroment = NULL;
	m_pGroups = NULL;
}


/*void Test(const char * path)
{
	const char * name = NULL;
	IDirectory * dir = NULL;
	if((dir = libsys->OpenDirectory(path)) == NULL)
		return;

	g_pSM->LogMessage(myself,"Test: path %s",path);
	while(dir->MoreFiles())
	{
		if(dir->IsEntryValid() && (name = dir->GetEntryName()) != NULL)
		{
			if(dir->IsEntryDirectory())
			{
				g_pSM->LogMessage(myself,"Test: dir %s",name);
			}
			else if(dir->IsEntryFile())
			{
				g_pSM->LogMessage(myself,"Test: file %s",name);
			}
		}
		else
			g_pSM->LogMessage(myself,"Test: invalid %s",name);
		dir->NextEntry();
	}
}*/

bool SQLoader::LoadScripts()
{
	char path[PLATFORM_MAX_PATH];
	if(!g_pSM->BuildPath(PathType::Path_SM,path,sizeof(path),"plugins/squirrel"))
		return false;
	g_pSM->LogMessage(myself,path);

	libsys->CreateFolder(path);

	//g_pSM->LogMessage(myself,path);
	m_pGroups = new SqGroups();
	m_pRootEnviroment = new SqEnvironment();
	if(!m_pRootEnviroment->Initialize())
	{
		g_pSM->LogError(myself,"Error initializing the root scripting enviroment");
		return false;
	}

	m_pGroups->AddGroup("root",m_pRootEnviroment);
	//Test(path);
	//return true;
	return RecursiveFolderLoading(path,true,"root");
}

bool SQLoader::LoadConsts()
{

	return true;
}

bool SQLoader::LoadNatives()
{

	return true;
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
	SqEnvironment * env = NULL;
	while(dir->MoreFiles())
	{
		if(dir->IsEntryValid() && (name = dir->GetEntryName()) != NULL)
		{
			if(dir->IsEntryFile())
			{
				if(isNut(std::string(name)))
				{
					completepath.append(path);
					completepath.append("\\");
					completepath.append(name);
				
					if(env == NULL && !rootfolder)
					{
						env = m_pRootEnviroment->CreateFriendVM();
						env->Initialize();
					}

					if(rootfolder)
						m_pRootEnviroment->CreateScript(completepath.data());
					else
						env->LoadLibrary(completepath.data());
					
					completepath.clear();
				}
			}
			else if(dir->IsEntryDirectory() && strcmp(name,"disabled") != 0 && strcmp(name,".") != 0 && strcmp(name,"..") != 0)
			{
				completepath.append(path);
				completepath.append("\\");
				completepath.append(name);
				RecursiveFolderLoading(completepath.data(),false,(char*)name);
				completepath.clear();
			}
		}
		dir->NextEntry();
	}
	if(env && !rootfolder)
		m_pGroups->AddGroup(group,env);
	
	return true;
}

SQLoader::~SQLoader(void)
{
	//m_pEnviroment->Uninitialize();
	delete m_pGroups;
}
