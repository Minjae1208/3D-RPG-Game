#pragma once

#pragma comment(lib, "Winmm.lib")

#include "windows.h"
#include <mmsystem.h>
#include <map>
#include <queue>
#include "../Struct/stTimer.h"


using namespace std;

typedef void(*CALLBACK_FUNC)(int);

static CALLBACK_FUNC CallBackSuccess;
static CALLBACK_FUNC CallBackRespawn;
static CALLBACK_FUNC CallBackInteractionMove;
static CALLBACK_FUNC CallBackFatigue;

class TimeManager
{
public:
	TimeManager();
	~TimeManager();

public:
	std::map<int, struct TGather*> MGather;
	std::map<int, struct TLevel*> MLevel;
	std::queue<struct TRespawn*> MRespawn;
	map<int, TFatigue*> MFatigue;

	bool InitCallBackSuccess(CALLBACK_FUNC Function);
	bool InitCallBackRespawn(CALLBACK_FUNC Function);
	bool InitCallBackInteractionMove(CALLBACK_FUNC Function);

	void GatherTimer(int nSessionID, int nInteraction_ID);
	void RespawnTimer(int nLevel_ID, int nInteraction_ID);
	void InterationMoveTImer(int nLevel_ID, int nInteraction_ID, int nTimerID);
	void FatigueTimer(int nSessionID);

	void StopGatherTimer(int nSessionID);
	void StopInteractionMoveTimer(int nInteraction_ID);
	void StopFatigueTimer(int nSeesionID);

	int SetGratherTimer(int ndelay, int nresolution, int nSessionID);
	int SetRespawnTimer(int ndelay, int nresolution);
	int SetInteractionMoveTimer(int ndelay, int nresolution, int nInteraction_ID);
	int SetFatigueTimer(int ndelay, int nresolution, int nSessionID);

	void KillTimer(int n_ID);


private:
	int GatherTime = 3000;
	int InteractionRespawnTime = 30000;
	int InteractionMoveTime = 5000;
	int FatigueTime = 10000;

public:
	int Fatigue_Set = 5000000;
};
