#include "stdafx.h"
#include "potal.h"

HRESULT potal::init(float x, float y, POTALSTATE state)
{
	_x = x;
	_y = y;
	_potalState = state;

	_init = new animation;
	_update = new animation;
	_break = new animation;
	_playerIn = new animation;
	_playerOut = new animation;

	_init->init(IMAGEMANAGER->findImage("potalInit"), 0, 7);
	_update->init(IMAGEMANAGER->findImage("potalUpdate"), 0, 7, true);
	_break->initReverse(IMAGEMANAGER->findImage("potalInit"), 0, 7);
	_playerIn->init(IMAGEMANAGER->findImage("potalPlayer"), 0, 7);
	_playerOut->initReverse(IMAGEMANAGER->findImage("potalPlayer"), 0, 7);

	_isActivate = true;	
	_isInRange = false;


	return S_OK;
}

void potal::update()
{
	switch (_potalState)
	{
	case POTAL_INIT:
		_init->update();
		if (_init->getAniState() == ANIMATION_END)
			_potalState = POTAL_UPDATE;
		break;
	case POTAL_UPDATE:
		_update->update();
		if (getDistance(PLAYER->getX(), PLAYER->getY(), _x, _y) < 30)
		{
			_isInRange = true;
		}
		else _isInRange = false;
		break;
	case POTAL_BREAK:
		_break->update();
		if (_break->getAniState() == ANIMATION_END)
			_isActivate = false;
		break;
	case POTAL_PLAYERIN:
		_playerIn->update();
		break;
	case POTAL_PLAYEROUT:
		_playerOut->update();
		break;
	default:
		break;
	}
}

void potal::render()
{
	switch (_potalState)
	{
	case POTAL_INIT:
		_init->ZorderStretchRender(_y,_x,_y, 2.f);
		break;
	case POTAL_UPDATE:
		_update->ZorderStretchRender(_y, _x, _y, 2.f);
		if (_isInRange)
		{
			RECT txtRC = RectMake(_x + 80, _y - 90, 110, 50);
			HFONT hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
				0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("JejuGothic"));
			//IMAGEMANAGER->findImage("messegeBox_potal")->render(getMemDC(), _potal->x + 30, _potal->y - 90);
			CAMERAMANAGER->ZorderRender(IMAGEMANAGER->findImage("messegeBox_potal"), 1999, _x + 30, _y - 90);
			CAMERAMANAGER->ZorderDrawText("텔레포트", 2000, txtRC, hFont, RGB(0, 0, 0), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		break;
	case POTAL_BREAK:
		_break->ZorderStretchRender(_y, _x, _y, 2.f);


		break;
	case POTAL_PLAYERIN:
		_playerIn->ZorderStretchRender(_y, _x, _y, 2.f);

		break;
	case POTAL_PLAYEROUT:
		_playerOut->ZorderStretchRender(_y, _x, _y, 2.f);

		break;
	default:
		break;
	}
}

animation * potal::getAnimation()
{
	switch (_potalState)
	{
	case POTAL_INIT:
		return _init;
		break;
	case POTAL_UPDATE:
		return _update;
		break;
	case POTAL_BREAK:
		return _break;
		break;
	case POTAL_PLAYERIN:
		return _playerIn;
		break;
	case POTAL_PLAYEROUT:
		break;
		return _playerOut;
	default:
		break;
	}
	return nullptr;
}

void potal::setPotalState(POTALSTATE state)
{
	_potalState = state;
	switch (state)
	{
	case POTAL_INIT:
		_init->aniRestart();
		break;
	case POTAL_UPDATE:
		_update->aniRestart();
		break;
	case POTAL_BREAK:
		_break->aniRestart();
		break;
	case POTAL_PLAYERIN:
		_playerIn->aniRestart();
		break;
	case POTAL_PLAYEROUT:
		_playerOut->aniRestart();
		break;
	}
}
