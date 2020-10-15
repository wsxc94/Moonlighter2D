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

	for (int i = 0; i < MAX_NPC; i++)
	{
		v_npcs[i] = new npc;
		v_npcs[i]->init(_pos, _npcName[i], NPC_SHOP, i, _displayStand);
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

}

void ShopNpcManager::render()
{

}

