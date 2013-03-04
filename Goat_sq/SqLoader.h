/*
*
*	This file is licensed under the GNU GPLv3
*	All the licenses are located at the root folder
*
*/

#pragma once
#include "SqEnvironment.h"
#include "SqGroups.h"
#include "smsdk_ext.h"
#include <stdio.h>
#include <string.h>

class SQLoader
{
protected:
	bool RecursiveFolderLoading(const char * path, bool rootfolder, char * group);
private:
	SqEnvironment * m_pRootEnviroment;
	SqGroups * m_pGroups;
public:
	SQLoader(void);
	bool LoadConsts();
	bool LoadNatives();
	bool LoadScripts();
	~SQLoader(void);
};

