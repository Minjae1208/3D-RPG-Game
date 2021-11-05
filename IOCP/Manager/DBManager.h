#pragma once

#pragma comment (lib, "libmysql.lib")
#pragma warning(disable: 4996)

#include <mysql.h>
#include <string>
#include <ctime>


#define DB_CHECK TRUE

#define DB_ADDRESS "localhost"
#define DB_PORT 3306
#define DB_ID "root"
#define DB_PW "chaminjae123@"
#define DB_SCHEMA "3Dgameuser"

class DB
{
public:
	DB();
	~DB();

	int SQL_CName;
	int SQL_SessionID;
	int SQL_Level;
	int SQL_Occupation;
	int SQL_SlotCount;
	int SQL_Experence;
	int SQL_Fatigue;
	int SQL_LV;
	int SQL_Date;

	bool Connect(const std::string& Server, const std::string& User, const std::string& Password, const std::string& Database, const int& Port);

	// DB종료
	void Close();

	bool Login(const std::string& ID, const std::string& Password);													// 로그인
	bool SignUp(const std::string& ID, const std::string& Password, const std::string& Name);												// 회원 가입
	int GetMasterSessionID();																						// Master SessionID 가져오기
	bool UpdateMasterSessionID(const int SessionID);																// Master SessionID 업데이트
	bool MakeSessionID(const std::string& ID);																		// 새로운 유저의 SessionID 생성
	bool InsertInfo(const std::string& ID, const int nOccupation);													// 새로운 유저의 기본 정보 생성
	
	int GetSessinID(const std::string& ID);																			// SessionID 가져오기
	std::string GetCName(const int nSessionID);																		// 캐릭터 닉네임 가져오기
	int GetLevel(const int nSessionID);																				// Level(Map)_ID 가져오기
	int GetOccupation(const int nSessionID);																		// 직업 정보 가져오기
	int GetLV(const int nSessionID);																				// Character 레벨 가져오기
	int GetExperience(const int nSessionID);																		// 경험치 가져오기
	int GetFatigue(const int nSessionID);																			// 피로도 가져오기
	int GetUserInventory(const int nSessionID, const int nSlotCount);												// 유저 인벤토리 가져오기
	int GetInventorySize(const int nSessionID);																		// 
	int GetUserEquipment(const int nSessionID, const int nSlotCount);												// 유저 장비 가져오기
	int GetDate(const int nSessionID);

	bool UpdateLevel(const int nSessionID, const int nLevel);														// Level(Map)_ID 업데이트
	bool UpdateLV(const int nSessionID, const int nLV);																// Character 레벨 업데이트
	bool UpdateExperience(const int nSessionID, const int nExperience);												// 경험치 업데이트
	bool UpdateFatigue(const int nSessionID, const int nFatigue);													// 피로도 업데이트
	bool UpdateInventory(const int nSessionID, const int nSlotNumber, const int nItemID, bool Type);				// 인벤토리 업데이트 (장비)
	bool UpdateInventory(const int nSessionID, const int nSlotNumber, const int nItemID, const int nItemCount);		// 인벤토리 업데이트 (기타/사용)
	bool UpdateEquipment(const int nSessionID, const int nSlotNumber, const int nItemID);							// 착용 아이템 업데이트
	bool UpdateInventorySize(const int nSessionID, const int nMaxSize);												// 인벤토리 사이즈 업데이트
	bool UpdateDate(const int nSessionID);																			// 마지막 접속 업데이트

private:
	MYSQL  *Con;
	MYSQL_RES* Res;
	MYSQL_ROW Row;

	time_t Time;
	tm stTime;
};