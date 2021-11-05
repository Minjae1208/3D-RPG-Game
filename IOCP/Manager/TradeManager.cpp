#include "TradeManager.h"

void Trade::Matching(const int nSendSessionID, const int nRecvSessionID)
{
	Players[nSendSessionID].SessionID = nSendSessionID;
	Players[nSendSessionID].RecentSlotNumber = -1;

	Players[nRecvSessionID].SessionID = nRecvSessionID;
	Players[nRecvSessionID].RecentSlotNumber = -1;
}

int Trade::GetOtherSessionID(const int nSessionID)
{
	for (auto& Player : Players)
	{
		if (Player.second.SessionID != nSessionID)
			return Player.second.SessionID;
	}
	return -1;
}

void Trade::PutOnItem(const int nSessionID, const int nItemID, const int nInventorySlot, const int nItemType, const int nItemCount = 0)
{
	if (Players[nSessionID].RecentSlotNumber >= 6)
		return;

	TradeItemInfo* m_kItemInfo = new TradeItemInfo;
	m_kItemInfo->ItemCount = nItemCount;
	m_kItemInfo->InventorySlot = nInventorySlot;
	m_kItemInfo->ItemType = nItemType;

	Players[nSessionID].OnPutItem[nItemID] = m_kItemInfo;
	Players[nSessionID].RecentSlotNumber++;
}

bool Trade::Accept(const int nSessionID)
{
	bool m_bAccept = true;

	Players[nSessionID].Accept = true;

	for (auto& Player : Players)
	{
		if (!Player.second.Accept)
			m_bAccept = false;
	}

	return m_bAccept;
}

void Trade::cancel(User * player, Packet_SC_TradeCancel & kMyTradeItemInfo)
{
	for (auto& itTradeInfo : Players[player->cPlayer.SessionID].OnPutItem)
	{
		kMyTradeItemInfo.InventoryInfo[itTradeInfo.first].ItemID = itTradeInfo.first;
		kMyTradeItemInfo.InventoryInfo[itTradeInfo.first].ItemCount = player->stInventory.Inventroy[itTradeInfo.second->InventorySlot].ItemCount;
		kMyTradeItemInfo.InventoryInfo[itTradeInfo.first].SlotNumber = itTradeInfo.second->InventorySlot;
		kMyTradeItemInfo.InventoryInfo[itTradeInfo.first].ItemType = itTradeInfo.second->ItemType;
	}
}

void Trade::Success(User * player, const int nOtherSessionID, Packet_SC_TradeCompletion & kTradeCompletion)
{
	kTradeCompletion.SessionID = player->cPlayer.SessionID;

	bool m_bIsHas = false;

	for (auto& itTradeInfo : Players[nOtherSessionID].OnPutItem)
	{
		int m_nItemID = itTradeInfo.first;
		for (auto& ItemInfo : player->stInventory.Inventroy)
		{
			if (ItemInfo.second.ItemID == m_nItemID)
			{
				ItemInfo.second.ItemCount += itTradeInfo.second->ItemCount;

				kTradeCompletion.InventoryInfo[m_nItemID].ItemID = m_nItemID;
				kTradeCompletion.InventoryInfo[m_nItemID].ItemCount = ItemInfo.second.ItemCount;
				kTradeCompletion.InventoryInfo[m_nItemID].ItemType = itTradeInfo.second->ItemType;
				kTradeCompletion.InventoryInfo[m_nItemID].SlotNumber = ItemInfo.second.SlotNumber;
				m_bIsHas = true;
				break;
			}
		}

		if (m_bIsHas == false)
		{
			for (int i = 0; i < player->stInventory.MaxSize; i++)
			{
				if (player->stInventory.Inventroy[i].ItemID == 0)
				{
					player->stInventory.Inventroy[i].ItemID = m_nItemID;
					player->stInventory.Inventroy[i].ItemCount = itTradeInfo.second->ItemCount;
					player->stInventory.Inventroy[i].ItemType = itTradeInfo.second->ItemType;
					player->stInventory.Inventroy[i].SlotNumber = i;

					kTradeCompletion.InventoryInfo[m_nItemID].ItemID = m_nItemID;
					kTradeCompletion.InventoryInfo[m_nItemID].ItemCount = itTradeInfo.second->ItemCount;
					kTradeCompletion.InventoryInfo[m_nItemID].ItemType = itTradeInfo.second->ItemType;
					kTradeCompletion.InventoryInfo[m_nItemID].SlotNumber = i;
					break;
				}
			}
		}
	}
}

int Trade::GetRecentSlotNumber(const int nSessionID)
{
	return Players[nSessionID].RecentSlotNumber;
}

void Trade::Clear()
{
	Players.clear();
}
