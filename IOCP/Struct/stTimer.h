#pragma once

struct TGather {
	int TimerID;
	int SessionID;
	int Interaction_ID;
};

struct TLevel {
	int TimerID;
	int Level_ID;
	int Interaction_ID;
};

struct TRespawn {
	int m_nLevel_ID;
	int m_nInteraction_ID;
};

struct TFatigue {
	int TimerID;
	int SessionID;
	int Fatigue;
};