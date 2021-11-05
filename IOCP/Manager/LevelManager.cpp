#include "LevelManager.h"

#include <random>
#include <ctime>
#include <functional>


bool Level::Init(DataLevel* pDataLevel)
{
	return false;
}

void Level::EnterUser(std::stringstream & SendStream, User* player)
{

}

void Level::EnterSpawnInteraction(User * player)
{
}

void Level::OutUser(User * player)
{

}

int Level::Update()
{
	return -1;
}

void Level::Zone_Broadcast(std::stringstream & SendStream)
{
	stOver* pOver = new stOver;

	for (auto& Player : mPlayers)
	{
		pOver->socket = Player.second->sockUser;

		MakeSendPacket(SendStream, pOver);
	}
}

void Level::Send(stOver* pOver)
{
	int nResult;
	DWORD	sendBytes;
	DWORD	dwFlags = 0;

	nResult = WSASend(
		pOver->socket,
		&(pOver->WSAbuf),
		1,
		&sendBytes,
		dwFlags,
		NULL,
		NULL
	);

	if (nResult == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		printf_s("WSASend Fail : %d", WSAGetLastError());
	}
}

void Level::MakeSendPacket(std::stringstream & SendStream, stOver* pOver)
{
	CopyMemory(pOver->buf, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
	pOver->WSAbuf.buf = pOver->buf;
	pOver->WSAbuf.len = SendStream.str().length();

	Send(pOver);
}

bool Level::GatherTry(int nInteraction_ID, int nSessionID, int nOccupation)
{
	return false;
}

void Level::StopGather(int nInteraction_ID)
{
}

bool Level::GatherSuccess(User* player, DataCharacters* LV, int nInteraction_ID)
{
	return false;
}

void Level::InteractionRespawn(int nInteraction_ID)
{
}

bool Level::InteractionMoveInfo(int m_nInteraction_ID)
{
	return false;
}



bool HomeLevel::Init(DataLevel* pDataLevel)
{
	if (pDataLevel == nullptr)
		return false;

	m_pDataLevel = pDataLevel;
	return true;
}

void HomeLevel::EnterUser(std::stringstream & SendStream, User * player)
{
	mPlayers[player->cPlayer.SessionID] = player;

	Zone_Broadcast(SendStream);
}

void HomeLevel::OutUser(User * player)
{
	mPlayers.erase(player->cPlayer.SessionID);
	CharactersInfo.players.erase(player->cPlayer.SessionID);
	EnterInfo.players.erase(player->cPlayer.SessionID);
}


bool InteractionLevel::Init(DataLevel* pDataLevel)
{
	if (pDataLevel == nullptr)
		return false;

	m_pDataLevel = pDataLevel;

	return true;
}

void InteractionLevel::EnterUser(std::stringstream & SendStream, User* player)
{
	mPlayers[player->cPlayer.SessionID] = player;

	Zone_Broadcast(SendStream);
}

void InteractionLevel::OutUser(User * player)
{
	mPlayers.erase(player->cPlayer.SessionID);
	CharactersInfo.players.erase(player->cPlayer.SessionID);
	EnterInfo.players.erase(player->cPlayer.SessionID);
}

void InteractionLevel::EnterSpawnInteraction(User* player)
{
	PacketInteractionInfo kInteractionInfo;
	

	for (auto& itInteraction : m_pDataLevel->Interactions)
		kInteractionInfo.Interactions[itInteraction.first] = *itInteraction.second;

	std::stringstream SendStream;
	SendStream << EType::SC_SPAWN_INTERACTION_INFO << std::endl;
	SendStream << kInteractionInfo << std::endl;

	stOver* m_pOver = new stOver;
	m_pOver->socket = player->sockUser;
	MakeSendPacket(SendStream, m_pOver);
}

int InteractionLevel::Update()
{
	if (mPlayers.size() == 0)
	{
		InteractionMove = false;
	}
	else
	{
		InteractionMove = true;
	}

	return Level_ID;
}

bool InteractionLevel::GatherTry(int nInteraction_ID, int nSessionID, int nOccupation) 
{
	// 채집중이거나 소멸상태 검사
	if (m_pDataLevel->Interactions[nInteraction_ID]->State != 1)
		return false;

	// 채집물의 타입과 유저의 직업정보가 일치하는지 검사
	if (m_pDataLevel->Interactions[nInteraction_ID]->Type_ID != nOccupation)
		return false;

	// 채집할때의 소모 피로도보다 현재 피로도가 더 적을 경우
	if (mPlayers[nSessionID]->cPlayer.Fatigue < m_pDataLevel->Interactions[nInteraction_ID]->Fatigue)
		return false;

	m_pDataLevel->Interactions[nInteraction_ID]->State = 2;

	return true;
}

void InteractionLevel::StopGather(int nInteraction_ID)
{
	m_pDataLevel->Interactions[nInteraction_ID]->State = 1;
}

bool InteractionLevel::GatherSuccess(User* player, DataCharacters* LV, int nInteraction_ID)
{
	m_pDataLevel->Interactions[nInteraction_ID]->State = 3;

	Packet_SC_SuccessGather kSend_SuccessGather;
	bool m_bResult = false;

	// 아이템 획득
	// 드롭된 아이템을 가지고 있는 경우
	for (auto& itInventory : player->stInventory.Inventroy)
	{
		if (itInventory.second.ItemID == m_pDataLevel->Interactions[nInteraction_ID]->Drop_ID)
		{
			itInventory.second.ItemCount++;
			kSend_SuccessGather.SlotNumber = itInventory.first;
			kSend_SuccessGather.DropItem_ID = itInventory.second.ItemID;
			kSend_SuccessGather.ItemCount = itInventory.second.ItemCount;
			m_bResult = true;
		}
	}

	// 드롭된 아이템을 가지고있지 않는 경우
	if (!m_bResult)
	{
		for (int i = 0; i < player->stInventory.MaxSize; i++)
		{
			if (player->stInventory.Inventroy[i].ItemID == 0)
			{
				player->stInventory.Inventroy[i].ItemID = m_pDataLevel->Interactions[nInteraction_ID]->Drop_ID;
				player->stInventory.Inventroy[i].ItemCount = 1;
				player->stInventory.Inventroy[i].SlotNumber = i;

				kSend_SuccessGather.SlotNumber = i;
				kSend_SuccessGather.DropItem_ID = player->stInventory.Inventroy[i].ItemID;
				kSend_SuccessGather.ItemCount = 1;
				break;
			}
		}
	}

	// 피로도 하락, 경험치 획득
	bool m_bLVResult = false;

	player->cPlayer.Experience += m_pDataLevel->Interactions[nInteraction_ID]->Experience;
	if (player->cPlayer.Experience >= LV->Character_Data[player->cPlayer.LV]->GetExpereience())
	{
		kSend_SuccessGather.Experience = player->cPlayer.Experience - LV->Character_Data[player->cPlayer.LV]->GetExpereience();
		player->cPlayer.Experience = kSend_SuccessGather.Experience;
		player->cPlayer.LV += 1;
		m_bLVResult = true;
	}
	else
	{
		kSend_SuccessGather.Experience = player->cPlayer.Experience;
	}

	// 레벨업 부분 추가 라인
	kSend_SuccessGather.LV = player->cPlayer.LV;
	kSend_SuccessGather.MaxExp = LV->Character_Data[player->cPlayer.LV]->GetExpereience();
	kSend_SuccessGather.MaxFatigue = LV->Character_Data[player->cPlayer.LV]->GetFatigue();

	int m_nFatigue = 0;

	// 피로도 감소 부분 - 장비에 의한 하락 제어
	for (auto itItem : player->stEquipment.Equipment)
	{
		if (itItem.second.SlotNumber == 4)
			continue;

		m_nFatigue += itItem.second.ItemStat;
	}

	if (m_nFatigue < m_pDataLevel->Interactions[nInteraction_ID]->Fatigue)
		player->cPlayer.Fatigue -= (m_pDataLevel->Interactions[nInteraction_ID]->Fatigue - m_nFatigue);

	kSend_SuccessGather.Fatigue = player->cPlayer.Fatigue;


	//채집 성공 로그 표시
	printf_s("[Gather Success] [SessionID : %d] 채집 성공\n", player->cPlayer.SessionID);
	printf_s("[피로도 %d 만큼 하락] [남은 피로도 : %d]\n", m_pDataLevel->Interactions[nInteraction_ID]->Fatigue, player->cPlayer.Fatigue);
	printf_s("[경험치 %d 만큼 증가] [현재 경험치 : %d]\n", m_pDataLevel->Interactions[nInteraction_ID]->Experience, player->cPlayer.Experience);

	int m_nSessionID = player->cPlayer.SessionID;

	std::stringstream SendStream;
	SendStream << EType::SC_DROP_ITEM_INFO << std::endl;
	SendStream << kSend_SuccessGather << std::endl;

	stOver* m_pOver = new stOver;
	m_pOver->socket = player->sockUser;

	MakeSendPacket(SendStream, m_pOver);

	return m_bLVResult;
}

void InteractionLevel::InteractionRespawn(int nInteraction_ID)
{
	m_pDataLevel->Interactions[nInteraction_ID]->State = 1;

	Packet_SC_RespawnInteraction kSend_RespawnInteraction;
	kSend_RespawnInteraction.kInteraction = *m_pDataLevel->Interactions[nInteraction_ID];

	std::stringstream SendStream;
	SendStream << EType::SC_RESPAWN_INTERACTION_INFO << std::endl;
	SendStream << kSend_RespawnInteraction << std::endl;

	Zone_Broadcast(SendStream);
}

bool InteractionLevel::InteractionMoveInfo(int nInteraction_ID)
{
	if (!InteractionMove)
		return false;

	if (m_pDataLevel->Interactions[nInteraction_ID]->State == 2)
		return false;

	std::mt19937 engine((unsigned int)time(NULL));
	std::uniform_int_distribution<int> XY(1, 10);
	std::uniform_int_distribution<int> Move(100, 800);
	auto Generator_XY = bind(XY, engine);
	auto Generator_Move = bind(Move, engine);

	Packet_CS_InteractionMove kSend_InteractionMove;

	if (m_pDataLevel->Interactions[nInteraction_ID]->Type_ID == 3)
	{
		if (m_pDataLevel->Interactions[nInteraction_ID]->X + 1000 >= m_pDataLevel->x_max || m_pDataLevel->Interactions[nInteraction_ID]->Y + 1000 >= m_pDataLevel->y_max)
		{
			m_pDataLevel->Interactions[nInteraction_ID]->X -= 1100;
			m_pDataLevel->Interactions[nInteraction_ID]->Y -= 1100;
		}
		else if (m_pDataLevel->Interactions[nInteraction_ID]->X + 1000 >= m_pDataLevel->x_max || m_pDataLevel->Interactions[nInteraction_ID]->Y - 1000 <= m_pDataLevel->y_min)
		{
			m_pDataLevel->Interactions[nInteraction_ID]->X -= 1100;
			m_pDataLevel->Interactions[nInteraction_ID]->Y += 1100;
		}
		else if (m_pDataLevel->Interactions[nInteraction_ID]->X - 1000 <= m_pDataLevel->x_min || m_pDataLevel->Interactions[nInteraction_ID]->Y + 1000 >= m_pDataLevel->y_max)
		{
			m_pDataLevel->Interactions[nInteraction_ID]->X += 1100;
			m_pDataLevel->Interactions[nInteraction_ID]->Y -= 1100;
		}
		else if (m_pDataLevel->Interactions[nInteraction_ID]->X - 1000 <= m_pDataLevel->x_min || m_pDataLevel->Interactions[nInteraction_ID]->Y - 1000 <= m_pDataLevel->y_min)
		{
			m_pDataLevel->Interactions[nInteraction_ID]->X -= 1100;
			m_pDataLevel->Interactions[nInteraction_ID]->Y -= 1100;
		}
		else
		{
			int m_nNumber = Generator_XY();
			int m_nMove = Generator_Move();
			if (m_nNumber > 5)
			{
				if (m_nNumber % 2 == 0)
				{
					m_pDataLevel->Interactions[nInteraction_ID]->X += m_nMove;
				}
				else
				{
					m_pDataLevel->Interactions[nInteraction_ID]->X -= m_nMove;
				}
			}
			else
			{
				if (m_nNumber % 2 == 0)
				{
					m_pDataLevel->Interactions[nInteraction_ID]->Y += m_nMove;
				}
				else
				{
					m_pDataLevel->Interactions[nInteraction_ID]->Y -= m_nMove;
				}
			}
		}
		kSend_InteractionMove.kInteraction = *m_pDataLevel->Interactions[nInteraction_ID];
	}


	std::stringstream SendStream;
	SendStream << EType::SC_INTERACTION_MOVE_INFO << std::endl;
	SendStream << kSend_InteractionMove << std::endl;

	if (m_pDataLevel->Interactions[nInteraction_ID]->State == 2)
		return false;

	if (mPlayers.size() == 0)
		return false;

	Zone_Broadcast(SendStream);
	return true;
}