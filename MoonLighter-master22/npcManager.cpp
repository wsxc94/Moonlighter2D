#include "stdafx.h"
#include "npcManager.h"

HRESULT npcManager::init(vector<tagTile>& vec)
{
	
	IMAGEMANAGER->addFrameImage("npc말풍선", "Images/npc/talkBox.bmp", 450, 180, 1, 1);
	IMAGEMANAGER->addFrameImage("npc대화", "Images/npc/말하기.bmp", 363, 50, 3, 1);

	for (int i = 0; i < vec.size(); i++)
	{
		if (vec[i].key == "에리스") {
			_witch = new PotionShopNpc;
			//_witch->init(tagPosF(vec[i].rc.left, vec[i].rc.top), vec[i].key);
			_witch->init(tagPosF(vec[i].rc.left, vec[i].rc.top), vec[i].key);

			_npcs.push_back(_witch);
		}
		if (vec[i].key == "토모")
		{
			_girl = new npc;
			_girl->init(tagPosF(vec[i].rc.left, vec[i].rc.top), vec[i].key);
			_npcs.push_back(_girl);
		}
		if (vec[i].key == "모리")
		{
			_blackOctopus = new npc;
			_blackOctopus->init(tagPosF(vec[i].rc.left, vec[i].rc.top), vec[i].key);
			_npcs.push_back(_blackOctopus);
		}
		if (vec[i].key == "마사")
		{
			_masa = new npc;
			_masa->init(tagPosF(vec[i].rc.left, vec[i].rc.top), vec[i].key);
			_npcs.push_back(_masa);
		}
		if (vec[i].key == "레드먼드")
		{
			_redmond = new npc;
			_redmond->init(tagPosF(vec[i].rc.left, vec[i].rc.top), vec[i].key);
			_npcs.push_back(_redmond);
		}
		if (vec[i].key == "강아지npc")
		{
			_doberman = new npc;
			_doberman->init(tagPosF(vec[i].rc.left, vec[i].rc.top), vec[i].key);
			_npcs.push_back(_doberman);
		}
	
	}
	
	npcColl = false;

	return S_OK;
}

void npcManager::release()
{
	SAFE_DELETE(_witch);
	SAFE_DELETE(_girl);
	SAFE_DELETE(_blackOctopus);
	SAFE_DELETE(_redmond);
	SAFE_DELETE(_masa);
	SAFE_DELETE(_doberman);
}

void npcManager::update()
{

	for (int i = 0; i < _npcs.size(); i++)
	{
		if(_npcs[i]->getKey() != "강아지npc")
		_npcs[i]->update();

		_npcs[i]->collision();
	}
	
	_doberman->lookPlayer();

	_blackOctopus->move();
	_girl->move();
	_redmond->move();
	_masa->move();

	_witch->action("잘 왔어, 안녕 윌! 오늘은 어떤 걸 만들고 싶어?");
	_girl->action("윌!!! 전에 골렘 던전에서 널 본 적이 있어! 신비한 유물은 좀 찾았어!?!");
	_blackOctopus->action("던전에서 새로운 문명 유물은 좀 찾았니, 윌?");
	_masa->action("윌! 다음에 상점에 놀러가면 술래잡기 해도 돼?");
	_redmond->action("던전에서 치유의 샘을 발견한다면 활용하는 거 잊      지마!");
	_doberman->action();

	Coll();

}

void npcManager::render()
{
	RECT tmp;
	for (int i = 0; i < _npcs.size(); i++)
	{
		if (!IntersectRect(&tmp, &CAMERAMANAGER->getRect(), &_npcs[i]->getRect())) continue;
		_npcs[i]->render();
	}

}

void npcManager::Coll()
{
	RECT tmp;

	int cnt = 0;

	for (int i = 0; i < _npcs.size(); i++)
	{
		if (!IntersectRect(&tmp, &_npcs[i]->getRect(), &PLAYER->getRect()))
		{
			cnt++;
		}	
	}

	if (cnt == _npcs.size() - 1) {
		npcColl = true;
	}
	else {
		npcColl = false;
	}


	PLAYER->npcTalk(npcColl);
}
