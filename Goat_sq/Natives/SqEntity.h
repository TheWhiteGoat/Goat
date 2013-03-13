/*
*
*	This file is licensed under the GNU GPLv3
*	All the licenses are located at the root folder
*
*/

#pragma once

#include "SqEnvironment.h"
#include "SqGroups.h"
#include <iservernetworkable.h>
#include <IServerUnknown.h>

class SqEntity
{
public:
	static bool RegisterNatives(SqGroups * pGroups);
	static edict_t * BaseEntityToEdict(CBaseEntity *pEntity);
	//SqEntity(void);
	//~SqEntity(void);
};

