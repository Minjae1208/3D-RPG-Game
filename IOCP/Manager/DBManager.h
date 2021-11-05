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

	// DB����
	void Close();

	bool Login(const std::string& ID, const std::string& Password);													// �α���
	bool SignUp(const std::string& ID, const std::string& Password, const std::string& Name);												// ȸ�� ����
	int GetMasterSessionID();																						// Master SessionID ��������
	bool UpdateMasterSessionID(const int SessionID);																// Master SessionID ������Ʈ
	bool MakeSessionID(const std::string& ID);																		// ���ο� ������ SessionID ����
	bool InsertInfo(const std::string& ID, const int nOccupation);													// ���ο� ������ �⺻ ���� ����
	
	int GetSessinID(const std::string& ID);																			// SessionID ��������
	std::string GetCName(const int nSessionID);																		// ĳ���� �г��� ��������
	int GetLevel(const int nSessionID);																				// Level(Map)_ID ��������
	int GetOccupation(const int nSessionID);																		// ���� ���� ��������
	int GetLV(const int nSessionID);																				// Character ���� ��������
	int GetExperience(const int nSessionID);																		// ����ġ ��������
	int GetFatigue(const int nSessionID);																			// �Ƿε� ��������
	int GetUserInventory(const int nSessionID, const int nSlotCount);												// ���� �κ��丮 ��������
	int GetInventorySize(const int nSessionID);																		// 
	int GetUserEquipment(const int nSessionID, const int nSlotCount);												// ���� ��� ��������
	int GetDate(const int nSessionID);

	bool UpdateLevel(const int nSessionID, const int nLevel);														// Level(Map)_ID ������Ʈ
	bool UpdateLV(const int nSessionID, const int nLV);																// Character ���� ������Ʈ
	bool UpdateExperience(const int nSessionID, const int nExperience);												// ����ġ ������Ʈ
	bool UpdateFatigue(const int nSessionID, const int nFatigue);													// �Ƿε� ������Ʈ
	bool UpdateInventory(const int nSessionID, const int nSlotNumber, const int nItemID, bool Type);				// �κ��丮 ������Ʈ (���)
	bool UpdateInventory(const int nSessionID, const int nSlotNumber, const int nItemID, const int nItemCount);		// �κ��丮 ������Ʈ (��Ÿ/���)
	bool UpdateEquipment(const int nSessionID, const int nSlotNumber, const int nItemID);							// ���� ������ ������Ʈ
	bool UpdateInventorySize(const int nSessionID, const int nMaxSize);												// �κ��丮 ������ ������Ʈ
	bool UpdateDate(const int nSessionID);																			// ������ ���� ������Ʈ

private:
	MYSQL  *Con;
	MYSQL_RES* Res;
	MYSQL_ROW Row;

	time_t Time;
	tm stTime;
};