#pragma once

#include "../Packet/TradePacket.h"

class Trade
{
public:
	std::map<int, struct TradeInfo> Players;

public:
	void Matching(const int nSendSessionID, const int nRecvSessionID);																// ��ȯ ��Ī
	int GetOtherSessionID(const int nSessionID);																					// ��� SessionID ��������
	void PutOnItem(const int nSessionID, const int nItemID, const int nInventorySlot, const int nItemType, const int nItemCount);	// ������ ���
	bool Accept(const int nSessionID);																								// ��� �Ϸ�
	void cancel(struct User* player, Packet_SC_TradeCancel & kMyTradeItemInfo);												// ��ȯ ���
	void Success(struct User* player, const int nOtherSessionID, Packet_SC_TradeCompletion & kTradeCompletion);					// ��ȯ ����
	int GetRecentSlotNumber(const int nSessionID);																					// ���� ���� ����
	void Clear();
};