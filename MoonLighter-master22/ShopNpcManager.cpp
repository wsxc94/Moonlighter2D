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
		v_npcs[i]->init(_pos, _npcName[i].first, NPC_SHOP, i, _displayStand);
		_npcName[i].second = true;
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
	v_npcs[idx]->init(_pos, _npcName[rnd].first, NPC_SHOP, idx, _displayStand);
	_npcName[rnd].second = true;

}

void ShopNpcManager::UseNpcCheck()
{

	for (int i = 0; i < v_npcs.size(); i++)
	{
		//cout << _npcName[i].first << " " << _npcName[i].second << endl;
		if (v_npcs[i]->getState() == NPC_GO_HOME || v_npcs[i]->getState() == NPC_START) {
			_npcName[i].second = false;
		}
		else {
			_npcName[i].second = true;
		}
		
	}
}

