#pragma once

enum EType
{
	CS_SIGNUP,						// ȸ�� ����
	SC_SIGNUP,

	CS_LOGIN_TRY,
	SC_LOGIN_TRY,

	CS_XML_DATA,					// XML Data ���ε�

	SC_XML_DROP_ITEM,				// XML ����
	SC_XML_CB_FIRST,
	SC_XML_CB_SECOND,
	SC_XML_EQ_ITEM,
	SC_XML_SHOP_ITEM,
	SC_XML_LEVEL,

	CS_LOGIN_SUCCESS,
	SC_LOGIN_SUCCESS,

	CS_ENTER_ZONE,					// �ΰ��� �� ����
	SC_ENTER_ZONE,

	CS_CHANGE_ZONE,					// �ΰ��� �� ����
	SC_CHANGE_ZONE,

	SC_MAP_INFO,					// 

	CS_MOVE_INFO,					// �̵� ����ȭ
	SC_MOVE_INFO,

	CS_WORLD_CHAT,					// ���� ä��
	SC_WORLD_CHAT,

	CS_LEVEL_CHAT,					// Level ä��
	SC_LEVEL_CHAT,

	CS_WHISPER_CHAT,				// �ӼӸ�
	SC_WHISPER_CHAT,

	SC_ENTER_NEW_PLAYER,			// ���ο� �÷��̾� ����

	SC_MONSTER_INFO,				// ���� ����

	CS_GATHER_START_INFO,			// ä�� ����
	SC_GATHER_START_INFO,

	CS_GATHER_CANCEL_INFO,			// ä�� ���
	SC_GATHER_CANCEL_INFO,

	SC_GATHER_SUCCESS_INFO,			// ä�� ����

	SC_DROP_ITEM_INFO,				// ��� ������ ����

	SC_INTERACTION_MOVE_INFO,		// ä���� �̵� ����ȭ

	SC_RESPAWN_MONSTER_INFO,		// ���� ������

	SC_SPAWN_INTERACTION_INFO,		// ä���� ������ (��ü)
	SC_RESPAWN_INTERACTION_INFO,	// ä���� ������ (�Ѱ�)

	CS_COMBINATION_INFO,			// ������ ����
	SC_COMBINATION_INFO,

	CS_THROWITEM_INFO,				// ������ ������
	SC_THROWITEM_INFO,

	CS_MOUNTEQUIPMENT_INFO,			// ��� ������ ����
	SC_MOUNTEQUIPMENT_INFO,

	CS_UNMOUNTEQUIPMENT_INFO,		// ��� ������ ����
	SC_UNMOUNTEQUIPMENT_INFO,

	CS_TRYTRADE_INFO,				// ��ȯ ��û
	SC_TRYTRADE_INFO,

	SC_TRYTRADE_FAIL,

	CS_TRADEMACTCHING_INFO,			// ��ȯ��û ���
	SC_TRADEMACTCHING_INFO,

	CS_TRADEPUTONITEM_INFO,			// ������ �ø���
	SC_TRADEPUTONITEM_INFO,

	CS_TRADEACCEPT_INFO,			// �����ϱ�
	SC_TRADEACCEPT_INFO,

	CS_TRADECANCEL_INFO,			// ��ȯ ���
	SC_TRADECANCEL_INFO,

	SC_COMPLETION_INFO,				// ��ȯ �Ϸ�

	CS_SHOP_BUY_ITEM,				// ���� ����
	SC_SHOP_BUY_ITEM,

	CS_ITEMUSE_INFO,				// ������ ���
	SC_ITEMUSE_INFO,

	SC_FATIGUE_UPDATE,				// �Ƿε� ȸ��

	CS_LOGOUT_PLAYER,				// �α� �ƿ�
	SC_LOGOUT_PLAYER,
};