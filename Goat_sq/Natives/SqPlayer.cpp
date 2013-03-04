#include "SqPlayer.h"



//#define GETINSTANCE int ref = sq_getinstance<int>(vm);


SQ_CLASS_FUNC_DEF(Player,constructor)
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

SQ_CLASS_FUNC_DEF(Player,HintTextMsg)
{
	int ref = sq_getinstance<int>(vm);
	int index = gamehelpers->ReferenceToIndex(ref);
	bool success = false;
	SQChar * msg = NULL;
	if((success = SQ_SUCCEEDED(sq_getstring(vm,-1,(const SQChar **)&msg))))
		success = gamehelpers->HintTextMsg(index,msg);
	else
		sq_pop(vm,1);

	sq_pushbool(vm,success);
	return 1;
}

SQ_CLASS_FUNC_DEF(Player,TextMsg)
{

	int ref = sq_getinstance<int>(vm);
	int index = gamehelpers->ReferenceToIndex(ref);
	int txtdest = 1;

	bool success = false;
	SQChar * msg = NULL;
	if((success = SQ_SUCCEEDED(sq_getinteger(vm,-2,&txtdest))))
		if((success = SQ_SUCCEEDED(sq_getstring(vm,-1,(const SQChar **)&msg))))
			success = gamehelpers->TextMsg(index,txtdest,msg);
		else
			sq_pop(vm,1);
	else
		sq_pop(vm,2);

	sq_pushbool(vm,success);
	return 1;
}

SQ_CLASS_BUILD_START(Player)
SQ_CLASS_FUNC(Player,constructor,2,".i")
SQ_CLASS_FUNC(Player,HintTextMsg,2,".s")
SQ_CLASS_FUNC(Player,TextMsg,2,".is")
/*SQ_CLASS_FUNC(Player,GetName,1,".")
SQ_CLASS_FUNC(Player,GetIPAddress,1,".")
SQ_CLASS_FUNC(Player,GetAuthString,1,".")
SQ_CLASS_FUNC(Player,IsInGame,1,".")
SQ_CLASS_FUNC(Player,IsConnected,1,".")
SQ_CLASS_FUNC(Player,IsFakeClient,1,".")
SQ_CLASS_FUNC(Player,GetAdminId,1,".")
SQ_CLASS_FUNC(Player,SetAdminId,1,".")
SQ_CLASS_FUNC(Player,GetUserId,1,".")
SQ_CLASS_FUNC(Player,GetLanguageId,1,".")
SQ_CLASS_FUNC(Player,GetSerial,1,".")
SQ_CLASS_FUNC(Player,IsAuthorized,1,".")
SQ_CLASS_FUNC(Player,Kick,1,".")
SQ_CLASS_FUNC(Player,IsInKickQueue,1,".")
SQ_CLASS_FUNC(Player,IsSourceTV,1,".")
SQ_CLASS_FUNC(Player,IsReplay,1,".")*/
SQ_CLASS_BUILD_END_BASE(Player,Entity)

bool SqPlayer::RegisterNatives(SqScript * pScript)
{
	return pScript->RegisterClass(&SQ_CLASS_GET(Player));
}