#include "stdafx.h"
#include "ShopNpcManager.h"
HRESULT ShopNpcManager::init(displayStand* dis)
{
	_npcCnt = 0;

	_pos.x = 660;
	_pos.y = 830;

	v_npcs.resize(MAX_NPC);

	_npcTime = 0;
	_displayStand = dis;

	random_shuffle(_npcName.begin(), _npcName.end());

	for (int i = 0; i < MAX_NPC; i++)
	{
		v_npcs[i] = new npc;
		
		int rnd = RANDOM->range(0, 1);
		//int rnd = 0;

		if(rnd == 0)
		v_npcs[i]->init(_pos, _npcName[i].first, NPC_SHOP, i, _displayStand);
		else
		v_npcs[i]->init(_pos, _npcName[i].second, NPC_SHOP, i, _displayStand);
	}
	return S_OK;
}

void ShopNpcManager::release()
{
	for (int i = 0; i < MAX_NPC; i++)
	{
		v_npcs[i]->release();
		SAFE_DELETE(v_npcs[i]);
	}
}

void ShopNpcManager::update()
{

	for (int i = 0; i < MAX_NPC; i++)
	{
		v_npcs[i]->collision(); // npc 충돌렉트 업데이트

	}

	UseNpcCheck();

}

void ShopNpcManager::render()
{

}

void ShopNpcManager::getInit(int idx , int rnd)
{
	int tmp = RANDOM->range(0, 1);
	//int tmp = 0;

	if(tmp == 0)
	v_npcs[idx]->init(_pos, _npcName[rnd].first, NPC_SHOP, idx, _displayStand);
	else
	v_npcs[idx]->init(_pos, _npcName[rnd].second, NPC_SHOP, idx, _displayStand);
}

void ShopNpcManager::UseNpcCheck()
{
}

