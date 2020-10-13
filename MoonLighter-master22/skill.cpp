#include "stdafx.h"
#include "skill.h"

HRESULT skill::init()
{
	IMAGEMANAGER->addFrameImage("방패", "Images/플레이어/shiled_state1X4.bmp", 120, 480, 1, 4);

	_shiled.x = PLAYER->getX();
	_shiled.y = PLAYER->getY();
	_shiled.rc = PLAYER->getRect();
	_shiled.isColi = false;

	_state = SHILED;
	return S_OK;
}

void skill::release()
{

}

void skill::update()
{
	this->animation();
	this->skillset();
}

void skill::render(HDC hdc)
{
	switch (_state)
	{
	case SHILED:
		break;
	case BOW_CHARGE:
		break;
	}
}

void skill::animation()
{
	switch (_state)
	{
	case SHILED:

		break;
	case BOW_CHARGE:
		break;
	}
}

void skill::skillset()
{
	if (INPUT->GetKey('K'))
	{
		switch (PLAYER->getWeaponState())
		{
			case EMPTY:
				break;
			case SHORT_SOWRD:
				_state = SHILED;
				break;
			case BOW:
				_state = BOW_CHARGE;
				break;
		}
	}
}
