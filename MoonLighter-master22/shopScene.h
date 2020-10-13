#pragma once
#include "gameNode.h"
#include "ShopNpcManager.h"
#include "displayStand.h"
class shopScene : public gameNode
{
private:
	ShopNpcManager* _npc;
	displayStand *_displayStand;

	RECT GoTownPortal;

	float _pivotX;
	float _pivotY;

	int _npcTime;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	void PortaltoTown(); // ¸¶À»Çà Æ÷Å»
	void PlayerCol();
};

