#include "stdafx.h"
#include "ShopNpcManager.h"

HRESULT ShopNpcManager::init()
{
	_npcCnt = 0;

	_pos.x = 660;
	_pos.y = 830;

	v_npcs.resize(MAX_NPC);

	_npcTime = 0;

	for (int i = 0; i < MAX_NPC; i++)
	{
		v_npcs[i] = new npc;
		v_npcs[i]->init(_pos , _npcName[i] , NPC_SHOP);
	}
	return S_OK;
}

void ShopNpcManager::release()
{
	for (int i = 0; i < _npcCnt; i++)
	{
		v_npcs[i]->release();
		SAFE_DELETE(v_npcs[i]);
	}
}

void ShopNpcManager::update()
{

	for (int i = 0; i < _npcCnt; i++)
	{
		v_npcs[i]->update(NPC_SHOP);
		v_npcs[i]->move(NPC_SHOP);
		v_npcs[i]->collision();

	}

	if(_npcCnt < 4)
	npcSpawn();

	//for (int i = 0; i < _npcCnt; i++)
	//{
	//	
	//	COLORREF color = GetPixel(getMemDC(),v_npcs[i]->getRect().left , v_npcs[i]->getRect().top);
	//	COLORREF color1 = GetPixel(getMemDC(),v_npcs[i]->getRect().right , v_npcs[i]->getRect().top);
	//	COLORREF color2 = GetPixel(getMemDC(),v_npcs[i]->getRect().left , v_npcs[i]->getRect().bottom);
	//	COLORREF color3 = GetPixel(getMemDC(),v_npcs[i]->getRect().right , v_npcs[i]->getRect().bottom);

	//	if (color == RGB(255, 0, 0) ||
	//		color1 == RGB(255, 0, 0) ||
	//		color2 == RGB(255, 0, 0) ||
	//		color3 == RGB(255, 0, 0))
	//	{
	//		
	//		v_npcs[i]->setPosX(v_npcs[i]->getPos().x + 1);
	//		//v_npcs[i]->setPosY(v_npcs[i]->getPos().y + 2);
	//	}
	//}
}

void ShopNpcManager::render()
{
	for (int i = 0; i < _npcCnt; i++)
	{
		v_npcs[i]->render(NPC_SHOP);
	}
}

void ShopNpcManager::npcSpawn()
{
	_npcTime++;

	if (_npcTime % 240 == 0)
	{
		_npcCnt++;
		int rnd = RANDOM->range(0, 1);
		string str = "상점입장" + to_string(rnd);
		SOUNDMANAGER->play(str , 0.5f);
	}

}
