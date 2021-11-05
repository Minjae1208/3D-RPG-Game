#pragma once

#include <sstream>

#include "../Data/DataCharacter.h"

#include "../Packet/LevelPacket.h"
#include "../Packet/PlayerPacket.h"
#include "../Packet/Type.h"

#include "../Struct/stIOCP.h"
#include "../Struct/stUser.h"

class Level
{
public:
	DataLevel* m_pDataLevel;																	// 채집물
	std::map<int, struct User*> mPlayers;														// Level에 있는 유저
	Players CharactersInfo;																		// 플레이어 직렬화
	PacketEnterInfo EnterInfo;																	// 입장 플레이어 직렬화

	int Level_ID;
	bool InteractionMove = false;

	// 공용 함수
	static void Send(struct stOver* pOver);														// 패킷 전송
	static void MakeSendPacket(std::stringstream & SendStream, struct stOver* pOver);			// 패킷 전송 전 처리
	void Zone_Broadcast(std::stringstream & SendStream);										// 전체 전송

	virtual bool Init(DataLevel* pDataLevel);													// Level Class 초기화
	virtual void EnterUser(std::stringstream & SendStream, struct User* player);				// 유저 입장
	virtual void OutUser(struct User* player);													// 유저 퇴장

	// Interaction Level function
	virtual int Update();																		// 채집 맵 체크 및 ID 반환
	virtual void EnterSpawnInteraction(struct User* player);									// 채집물 정보 전송
	virtual bool GatherTry(int nInteraction_ID, int nSessionID, int nOccupation);				// 생활 시도
	virtual void StopGather(int nInteraction_ID);												// 생활 취소
	virtual bool GatherSuccess(User* player, DataCharacters* LV, int nInteraction_ID);			// 생활 성공
	virtual void InteractionRespawn(int nInteraction_ID);										// 생활몹 리스폰
	virtual bool InteractionMoveInfo(int nInteraction_ID);										// 동물 움직임
};

class HomeLevel : public Level
{
	virtual bool Init(struct DataLevel* pDataLevel);
	virtual void EnterUser(std::stringstream & SendStream, struct User* player);
	virtual void OutUser(struct User* player);
};

class InteractionLevel : public Level
{
	virtual bool Init(struct DataLevel* pDataLevel);
	virtual void EnterUser(std::stringstream & SendStream, struct User* player);
	virtual void OutUser(struct User* player);
	virtual void EnterSpawnInteraction(struct User* player);
	virtual int Update();

	virtual bool GatherTry(int nInteraction_ID, int nSessionID, int nOccupation);
	virtual void StopGather(int nInteraction_ID);
	virtual bool GatherSuccess(User* player, DataCharacters* LV, int nInteraction_ID);
	virtual void InteractionRespawn(int nInteraction_ID);
	virtual bool InteractionMoveInfo(int nInteraction_ID);
};
