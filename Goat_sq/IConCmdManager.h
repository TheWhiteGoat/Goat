//#include "ConCmdManager.h"
#pragma once

#include "IForwardSys.h"
#include "IAdminSystem.h"

#define SMINTERFACE_CONCMDMANAGER_NAME		"IConCmdManager"
#define SMINTERFACE_CONCMDMANAGER_VERSION	1

namespace SourceMod
{

	class IConCmdListener
	{
	public:
		virtual ResultType OnCommandDispatch(int client,const char *cmd, int args) { };
		//virtual 
	};

	class IConCmdManager : public SMInterface
	{
	public:
		const char *GetInterfaceName()
		{
			return SMINTERFACE_CONCMDMANAGER_NAME;
		}
		unsigned int GetInterfaceVersion()
		{
			return SMINTERFACE_CONCMDMANAGER_VERSION;
		}
		bool IsVersionCompatible(unsigned int version)
		{
			return true;
		}

	public:
		virtual bool AddCommandListener(IConCmdListener * listener) =0;
		virtual bool RemoveCommandListener(IConCmdListener * listener) =0;
		virtual bool ClientCommand(int client, const char *cmd) =0;
		//virtual bool CheckCommandAccess(int client, const char *cmd, FlagBits cmdflags) =0;
		//virtual bool CheckAccess(AdminId adm, const char *cmd, FlagBits cmdflags) =0;
		virtual bool GetCommandAdminFlags(const char *cmd, FlagBits *pFlags) =0;
		virtual void SetAdminCmdFlags(const char *cmd, OverrideType type, FlagBits bits, bool remove) =0;
	};
};
