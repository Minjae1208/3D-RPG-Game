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
	DataLevel* m_pDataLevel;																	// ä����
	std::map<int, struct User*> mPlayers;														// Level�� �ִ� ����
	Players CharactersInfo;																		// �÷��̾� ����ȭ
	PacketEnterInfo EnterInfo;																	// ���� �÷��̾� ����ȭ

	int Level_ID;
	bool InteractionMove = false;

	// ���� �Լ�
	static void Send(struct stOver* pOver);														// ��Ŷ ����
	static void MakeSendPacket(std::stringstream & SendStream, struct stOver* pOver);			// ��Ŷ ���� �� ó��
	void Zone_Broadcast(std::stringstream & SendStream);										// ��ü ����

	virtual bool Init(DataLevel* pDataLevel);													// Level Class �ʱ�ȭ
	virtual void EnterUser(std::stringstream & SendStream, struct User* player);				// ���� ����
	virtual void OutUser(struct User* player);													// ���� ����

	// Interaction Level function
	virtual int Update();																		// ä�� �� üũ �� ID ��ȯ
	virtual void EnterSpawnInteraction(struct User* player);									// ä���� ���� ����
	virtual bool GatherTry(int nInteraction_ID, int nSessionID, int nOccupation);				// ��Ȱ �õ�
	virtual void StopGather(int nInteraction_ID);												// ��Ȱ ���
	virtual bool GatherSuccess(User* player, DataCharacters* LV, int nInteraction_ID);			// ��Ȱ ����
	virtual void InteractionRespawn(int nInteraction_ID);										// ��Ȱ�� ������
	virtual bool InteractionMoveInfo(int nInteraction_ID);										// ���� ������
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
