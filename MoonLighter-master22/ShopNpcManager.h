#pragma once
#include "gameNode.h"
#include "npc.h"


class ShopNpcManager : public gameNode
{
private:

	int _npcCnt;

	tagPosF _pos;

	vector<npc*> v_npcs;

	int _npcTime;
	vector<string> _npcName = {
		"마사" , "토모" , "레드먼드" ,"모리"
	};
public:
	HRESULT init();
	void release();
	void update();
	void render();

	void npcSpawn();

};

