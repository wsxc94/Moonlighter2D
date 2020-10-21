#pragma once
#include "tileNode.h"
#include "PotionShopNpc.h"

class npcManager // 마을 npc 매니저
{
private:
	PotionShopNpc* _witch;
	npc* _girl;
	npc* _blackOctopus;
	npc* _masa;
	npc* _redmond;
	npc* _doberman;

	vector<npc*> _npcs;

	bool npcColl;
public:

	HRESULT init(vector<tagTile>& vec);
	void release();
	void update();
	void render();

	void Coll(); // npc 충돌 함수

	PotionShopNpc* getPotionShopNpc() { return dynamic_cast<PotionShopNpc*>(_witch); }
};

