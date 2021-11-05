#include "TimerManager.h"

void CALLBACK GatherTimeProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);

void CALLBACK RespawnTimeProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);

void CALLBACK InteractionMoveTimeProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);

void CALLBACK FatigueTimeProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);

TimeManager::TimeManager()
{

}

TimeManager::~TimeManager()
{

}

bool TimeManager::InitCallBackSuccess(CALLBACK_FUNC Function)
{
	CallBackSuccess = Function;
	if (CallBackSuccess == nullptr)
		return false;

	return true;
}

bool TimeManager::InitCallBackRespawn(CALLBACK_FUNC Function)
{
	CallBackRespawn = Function;
	if (CallBackRespawn == nullptr)
		return false;

	return true;
}

bool TimeManager::InitCallBackInteractionMove(CALLBACK_FUNC Function)
{
	CallBackInteractionMove = Function;
	if (CallBackInteractionMove == nullptr)
		return false;

	return true;
}


void TimeManager::GatherTimer(int nSessionID, int nInteraction_ID)
{
	TGather *m_pGather = new TGather;
	int m_nTimerID = SetGratherTimer(GatherTime, 1, nSessionID);
	m_pGather->TimerID = m_nTimerID;
	m_pGather->SessionID = nSessionID;
	m_pGather->Interaction_ID = nInteraction_ID;

	MGather[nSessionID] = m_pGather;
}

void TimeManager::RespawnTimer(int nLevel_ID, int nInteraction_ID)
{
	TRespawn *m_SRespawn = new TRespawn;
	m_SRespawn->m_nLevel_ID = nLevel_ID;
	m_SRespawn->m_nInteraction_ID = nInteraction_ID;

	MRespawn.push(m_SRespawn);

	SetRespawnTimer(InteractionRespawnTime, 1);
}

void TimeManager::InterationMoveTImer(int nLevel_ID, int nInteraction_ID, int nTimerID)
{
	TLevel* m_pLevel = new TLevel;
	int m_nTimerID = SetInteractionMoveTimer(InteractionMoveTime, 1, nInteraction_ID);
	m_pLevel->TimerID = m_nTimerID;
	m_pLevel->Level_ID = nLevel_ID;
	m_pLevel->Interaction_ID = nInteraction_ID;

	MLevel[nInteraction_ID] = m_pLevel;
}

void TimeManager::FatigueTimer(int nSessionID)
{
	if (MFatigue[nSessionID + Fatigue_Set] != nullptr)
		return;

	printf_s("피로도 타이머 동작\n");

	TFatigue* m_pFatigue = new TFatigue;
	int m_nTimerID = SetFatigueTimer(FatigueTime, 1, nSessionID + Fatigue_Set);
	m_pFatigue->TimerID = m_nTimerID;
	m_pFatigue->SessionID = nSessionID + Fatigue_Set;

	MFatigue[m_pFatigue->SessionID] = m_pFatigue;
}

void TimeManager::StopGatherTimer(int nSessionID)
{
	if (MGather[nSessionID] == nullptr)
		return;

	int m_nID = MGather[nSessionID]->TimerID;

	KillTimer(m_nID);
	MGather.erase(nSessionID);
}

void TimeManager::StopInteractionMoveTimer(int nInteraction_ID)
{
	if (MLevel[nInteraction_ID] == nullptr)
		return;

	int m_nID = MLevel[nInteraction_ID]->TimerID;

	KillTimer(m_nID);
	MLevel.erase(nInteraction_ID);
}

void TimeManager::StopFatigueTimer(int nSessionID)
{
	if (MFatigue[nSessionID] == nullptr)
		return;

	int m_nID = MFatigue[nSessionID]->TimerID;

	KillTimer(m_nID);
	MFatigue.erase(nSessionID);
}

int TimeManager::SetGratherTimer(int ndelay, int nresolution, int nSessionID)
{
	return timeSetEvent(ndelay, nresolution, GatherTimeProc, nSessionID, TIME_ONESHOT);
}

int TimeManager::SetRespawnTimer(int ndelay, int nresolution)
{
	return timeSetEvent(ndelay, nresolution, RespawnTimeProc, 0, TIME_ONESHOT);
}

int TimeManager::SetInteractionMoveTimer(int ndelay, int nresolution, int nInteraction_ID)
{
	return timeSetEvent(ndelay, nresolution, InteractionMoveTimeProc, nInteraction_ID, TIME_PERIODIC);
}

int TimeManager::SetFatigueTimer(int ndelay, int nresolution, int nSessionID)
{
	return timeSetEvent(ndelay, nresolution, FatigueTimeProc, nSessionID, TIME_PERIODIC);
}

void TimeManager::KillTimer(int n_ID)
{
	timeKillEvent(n_ID);
}

void CALLBACK GatherTimeProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	//dwUser : SessionID
	CallBackSuccess(dwUser);
}

void CALLBACK RespawnTimeProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	//dwUser : Interaction_ID
	CallBackRespawn(dwUser);
}

void CALLBACK InteractionMoveTimeProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	CallBackInteractionMove(dwUser);
}

void CALLBACK FatigueTimeProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	//dwUser : Interaction_ID
	CallBackFatigue(dwUser);
}