/*
*
*	This file is licensed under the GNU GPLv3
*	All the licenses are located at the root folder
*
*/

#include "SqEvent.h"

SQInteger Native_HookEvent(HSQUIRRELVM vm)
{

	return 0;
}

SqEvent::SqEvent()
{
}


bool SqEvent::RegisterNatives(SqScript * pScript)
{
	pScript->RegisterFunction("HookEvent",Native_HookEvent,3,".sx|c");
}
