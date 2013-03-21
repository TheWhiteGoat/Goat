#pragma once
#include <sqrat.h>
#include "SqEntity.h"
#include <iplayerinfo.h>

class SqPlayer : public SqEntity
{
public:
	SqPlayer(int index);
	~SqPlayer(void);

	static void RegisterInVm(Script *vm);

	Sqrat::string GetName();
	Sqrat::string GetIPAddress();
	Sqrat::string GetAuthString();

	bool IsInGame();
	bool IsConnected();
	bool IsFakeClient();

	int GetAdminId(); //int for right now
	void SetAdminId(int id, bool temp);
	int GetUserId();
	int GetLanguageId();
	//PlayerInfo struct/table need to be implmented

	bool RunAdminCacheChecks();
	void NotifyPostAdminChecks();
	unsigned int GetSerial();
	bool IsAuthorized();
	void Kick (Sqrat::string message);
	bool IsInKickQueue();
	void MarkAsBeingKicked();
	void SetLanguageId(unsigned int id);
	bool IsSourceTV();
	bool IsReplay();

	void AddDelayedKick (Sqrat::string msg);
	bool HintTextMsg (Sqrat::string msg);
	bool TextMsg (int dest, Sqrat::string msg);

	int GetTeamIndex();
	void ChangeTeam(int TeamNum);
	Array GetAbsAngles();

	static int GetClientOfUserId(int userid);
	static int GetClientFromSerial(unsigned int serial);
	static int GetMaxClients();
	static int GetNumPlayers();
};

