// Tile.h: interface for the CTile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILE_H__DA7A0761_758B_11D2_A8E6_00001C7030A6__INCLUDED_)
#define AFX_TILE_H__DA7A0761_758B_11D2_A8E6_00001C7030A6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <windows.h>
#include <mmsystem.h>

class CTile  
{
public:
	void Clear();
	
	CTile();
	virtual ~CTile();
	// 데이터 정렬되었음
	
	DWORD m_dwOwnerTime;
	DWORD m_dwEffectTime;
	DWORD m_dwDeadOwnerTime;
	DWORD m_dwDynamicObjectTime;
	
	int   m_iChatMsg;
	int   m_cItemColor; // v1.4
	int   m_iEffectType;
	int   m_iDeadApprColor; // v1.4
	int   m_iEffectFrame, m_iEffectTotalFrame;
	int   m_iApprColor; // v1.4
	int   m_iDeadChatMsg;

	WORD  m_wDeadObjectID;
	WORD  m_wObjectID;

	short m_sOwnerType;
	short m_sAppr1;
	short m_sAppr2;
	short m_sAppr3;
	short m_sAppr4;
	short m_sStatus;
	
	short m_sDeadOwnerType;
	short m_sDeadAppr1;
	short m_sDeadAppr2;
	short m_sDeadAppr3;
	short m_sDeadAppr4;
	
	short m_sDeadStatus;
	short m_sV1;
	short m_sV2;
	short m_sV3;
	short m_sDynamicObjectType;

	short m_sItemSprite;
	short m_sItemSpriteFrame;

	char  m_cDeadOwnerFrame;// !!! 이 프레임은 -1로 초기화 되어 있다가 0으로 되는 순간부터 카운팅 되어 8이면 삭제된다.
	char  m_cOwnerAction;
	char  m_cOwnerFrame;
	char  m_cDir;
	char  m_cDeadDir;
	
	char  m_cDynamicObjectFrame;
	char  m_cDynamicObjectData1, m_cDynamicObjectData2, m_cDynamicObjectData3, m_cDynamicObjectData4;
	char  m_cOwnerName[12];
	char  m_cDeadOwnerName[12];
};

#endif // !defined(AFX_TILE_H__DA7A0761_758B_11D2_A8E6_00001C7030A6__INCLUDED_)
