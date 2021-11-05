#pragma once

#define _WINSOCK_DEPREACTED_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")

#include <map>
#include <iostream>
#include <WinSock2.h>

// Data 헤더 파일
#include "../Data/Data.h"

// Manager 헤더 파일
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
	
	// 서버 동작 함수
	bool Init();															// 초기화
	void Start();															// 시작
	bool CreateThread();													// 스레드 생성
	void WorkerThread();													// 스레드 작업
	static void Send(stOver* pSocket);										// 패킷 전송
	static void SendPacket(std::stringstream& SendStream, stOver* pOver);	// 패킷 전송 전 작업
	static void Recv(stOver* pSocket);										// 수신 작업

private:
	stOver * Over;										// 소켓정보
	SOCKET sock_listen;									// 서버 리슨 소켓
	HANDLE handle_IOCP;									// IOCP 핸들
	bool bAccept;										// 요청 동작 플래그
	bool bWorkerThread;									// 작업 스레드 동작 플래그
	HANDLE* handle_Thread;								// 작업 스레드 핸들


	static CRITICAL_SECTION	cs_Section;					// CharactersInfo 임계영역
	
	static Data data;									// XML Data
	static std::map<int, struct User> User_Data;		// Players 데이터
	static std::map<int, std::string> UserName;			// ?
	
	static DB DB_Manager;								// DB Manager
	static TimeManager Timer_Manager;					// Timer Manager
	static std::map<int, Trade*> Trade_Manager;			// Trade Manager
	static std::map<int, Level*> Level_Manager;			// Level Manager
	std::map<int, struct DataLevel*> Level_Data;		// Level Data Class

	stProcess			stProc[100];					// 패킷 처리



// 서버 패킷 처리 함수
private:  
	// XML Data
	static void XMLData(stringstream & RecvStream, stOver * pOver);					// XML 데이터 요청
	static void XML_Drop(stOver * pOver);											// 채집 드랍 아이템 데이터
	static void XML_CB_First(stOver * pOver);										// 조합 1 데이터
	static void XML_CB_Second(stOver * pOver);										// 조합 2 데이터
	static void XML_EQItem(stOver * pOver);											// 장비 아이템 데이터
	static void XML_Level(stOver * pOver);											// Level 데이터
	static void XML_Shop(int nOccupationID, stOver * pOver);						// 상점 아이템 데이터

	// 회원가입, 로그인, 로그아웃
	static void SignUp(std::stringstream & RecvStream, stOver * pOver);				// 회원 가입
	static void Login(std::stringstream & RecvStream, stOver * pOver);				// 로그인
	static void Login_Success(std::stringstream & RecvStream, stOver * pOver);		// 로그인 성공시 접속 전 데이터 전송
	static void LogoutPlayer(std::stringstream & RecvStream, stOver * pOver);		// 로그아웃
	static void LogOutUser(int nSessionID);											// 관련 타이머 및 DB 업데이트

	// 유저 동기화 함수
	static void EnterPlayer(std::stringstream & RecvStream, stOver * pOver);		// 유저 입장
	static void SyncPlayers(std::stringstream & RecvStream, stOver * pOver);		// 동기화
	static void BroadcastNewPlayer(Player & player, Level * pLevel);				// ?

	// Level(Map) 관리 함수
	static void ChangeLevel(std::stringstream & RecvStream, stOver * pOver);		// 맵 이동
	static void SendLevelName(stOver * pOver, int nSessionID, int nAfterID);		// 맵 이름 전송
	static void UpdateLevel(int nSessionID);										// Level Update

	// 채팅 관련 함수
	static void World_Chat(stringstream & RecvStream, stOver * pSocket);			// 월드 채팅 (전체)
	static void Level_Chat(stringstream & RecvStream, stOver * pSocket);			// Level 채팅 (Map)
	static void Whisper_Chat(stringstream & RecvStream, stOver * pSocket);			// 귓속말

	// 채집 관련 함수
	static void RecvGatherStart(std::stringstream & RecvStream, stOver * pOver);	// 시작
	static void RecvGatherCancel(std::stringstream & RecvStream, stOver * pOver);	// 취소
	static void SendGatherSuccess(int nSessionID);									// 성공

	// Timer 관련 함수
	static void GatherRespawn(int Thresh);											// 리스폰
	static void InteractionMove(int nTimerID);										// 동물 이동
	static void SetFatigue(int nSessionID);											// 피로도 회복

	// 아이템 관련 함수
	static void ItemCombination(std::stringstream & RecvStream, stOver * pOver);	// 아이템 조합
	static void ItemUse(std::stringstream & RecvStream, stOver * pOver);			// 아이템 사용
	static void ThrowItem(std::stringstream & RecvStream, stOver * pOver);			// 아이템 버리기
	static void MountEquipment(std::stringstream & RecvStream, stOver * pOver);		// 장비 착용
	static void UnMountEquipment(std::stringstream & RecvStream, stOver * pOver);	// 장비 해제

	// 교환 관련 함수
	static void TryTrade(std::stringstream & RecvStream, stOver * pOver);			// 교환 요청
	static void TradeMatching(std::stringstream & RecvStream, stOver * pOver);		// 교환 매칭
	static void TradePutOn(std::stringstream & RecvStream, stOver * pOver);			// 아이템 올리기
	static void TradeAccept(std::stringstream & RecvStream, stOver * pOver);		// 교환 버튼 클릭
	static void TradeCancel(std::stringstream & RecvStream, stOver * pOver);		// 교환 취소
	static void Tradecompletion(int nTradeID, int nSessionID);						// 교환 완료

	// Shop function
	static void ShopBuyItem(std::stringstream & RecvStream, stOver * pOver);		// 상점 아이템 구매

};