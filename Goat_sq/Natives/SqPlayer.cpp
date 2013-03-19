#include "SqPlayer.h"


#define GETPLAYER \
IGamePlayer *player = playerhelpers->GetGamePlayer(m_iIndex);\
	if(player)\

#define GETINFO \
	IPlayerInfo *info = player->GetPlayerInfo();\
	if(info)\

SqPlayer::SqPlayer(int index) : SqEntity(index)
{
}


SqPlayer::~SqPlayer(void)
{
}

Sqrat::string SqPlayer::GetName()
{
	GETPLAYER
		return player->GetName();
	return "";
}

Sqrat::string  SqPlayer::GetIPAddress()
{
	GETPLAYER
		return player->GetIPAddress();
	return "";
}

Sqrat::string SqPlayer::GetAuthString()
{
	GETPLAYER
		return player->GetAuthString();
	return "";
}

bool SqPlayer::IsInGame()
{
	GETPLAYER
		return player->IsInGame();
	return false;
}

bool SqPlayer::IsConnected()
{
	GETPLAYER
		return player->IsConnected();
	return false;
}

bool SqPlayer::IsFakeClient()
{
	GETPLAYER
		return player->IsFakeClient();
	return false;
}

int SqPlayer::GetAdminId() //int for right now
{
	GETPLAYER
		return player->GetAdminId();
	return -1;
}

void SqPlayer::SetAdminId(int id, bool temp)
{
	GETPLAYER
		player->SetAdminId(id,temp);
}

int SqPlayer::GetUserId()
{
	GETPLAYER
		return player->GetUserId();
	return -1;
}

int SqPlayer::GetLanguageId()
{
	GETPLAYER
		return player->GetLanguageId();
	return -1;
}

bool SqPlayer::RunAdminCacheChecks()
{
	GETPLAYER
		return player->RunAdminCacheChecks();
	return false;
}

void SqPlayer::NotifyPostAdminChecks()
{
	GETPLAYER
		player->NotifyPostAdminChecks();
}

unsigned int SqPlayer::GetSerial()
{
	GETPLAYER
		return player->GetSerial();
	return 0;
}

bool SqPlayer::IsAuthorized()
{
	GETPLAYER
		return player->IsAuthorized();
	return false;
}

void SqPlayer::Kick (Sqrat::string message)
{
	GETPLAYER
		player->Kick(message.data());
}

bool SqPlayer::IsInKickQueue()
{
	GETPLAYER
		return player->IsInKickQueue();
	return false;
}

void SqPlayer::MarkAsBeingKicked()
{
	GETPLAYER
		player->MarkAsBeingKicked();
}

void SqPlayer::SetLanguageId(unsigned int id)
{
	GETPLAYER
		player->SetLanguageId(id);
}

bool SqPlayer::IsSourceTV()
{
	GETPLAYER
		return player->IsSourceTV();
	return false;
}

bool SqPlayer::IsReplay()
{
	GETPLAYER
		return player->IsReplay();
	return false;
}

void SqPlayer::AddDelayedKick(Sqrat::string msg)
{
	int userid = -1;
	GETPLAYER
		userid = player->GetUserId();
	else
		return;

	if(userid)
		gamehelpers->AddDelayedKick(m_iIndex,userid,msg.data());
}

bool SqPlayer::HintTextMsg(Sqrat::string msg)
{
	return gamehelpers->HintTextMsg(m_iIndex, msg.data());
}

bool SqPlayer::TextMsg(int dest, Sqrat::string msg)
{
	return gamehelpers->TextMsg(m_iIndex, dest, msg.data());
}

int SqPlayer::GetTeamIndex()
{
	GETPLAYER
	{
		GETINFO
			return info->GetTeamIndex();
	}
	return -1;
}

void SqPlayer::ChangeTeam(int TeamNum)
{
	GETPLAYER
	{
		GETINFO
			info->ChangeTeam(TeamNum);
	}
}




int SqPlayer::GetClientOfUserId(int userid)
{
	return playerhelpers->GetClientOfUserId(userid);
}

int SqPlayer::GetClientFromSerial(unsigned int serial)
{
	return playerhelpers->GetClientFromSerial(serial);
}

int SqPlayer::GetMaxClients()
{
	return playerhelpers->GetMaxClients();
}

int SqPlayer::GetNumPlayers()
{
	return playerhelpers->GetNumPlayers();
}

void SqPlayer::RegisterInVm(Script *vm)
{
	DefaultVM::Set(vm->GetVM());
	RootTable().Bind(_SC("Player"),
		DerivedClass<SqPlayer,SqEntity,SqEntityAlloc<SqPlayer>>()
		.Func(_SC("GetName"),&SqPlayer::GetName)
		.Func(_SC("GetIPAddress"),&SqPlayer::GetIPAddress)
		.Func(_SC("GetAuthString"),&SqPlayer::GetAuthString)
		.Func(_SC("IsInGame"),&SqPlayer::IsInGame)
		.Func(_SC("IsConnected"),&SqPlayer::IsConnected)
		.Func(_SC("IsFakeClient"),&SqPlayer::IsFakeClient)
		.Func(_SC("GetAdminId"),&SqPlayer::GetAdminId)
		.Func(_SC("SetAdminId"),&SqPlayer::SetAdminId)
		.Func(_SC("GetUserId"),&SqPlayer::GetUserId)
		.Func(_SC("GetLanguageId"),&SqPlayer::GetLanguageId)
		.Func(_SC("RunAdminCacheChecks"),&SqPlayer::RunAdminCacheChecks)
		.Func(_SC("NotifyPostAdminChecks"),&SqPlayer::NotifyPostAdminChecks)
		.Func(_SC("GetSerial"),&SqPlayer::GetSerial)
		.Func(_SC("IsAuthorized"),&SqPlayer::IsAuthorized)
		.Func(_SC("Kick"),&SqPlayer::Kick)
		.Func(_SC("IsInKickQueue"),&SqPlayer::IsInKickQueue)
		.Func(_SC("MarkAsBeingKicked"),&SqPlayer::MarkAsBeingKicked)
		.Func(_SC("SetLanguageId"),&SqPlayer::SetLanguageId)
		.Func(_SC("IsSourceTV "),&SqPlayer::IsSourceTV)
		.Func(_SC("IsReplay"),&SqPlayer::IsReplay)
		.Func(_SC("AddDelayedKick"),&SqPlayer::AddDelayedKick)
		.Func(_SC("HintTextMsg"),&SqPlayer::HintTextMsg)
		.Func(_SC("TextMsg"),&SqPlayer::TextMsg)
		
		//Static functions
		.StaticFunc(_SC("IndexOfUserId"),&SqPlayer::GetClientOfUserId)
		.StaticFunc(_SC("IndexOfSerial"),&SqPlayer::GetClientFromSerial)
		.StaticFunc(_SC("GetMaxClients"),&SqPlayer::GetMaxClients)
		.StaticFunc(_SC("GetNumPlayers"),&SqPlayer::GetNumPlayers)
		);
}