#pragma once

#include "../Packet/TradePacket.h"

class Trade
{
public:
	std::map<int, struct TradeInfo> Players;

public:
	void Matching(const int nSendSessionID, const int nRecvSessionID);																// 교환 매칭
	int GetOtherSessionID(const int nSessionID);																					// 상대 SessionID 가져오기
	void PutOnItem(const int nSessionID, const int nItemID, const int nInventorySlot, const int nItemType, const int nItemCount);	// 아이템 등록
	bool Accept(const int nSessionID);																								// 등록 완료
	void cancel(struct User* player, Packet_SC_TradeCancel & kMyTradeItemInfo);												// 교환 취소
	void Success(struct User* player, const int nOtherSessionID, Packet_SC_TradeCompletion & kTradeCompletion);					// 교환 성공
	int GetRecentSlotNumber(const int nSessionID);																					// 이전 슬롯 정보
	void Clear();
};