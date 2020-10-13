#include "stdafx.h"
#include "npcManager.h"

HRESULT npcManager::init(vector<tagTile>& vec)
{
	
	IMAGEMANAGER->addFrameImage("npc∏ª«≥º±", "Images/npc/talkBox.bmp", 450, 180, 1, 1);
	IMAGEMANAGER->addFrameImage("npc¥Î»≠", "Images/npc/∏ª«œ±‚.bmp", 363, 50, 3, 1);

	for (int i = 0; i < vec.size(); i++)
	{
		if (vec[i].key == "ø°∏ÆΩ∫") {
			_witch = new npc;
			_witch->init(tagPosF(vec[i].rc.left, vec[i].rc.top), vec[i].key);
			_npcs.push_back(_witch);
		}
		if (vec[i].key == "≈‰∏")
		{
			_girl = new npc;
			_girl->init(tagPosF(vec[i].rc.left, vec[i].rc.top), vec[i].key);
			_npcs.push_back(_girl);
		}
		if (vec[i].key == "∏∏Æ")
		{
			_blackOctopus = new npc;
			_blackOctopus->init(tagPosF(vec[i].rc.left, vec[i].rc.top), vec[i].key);
			_npcs.push_back(_blackOctopus);
		}
		if (vec[i].key == "∏∂ªÁ")
		{
			_masa = new npc;
			_masa->init(tagPosF(vec[i].rc.left, vec[i].rc.top), vec[i].key);
			_npcs.push_back(_masa);
		}
		if (vec[i].key == "∑πµÂ∏’µÂ")
		{
			_redmond = new npc;
			_redmond->init(tagPosF(vec[i].rc.left, vec[i].rc.top), vec[i].key);
			_npcs.push_back(_redmond);
		}
		if (vec[i].key == "∞≠æ∆¡ˆnpc")
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
}

void npcManager::update()
{

	_witch->update();
	_girl->update();
	_blackOctopus->update();
	_redmond->update();
	_masa->update();
	_doberman->lookPlayer();

	_blackOctopus->move();
	_girl->move();
	_redmond->move();
	_masa->move();

	_witch->collision();
	_girl->collision();
	_blackOctopus->collision();
	_redmond->collision();
	_masa->collision();

	_witch->action("≥  æÁƒ° æ»«ﬂ¡ˆ? ¿‘ ¥›æ∆ ≥øªı≥™");
	_girl->action("§æ§∑");
	_blackOctopus->action("≈ª∏∫ˆ !!!!");
	_masa->action("ƒ‡π∞ ∞≥≤‹∏¿ π‰µµµœ");
	_redmond->action("πª∫¡ §ª");

	Coll();

}

void npcManager::render()
{
	_witch->render();
	_girl->render();
	_blackOctopus->render();
	_masa->render();
	_redmond->render();
	_doberman->render();

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
