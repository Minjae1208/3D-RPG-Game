#pragma once

#define _WINSOCK_DEPREACTED_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")

#include <map>
#include <iostream>
#include <WinSock2.h>

// Data ��� ����
#include "../Data/Data.h"

// Manager ��� ����
#include "../Manager/LevelManager.h"
#include "../Manager/DBManager.h"
#include "../Manager/TimerManager.h"
#include "../Manager/TradeManager.h"

#include "../Packet/ServerPacket.h"
#include "../Struct/stServer.h"

class Server
{
public:
	Server();
	~Server();
	
	// ���� ���� �Լ�
	bool Init();															// �ʱ�ȭ
	void Start();															// ����
	bool CreateThread();													// ������ ����
	void WorkerThread();													// ������ �۾�
	static void Send(stOver* pSocket);										// ��Ŷ ����
	static void SendPacket(std::stringstream& SendStream, stOver* pOver);	// ��Ŷ ���� �� �۾�
	static void Recv(stOver* pSocket);										// ���� �۾�

private:
	stOver * Over;										// ��������
	SOCKET sock_listen;									// ���� ���� ����
	HANDLE handle_IOCP;									// IOCP �ڵ�
	bool bAccept;										// ��û ���� �÷���
	bool bWorkerThread;									// �۾� ������ ���� �÷���
	HANDLE* handle_Thread;								// �۾� ������ �ڵ�


	static CRITICAL_SECTION	cs_Section;					// CharactersInfo �Ӱ迵��
	
	static Data data;									// XML Data
	static std::map<int, struct User> User_Data;		// Players ������
	static std::map<int, std::string> UserName;			// ?
	
	static DB DB_Manager;								// DB Manager
	static TimeManager Timer_Manager;					// Timer Manager
	static std::map<int, Trade*> Trade_Manager;			// Trade Manager
	static std::map<int, Level*> Level_Manager;			// Level Manager
	std::map<int, struct DataLevel*> Level_Data;		// Level Data Class

	stProcess			stProc[100];					// ��Ŷ ó��



// ���� ��Ŷ ó�� �Լ�
private:  
	// XML Data
	static void XMLData(stringstream & RecvStream, stOver * pOver);					// XML ������ ��û
	static void XML_Drop(stOver * pOver);											// ä�� ��� ������ ������
	static void XML_CB_First(stOver * pOver);										// ���� 1 ������
	static void XML_CB_Second(stOver * pOver);										// ���� 2 ������
	static void XML_EQItem(stOver * pOver);											// ��� ������ ������
	static void XML_Level(stOver * pOver);											// Level ������
	static void XML_Shop(int nOccupationID, stOver * pOver);						// ���� ������ ������

	// ȸ������, �α���, �α׾ƿ�
	static void SignUp(std::stringstream & RecvStream, stOver * pOver);				// ȸ�� ����
	static void Login(std::stringstream & RecvStream, stOver * pOver);				// �α���
	static void Login_Success(std::stringstream & RecvStream, stOver * pOver);		// �α��� ������ ���� �� ������ ����
	static void LogoutPlayer(std::stringstream & RecvStream, stOver * pOver);		// �α׾ƿ�
	static void LogOutUser(int nSessionID);											// ���� Ÿ�̸� �� DB ������Ʈ

	// ���� ����ȭ �Լ�
	static void EnterPlayer(std::stringstream & RecvStream, stOver * pOver);		// ���� ����
	static void SyncPlayers(std::stringstream & RecvStream, stOver * pOver);		// ����ȭ
	static void BroadcastNewPlayer(Player & player, Level * pLevel);				// ?

	// Level(Map) ���� �Լ�
	static void ChangeLevel(std::stringstream & RecvStream, stOver * pOver);		// �� �̵�
	static void SendLevelName(stOver * pOver, int nSessionID, int nAfterID);		// �� �̸� ����
	static void UpdateLevel(int nSessionID);										// Level Update

	// ä�� ���� �Լ�
	static void World_Chat(stringstream & RecvStream, stOver * pSocket);			// ���� ä�� (��ü)
	static void Level_Chat(stringstream & RecvStream, stOver * pSocket);			// Level ä�� (Map)
	static void Whisper_Chat(stringstream & RecvStream, stOver * pSocket);			// �ӼӸ�

	// ä�� ���� �Լ�
	static void RecvGatherStart(std::stringstream & RecvStream, stOver * pOver);	// ����
	static void RecvGatherCancel(std::stringstream & RecvStream, stOver * pOver);	// ���
	static void SendGatherSuccess(int nSessionID);									// ����

	// Timer ���� �Լ�
	static void GatherRespawn(int Thresh);											// ������
	static void InteractionMove(int nTimerID);										// ���� �̵�
	static void SetFatigue(int nSessionID);											// �Ƿε� ȸ��

	// ������ ���� �Լ�
	static void ItemCombination(std::stringstream & RecvStream, stOver * pOver);	// ������ ����
	static void ItemUse(std::stringstream & RecvStream, stOver * pOver);			// ������ ���
	static void ThrowItem(std::stringstream & RecvStream, stOver * pOver);			// ������ ������
	static void MountEquipment(std::stringstream & RecvStream, stOver * pOver);		// ��� ����
	static void UnMountEquipment(std::stringstream & RecvStream, stOver * pOver);	// ��� ����

	// ��ȯ ���� �Լ�
	static void TryTrade(std::stringstream & RecvStream, stOver * pOver);			// ��ȯ ��û
	static void TradeMatching(std::stringstream & RecvStream, stOver * pOver);		// ��ȯ ��Ī
	static void TradePutOn(std::stringstream & RecvStream, stOver * pOver);			// ������ �ø���
	static void TradeAccept(std::stringstream & RecvStream, stOver * pOver);		// ��ȯ ��ư Ŭ��
	static void TradeCancel(std::stringstream & RecvStream, stOver * pOver);		// ��ȯ ���
	static void Tradecompletion(int nTradeID, int nSessionID);						// ��ȯ �Ϸ�

	// Shop function
	static void ShopBuyItem(std::stringstream & RecvStream, stOver * pOver);		// ���� ������ ����

};