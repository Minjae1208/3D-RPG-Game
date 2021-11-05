#include "DBManager.h"


DB::DB()
{
	SQL_CName = 2;
	SQL_SessionID = 3;
	SQL_Level = 4;
	SQL_Occupation = 5;
	SQL_SlotCount = 6;
	SQL_Experence = 7;
	SQL_Fatigue = 8;
	SQL_LV = 9;
	SQL_Date = 10;
}

DB::~DB()
{
}

bool DB::Connect(const std::string& Server, const std::string& User, const std::string& Password, const std::string& Database, const int& Port)
{
	Con = mysql_init(NULL);
	if (!mysql_real_connect(Con, Server.c_str(), User.c_str(), Password.c_str(), Database.c_str(), Port, NULL, 0))
	{
		printf_s("[DB] DB���� ����\n");
		return false;
	}

	return true;
}

void DB::Close()
{
	mysql_close(Con);
}

bool DB::Login(const std::string& ID, const std::string& Password)
{
	bool m_bResult = false;
	// ������ ���̽� UserAccount���̺��� colunm �˻�
	std::string sql = "SELECT * FROM UserAccount WHERE ID = '";
	sql += ID + "'and PW = '" + Password + "'";
	if (mysql_query(Con, sql.c_str()))
	{
		printf_s("[DB] �˻� ����\n");
		return false;
	}

	Res = mysql_use_result(Con);
	Row = mysql_fetch_row(Res);

	if (Row != NULL)
		m_bResult = true;
	else
	{
		printf_s("[ERROR] �ش� ���̵� ����\n");
		m_bResult = false;
	}
	mysql_free_result(Res);
	return m_bResult;
}

bool DB::SignUp(const std::string& ID, const std::string& Password, const std::string& Name)
{
	bool m_bSignUp = true;

	// ������ ���̽� UserAccount ���̺� ������ ����
	// ���� �ʿ�
	std::string m_strNameSearch = "SELECT * FROM UserAccount CName='";
	m_strNameSearch += Name + "'";
	if (!mysql_query(Con, m_strNameSearch.c_str()))
	{
		printf_s("[DB] �ߺ��� �г���\n");
		return false;
	}

	std::string m_strDBInsert = "INSERT INTO UserAccount (ID, PW, CName) VALUES";
	m_strDBInsert += "('" + ID + "','" + Password + "','" + Name + "')";
	if (mysql_query(Con, m_strDBInsert.c_str()))
	{
		printf_s("[DB] �ߺ��� ����\n");
		return false;
	}

	return m_bSignUp;
}

int DB::GetMasterSessionID()
{
	int m_nSessionID;
	std::string m_strMaster = "SELECT * FROM UserAccount WHERE ID='Master'";

	if (mysql_query(Con, m_strMaster.c_str()))
	{
		return 0;
	}
	Res = mysql_store_result(Con);
	Row = mysql_fetch_row(Res);
	mysql_free_result(Res);

	m_nSessionID = std::stoi(Row[SQL_SessionID]);
	return m_nSessionID;
}

bool DB::UpdateMasterSessionID(const int SessionID)
{
	bool m_bCheck = true;

	std::string m_strMaster = "update useraccount set SessionID = ";
	m_strMaster += std::to_string(SessionID + 1) + " where ID = \'Master\'";
	if (mysql_query(Con, m_strMaster.c_str()))
	{
		printf_s("Master SessionID Update ����\n");
		return false;
	}
	return m_bCheck;
}

bool DB::MakeSessionID(const std::string& ID)
{
	bool m_bMake = true;

	int m_nSessionID = GetMasterSessionID();
	m_bMake = UpdateMasterSessionID(m_nSessionID);

	std::string m_strSessionIDUpdate = "UPDATE UserAccount set SessionID = '";
	m_strSessionIDUpdate += std::to_string(m_nSessionID) + "' WHERE ID = \'" + ID + "\'";
	if (mysql_query(Con, m_strSessionIDUpdate.c_str()))
	{
		printf_s("[DB] SessionID ���� ����\n");
		return false;
	}

	return m_bMake;
}

bool DB::InsertInfo(const std::string& ID, const int nOccupation)
{
	// ������ �÷��� �ð� �߰� �ʿ���
	std::string m_strInfo = "UPDATE UserAccount set Level_ID=1, Occupation=";
	m_strInfo += std::to_string(nOccupation) + ", SlotCount=20, Experience=0, Fatigue=200, LV=1 where ID=\'" + ID + "\'";

	if (mysql_query(Con, m_strInfo.c_str()))
	{
		printf_s("[DB] ���� �ʱ�ȭ ����\n");
		return false;
	}

	int m_nSessionID = GetSessinID(ID);

	std::string m_strEquipment = "INSERT INTO UserEquipment (SessionID, Hat, Cloth, Boots, Weapon, Bag) VALUES ('" + std::to_string(m_nSessionID) + "', '-1', '-1', '-1', '-1', '-1')";

	if (mysql_query(Con, m_strEquipment.c_str()))
	{
		printf_s("[DB] ��� �ʱ�ȭ ����\n");
		return false;
	}

	std::string m_strSlot = "Slot";
	std::string m_strID = "_ID";
	std::string m_strCount = "_Count";

	std::string m_strInventory = "INSERT UserInventory (SessionID, ";
	for (int i = 1; i <= 20; i++)
	{
		if (i != 20)
		{
			m_strInventory += m_strSlot + std::to_string(i) + m_strID + ", ";
			m_strInventory += m_strSlot + std::to_string(i) + m_strCount + ", ";
		}
		else
		{
			m_strInventory += m_strSlot + std::to_string(i) + m_strID + ", ";
			m_strInventory += m_strSlot + std::to_string(i) + m_strCount;
		}

	}
	//cout << m_strInventory << std::endl;
	m_strInventory += ") VALUES ('" + std::to_string(m_nSessionID) + "',";
	for (int i = 1; i <= 20; i++)
	{
		if (i != 20)
		{
			m_strInventory += "'" + std::to_string(-1) + "',";
			m_strInventory += "'" + std::to_string(-1) + "',";
		}
		else
		{
			m_strInventory += "'" + std::to_string(-1) + "',";
			m_strInventory += "'" + std::to_string(-1) + "')";
		}

	}

	if (mysql_query(Con, m_strInventory.c_str()))
	{
		printf_s("[DB] �κ��丮 �ʱ�ȭ ����\n");
		return false;
	}

	return true;
}


int DB::GetSessinID(const std::string & ID)
{
	std::string m_strDBSearch = "SELECT * FROM UserAccount Where ID = '";
	m_strDBSearch += ID + "'";

	if (mysql_query(Con, m_strDBSearch.c_str()))
	{
		printf_s("[DB] SessionID �˻� ����\n");
		return false;
	}

	Res = mysql_store_result(Con);
	Row = mysql_fetch_row(Res);
	mysql_free_result(Res);

	int m_nSessinID = std::stoi(Row[SQL_SessionID]);
	return m_nSessinID;
}

std::string DB::GetCName(const int nSessionID)
{
	std::string m_strDBSearch = "SELECT * FROM UserAccount Where SessionID = '";
	m_strDBSearch += std::to_string(nSessionID) + "'";

	if (mysql_query(Con, m_strDBSearch.c_str()))
	{
		printf_s("[DB] Name �˻� ����\n");
		return false;
	}

	Res = mysql_store_result(Con);
	Row = mysql_fetch_row(Res);
	mysql_free_result(Res);

	std::string m_strName = Row[SQL_CName];
	return m_strName;

}

int DB::GetLevel(const int nSessionID)
{
	std::string m_strDBSearch = "SELECT * FROM UserAccount Where SessionID = '";
	m_strDBSearch += std::to_string(nSessionID) + "'";

	if (mysql_query(Con, m_strDBSearch.c_str()))
	{
		printf_s("[DB] Level �˻� ����\n");
		return false;
	}

	Res = mysql_store_result(Con);
	Row = mysql_fetch_row(Res);
	mysql_free_result(Res);

	int m_nLevel_ID = std::stoi(Row[SQL_Level]);
	return m_nLevel_ID;
}

int DB::GetOccupation(const int nSessionID)
{
	std::string m_strDBSearch = "SELECT * FROM UserAccount Where SessionID = '";
	m_strDBSearch += std::to_string(nSessionID) + "'";

	if (mysql_query(Con, m_strDBSearch.c_str()))
	{
		printf_s("[DB] Map �˻� ����\n");
		return false;
	}
	Res = mysql_store_result(Con);
	Row = mysql_fetch_row(Res);
	mysql_free_result(Res);

	int m_nOccupation = std::stoi(Row[SQL_Occupation]);
	return m_nOccupation;
}

int DB::GetLV(const int nSessionID)
{
	std::string m_strDBSearch = "SELECT * FROM UserAccount Where SessionID = '";
	m_strDBSearch += std::to_string(nSessionID) + "'";

	if (mysql_query(Con, m_strDBSearch.c_str()))
	{
		printf_s("[DB] ���� ���� �˻� ����\n");
		return false;
	}

	Res = mysql_store_result(Con);
	Row = mysql_fetch_row(Res);
	mysql_free_result(Res);

	int m_nLV = std::stoi(Row[SQL_LV]);
	return m_nLV;
}

int DB::GetExperience(const int nSessionID)
{
	std::string m_strDBSearch = "SELECT * FROM UserAccount Where SessionID = '";
	m_strDBSearch += std::to_string(nSessionID) + "'";

	if (mysql_query(Con, m_strDBSearch.c_str()))
	{
		printf_s("[DB] ����ġ �˻� ����\n");
		return false;
	}

	Res = mysql_store_result(Con);
	Row = mysql_fetch_row(Res);
	mysql_free_result(Res);

	int m_nExperence = std::stoi(Row[SQL_Experence]);
	return m_nExperence;
}

int DB::GetFatigue(const int nSessionID)
{
	std::string m_strDBSearch = "SELECT * FROM UserAccount Where SessionID = '";
	m_strDBSearch += std::to_string(nSessionID) + "'";

	if (mysql_query(Con, m_strDBSearch.c_str()))
	{
		printf_s("[DB] �Ƿε� �˻� ����\n");
		return false;
	}

	Res = mysql_store_result(Con);
	Row = mysql_fetch_row(Res);
	mysql_free_result(Res);

	int m_nFatigue = std::stoi(Row[SQL_Fatigue]);
	return m_nFatigue;
}

int DB::GetUserInventory(const int nSessionID, const int nSlotCount)
{
	std::string m_strSessionID = std::to_string(nSessionID);
	std::string m_strDBSearch = "SELECT * FROM UserInventory Where SessionID = '";
	m_strDBSearch += m_strSessionID + "'";

	if (mysql_query(Con, m_strDBSearch.c_str()))
	{
		printf_s("[DB] Inventory �˻� ����\n");
		return -1;
	}
	Res = mysql_store_result(Con);
	Row = mysql_fetch_row(Res);
	mysql_free_result(Res);

	int m_nInventoryInfo = std::stoi(Row[nSlotCount]);
	return m_nInventoryInfo;
}

int DB::GetInventorySize(const int nSessionID)
{
	return 0;
}

int DB::GetUserEquipment(const int nSessionID, const int nSlotCount)
{
	std::string m_strSessionID = std::to_string(nSessionID);
	std::string m_strDBSearch = "SELECT * FROM UserEquipment Where SessionID = '";
	m_strDBSearch += m_strSessionID + "'";

	if (mysql_query(Con, m_strDBSearch.c_str()))
	{
		printf_s("[DB] Euqipment �˻� ����\n");
		return -1;
	}
	Res = mysql_store_result(Con);
	Row = mysql_fetch_row(Res);
	mysql_free_result(Res);

	int m_nEquipmentInfo = std::stoi(Row[nSlotCount]);
	return m_nEquipmentInfo;
}

int DB::GetDate(const int nSessionID)
{
	std::string m_strDBSearch = "SELECT * FROM UserAccount Where SessionID = '";
	m_strDBSearch += std::to_string(nSessionID) + "'";

	if (mysql_query(Con, m_strDBSearch.c_str()))
	{
		printf_s("[DB] ������ȣ �˻� ����\n");
		return false;
	}

	Res = mysql_store_result(Con);
	Row = mysql_fetch_row(Res);
	mysql_free_result(Res);

	time_t m_time = (time_t)std::stoi(Row[SQL_Date]);

	Time = time(0);

	double m_diff = difftime(Time, m_time);

	return 	m_diff / 3600;
}

bool DB::UpdateLevel(const int nSessionID, const int nLevel)
{
	std::string m_strUpdate = "UPDATE UserAccount set Level_ID='";
	m_strUpdate += std::to_string(nLevel) + "' WHERE SessionID = \'" + std::to_string(nSessionID) + "\'";

	if (mysql_query(Con, m_strUpdate.c_str()))
	{
		printf_s("[DB] Level(Map) ������Ʈ ����\n");
		return false;
	}

	return true;
}

bool DB::UpdateLV(const int nSessionID, const int nLV)
{
	std::string m_strUpdate = "UPDATE UserAccount set LV='";
	m_strUpdate += std::to_string(nLV) + "' WHERE SessionID = \'" + std::to_string(nSessionID) + "\'";

	if (mysql_query(Con, m_strUpdate.c_str()))
	{
		printf_s("[DB] LV ������Ʈ ����\n");
		return false;
	}

	return true;
}

bool DB::UpdateExperience(const int nSessionID, const int nExperience)
{
	std::string m_strUpdate = "UPDATE UserAccount set Experience='";
	m_strUpdate += std::to_string(nExperience) + "' WHERE SessionID = \'" + std::to_string(nSessionID) + "\'";

	if (mysql_query(Con, m_strUpdate.c_str()))
	{
		printf_s("[DB] Experience ������Ʈ ����\n");
		return false;
	}

	return true;
}

bool DB::UpdateFatigue(const int nSessionID, const int nFatigue)
{
	std::string m_strUpdate = "UPDATE UserAccount set Fatigue='";
	m_strUpdate += std::to_string(nFatigue) + "' WHERE SessionID = \'" + std::to_string(nSessionID) + "\'";

	if (mysql_query(Con, m_strUpdate.c_str()))
	{
		printf_s("[DB] Fatigue ������Ʈ ����\n");
		return false;
	}

	return true;
}

bool DB::UpdateInventory(const int nSessionID, const int nSlotNumber, const int nItemID, bool Type)
{
	std::string m_strUpdate = "UPDATE UserInventory SET ";

	if (Type) // �κ��丮�� ���
	{
		m_strUpdate += "Slot" + std::to_string(nSlotNumber + 1) + "_ID = '" + std::to_string(nItemID) +
			"', Slot" + std::to_string(nSlotNumber + 1) + "_Count = '" + std::to_string(0) +
			"' where SessionID = \'" + std::to_string(nSessionID) + "\'";
	}
	else // �κ��丮���� ����
	{
		m_strUpdate += "Slot" + std::to_string(nSlotNumber + 1) + "_ID = '" + std::to_string(-1) +
			"', Slot" + std::to_string(nSlotNumber + 1) + "_Count = '" + std::to_string(-1) +
			"' where SessionID = \'" + std::to_string(nSessionID) + "\'";
	}

	if (mysql_query(Con, m_strUpdate.c_str()))
	{
		printf_s("[DB] Inventory(���) ������Ʈ ����\n");
		return false;
	}

	return true;
}

bool DB::UpdateInventory(const int nSessionID, const int nSlotNumber, const int nItemID, const int nItemCount)
{
	std::string m_strUpdate = "UPDATE UserInventory SET ";

	if (nItemCount == 0)
	{
		// ������ �Ҹ�
		m_strUpdate += "Slot" + std::to_string(nSlotNumber + 1) + "_ID = '" + std::to_string(-1) +
			"', Slot" + std::to_string(nSlotNumber + 1) + "_Count = '" + std::to_string(-1) +
			"' where SessionID = \'" + std::to_string(nSessionID) + "\'";
	}
	else
	{
		m_strUpdate += "Slot" + std::to_string(nSlotNumber + 1) + "_ID = '" + std::to_string(nItemID) +
			"', Slot" + std::to_string(nSlotNumber + 1) + "_Count = '" + std::to_string(nItemCount) +
			"' where SessionID = \'" + std::to_string(nSessionID) + "\'";
	}

	printf_s("%s \n", m_strUpdate.c_str());

	if (mysql_query(Con, m_strUpdate.c_str()))
	{
		printf_s("[DB] Inventory(��Ÿ) ������Ʈ ����\n");
		return false;
	}

	return true;
}

bool DB::UpdateEquipment(const int nSessionID, const int nSlotNumber, const int nItemID)
{
	std::string m_strUpdate = "UPDATE UserEquipment SET ";

	printf_s("%d \n", nSlotNumber);

	switch (nSlotNumber)
	{
	case 0:
		m_strUpdate += "Hat = " + std::to_string(nItemID) + " WHERE SessionID=\'" + std::to_string(nSessionID) + "\'";
		break;
	case 1:
		m_strUpdate += "Cloth = " + std::to_string(nItemID) + " WHERE SessionID=\'" + std::to_string(nSessionID) + "\'";
		break;
	case 2:
		m_strUpdate += "Boots = " + std::to_string(nItemID) + " WHERE SessionID=\'" + std::to_string(nSessionID) + "\'";
		break;
	case 3:
		m_strUpdate += "Weapon = " + std::to_string(nItemID) + " WHERE SessionID=\'" + std::to_string(nSessionID) + "\'";
		break;
	case 4:
		m_strUpdate += "Bag = " + std::to_string(nItemID) + " WHERE SessionID=\'" + std::to_string(nSessionID) + "\'";
		break;
	}

	if (mysql_query(Con, m_strUpdate.c_str()))
	{
		printf_s("[DB] Equipment ������Ʈ ����\n");
		return false;
	}

	return true;
}

bool DB::UpdateInventorySize(const int nSessionID, const int nMaxSize)
{
	std::string m_strUpdate = "UPDATE UserAccount set SlotCount='";
	m_strUpdate += std::to_string(nMaxSize) + "' WHERE SessionID = \'" + std::to_string(nSessionID) + "\'";

	if (mysql_query(Con, m_strUpdate.c_str()))
	{
		printf_s("[DB] Inventory Max Size ������Ʈ ����\n");
		return false;
	}

	return true;
}

bool DB::UpdateDate(const int nSessionID)
{
	Time = time(0);
	stTime = *localtime(&Time);

	char buf[80];
	strftime(buf, sizeof(buf), "%Y-%m-%d %X", &stTime);
	std::string m_strNewDate = buf;

	// ���� ���� �� ������ �ð� ����
	std::string m_strDateUpdate = "UPDATE UserAccount set Date=UNIX_TIMESTAMP('" + m_strNewDate + "') where SessionID=\'";
	m_strDateUpdate += std::to_string(nSessionID) + "\'";

	if (mysql_query(Con, m_strDateUpdate.c_str()))
	{
		printf_s("[DB] Date ������Ʈ ����\n");
		return false;
	}

	return true;
}
