#include "stdafx.h"
#include "PotionShopNpc.h"

HRESULT PotionShopNpc::init(tagPosF pos, string key)
{
	_idx = 0;
	_count = 0;
	_time = 0;
	_angle = 0;
	_targetIdx = 0;
	_Istalk = false;
	_boxidx = 0;
	_boxCnt = 0;

	_key = key;
	_pos = pos;
	_name = key + " : ";

	_speed = 1.0f;

	_stop = false;
	_state = NPC_MOVE;

	_aniNpc = make_unique<animation>();
	_aniNpc->init(IMAGEMANAGER->findImage(_key), 0, 7, true);

	_isBarking = false;

	_potionShop = new potionShop;
	_potionShop->init();

	return S_OK;
}

void PotionShopNpc::update()
{
	if (_Istalk && INPUT->GetKeyDown('J')) {
		_Istalk = false;
		_potionShop->openPotionMenu();
	}
		_potionShop->update();
	anim(); // 각도에 따라 애니메이션을 변경하는 함수

	RECT tmp;

	if (IntersectRect(&tmp, &PLAYER->getRect(), &_rc))
		boxAnim();
	else
		_boxidx = 0;



}

void PotionShopNpc::render()
{

	_potionShop->render();

	RECT tmp;

	if (IntersectRect(&tmp, &PLAYER->getRect(), &_rc) && !_Istalk)
	{
		CAMERAMANAGER->ZorderFrameRender(IMAGEMANAGER->findImage("npc대화"), 2000, _pos.x + 30, _pos.y - 50, _boxidx, 0);
	}

	if (_stop)
	{

		_aniNpc->aniStop();
		_aniNpc->ZoderRender(_pos.y + (IMAGEMANAGER->findImage(_key)->getFrameHeight() >> 1), _pos.x, _pos.y);
	}
	else {

		if (_aniNpc->getAniState() == ANIMATION_END) _aniNpc->aniRestart();
		_aniNpc->ZoderRender(_pos.y + (IMAGEMANAGER->findImage(_key)->getFrameHeight() >> 1), _pos.x, _pos.y);
	}

	ShadowPosRender();

	if(!_potionShop->getMenuOn())
	TalkInterfaceRender();
}

void PotionShopNpc::action(string talk)
{
	_talkRect = RectMake(_pos.x, _pos.y,
		IMAGEMANAGER->findImage(_key)->getFrameWidth() * 2, IMAGEMANAGER->findImage(_key)->getFrameHeight() * 2);

	_nameRect = CAMERAMANAGER->RectMake(_pos.x + 200, _pos.y - 150, 100, 30);
	_textRect = CAMERAMANAGER->RectMake(_pos.x + 200, _pos.y - 120, 200, 200);

	_talk = talk;

	RECT tmp;
	if (!ITEMMENU->getOpenMenu() && !_potionShop->getMenuOn())
	{
		if (IntersectRect(&tmp, &PLAYER->getRect(), &_rc) && INPUT->GetKeyDown('J'))
		{
				_Istalk = true;
				_stop = true;
		}
	}

	if (INPUT->GetKeyDown('L')) {
		if (_potionShop->getMenuOn())
		{
			_potionShop->closePotionMenu();
			_stop = false;
		}
	}
	if (!IntersectRect(&tmp, &PLAYER->getRect(), &_rc))
	{
		_potionShop->closePotionMenu();
		_Istalk = false;
		_stop = false;
	}
}
