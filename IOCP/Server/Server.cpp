#include "Server.h"

#include <process.h>
#include <sstream>
#include <algorithm>
#include <string>
#include <cmath>


// static ���� �ʱ�
CRITICAL_SECTION				Server::cs_Section;
Data							Server::data;
map<int, User>					Server::User_Data;
map<int, string>				Server::UserName;

DB								Server::DB_Manager;
TimeManager						Server::Timer_Manager;
map<int, Trade*>				Server::Trade_Manager;
map<int, Level*>				Server::Level_Manager;


unsigned int WINAPI CallWorkerThread(LPVOID p)
{
	Server* pOverlappedEvent = (Server*)p;
	pOverlappedEvent->WorkerThread();
	return 0;
}

Server::Server()
{
	InitializeCriticalSection(&cs_Section);

#if DB_CHECK
	// DB Connect
	if (DB_Manager.Connect(DB_ADDRESS, DB_ID, DB_PW, DB_SCHEMA, DB_PORT) == false)
	{
		printf_s("DB Connect Fail..!\n");
	}
#endif
	// XML Parsing
	if (data.Parse(Level_Manager, Level_Data) == false)
	{
		printf_s("XML Parsing Fail..!\n");
	}

	// MMTimer Init
	if (Timer_Manager.InitCallBackSuccess(SendGatherSuccess) && Timer_Manager.InitCallBackRespawn(GatherRespawn) && Timer_Manager.InitCallBackInteractionMove(InteractionMove) == false)
	{
		printf_s("Init Timer Fail..!\n");
	}

	// ��Ŷ �Լ� �����Ϳ� �Լ� ����
	stProc[EType::CS_XML_DATA].Process = XMLData;							// XML Data ��û
	stProc[EType::CS_SIGNUP].Process = SignUp;								// ȸ������
	stProc[EType::CS_LOGIN_TRY].Process = Login;							// �α���
	stProc[EType::CS_LOGIN_SUCCESS].Process = Login_Success;				// �α��� ������ ���� �� ������ ����
	stProc[EType::CS_ENTER_ZONE].Process = EnterPlayer;						// ���� ����
	stProc[EType::CS_MOVE_INFO].Process = SyncPlayers;						// ������ ����ȭ
	stProc[EType::CS_LOGOUT_PLAYER].Process = LogoutPlayer;					// �α׾ƿ�
	stProc[EType::CS_CHANGE_ZONE].Process = ChangeLevel;					// �� �̵�
	stProc[EType::CS_WORLD_CHAT].Process = World_Chat;						// ���� ä��
	stProc[EType::CS_LEVEL_CHAT].Process = Level_Chat;						// ����(��) ä��
	stProc[EType::CS_WHISPER_CHAT].Process = Whisper_Chat;					// �ӼӸ�
	stProc[EType::CS_GATHER_START_INFO].Process = RecvGatherStart;			// ä�� ����
	stProc[EType::CS_GATHER_CANCEL_INFO].Process = RecvGatherCancel;		// ä�� ���
	stProc[EType::CS_COMBINATION_INFO].Process = ItemCombination;			// ������ ����
	stProc[EType::CS_ITEMUSE_INFO].Process = ItemUse;						// ������ ���
	stProc[EType::CS_THROWITEM_INFO].Process = ThrowItem;					// ������ ������
	stProc[EType::CS_MOUNTEQUIPMENT_INFO].Process = MountEquipment;			// ��� ����
	stProc[EType::CS_UNMOUNTEQUIPMENT_INFO].Process = UnMountEquipment;		// ��� ����
	stProc[EType::CS_TRYTRADE_INFO].Process = TryTrade;						// ��ȯ ��û
	stProc[EType::CS_TRADEMACTCHING_INFO].Process = TradeMatching;			// ��Ī
	stProc[EType::CS_TRADEPUTONITEM_INFO].Process = TradePutOn;				// ������ �ø���
	stProc[EType::CS_TRADEACCEPT_INFO].Process = TradeAccept;				// ��ȯ ��ư ����
	stProc[EType::CS_TRADECANCEL_INFO].Process = TradeCancel;				// ��ȯ ���
	stProc[EType::CS_SHOP_BUY_ITEM].Process = ShopBuyItem;					// ���� ������ ����
}

Server::~Server()
{
	WSACleanup();

	if (Over)
	{
		delete[] Over;
		Over = NULL;
	}
	if (handle_Thread)
	{
		delete[] handle_Thread;
		handle_Thread = NULL;
	}
}

bool Server::Init()
{
	WSADATA wsaData;
	int nResult;

	nResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (nResult != 0)
	{
		printf_s("Winsock Init Fail\n");
		return false;
	}

	sock_listen = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (sock_listen == INVALID_SOCKET)
	{
		printf("Socket Create Fail..!\n");
		return false;
	}
	
	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(SERVER_PORT);

	if (bind(sock_listen, (SOCKADDR*)&serverAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		printf_s("bind Fail..! \n");
		closesocket(sock_listen);
		WSACleanup();
		return false;
	}

	nResult = listen(sock_listen, 5);
	if (nResult == SOCKET_ERROR)
	{
		printf_s("listen Fail..!\n");
		closesocket(sock_listen);
		WSACleanup();
		return false;
	}

	return true;
}

void Server::Start()
{
	int nResult;
	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(SOCKADDR_IN);
	SOCKET clientSocket;
	DWORD recvBytes;
	DWORD flags;

	handle_IOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	if (!CreateThread()) return;

	printf_s("Start Server... \n");

	while (true)
	{
		clientSocket = WSAAccept(sock_listen, (struct sockaddr*)&clientAddr, &addrLen, NULL, NULL);
		if (clientSocket == INVALID_SOCKET)
		{
			printf_s("Accept Fail..! \n");
			return;
		}
		Over = new stOver();
		Over->socket = clientSocket;
		Over->WSAbuf.len = MAX_BUFFER;
		Over->WSAbuf.buf = Over->buf;
		flags = 0;

		handle_IOCP = CreateIoCompletionPort((HANDLE)clientSocket, handle_IOCP, (DWORD)Over, 0);

		nResult = WSARecv(Over->socket, &Over->WSAbuf, 1, &recvBytes, &flags, &(Over->overlapped), NULL);



		if (nResult == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
		{
			printf_s("IO Fail : %d", WSAGetLastError());
			return;
		}
	}
}

bool Server::CreateThread()
{
	unsigned int m_nThreadID;
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	int m_nThreadCount = sysInfo.dwNumberOfProcessors * 2;

	printf_s("Thread Count : %d\n", m_nThreadCount);

	handle_Thread = new HANDLE[m_nThreadCount];

	// Create Thread
	for (int i = 0; i < m_nThreadCount; i++)
	{
		handle_Thread[i] = (HANDLE *)_beginthreadex(NULL, 0, &CallWorkerThread, this, CREATE_SUSPENDED, &m_nThreadID);
		if (handle_Thread[i] == NULL)
		{
			printf_s("Worker Thread Create Fail..!\n");
			return false;
		}
		ResumeThread(handle_Thread[i]);
	}
	return true;
}

void Server::WorkerThread()
{
	// �Լ� ȣ�� ���� ����
	BOOL m_bResult;
	// Overlapped I/O �۾����� ���۵� ������ ũ��
	DWORD m_dwRecvBytes;
	// Completion Key�� ���� ������ ����
	stOver * m_pOverKey;
	// I/O �۾��� ���� ��û�� Overlapped ����ü�� ���� ������
	stOver * m_pOver;

	while (bWorkerThread)
	{
		m_bResult = GetQueuedCompletionStatus(Over,
			&m_dwRecvBytes,								// ������ ���۵� ����Ʈ
			(PULONG_PTR)&m_pOverKey,					// completion key
			(LPOVERLAPPED *)&m_pOver,					// overlapped I/O
			INFINITE									// ����� �ð�
		);
		if (m_bResult == false || m_dwRecvBytes == 0)
		{
			cout << "[Packet] Socket(" << m_pOver->socket << ") ���� ����\n" << endl;
			int m_nSessionID = 0;
			for (auto& itUser : User_Data)
			{
				if (itUser.second.sockUser == m_pOver->socket)
				{
					m_nSessionID = itUser.second.cPlayer.SessionID;
					map<int, Level*>::iterator itFind = Level_Manager.find(itUser.second.cPlayer.Level_ID);
					if (itFind != Level_Manager.end())
					{
						Level* pLevel = itFind->second;
						if (pLevel != nullptr)
						{
							EnterCriticalSection(&cs_Section);
							pLevel->OutUser(&itUser.second);
							LeaveCriticalSection(&cs_Section);
							pLevel->Update();
						}
					}
				}
			}
			User_Data.erase(m_nSessionID);
			closesocket(m_pOver->socket);
			free(m_pOver);
			continue;
		}
		m_pOver->WSAbuf.len = m_dwRecvBytes;

		if (m_bResult == false && m_dwRecvBytes == 0 && m_pOver == NULL)
		{
			closesocket(m_pOver->socket);
			free(m_pOver);
			continue;
		}

		try
		{
			// ��Ŷ ����
			int m_nPacketType;
			std::stringstream RecvStream;

			RecvStream << m_pOver->WSAbuf.buf;
			RecvStream >> m_nPacketType;
			// ��Ŷ ó��

			if (stProc[m_nPacketType].Process != nullptr)
			{
				stProc[m_nPacketType].Process(RecvStream, m_pOver);
			}
			else
			{
				printf_s("Not Found Packet Type : %d\n", m_nPacketType);
			}
		}
		catch (const std::exception& e)
		{
			printf_s("Error  : %s\n", e.what());
		}

		Recv(m_pOver);
	}
}

void Server::Send(stOver * pOver)
{
	int m_nResult;
	DWORD m_dwSendBytes;
	DWORD m_dwFlags = 0;

	m_nResult = WSASend(
		pOver->socket,
		&(pOver->WSAbuf),
		1,
		&m_dwSendBytes,
		m_dwFlags,
		NULL,
		NULL
	);

	if (m_nResult == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		printf_s("WSASend Fail : %d\n", WSAGetLastError());
	}
}

void Server::SendPacket(std::stringstream & SendStream, stOver * pOver)
{
	CopyMemory(pOver->buf, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
	pOver->WSAbuf.buf = pOver->buf;
	pOver->WSAbuf.len = SendStream.str().length();

	Send(pOver);
}

void Server::Recv(stOver * pOver)
{
	int m_nResult;

	DWORD m_dwFlags = 0;

	ZeroMemory(&(pOver->overlapped), sizeof(OVERLAPPED));
	ZeroMemory(pOver->buf, MAX_BUFFER);

	pOver->WSAbuf.len = MAX_BUFFER;
	pOver->WSAbuf.buf = pOver->buf;

	m_dwFlags = 0;
	m_nResult = WSARecv(pOver->socket, &(pOver->WSAbuf), 1, (LPDWORD)&pOver, &m_dwFlags, (LPWSAOVERLAPPED)&(pOver->overlapped), NULL);

	if (m_nResult == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		printf_s("WSARecv Fail : %d", WSAGetLastError());
	}
}


void Server::XMLData(stringstream & RecvStream, stOver * pOver)
{
	XML_Drop(pOver);
	Sleep(100);

	XML_CB_First(pOver);
	Sleep(100);

	XML_CB_Second(pOver);
	Sleep(100);

	XML_EQItem(pOver);
	Sleep(100);

	XML_Level(pOver);

	printf_s("XML Data Send Success!\n");
}

void Server::XML_Drop(stOver * pOver)
{
	Packet_XMLDropItem kXMLDropItem;

	Drop_ItemInfo kDrop_ItemInfo;

	for (auto& itItemInfo : data.DropItems->DropItem_Data)
	{
		itItemInfo.second->GetAllData(&kDrop_ItemInfo);
		kXMLDropItem.DropItems[itItemInfo.first] = kDrop_ItemInfo;
	}

	std::stringstream SendStream;
	SendStream << EType::SC_XML_DROP_ITEM << std::endl;
	SendStream << kXMLDropItem << std::endl;

	SendPacket(SendStream, pOver);
}

void Server::XML_CB_First(stOver * pOver)
{
	Packet_XMLCB_First kXMLCB_First;

	CB_ItemInfo * kCB_ItemInfo;

	for (auto& itCBItemInfo : data.CBFirsts->CBFirst_Data)
	{
		kCB_ItemInfo = new CB_ItemInfo;

		itCBItemInfo.second->GetAllData(kCB_ItemInfo);
		kXMLCB_First.CB_FirstItems[itCBItemInfo.first] = *kCB_ItemInfo;
	}

	std::stringstream SendStream;
	SendStream << EType::SC_XML_CB_FIRST << std::endl;
	SendStream << kXMLCB_First << std::endl;

	SendPacket(SendStream, pOver);
}

void Server::XML_CB_Second(stOver * pOver)
{
	Packet_XMLCB_Second kXMLCB_Second_1;
	Packet_XMLCB_Second kXMLCB_Second_2;

	CB_ItemInfo * kCB_ItemInfo;

	int Count = 1;

	for (auto& itCBItemInfo : data.CBSeconds->CBSecond_Data)
	{
		kCB_ItemInfo = new CB_ItemInfo;
		if (Count <= 11)
		{
			itCBItemInfo.second->GetAllData(kCB_ItemInfo);
			kXMLCB_Second_1.CB_SecondItems[itCBItemInfo.first] = *kCB_ItemInfo;
		}
		else
		{
			itCBItemInfo.second->GetAllData(kCB_ItemInfo);
			kXMLCB_Second_2.CB_SecondItems[itCBItemInfo.first] = *kCB_ItemInfo;
		}
		Count++;
	}

	std::stringstream SendStream_1;
	SendStream_1 << EType::SC_XML_CB_SECOND << std::endl;
	SendStream_1 << kXMLCB_Second_1 << std::endl;

	SendPacket(SendStream_1, pOver);

	Sleep(100);

	std::stringstream SendStream_2;
	SendStream_2 << EType::SC_XML_CB_SECOND << std::endl;
	SendStream_2 << kXMLCB_Second_2 << std::endl;


	SendPacket(SendStream_2, pOver);
}

void Server::XML_EQItem(stOver * pOver)
{
	Packet_XMLEQItem kXMLEQItem;

	Equipment_ItemInfo kEquipment_ItemInfo;

	for (auto& itEQItemInfo : data.Equipments->Equipment_Data)
	{
		itEQItemInfo.second->GetAllData(&kEquipment_ItemInfo);
		kXMLEQItem.Equipment_Items[itEQItemInfo.first] = kEquipment_ItemInfo;
	}

	std::stringstream SendStream;
	SendStream << EType::SC_XML_EQ_ITEM << std::endl;
	SendStream << kXMLEQItem << std::endl;

	SendPacket(SendStream, pOver);
}

void Server::XML_Level(stOver * pOver)
{
	Packet_XMLLevel kXMLLevel;

	Level_Info kLevel_Info;

	for (auto& itLevel : data.Maps->Map_Data)
	{
		kLevel_Info.Level_ID = itLevel.second->ID;
		kLevel_Info.Level_Name = itLevel.second->NameID;
		kXMLLevel.Levels[itLevel.first] = kLevel_Info;
	}

	std::stringstream SendStream;
	SendStream << EType::SC_XML_LEVEL << std::endl;
	SendStream << kXMLLevel << std::endl;

	SendPacket(SendStream, pOver);
}

void Server::XML_Shop(int nOccupationID, stOver * pOver)
{
	Packet_XMLShopInfo kXMLShopInfo;

	Shop_ItemInfo ksShop_ItemInfo;

	switch (nOccupationID)
	{
	case EOccupation::GATHERER:
		for (auto& ItShop : data.Shops->GathererShop_Data)
		{
			ItShop.second->GetAllData(&ksShop_ItemInfo);
			kXMLShopInfo.Shop_Items[ItShop.first] = ksShop_ItemInfo;
		}

		break;

	case EOccupation::MINOR:
		for (auto& ItShop : data.Shops->MinorShop_Data)
		{
			ItShop.second->GetAllData(&ksShop_ItemInfo);
			kXMLShopInfo.Shop_Items[ItShop.first] = ksShop_ItemInfo;
		}

		break;

	case EOccupation::HUNTER:
		for (auto& ItShop : data.Shops->HunterShop_Data)
		{
			ItShop.second->GetAllData(&ksShop_ItemInfo);
			kXMLShopInfo.Shop_Items[ItShop.first] = ksShop_ItemInfo;
		}

		break;
	}

	std::stringstream SendStream;
	SendStream << EType::SC_XML_SHOP_ITEM << std::endl;
	SendStream << kXMLShopInfo << std::endl;

	SendPacket(SendStream, pOver);
}

void Server::SignUp(std::stringstream & RecvStream, stOver * pOver)
{
	bool m_bResult = false;
	Packet_CS_SignUp kRecv_SignUp;

	RecvStream >> kRecv_SignUp;

	printf_s("[SignUp] [%s / %s / %s]\n", kRecv_SignUp.m_strID.c_str(), kRecv_SignUp.m_strPW.c_str(), kRecv_SignUp.m_strName.c_str());

	if (DB_Manager.SignUp(kRecv_SignUp.m_strID, kRecv_SignUp.m_strPW, kRecv_SignUp.m_strName))
	{
		if (DB_Manager.MakeSessionID(kRecv_SignUp.m_strID))
		{
			m_bResult = DB_Manager.InsertInfo(kRecv_SignUp.m_strID, kRecv_SignUp.m_nOccupation);
		}
	}

	std::stringstream SendStream;
	SendStream << EType::SC_SIGNUP << endl;
	SendStream << m_bResult << endl;

	SendPacket(SendStream, pOver);
}

void Server::Login(std::stringstream & RecvStream, stOver * pOver)
{
	Packet_Login sendLogin;
	std::string m_strID;
	std::string m_strPW;
	std::string m_strCharacterName;
	std::string m_strLevel;

	int m_nLevel_ID;
	int m_nSessionID;
	int m_nOccupationID;
	int m_nLV;
	int m_nMaxExp;
	int m_nExperence;
	int m_nMaxFatigue;
	int m_nFatigue;;

	RecvStream >> m_strID;
	RecvStream >> m_strPW;
	printf_s("[Login] �α��� �õ� {%s},{%s}\n", m_strID.c_str(), m_strPW.c_str());

#if DB_CHECK
	if (DB_Manager.Login(m_strID, m_strPW))
	{
		m_nSessionID = DB_Manager.GetSessinID(m_strID);
		m_nLevel_ID = DB_Manager.GetLevel(m_nSessionID);
		m_nOccupationID = DB_Manager.GetOccupation(m_nSessionID);
		m_nLV = DB_Manager.GetLV(m_nSessionID);
		m_nMaxExp = data.characters->Character_Data[m_nLV]->GetExpereience();
		m_nExperence = DB_Manager.GetExperience(m_nSessionID);
		m_nFatigue = DB_Manager.GetFatigue(m_nSessionID);
		m_nMaxFatigue = data.characters->Character_Data[m_nLV]->GetFatigue();
		m_strCharacterName = DB_Manager.GetCName(m_nSessionID);

		m_strCharacterName += "\0";
		cout << "[Login] " << m_strCharacterName.c_str() << " �α��� ����!" << endl;

		// ��Ŷ RecvPacket_Login�� �� ����
		sendLogin.LoginResult = true;
		sendLogin.UserCName = m_strCharacterName;
		sendLogin.Level = data.Maps->Map_Data[m_nLevel_ID]->NameID;
		sendLogin.Level_Name = data.Maps->Map_Data[m_nLevel_ID]->Name;
		sendLogin.SessionID = m_nSessionID;
		sendLogin.OccupationID = m_nOccupationID;
		sendLogin.LV = m_nLV;
		sendLogin.MaxExp = m_nMaxExp;
		sendLogin.Experence = m_nExperence;
		sendLogin.MaxFatigue = m_nMaxFatigue;
		sendLogin.Fatigue = m_nFatigue;
	}
	else
	{
		m_nSessionID = 0;
		m_nOccupationID = 0;
		cout << "[Login] �α��� ����!" << endl;
	}
#else
	sendLogin.LoginResult = true;
	sendLogin.UserCName = "TESTGAME";
	sendLogin.SessionID = 1111;
	sendLogin.OccupationID = 1;
	sendLogin.Experence = 0;
	sendLogin.Fatigue = 200;

	cout << "[Login] " << sendLogin.UserCName << " �α��� ����!" << endl;

#endif

	std::stringstream SendStream;
	SendStream << EType::SC_LOGIN_TRY << std::endl;
	SendStream << sendLogin << std::endl;

	SendPacket(SendStream, pOver);

	Sleep(100);
	XML_Shop(m_nOccupationID, pOver);
}

void Server::Login_Success(std::stringstream & RecvStream, stOver * pOver)
{
	bool m_bRecvCheck;
	bool m_bSendCheck = false;
	int m_nSessionID;
	std::string m_sUserCName;
	int m_nLevel_ID = 0;

	RecvStream >> m_bRecvCheck;
	RecvStream >> m_sUserCName;
	RecvStream >> m_nSessionID;


	UserName[m_nSessionID] = m_sUserCName;

#if DB_CHECK
	m_nLevel_ID = DB_Manager.GetLevel(m_nSessionID);
#else
	m_nLevel_ID = 1;
#endif

	int m_nID;
	int m_nCount;
	int m_nSlot;

#if DB_CHECK
	//Get Inventory

	int m_nMaxSlot = DB_Manager.GetInventorySize(m_nSessionID);
	User_Data[m_nSessionID].stInventory.MaxSize = m_nMaxSlot;

	for (int i = 1; i <= m_nMaxSlot * 2; i += 2)
	{
		m_nID = DB_Manager.GetUserInventory(m_nSessionID, i);
		if (m_nID == -1)
			continue;

		m_nCount = DB_Manager.GetUserInventory(m_nSessionID, i + 1);
		if (m_nCount == -1)
			continue;

		m_nSlot = ((i + 1) / 2) - 1;

		User_Data[m_nSessionID].stInventory.Inventroy[m_nSlot].ItemID = m_nID;
		User_Data[m_nSessionID].stInventory.Inventroy[m_nSlot].ItemCount = m_nCount;
		User_Data[m_nSessionID].stInventory.Inventroy[m_nSlot].SlotNumber = m_nSlot;
	}

	// Get Equipment
	for (int i = 1; i <= 5; i++)
	{
		m_nID = DB_Manager.GetUserEquipment(m_nSessionID, i);
		if (m_nID == -1)
			continue;

		m_nSlot = i - 1;

		User_Data[m_nSessionID].stEquipment.Equipment[m_nSlot].ItemID = m_nID;
		User_Data[m_nSessionID].stEquipment.Equipment[m_nSlot].SlotNumber = m_nSlot;
		User_Data[m_nSessionID].stEquipment.Equipment[m_nSlot].ItemStat = data.Equipments->Equipment_Data[m_nID]->GetStat();
	}
#endif

	if (m_bRecvCheck && m_nLevel_ID != 0)
	{
		printf_s("[Login] %s���� �ΰ������� �����߽��ϴ�.\n", m_sUserCName.c_str());
		m_bSendCheck = true;
	}

	Packet_Ingame kPacket_Ingame;

	kPacket_Ingame.Result = m_bSendCheck;
	kPacket_Ingame.Level_ID = m_nLevel_ID;
	kPacket_Ingame.InventoryMax = m_nMaxSlot;

	for (auto& itInventroy : User_Data[m_nSessionID].stInventory.Inventroy)
		kPacket_Ingame.Inventroys[itInventroy.first] = itInventroy.second;

	for (auto& itEquipment : User_Data[m_nSessionID].stEquipment.Equipment)
		kPacket_Ingame.Equipments[itEquipment.first] = itEquipment.second;

	int m_nLV = DB_Manager.GetLV(m_nSessionID);
	int m_nFatigue = DB_Manager.GetFatigue(m_nSessionID);

	// �������� ȸ��
	int m_nDate = DB_Manager.GetDate(m_nSessionID);
	m_nFatigue += m_nDate * 50;

	if (m_nFatigue > data.characters->Character_Data[m_nLV]->GetFatigue())
		m_nFatigue = data.characters->Character_Data[m_nLV]->GetFatigue();

	DB_Manager.UpdateFatigue(m_nSessionID, m_nFatigue);

	if (m_nFatigue < data.characters->Character_Data[m_nLV]->GetFatigue())
	{
		Timer_Manager.FatigueTimer(m_nSessionID);
	}

	std::stringstream SendStream;
	SendStream << EType::SC_LOGIN_SUCCESS << std::endl;
	SendStream << kPacket_Ingame << std::endl;

	SendPacket(SendStream, pOver);
}

void Server::LogoutPlayer(std::stringstream & RecvStream, stOver * pOver)
{
	int m_nSessionID;
	RecvStream >> m_nSessionID;

	LogOutUser(m_nSessionID);

	map<int, Level*>::iterator itFind = Level_Manager.find(m_nSessionID);
	if (itFind != Level_Manager.end())
	{
		Level* pLevel = itFind->second;
		if (pLevel != nullptr)
		{
			EnterCriticalSection(&cs_Section);
			pLevel->OutUser(&User_Data[m_nSessionID]);
			LeaveCriticalSection(&cs_Section);
		}
	}

	User_Data.erase(m_nSessionID);
	printf_s("[LogOut] [SessionID : %d] �α׾ƿ�\nŬ���̾�Ʈ �� : %zd\n", m_nSessionID, User_Data.size());
}

void Server::LogOutUser(int nSessionID)
{
	Timer_Manager.StopFatigueTimer(nSessionID + Timer_Manager.Fatigue_Set);
	DB_Manager.UpdateDate(nSessionID);
}

void Server::EnterPlayer(std::stringstream & RecvStream, stOver * pOver)
{
	Player m_cPlayer;
	RecvStream >> m_cPlayer;
	printf_s("[Enter Player] [SessionID : %d] ĳ���� ����\n", m_cPlayer.SessionID);

	std::map<int, Level*>::iterator itFind = Level_Manager.find(m_cPlayer.Level_ID);
	if (itFind != Level_Manager.end())
	{
		Level* pLevel = itFind->second;
		if (pLevel != nullptr)
		{
			EnterCriticalSection(&cs_Section);

			Player* m_pPlayer = &pLevel->CharactersInfo.players[m_cPlayer.SessionID];

			// ĳ������ ��ġ�� ����						
			m_pPlayer->SessionID = m_cPlayer.SessionID;
			m_pPlayer->OccupationID = m_cPlayer.OccupationID;
			m_pPlayer->LV = m_cPlayer.LV;
			m_pPlayer->Experience = m_cPlayer.Experience;
			m_pPlayer->Fatigue = m_cPlayer.Fatigue;
			m_pPlayer->Level_ID = m_cPlayer.Level_ID;

			m_pPlayer->X = m_cPlayer.X;
			m_pPlayer->Y = m_cPlayer.Y;
			m_pPlayer->Z = m_cPlayer.Z;

			// ĳ������ ȸ������ ����
			m_pPlayer->Yaw = m_cPlayer.Yaw;
			m_pPlayer->Pitch = m_cPlayer.Pitch;
			m_pPlayer->Roll = m_cPlayer.Roll;

			//m_pPlayer->Name = UserName[m_cPlayer.SessionID];

			LeaveCriticalSection(&cs_Section);

			User_Data[m_cPlayer.SessionID].cPlayer = m_cPlayer;
			User_Data[m_cPlayer.SessionID].sockUser = pOver->socket;

			BroadcastNewPlayer(m_cPlayer, pLevel);
		}
	}
}

void Server::SyncPlayers(std::stringstream & RecvStream, stOver * pOver)
{
	Player m_cPlayer;
	RecvStream >> m_cPlayer;

	std::map<int, Level*>::iterator itFind = Level_Manager.find(m_cPlayer.Level_ID);
	if (itFind != Level_Manager.end())
	{
		Level* pLevel = itFind->second;
		if (pLevel != nullptr)
		{
			EnterCriticalSection(&cs_Section);
			Player * m_pPlayer = &pLevel->CharactersInfo.players[m_cPlayer.SessionID];

			m_pPlayer->SessionID = m_cPlayer.SessionID;
			m_pPlayer->X = m_cPlayer.X;
			m_pPlayer->Y = m_cPlayer.Y;
			m_pPlayer->Z = m_cPlayer.Z;

			// ĳ������ ȸ������ ����
			m_pPlayer->Yaw = m_cPlayer.Yaw;
			m_pPlayer->Pitch = m_cPlayer.Pitch;
			m_pPlayer->Roll = m_cPlayer.Roll;

			m_pPlayer->OccupationID = m_cPlayer.OccupationID;
			m_pPlayer->LV = m_cPlayer.LV;
			m_pPlayer->Experience = m_cPlayer.Experience;
			m_pPlayer->Fatigue = m_cPlayer.Fatigue;

			m_pPlayer->Level_ID = m_cPlayer.Level_ID;

			LeaveCriticalSection(&cs_Section);

			User_Data[m_cPlayer.SessionID].cPlayer = m_cPlayer;

			std::stringstream SendStream;
			SendStream << EType::SC_MOVE_INFO << std::endl;
			SendStream << pLevel->CharactersInfo << std::endl;

			pLevel->Zone_Broadcast(SendStream);
		}
	}
}

void Server::BroadcastNewPlayer(Player & player, Level * pLevel)
{
	EnterCriticalSection(&cs_Section);
	PacketEnter* kEnter = &pLevel->EnterInfo.players[player.SessionID];
	kEnter->SessionID = player.SessionID;
	kEnter->Name = UserName[player.SessionID];
	kEnter->OccupationID = player.OccupationID;
	kEnter->X = player.X;
	kEnter->Y = player.Y;
	kEnter->Z = player.Z;
	LeaveCriticalSection(&cs_Section);

	std::stringstream SendStream;
	SendStream << EType::SC_ENTER_NEW_PLAYER << std::endl;
	SendStream << pLevel->EnterInfo << std::endl;

	pLevel->EnterUser(SendStream, &User_Data[player.SessionID]);
	pLevel->EnterSpawnInteraction(&User_Data[player.SessionID]);
	
	int m_nLevel_ID = pLevel->Update();
	if (m_nLevel_ID != -1)
	{
		int m_nInteractionID;
		int m_nTimerID;

		for (auto Interaction : pLevel->m_pDataLevel->Interactions)
		{
			if (Interaction.second->Type_ID == 3)
			{
				m_nInteractionID = Interaction.second->ID;
				m_nTimerID = Interaction.second->TimerID;
				Timer_Manager.InterationMoveTImer(m_nLevel_ID, m_nInteractionID, m_nTimerID);
			}
		}
	}
}


void Server::ChangeLevel(std::stringstream & RecvStream, stOver * pOver)
{
	Packet_CS_LevelChange kRecv_LevelChange;
	RecvStream >> kRecv_LevelChange;

	printf_s("[Change Level] [SessionID : %d] %s ������ �̵� ��û\n", kRecv_LevelChange.SessionID, data.Maps->Map_Data[kRecv_LevelChange.AfterLevel_ID]->Name.c_str());

	std::stringstream SendStream;
	SendStream << EType::SC_CHANGE_ZONE << std::endl;
	SendStream << kRecv_LevelChange.SessionID << std::endl;

	std::map<int, Level*>::iterator itFind = Level_Manager.find(kRecv_LevelChange.BefroeLevel_ID);
	if (itFind != Level_Manager.end())
	{
		Level* pLevel = itFind->second;
		if (pLevel != nullptr)
		{
			EnterCriticalSection(&cs_Section);
			pLevel->OutUser(&User_Data[kRecv_LevelChange.SessionID]);
			LeaveCriticalSection(&cs_Section);
			pLevel->Update();
			pLevel->Zone_Broadcast(SendStream);
		}
	}
}

void Server::SendLevelName(stOver * pOver, int nSessionID, int nAfterID)
{
	std::string m_strName;
	m_strName = data.Maps->Map_Data[nAfterID]->Name;

	Packet_SC_LevelInfo kSend_LevelInfo;
	kSend_LevelInfo.SessionID = nSessionID;
	kSend_LevelInfo.Level_Name = m_strName;

	std::stringstream SendStream;
	SendStream << EType::SC_MAP_INFO << std::endl;
	SendStream << kSend_LevelInfo << std::endl;

	SendPacket(SendStream, pOver);
}

void Server::UpdateLevel(int nSessionID)
{
	Packet_SC_UpdateLevel kSend_UpdateLevel;

	int m_nLevel_ID = 0;

	switch (User_Data[nSessionID].cPlayer.LV)
	{
	case 10:
		m_nLevel_ID = 4;
		break;
	case 20:
		m_nLevel_ID = 8;
		break;
	}
	kSend_UpdateLevel.SessionID = nSessionID;
	kSend_UpdateLevel.Level_Name = data.Maps->Map_Data[m_nLevel_ID]->Name;
	kSend_UpdateLevel.Level_Name_ID = data.Maps->Map_Data[m_nLevel_ID]->NameID;

	DB_Manager.UpdateLevel(nSessionID, m_nLevel_ID);

	std::stringstream SendStream;
	SendStream << kSend_UpdateLevel << std::endl;

	stOver * m_pOver = new stOver;
	m_pOver->socket = User_Data[nSessionID].sockUser;

	SendPacket(SendStream, m_pOver);
}

void Server::World_Chat(std::stringstream & RecvStream, stOver * pOver)
{
	
	Packet_CS_WorldChat kRecv_WorldChat;

	RecvStream >> kRecv_WorldChat;

	cout << "[CHAT(World)] " << kRecv_WorldChat.SendUserName << " : ";
	cout << kRecv_WorldChat.Message << std::endl;

	// �޼��� ��Ŷ ����
	Packet_SC_Message kSend_Message;
	kSend_Message.ChatType = ChatType::World_Chat;
	kSend_Message.SendUserName = kRecv_WorldChat.SendUserName;
	kSend_Message.Message = kRecv_WorldChat.Message;


	std::stringstream SendStream;
	SendStream << EType::SC_WORLD_CHAT << std::endl;
	SendStream << kSend_Message << std::endl;

	for (auto& level : Level_Manager)
	{
		if (level.second->mPlayers.empty())
			continue;
		level.second->Zone_Broadcast(SendStream);
	}
	
}

void Server::Level_Chat(std::stringstream& RecvStream, stOver* pOver)
{
	
	Packet_CS_LevelChat kRecv_LevelChat;

	RecvStream >> kRecv_LevelChat;
	cout << "[CHAT(Level)] " << kRecv_LevelChat.SendUserName << " : ";
	cout << kRecv_LevelChat.Message << std::endl;

	Packet_SC_Message kSend_Message;
	kSend_Message.ChatType = ChatType::Level_Chat;
	kSend_Message.SendUserName = kRecv_LevelChat.SendUserName;
	kSend_Message.Message = kRecv_LevelChat.Message;

	std::stringstream SendStream;
	SendStream << EType::SC_LEVEL_CHAT << std::endl;
	SendStream << kSend_Message << std::endl;

	int m_nLevel_ID = User_Data[kRecv_LevelChat.SendUserID].cPlayer.Level_ID;

	map<int, Level*>::iterator itFind = Level_Manager.find(m_nLevel_ID);
	if (itFind != Level_Manager.end())
	{
		Level* pLevel = itFind->second;
		if (pLevel != nullptr)
		{
			pLevel->Zone_Broadcast(SendStream);
		}
	}
	
}

void Server::Whisper_Chat(std::stringstream & RecvStream, stOver * pOver)
{
	// �ӼӸ� ��Ŷ ����
	Packet_CS_WhisperChat kRecv_WhisperChat;
	RecvStream >> kRecv_WhisperChat;

	cout << "[CHAT(Whisper)] " << kRecv_WhisperChat.SendUserName << " : ";
	cout << kRecv_WhisperChat.Message << std::endl;

	// �޼��� ��Ŷ ����
	Packet_SC_Message kSend_Message;
	kSend_Message.ChatType = ChatType::Whisper_Chat;
	kSend_Message.SendUserName = kRecv_WhisperChat.SendUserName;
	kSend_Message.Message = kRecv_WhisperChat.Message;

	std::stringstream SendStream;
	SendStream << EType::SC_WHISPER_CHAT << std::endl;
	SendStream << kSend_Message << std::endl;

	stOver* m_pOver = new stOver;
	m_pOver->socket = User_Data[kRecv_WhisperChat.RecvUserID].sockUser;

	SendPacket(SendStream, pOver); // �����ϴ� ���
	SendPacket(SendStream, m_pOver); // �޴� ���
	
}

void Server::RecvGatherStart(std::stringstream & RecvStream, stOver * pOver)
{
	Packet_CS_TryGather kRecv_TryGather;

	bool m_bCheckGather;

	RecvStream >> kRecv_TryGather;

	printf_s("[SessionID : %d] [InteractionID : %d] ä���õ�\n", kRecv_TryGather.SessionID, kRecv_TryGather.Interaction_ID);

	Packet_SC_TryGather kSend_TryGather;
	std::stringstream SendStream;
	SendStream << EType::SC_GATHER_START_INFO << std::endl;

	std::map<int, Level*>::iterator itFind = Level_Manager.find(User_Data[kRecv_TryGather.SessionID].cPlayer.Level_ID);
	if (itFind != Level_Manager.end())
	{
		Level* pLevel = itFind->second;
		if (pLevel != nullptr)
		{
			m_bCheckGather = pLevel->GatherTry(kRecv_TryGather.Interaction_ID, kRecv_TryGather.SessionID, kRecv_TryGather.Occupation);
			kSend_TryGather.result = m_bCheckGather;
			if (!m_bCheckGather)
			{
				SendStream << kSend_TryGather << std::endl;
				SendPacket(SendStream, pOver);
				return;
			}
			kSend_TryGather.SessionID = kRecv_TryGather.SessionID;
			kSend_TryGather.Interaction_ID = kRecv_TryGather.Interaction_ID;
			SendStream << kSend_TryGather << std::endl;
			
			pLevel->Zone_Broadcast(SendStream);
			Timer_Manager.GatherTimer(kRecv_TryGather.SessionID, kRecv_TryGather.Interaction_ID);
		}
	}
}

void Server::RecvGatherCancel(std::stringstream & RecvStream, stOver * pOver)
{
	Packet_CS_CancelGather kRecv_CancelGather;

	// ����� ���� ����, ��ҵ� ä���� ID
	RecvStream >> kRecv_CancelGather;

	std::map<int, Level*>::iterator itFind = Level_Manager.find(User_Data[kRecv_CancelGather.SessionID].cPlayer.Level_ID);
	if (itFind != Level_Manager.end())
	{
		Level* pLevel = itFind->second;
		if (pLevel != nullptr)
		{
			Timer_Manager.StopGatherTimer(kRecv_CancelGather.SessionID);
			pLevel->StopGather(kRecv_CancelGather.Interaction_ID);

			Packet_SC_CancelGather kSend_CancelGather;
			kSend_CancelGather.SessionID = kRecv_CancelGather.SessionID;
			kSend_CancelGather.Interaction_ID = kRecv_CancelGather.Interaction_ID;

			std::stringstream SendStream;
			SendStream << EType::SC_GATHER_CANCEL_INFO << std::endl;
			SendStream << kSend_CancelGather << std::endl;

			pLevel->Zone_Broadcast(SendStream);
		}
	}
}

void Server::SendGatherSuccess(int nSessionID)
{
	int m_nSessionID = Timer_Manager.MGather[nSessionID]->SessionID;
	int m_nInteraction_ID = Timer_Manager.MGather[nSessionID]->Interaction_ID;

	int m_nLevel_ID = User_Data[nSessionID].cPlayer.Level_ID;

	Timer_Manager.MGather.erase(m_nSessionID);
	Timer_Manager.RespawnTimer(m_nLevel_ID, m_nInteraction_ID);

	std::stringstream SendStream;
	SendStream << EType::SC_GATHER_SUCCESS_INFO << std::endl;
	SendStream << m_nSessionID << std::endl;
	SendStream << m_nInteraction_ID << std::endl;


	map<int, Level*>::iterator itFind = Level_Manager.find(User_Data[m_nSessionID].cPlayer.Level_ID);
	if (itFind != Level_Manager.end())
	{
		Level* pLevel = itFind->second;
		if (pLevel != nullptr)
		{
			
			SendStream << pLevel->m_pDataLevel->Interactions[m_nInteraction_ID]->Type_ID << std::endl;

			bool m_bLVResult = pLevel->GatherSuccess(&User_Data[m_nSessionID], data.characters, m_nInteraction_ID);
			int m_nItemID = pLevel->m_pDataLevel->Interactions[m_nInteraction_ID]->Drop_ID;
			int m_nSlotNumber;
			int m_nItemCount;
			for (auto Inventory : User_Data[m_nSessionID].stInventory.Inventroy)
			{
				if (m_nItemID == Inventory.second.ItemID)
				{
					m_nSlotNumber = Inventory.second.SlotNumber;
					m_nItemCount = Inventory.second.ItemCount;
				}
			}

			DB_Manager.UpdateFatigue(m_nSessionID, User_Data[m_nSessionID].cPlayer.Fatigue);
			if (m_bLVResult)
			{
				DB_Manager.UpdateLV(m_nSessionID, User_Data[m_nSessionID].cPlayer.LV);
				UpdateLevel(m_nSessionID);
			}

			DB_Manager.UpdateExperience(m_nSessionID, User_Data[m_nSessionID].cPlayer.Experience);
			DB_Manager.UpdateInventory(m_nSessionID, m_nSlotNumber, m_nItemID, m_nItemCount);

			pLevel->Zone_Broadcast(SendStream);

			if (pLevel->m_pDataLevel->Interactions[m_nInteraction_ID]->Type_ID == EInteractionType::ANIMAL)
			{
				Timer_Manager.StopInteractionMoveTimer(m_nInteraction_ID);
			}
		}
	}

	bool m_bResult = true;

	if (User_Data[nSessionID].cPlayer.Fatigue < data.characters->Character_Data[User_Data[nSessionID].cPlayer.LV]->GetFatigue())
	{
		for (auto itUser : Timer_Manager.MFatigue)
		{
			if (itUser.second->SessionID == nSessionID)
				m_bResult = false;
		}
		if (m_bResult)
			Timer_Manager.FatigueTimer(nSessionID);
	}
}

void Server::GatherRespawn(int Thresh)
{
	int m_nLevel_ID = Timer_Manager.MRespawn.front()->m_nLevel_ID;
	int m_nInteraction_ID = Timer_Manager.MRespawn.front()->m_nInteraction_ID;

	Timer_Manager.MRespawn.pop();

	std::map<int, Level*>::iterator itFind = Level_Manager.find(m_nLevel_ID);
	if (itFind != Level_Manager.end())
	{
		Level* pLevel = itFind->second;
		if (pLevel != nullptr)
		{
			pLevel->InteractionRespawn(m_nInteraction_ID);
		}
	}
}

void Server::InteractionMove(int nTimerID)
{
	int m_nLevel_ID = Timer_Manager.MLevel[nTimerID]->Level_ID;
	int m_nInteraction_ID = Timer_Manager.MLevel[nTimerID]->Interaction_ID;

	if (Level_Manager[m_nLevel_ID]->InteractionMoveInfo(m_nInteraction_ID) == false)
	{
		// Ÿ�̸� ����
		Timer_Manager.StopInteractionMoveTimer(nTimerID);
	}
}

void Server::SetFatigue(int nSessionID)
{
	int m_nSeesionID = nSessionID - Timer_Manager.Fatigue_Set;

	int m_nMaxFatigue = data.characters->Character_Data[User_Data[m_nSeesionID].cPlayer.LV]->GetFatigue();
	User_Data[m_nSeesionID].cPlayer.Fatigue += 10;

	if (User_Data[m_nSeesionID].cPlayer.Fatigue >= m_nMaxFatigue)
	{
		User_Data[m_nSeesionID].cPlayer.Fatigue = m_nMaxFatigue;
		Timer_Manager.StopFatigueTimer(m_nSeesionID + Timer_Manager.Fatigue_Set);
	}

	DB_Manager.UpdateFatigue(m_nSeesionID, User_Data[m_nSeesionID].cPlayer.Fatigue);

	std::stringstream SendStream;
	SendStream << EType::SC_FATIGUE_UPDATE << std::endl;
	SendStream << m_nSeesionID << std::endl;
	SendStream << User_Data[m_nSeesionID].cPlayer.Fatigue << std::endl;

	stOver* m_pOver = new stOver;
	m_pOver->socket = User_Data[m_nSeesionID].sockUser;

	SendPacket(SendStream, m_pOver);
}

void Server::ItemCombination(std::stringstream & RecvStream, stOver * pOver)
{
	int m_nSessionID;
	int m_nItem_ID;
	bool m_bResult = true;
	int m_nCount = 0;
	int m_nHasItemSlot = -1;


	RecvStream >> m_nSessionID;
	RecvStream >> m_nItem_ID;

	Packet_Combination kCombination;

	DataCBFirst* CB1;
	DataCBSecond* CB2;

	if (data.CBFirsts->CBFirst_Data[m_nItem_ID] != nullptr)
	{
		CB1 = data.CBFirsts->CBFirst_Data[m_nItem_ID];

		for (auto& itCB : CB1->mat_Data)
		{
			for (auto& itInventory : User_Data[m_nSessionID].stInventory.Inventroy)
			{
				if (itCB.second.GetID() == itInventory.second.ItemID)
				{
					if (itCB.second.GetCount() > itInventory.second.ItemCount)
						m_bResult = false;
					else
					{
						kCombination.UpdateHasItems[m_nCount].SlotNumber = itInventory.first;
						kCombination.UpdateHasItems[m_nCount].ItemID = itInventory.second.ItemID;
						kCombination.UpdateHasItems[m_nCount].ItemCount = itInventory.second.ItemCount - itCB.second.GetCount();
					}
					m_nCount++;
				}

				if (itInventory.second.ItemID == m_nItem_ID)
				{
					m_nHasItemSlot = itInventory.second.SlotNumber;
				}
			}
		}
	}
	else if (data.CBSeconds->CBSecond_Data[m_nItem_ID] != nullptr)
	{
		CB2 = data.CBSeconds->CBSecond_Data[m_nItem_ID];

		for (auto& itCB : CB2->mat_Data)
		{
			for (auto& itInventory : User_Data[m_nSessionID].stInventory.Inventroy)
			{
				if (itCB.second.GetID() == itInventory.second.ItemID)
				{
					if (itCB.second.GetCount() > itInventory.second.ItemCount)
						m_bResult = false;
					else
					{
						kCombination.UpdateHasItems[m_nCount].SlotNumber = itInventory.first;
						kCombination.UpdateHasItems[m_nCount].ItemID = itInventory.second.ItemID;
						kCombination.UpdateHasItems[m_nCount].ItemCount = itInventory.second.ItemCount - itCB.second.GetCount();
					}
					m_nCount++;
				}

				if (itInventory.second.ItemID == m_nItem_ID)
				{
					m_nHasItemSlot = itInventory.second.SlotNumber;
				}

			}
		}
	}

	if (m_bResult)
	{
		if (m_nHasItemSlot != -1)
		{
			User_Data[m_nSessionID].stInventory.Inventroy[m_nHasItemSlot].ItemCount++;
			kCombination.SlotNum = m_nHasItemSlot;
			kCombination.ItemCount = User_Data[m_nSessionID].stInventory.Inventroy[m_nHasItemSlot].ItemCount;
		}
		else
		{
			//�� ���� ã�Ƽ� �ֱ�
			for (int i = 0; i < User_Data[m_nSessionID].stInventory.MaxSize; i++)
			{
				if (User_Data[m_nSessionID].stInventory.Inventroy[i].ItemID == 0)
				{
					User_Data[m_nSessionID].stInventory.Inventroy[i].ItemID = m_nItem_ID;
					User_Data[m_nSessionID].stInventory.Inventroy[i].ItemCount = 1;
					User_Data[m_nSessionID].stInventory.Inventroy[i].SlotNumber = i;
					kCombination.SlotNum = i;
					kCombination.ItemCount = 1;
					break;
				}
			}
		}

		// 0�̵� ������ ������ ����
		for (auto& itConsume : kCombination.UpdateHasItems)
		{
			User_Data[m_nSessionID].stInventory.Inventroy[itConsume.second.SlotNumber].ItemCount = itConsume.second.ItemCount;

			if (User_Data[m_nSessionID].stInventory.Inventroy[itConsume.second.SlotNumber].ItemCount == 0)
				User_Data[m_nSessionID].stInventory.Inventroy.erase(itConsume.second.SlotNumber);
		}

		// ���� ���� ����
		int m_nGetCount = 1;

		printf_s("[SessionID : %d] ���� %d ������ %d���� ȹ���Ͽ����ϴ�.\n", m_nSessionID, m_nItem_ID, m_nGetCount);
	}
	else
	{
		printf_s("[SessionID : %d] ���� ������ ���� ����\n", m_nSessionID);
	}

	kCombination.Result = m_bResult;
	kCombination.Item_ID = m_nItem_ID;

	std::stringstream SendStream;
	SendStream << EType::SC_COMBINATION_INFO << std::endl;
	SendStream << kCombination << std::endl;

	SendPacket(SendStream, pOver);
}

void Server::ItemUse(std::stringstream & RecvStream, stOver * pOver)
{
	Packet_CS_ItemUse kRecv_ItemUse;
	RecvStream >> kRecv_ItemUse;

	bool m_bResult = false;
	int m_nFatigue = 0;

	Use_ItemInfo* m_kItemInfo = new Use_ItemInfo();
	std::map<int, DataUseItem*>::iterator it = data.UseItems->UseItem_Data.find(kRecv_ItemUse.ItemID);
	if (it != data.UseItems->UseItem_Data.end())
	{
		// XML ������ ȸ���� ����
		m_nFatigue = it->second->GetFatigue();
	}

	if (User_Data[kRecv_ItemUse.SessionID].stInventory.Inventroy[kRecv_ItemUse.SlotNumber].ItemCount != 0)
	{
		if (User_Data[kRecv_ItemUse.SessionID].stInventory.Inventroy[kRecv_ItemUse.SlotNumber].ItemID == kRecv_ItemUse.ItemID)
		{
			m_bResult = true;

			User_Data[kRecv_ItemUse.SessionID].stInventory.Inventroy[kRecv_ItemUse.SlotNumber].ItemCount -= 1;
			User_Data[kRecv_ItemUse.SessionID].cPlayer.Fatigue += m_nFatigue;
			
			if (User_Data[kRecv_ItemUse.SessionID].cPlayer.Fatigue >= data.characters->Character_Data[User_Data[kRecv_ItemUse.SessionID].cPlayer.LV]->GetFatigue())
			{
				User_Data[kRecv_ItemUse.SessionID].cPlayer.Fatigue = data.characters->Character_Data[User_Data[kRecv_ItemUse.SessionID].cPlayer.LV]->GetFatigue();
				Timer_Manager.StopFatigueTimer(kRecv_ItemUse.SessionID + Timer_Manager.Fatigue_Set);
			}

			int m_nSlotNumber = kRecv_ItemUse.SlotNumber;
			int m_nItemID = User_Data[kRecv_ItemUse.SessionID].stInventory.Inventroy[m_nSlotNumber].ItemID;
			int m_nItemCount = User_Data[kRecv_ItemUse.SessionID].stInventory.Inventroy[m_nSlotNumber].ItemCount;

			DB_Manager.UpdateFatigue(kRecv_ItemUse.SessionID, User_Data[kRecv_ItemUse.SessionID].cPlayer.Fatigue);
			DB_Manager.UpdateInventory(kRecv_ItemUse.SessionID, m_nSlotNumber, m_nItemID, m_nItemCount);

			printf_s("[Item Use] [SessionID : %d] ���� ������ ���� : %d��\n", kRecv_ItemUse.SessionID, User_Data[kRecv_ItemUse.SessionID].stInventory.Inventroy[kRecv_ItemUse.SlotNumber].ItemCount);
			// �������� 0�� �� ��� �������ִ� �κ� �ʿ���
			if (User_Data[kRecv_ItemUse.SessionID].stInventory.Inventroy[kRecv_ItemUse.SlotNumber].ItemCount == 0)
				User_Data[kRecv_ItemUse.SessionID].stInventory.Inventroy.erase(kRecv_ItemUse.SlotNumber);
		}
	}

	Packet_SC_ItemUse kSend_ItemUse;
	kSend_ItemUse.Result = m_bResult;
	kSend_ItemUse.SessionID = kRecv_ItemUse.SessionID;
	kSend_ItemUse.ItemCount = User_Data[kRecv_ItemUse.SessionID].stInventory.Inventroy[kRecv_ItemUse.SlotNumber].ItemCount;
	kSend_ItemUse.SlotNumber = kRecv_ItemUse.SlotNumber;
	kSend_ItemUse.User_Fatigue = User_Data[kRecv_ItemUse.SessionID].cPlayer.Fatigue;

	std::stringstream SendStream;
	SendStream << EType::SC_ITEMUSE_INFO << std::endl;
	SendStream << kSend_ItemUse << std::endl;

	SendPacket(SendStream, pOver);
}

void Server::ThrowItem(std::stringstream & RecvStream, stOver * pOver)
{
	Packet_CS_ThrowItem kRecv_ThrowItem;
	RecvStream >> kRecv_ThrowItem;

	bool m_bResult = false;
	int m_nItemCount = 0;
	int m_nItemSlot = kRecv_ThrowItem.SlotNumber;
	int m_nItemType = kRecv_ThrowItem.ItemType;

	if (kRecv_ThrowItem.ItemType == ItemType::EQUIPEMENT)
	{
		// �׳� ����
		User_Data[kRecv_ThrowItem.SessionID].stInventory.Inventroy.erase(m_nItemSlot);
		m_bResult = true;
	}
	else
	{
		m_nItemCount = User_Data[kRecv_ThrowItem.SessionID].stInventory.Inventroy[m_nItemSlot].ItemCount - kRecv_ThrowItem.ItemCount;
		User_Data[kRecv_ThrowItem.SessionID].stInventory.Inventroy[m_nItemSlot].ItemCount = m_nItemCount;
		// 0���� ����
		if (m_nItemCount == 0)
			User_Data[kRecv_ThrowItem.SessionID].stInventory.Inventroy.erase(m_nItemSlot);

		m_bResult = true;
	}

	Packet_SC_ThrowITem kSend_ThrowITem;
	kSend_ThrowITem.Result = m_bResult;
	kSend_ThrowITem.ItemType = m_nItemType;
	kSend_ThrowITem.SlotNumber = m_nItemSlot;
	kSend_ThrowITem.ItemCount = m_nItemCount;

	printf_s("[Throw] [SessionID : %d] \n", kRecv_ThrowItem.SessionID);

	std::stringstream SendStream;
	SendStream << EType::SC_THROWITEM_INFO << std::endl;
	SendStream << kSend_ThrowITem << std::endl;

	SendPacket(SendStream, pOver);
}

void Server::MountEquipment(std::stringstream & RecvStream, stOver * pOver)
{
	std::map<int, ItemInfo>::iterator itFind;

	Packet_CS_MountEquipment kRecv_MountEquipment;
	RecvStream >> kRecv_MountEquipment;

	cout << kRecv_MountEquipment.Equipment_Type << endl;
	cout << kRecv_MountEquipment.SlotNumber << endl;

	bool m_bResult = false;
	int m_nInventoryMaxSize = User_Data[kRecv_MountEquipment.SessionID].stInventory.MaxSize;

	itFind = User_Data[kRecv_MountEquipment.SessionID].stEquipment.Equipment.find(kRecv_MountEquipment.Equipment_Type);

	if (itFind != User_Data[kRecv_MountEquipment.SessionID].stEquipment.Equipment.end())
	{
		// ������ �������� �ִ� ���
		ItemInfo kItemInfo;
		kItemInfo = itFind->second;

		if (kRecv_MountEquipment.Equipment_Type == 4)
		{
			int m_nID = User_Data[kRecv_MountEquipment.SessionID].stInventory.Inventroy[kRecv_MountEquipment.SlotNumber].ItemID;
			int m_nSize = data.Equipments->Equipment_Data[m_nID]->GetStat();

			m_nInventoryMaxSize = m_nInventoryMaxSize - User_Data[kRecv_MountEquipment.SessionID].stEquipment.Equipment[kRecv_MountEquipment.Equipment_Type].ItemStat + m_nSize;
			DB_Manager.UpdateInventorySize(kRecv_MountEquipment.SessionID, m_nInventoryMaxSize);
		}

		DB_Manager.UpdateInventory(kRecv_MountEquipment.SessionID, kRecv_MountEquipment.SlotNumber, User_Data[kRecv_MountEquipment.SessionID].stEquipment.Equipment[kRecv_MountEquipment.Equipment_Type].ItemID, true);
		DB_Manager.UpdateEquipment(kRecv_MountEquipment.SessionID, kRecv_MountEquipment.Equipment_Type, User_Data[kRecv_MountEquipment.SessionID].stInventory.Inventroy[kRecv_MountEquipment.SlotNumber].ItemID);

		User_Data[kRecv_MountEquipment.SessionID].stEquipment.Equipment[kRecv_MountEquipment.Equipment_Type] = User_Data[kRecv_MountEquipment.SessionID].stInventory.Inventroy[kRecv_MountEquipment.SlotNumber];
		User_Data[kRecv_MountEquipment.SessionID].stInventory.Inventroy[kRecv_MountEquipment.SlotNumber] = kItemInfo;

		m_bResult = true;
	}
	else
	{
		if (kRecv_MountEquipment.Equipment_Type == 4)
		{
			int m_nID = User_Data[kRecv_MountEquipment.SessionID].stInventory.Inventroy[kRecv_MountEquipment.SlotNumber].ItemID;
			int m_nSize = data.Equipments->Equipment_Data[m_nID]->GetStat();

			m_nInventoryMaxSize = m_nInventoryMaxSize + m_nSize;
			DB_Manager.UpdateInventorySize(kRecv_MountEquipment.SessionID, m_nInventoryMaxSize);
		}

		// ������ �������� ���� ���
		DB_Manager.UpdateInventory(kRecv_MountEquipment.SessionID, kRecv_MountEquipment.SlotNumber, User_Data[kRecv_MountEquipment.SessionID].stInventory.Inventroy[kRecv_MountEquipment.SlotNumber].ItemID, false);
		DB_Manager.UpdateEquipment(kRecv_MountEquipment.SessionID, kRecv_MountEquipment.Equipment_Type, User_Data[kRecv_MountEquipment.SessionID].stInventory.Inventroy[kRecv_MountEquipment.SlotNumber].ItemID);

		User_Data[kRecv_MountEquipment.SessionID].stEquipment.Equipment[kRecv_MountEquipment.Equipment_Type] = User_Data[kRecv_MountEquipment.SessionID].stInventory.Inventroy[kRecv_MountEquipment.SlotNumber];
		User_Data[kRecv_MountEquipment.SessionID].stInventory.Inventroy.erase(kRecv_MountEquipment.SlotNumber);
		m_bResult = true;
	}

	Packet_SC_MountEquipment kSend_MountEquipment;
	kSend_MountEquipment.Result = m_bResult;
	// ���� ������ �״�� ������
	kSend_MountEquipment.Equipment_SlotType = kRecv_MountEquipment.Equipment_Type;
	kSend_MountEquipment.Inventory_SlotNumber = kRecv_MountEquipment.SlotNumber;

	printf_s("[Mount] [SessionID : %d] ", kRecv_MountEquipment.SessionID);

	std::stringstream SendStream;
	SendStream << EType::SC_MOUNTEQUIPMENT_INFO << std::endl;
	SendStream << kSend_MountEquipment << std::endl;

	SendPacket(SendStream, pOver);
}

void Server::UnMountEquipment(std::stringstream & RecvStream, stOver * pOver)
{
	Packet_CS_UnMountEquipment kRecv_UnMountEquipment;
	RecvStream >> kRecv_UnMountEquipment;

	bool m_bResult = false;
	int m_nInventorySlot = -1;
	int m_nMaxSize = User_Data[kRecv_UnMountEquipment.SessionID].stInventory.MaxSize;

	int m_nID = User_Data[kRecv_UnMountEquipment.SessionID].stEquipment.Equipment[kRecv_UnMountEquipment.SlotNumber].ItemID;

	if (data.Equipments->Equipment_Data[m_nID]->GetEQType() == 4)
	{
		// ������ ���
		m_nMaxSize = User_Data[kRecv_UnMountEquipment.SessionID].stInventory.MaxSize - User_Data[kRecv_UnMountEquipment.SessionID].stEquipment.Equipment[kRecv_UnMountEquipment.SlotNumber].ItemStat;
		if (User_Data[kRecv_UnMountEquipment.SessionID].stInventory.Inventroy.size() < m_nMaxSize)
		{
			for (int i = 0; i < User_Data[kRecv_UnMountEquipment.SessionID].stInventory.MaxSize; i++)
			{
				if (User_Data[kRecv_UnMountEquipment.SessionID].stInventory.Inventroy[i].ItemID == 0)
				{
					User_Data[kRecv_UnMountEquipment.SessionID].stInventory.Inventroy[i] = User_Data[kRecv_UnMountEquipment.SessionID].stEquipment.Equipment[kRecv_UnMountEquipment.SlotNumber];

					DB_Manager.UpdateInventory(kRecv_UnMountEquipment.SessionID, i, User_Data[kRecv_UnMountEquipment.SessionID].stInventory.Inventroy[i].ItemID, true);
					DB_Manager.UpdateEquipment(kRecv_UnMountEquipment.SessionID, kRecv_UnMountEquipment.SlotNumber, -1);
					DB_Manager.UpdateInventorySize(kRecv_UnMountEquipment.SessionID, m_nMaxSize);
					User_Data[kRecv_UnMountEquipment.SessionID].stEquipment.Equipment.erase(kRecv_UnMountEquipment.SlotNumber);

					m_bResult = true;
					m_nInventorySlot = i;
					break;
				}
			}
		}
	}
	else
	{
		// �ƴ� ���
		if (User_Data[kRecv_UnMountEquipment.SessionID].stInventory.Inventroy.size() < User_Data[kRecv_UnMountEquipment.SessionID].stInventory.MaxSize)
		{
			for (int i = 0; i < User_Data[kRecv_UnMountEquipment.SessionID].stInventory.MaxSize; i++)
			{
				if (User_Data[kRecv_UnMountEquipment.SessionID].stInventory.Inventroy[i].ItemID == 0)
				{
					User_Data[kRecv_UnMountEquipment.SessionID].stInventory.Inventroy[i] = User_Data[kRecv_UnMountEquipment.SessionID].stEquipment.Equipment[kRecv_UnMountEquipment.SlotNumber];
					DB_Manager.UpdateInventory(kRecv_UnMountEquipment.SessionID, i, User_Data[kRecv_UnMountEquipment.SessionID].stInventory.Inventroy[i].ItemID, true);
					DB_Manager.UpdateEquipment(kRecv_UnMountEquipment.SessionID, kRecv_UnMountEquipment.SlotNumber, -1);
					User_Data[kRecv_UnMountEquipment.SessionID].stEquipment.Equipment.erase(kRecv_UnMountEquipment.SlotNumber);

					m_bResult = true;
					m_nInventorySlot = i;
					break;
				}
			}
		}
	}

	Packet_SC_UnMountEquipment kSend_UnMountEquipment;
	kSend_UnMountEquipment.Result = m_bResult;
	kSend_UnMountEquipment.Equipment_SlotType = kRecv_UnMountEquipment.SlotNumber;
	kSend_UnMountEquipment.Inventory_SlotNumber = m_nInventorySlot;

	printf_s("[UnMount] [SessionID : %d] ", kRecv_UnMountEquipment.SessionID);

	std::stringstream SendStream;
	SendStream << EType::SC_UNMOUNTEQUIPMENT_INFO << std::endl;
	SendStream << kSend_UnMountEquipment << std::endl;

	SendPacket(SendStream, pOver);
}

void Server::TryTrade(std::stringstream & RecvStream, stOver * pOver)
{
	Packet_Send_TryTrade kSend_TryTrade;
	RecvStream >> kSend_TryTrade;

	int m_nRecvSessionID = kSend_TryTrade.RecvSessionID;

	printf_s("[Trade Try]\n");
	// ���� �̹� �ŷ����� ��� ��ȯ�ȵȴٰ� Ŭ��� ������

	for (auto Trade : Trade_Manager)
	{
		for (auto Player : Trade.second->Players)
		{
			if (Player.second.SessionID == kSend_TryTrade.RecvSessionID)
			{
				// ��ȯ�� ����
				std::stringstream SendStream;
				SendStream << EType::SC_TRYTRADE_FAIL << std::endl;
				SendPacket(SendStream, pOver);
				return;
			}
		}
	}

	std::stringstream SendStream;
	SendStream << EType::SC_TRYTRADE_INFO << std::endl;
	SendStream << kSend_TryTrade << std::endl;

	stOver* m_pOver = new stOver;
	m_pOver->socket = User_Data[m_nRecvSessionID].sockUser;

	SendPacket(SendStream, m_pOver);
}

void Server::TradeMatching(std::stringstream & RecvStream, stOver * pOver)
{
	Packet_Recv_TryTrade kRecv_TryTrade;
	RecvStream >> kRecv_TryTrade;

	bool m_bResult = false;

	printf_s("[Trade Try Result ] ");
	if (kRecv_TryTrade.Result == true)
	{
		Trade *pTrade = new Trade();
		pTrade->Matching(kRecv_TryTrade.SendSessionID, kRecv_TryTrade.RecvSessionID);

		Trade_Manager[kRecv_TryTrade.SendSessionID] = pTrade;
		m_bResult = true;
		printf_s("[SessionID : %d]������ [SessionID : %d] ������ �ŷ���û�� �����߽��ϴ�.\n\n", kRecv_TryTrade.RecvSessionID, kRecv_TryTrade.SendSessionID);
	}
	else
	{
		printf_s("[SessionID : %d] ������ [SessionID : %d] ������ �ŷ���û�� �����߽��ϴ�.\n\n", kRecv_TryTrade.RecvSessionID, kRecv_TryTrade.SendSessionID);
	}

	Packet_SC_TradeMaching kSend_TradeMaching;
	kSend_TradeMaching.Result = m_bResult;
	kSend_TradeMaching.TradeID = kRecv_TryTrade.SendSessionID;

	stringstream SendStream;
	SendStream << EType::SC_TRADEMACTCHING_INFO << endl;
	SendStream << kSend_TradeMaching << endl;



	if (m_bResult)
	{
		SendPacket(SendStream, pOver);
	}

	stOver* m_pOver = new stOver;
	m_pOver->socket = User_Data[kRecv_TryTrade.SendSessionID].sockUser;
	SendPacket(SendStream, m_pOver);
}

void Server::TradePutOn(std::stringstream & RecvStream, stOver * pOver)
{
	Packet_SC_PutOnItem kRecv_PutOnItem;
	RecvStream >> kRecv_PutOnItem;

	Trade_Manager[kRecv_PutOnItem.TradeID]->PutOnItem(kRecv_PutOnItem.SessionID, kRecv_PutOnItem.itemID, kRecv_PutOnItem.InventorySlot, kRecv_PutOnItem.ItemType, kRecv_PutOnItem.ItemCount);

	kRecv_PutOnItem.TradSlot = Trade_Manager[kRecv_PutOnItem.TradeID]->GetRecentSlotNumber(kRecv_PutOnItem.SessionID);

	printf_s("[������ ���] ");
	if (kRecv_PutOnItem.ItemType == 2)
	{
		printf_s("[SessionID : %d]������ %d �������� ����Ͽ����ϴ�.\n", kRecv_PutOnItem.SessionID, kRecv_PutOnItem.itemID);
	}
	else
	{
		printf_s("[SessionID : %d]������ %d �������� %d�� ����Ͽ����ϴ�.\n", kRecv_PutOnItem.SessionID, kRecv_PutOnItem.itemID, kRecv_PutOnItem.ItemCount);
	}


	std::stringstream SendStream;
	SendStream << EType::SC_TRADEPUTONITEM_INFO << std::endl;
	SendStream << kRecv_PutOnItem << std::endl;

	int m_nOtherID = Trade_Manager[kRecv_PutOnItem.TradeID]->GetOtherSessionID(kRecv_PutOnItem.SessionID);
	stOver* m_pOver = new stOver;
	m_pOver->socket = User_Data[m_nOtherID].sockUser;

	SendPacket(SendStream, pOver);
	SendPacket(SendStream, m_pOver);
}

void Server::TradeAccept(std::stringstream & RecvStream, stOver * pOver)
{
	Packet_CS_TradeAccept kRecv_TradeAccept;
	RecvStream >> kRecv_TradeAccept;

	printf_s("[TradeAccept] [SessionID : %d]������ ��ȯ ��ư�� �������ϴ�.\n", kRecv_TradeAccept.SessionID);

	bool m_bResult;
	m_bResult = Trade_Manager[kRecv_TradeAccept.TradeID]->Accept(kRecv_TradeAccept.SessionID);

	if (m_bResult)
	{
		// �Ѵ� �������� ���
		Tradecompletion(kRecv_TradeAccept.TradeID, kRecv_TradeAccept.SessionID);
		return;
	}

	std::stringstream SendStream;
	SendStream << EType::SC_TRADEACCEPT_INFO << std::endl;
	SendStream << kRecv_TradeAccept << std::endl;

	int m_nOtherID = Trade_Manager[kRecv_TradeAccept.TradeID]->GetOtherSessionID(kRecv_TradeAccept.SessionID);
	stOver* m_pOver = new stOver;
	m_pOver->socket = User_Data[m_nOtherID].sockUser;

	SendPacket(SendStream, pOver);
	SendPacket(SendStream, m_pOver);
}

void Server::TradeCancel(std::stringstream & RecvStream, stOver * pOver)
{
	Packet_CS_TradeCancel kRecv_TradeCancel;
	RecvStream >> kRecv_TradeCancel;

	printf_s("[TradeCancel] [SessionID : %d] ������ ��ȯ�� ����Ͽ����ϴ�.\n", kRecv_TradeCancel.SessionID);

	int m_nTradeID = kRecv_TradeCancel.TradeID;
	int m_nMySessionID = kRecv_TradeCancel.SessionID;
	int m_nOtherSessionID = Trade_Manager[m_nTradeID]->GetOtherSessionID(m_nMySessionID);

	// ���� ������ ����
	Packet_SC_TradeCancel kSend_MyTradeItem;
	kSend_MyTradeItem.SessionID = m_nMySessionID;
	Trade_Manager[m_nTradeID]->cancel(&User_Data[m_nMySessionID], kSend_MyTradeItem);

	// ��� ������ ����
	Packet_SC_TradeCancel kSend_OtherTradeItem;
	kSend_OtherTradeItem.SessionID = m_nOtherSessionID;
	Trade_Manager[m_nTradeID]->cancel(&User_Data[m_nOtherSessionID], kSend_OtherTradeItem);

	std::stringstream SendStreamPlayer1;
	SendStreamPlayer1 << EType::SC_TRADECANCEL_INFO << std::endl;
	SendStreamPlayer1 << kSend_MyTradeItem << std::endl;

	std::stringstream SendStreamPlayer2;
	SendStreamPlayer2 << EType::SC_TRADECANCEL_INFO << std::endl;
	SendStreamPlayer2 << kSend_OtherTradeItem << std::endl;

	stOver* m_pOver = new stOver;
	m_pOver->socket = User_Data[m_nOtherSessionID].sockUser;

	SendPacket(SendStreamPlayer1, pOver);
	SendPacket(SendStreamPlayer2, m_pOver);

	Trade_Manager[m_nTradeID]->Clear();
	Trade_Manager.erase(m_nTradeID);
}

void Server::Tradecompletion(int nTradeID, int nSessionID)
{
	int m_nOtherID = Trade_Manager[nTradeID]->GetOtherSessionID(nSessionID);

	printf_s("[TradeCompletion] [SessionID : %d]�� [SessinID : %d]���� ��ȯ�� ���������� ó���Ǿ����ϴ�.\n", nSessionID, m_nOtherID);

	for (auto ItemInfo : Trade_Manager[nTradeID]->Players[nSessionID].OnPutItem)
	{
		int m_nItemID = ItemInfo.first;

		for (int i = 0; i < User_Data[nSessionID].stInventory.MaxSize; i++)
		{
			if (User_Data[nSessionID].stInventory.Inventroy[i].ItemID == m_nItemID)
			{
				User_Data[nSessionID].stInventory.Inventroy[i].ItemCount -= ItemInfo.second->ItemCount;

				DB_Manager.UpdateInventory(nSessionID, i, m_nItemID, User_Data[nSessionID].stInventory.Inventroy[i].ItemCount);

				if (User_Data[nSessionID].stInventory.Inventroy[i].ItemCount == 0)
					User_Data[nSessionID].stInventory.Inventroy.erase(i);
				break;
			}
		}
	}

	for (auto ItemInfo : Trade_Manager[nTradeID]->Players[m_nOtherID].OnPutItem)
	{
		int m_nItemID = ItemInfo.first;

		for (int i = 0; i < User_Data[m_nOtherID].stInventory.MaxSize; i++)
		{
			if (User_Data[m_nOtherID].stInventory.Inventroy[i].ItemID == m_nItemID)
			{
				User_Data[m_nOtherID].stInventory.Inventroy[i].ItemCount -= ItemInfo.second->ItemCount;

				DB_Manager.UpdateInventory(m_nOtherID, i, m_nItemID, User_Data[m_nOtherID].stInventory.Inventroy[i].ItemCount);

				if (User_Data[m_nOtherID].stInventory.Inventroy[i].ItemCount == 0)
					User_Data[m_nOtherID].stInventory.Inventroy.erase(i);
				break;
			}
		}
	}

	Packet_SC_TradeCompletion kSend_TradeCompletion_Player1;
	Trade_Manager[nTradeID]->Success(&User_Data[nSessionID], m_nOtherID, kSend_TradeCompletion_Player1);

	for (auto ItemInfo : kSend_TradeCompletion_Player1.InventoryInfo)
	{
		if (ItemInfo.second.ItemType == 2)
		{
			// ��� ������
			DB_Manager.UpdateInventory(kSend_TradeCompletion_Player1.SessionID, ItemInfo.second.SlotNumber, ItemInfo.second.ItemID, true);
		}
		else
		{
			// �Ϲ� ������
			DB_Manager.UpdateInventory(kSend_TradeCompletion_Player1.SessionID, ItemInfo.second.SlotNumber, ItemInfo.second.ItemID, ItemInfo.second.ItemCount);
		}
	}

	Packet_SC_TradeCompletion kSend_TradeCompletion_Player2;
	Trade_Manager[nTradeID]->Success(&User_Data[m_nOtherID], nSessionID, kSend_TradeCompletion_Player2);

	for (auto ItemInfo : kSend_TradeCompletion_Player2.InventoryInfo)
	{
		if (ItemInfo.second.ItemType == 2)
		{
			// ��� ������
			DB_Manager.UpdateInventory(kSend_TradeCompletion_Player2.SessionID, ItemInfo.second.SlotNumber, ItemInfo.second.ItemID, true);
		}
		else
		{
			// �Ϲ� ������
			DB_Manager.UpdateInventory(kSend_TradeCompletion_Player2.SessionID, ItemInfo.second.SlotNumber, ItemInfo.second.ItemID, ItemInfo.second.ItemCount);
		}
	}

	std::stringstream SendStreamPlayer1;
	SendStreamPlayer1 << EType::SC_COMPLETION_INFO << std::endl;
	SendStreamPlayer1 << kSend_TradeCompletion_Player1 << std::endl;

	std::stringstream SendStreamPlayer2;
	SendStreamPlayer2 << EType::SC_COMPLETION_INFO << std::endl;
	SendStreamPlayer2 << kSend_TradeCompletion_Player2 << std::endl;

	stOver* m_pAOver = new stOver;
	m_pAOver->socket = User_Data[nSessionID].sockUser;

	stOver* m_pBOver = new stOver;
	m_pBOver->socket = User_Data[m_nOtherID].sockUser;

	SendPacket(SendStreamPlayer1, m_pAOver);
	SendPacket(SendStreamPlayer2, m_pBOver);

	Trade_Manager[nTradeID]->Clear();
	Trade_Manager.erase(nTradeID);
}

void Server::ShopBuyItem(std::stringstream & RecvStream, stOver * pOver)
{
	Packet_CS_BuyShopItem kCS_BuyShopItem;
	int m_nSessionID;
	int m_nInventoryKey;
	bool m_bResult = false;

	RecvStream >> kCS_BuyShopItem;
	m_nSessionID = kCS_BuyShopItem.SessionID;

	Packet_SC_BuyShopItem kSC_BuyShopItem;

	DataShop* pItemInfo = new DataShop();

	switch (User_Data[m_nSessionID].cPlayer.OccupationID)
	{
	case EOccupation::GATHERER:
		pItemInfo = data.Shops->GathererShop_Data[kCS_BuyShopItem.BuyItemID];
		break;
	case EOccupation::MINOR:
		pItemInfo = data.Shops->MinorShop_Data[kCS_BuyShopItem.BuyItemID];
		break;
	case EOccupation::HUNTER:
		pItemInfo = data.Shops->HunterShop_Data[kCS_BuyShopItem.BuyItemID];
		break;
	}

	if (User_Data[m_nSessionID].stInventory.Inventroy.size() <= User_Data[m_nSessionID].stInventory.MaxSize)
	{
		for (auto& itInventory : User_Data[m_nSessionID].stInventory.Inventroy)
		{
			if (itInventory.second.ItemID == pItemInfo->GetNeedItemID())
			{
				if (itInventory.second.ItemCount >= (pItemInfo->GetNeedItemCount()) * kCS_BuyShopItem.BuyItemCount)
				{
					m_nInventoryKey = itInventory.first;
					m_bResult = true;
					break;
				}
			}
		}
	}

	kSC_BuyShopItem.Result = m_bResult;

	if (m_bResult)
	{
		User_Data[m_nSessionID].stInventory.Inventroy[m_nInventoryKey].ItemCount -= (pItemInfo->GetNeedItemCount()) * kCS_BuyShopItem.BuyItemCount;

		int m_nSlotNumber = m_nInventoryKey;
		int m_nItemID = pItemInfo->GetNeedItemID();
		int m_nItemCount = User_Data[m_nSessionID].stInventory.Inventroy[m_nInventoryKey].ItemCount;

		kSC_BuyShopItem.ConsumeItemSlot = m_nSlotNumber;
		kSC_BuyShopItem.ConsumeItemID = m_nItemID;
		kSC_BuyShopItem.ConsumeItemCount = m_nItemCount;


		DB_Manager.UpdateInventory(m_nSessionID, m_nSlotNumber, m_nItemID, m_nItemCount);

		if (User_Data[m_nSessionID].stInventory.Inventroy[m_nInventoryKey].ItemCount == 0)
			User_Data[m_nSessionID].stInventory.Inventroy.erase(m_nInventoryKey);

		int GetItemSlot = -1;

		for (auto& itInventory : User_Data[m_nSessionID].stInventory.Inventroy)
		{
			if (itInventory.second.ItemID == kCS_BuyShopItem.BuyItemID)
			{
				GetItemSlot = itInventory.first;
				break;
			}
		}

		if (GetItemSlot == -1)
		{
			for (auto& itInventory : User_Data[m_nSessionID].stInventory.Inventroy)
			{
				if (itInventory.second.ItemID == -1)
				{
					GetItemSlot = itInventory.first;
					itInventory.second.ItemID = kCS_BuyShopItem.BuyItemID;
					itInventory.second.ItemType = ItemType::ETC;
					itInventory.second.SlotNumber = GetItemSlot;
					itInventory.second.ItemCount = kCS_BuyShopItem.BuyItemCount;
					break;
				}
			}
		}
		else
		{
			User_Data[m_nSessionID].stInventory.Inventroy[GetItemSlot].ItemCount += kCS_BuyShopItem.BuyItemCount;
		}

		if (GetItemSlot != -1)
		{
			int m_nSlotNumber = GetItemSlot;
			int m_nItemID = kCS_BuyShopItem.BuyItemID;
			int m_nItemCount = User_Data[m_nSessionID].stInventory.Inventroy[GetItemSlot].ItemCount;

			kSC_BuyShopItem.GetItemSlot = m_nSlotNumber;
			kSC_BuyShopItem.GetItemID = m_nItemID;
			kSC_BuyShopItem.GetItemCount = m_nItemCount;

			DB_Manager.UpdateInventory(m_nSessionID, m_nSlotNumber, m_nItemID, m_nItemCount);

			printf_s("[Shop] [SessionID : %d] %d ������ %d�� ���� ����\n", kCS_BuyShopItem.SessionID, kSC_BuyShopItem.GetItemID, kCS_BuyShopItem.BuyItemCount);
		}
	}
	else
	{
		printf_s("[Shop] [����][SessionID : %d] %d ������ ���� ���� \n", m_nSessionID, kCS_BuyShopItem.BuyItemID);
	}

	std::stringstream SendStream;
	SendStream << EType::SC_SHOP_BUY_ITEM << std::endl;
	SendStream << kSC_BuyShopItem << std::endl;

	SendPacket(SendStream, pOver);
}
