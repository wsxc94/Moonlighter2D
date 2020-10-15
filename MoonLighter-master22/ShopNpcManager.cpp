#include "stdafx.h"
#include "ShopNpcManager.h"
#include "displayStand.h"
HRESULT ShopNpcManager::init(displayStand* dis)
{
	_npcCnt = 0;

	_pos.x = 660;
	_pos.y = 830;

	v_npcs.resize(MAX_NPC);

	_npcTime = 0;
	_displayStand  = dis;
	for (int i = 0; i < MAX_NPC; i++)
	{
		v_npcs[i] = new npc;
		v_npcs[i]->init(_pos , _npcName[i] , NPC_SHOP , i , _displayStand);
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
		/*v_npcs[i]->update(NPC_SHOP);
		v_npcs[i]->move(NPC_SHOP);*/

		v_npcs[i]->collision();

	}

	/*if(_npcCnt < 4)
	npcSpawn();*/
}

void ShopNpcManager::render()
{
	////RECT tmp;
	//for (int i = 0; i < MAX_NPC; i++)
	//{
	//	//if (!IntersectRect(&tmp, &CAMERAMANAGER->getRect(), &v_npcs[i]->getRect())) continue;
	//	v_npcs[i]->render(NPC_SHOP);
	//}
}

