#pragma once
#include "gameNode.h"
#include "ShopNpcManager.h"
#include "displayStand.h"

#define ITEMDESKCOUNT 4
class shopScene : public gameNode
{
private:
	ShopNpcManager* _npc;
	displayStand* _displayStand;

	RECT GoTownPortal;

	int _npcTime;
	bool _visit;

	vector <pair<int, int>> v_itemPos;
	vector <pair<int, int>> v_itemShadowPos;

	void ItemPosSet();

public:
	HRESULT init();
	void release();
	void update();
	void render();

	void PortaltoTown(); // ¸¶À»Çà Æ÷Å»
	void PlayerCol();

	void npcAI();
};

