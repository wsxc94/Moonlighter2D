#pragma once
#include "gameNode.h"
#include "ShopNpcManager.h"
#include "displayStand.h"

#define ITEMDESKCOUNT 4
class shopScene : public gameNode
{
private:
	ShopNpcManager* _npc; // npc매니저
	displayStand* _displayStand; //아이템 좌판 정보

	RECT GoTownPortal; // 마을로가는 포탈

	int _npcTime;
	bool _visit; //플레이어 방문여부

	vector <pair<int, int>> v_itemPos; //아이템위치
	vector <pair<int, int>> v_itemShadowPos;//아이템그림자위치

	RECT _desk;
	void ItemPosSet(); // 아이템 위치 세팅

public:
	HRESULT init();
	void release();
	void update();
	void render();

	void PortaltoTown(); // 마을행 포탈
	void PlayerCol(); // 플레이어 픽셀충돌 함수 
	void PlayerSell(); // 아이템 파는 함수

	void npcAI(); // npc update 여기다 넣음
};

