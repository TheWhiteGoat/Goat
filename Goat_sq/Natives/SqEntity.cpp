/*
*
*	This file is licensed under the GNU GPLv3
*	All the licenses are located at the root folder
*
*/

#include "SqEntity.h"


edict_t * SqEntity::BaseEntityToEdict(CBaseEntity *pEntity)
{
	IServerUnknown *pUnk = (IServerUnknown *)pEntity;
	IServerNetworkable *pNet = pUnk->GetNetworkable();

	if (!pNet)
	{
		return NULL;
	}
	return pNet->GetEdict();
}

SQ_CLASS_FUNC_DEF(Entity,constructor)
{
	int index = 0;
	int ref = 0;
	bool success = false;
	if((success = SQ_SUCCEEDED(sq_getinteger(vm,-1,&index))))
	{
		ref = gamehelpers->IndexToReference(index);
		success = SQ_SUCCEEDED(sq_setinstance(vm,ref));
	}
	else
		sq_pop(vm,1);

	sq_pushbool(vm,success);
	return 1;
}

SQ_CLASS_FUNC_DEF(Entity,GetSendProp)
{

	return 0;
}

SQ_CLASS_FUNC_DEF(Entity,SetSendProp)
{

	return 0;
}

SQ_CLASS_FUNC_DEF(Entity,index)
{
	int ref = sq_getinstance<int>(vm);
	int index = gamehelpers->ReferenceToIndex(ref);
	sq_pushinteger(vm,index);
	return 1;
}

SQ_CLASS_FUNC_DEF(Entity,toPlayer)
{

	return 0;
}

SQ_CLASS_BUILD_START(Entity)
SQ_CLASS_FUNC(Entity,constructor,2,".i")
SQ_CLASS_FUNC(Entity,index,1,".")
//SQ_CLASS_FUNC(Entity,GetSendProp,-2,".i|si")
//SQ_CLASS_FUNC(Entity,SetSendProp,-3,".i|sn|s|xi")
//SQ_CLASS_FUNC(Entity,toPlayer,1,".")
SQ_CLASS_BUILD_END(Entity)

bool SqEntity::RegisterNatives(SqScript * pScript)
{
	return pScript->RegisterClass(&SQ_CLASS_GET(Entity));
}