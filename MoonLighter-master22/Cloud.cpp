#include "stdafx.h"
#include "Cloud.h"


HRESULT Cloud::init(tagPosF pos)
{
	_pos = pos;
	_speed = 0.8f;
	_angle = getAngle(_pos.x, _pos.y, _pos.x - RANDOM->range(500,2500), _pos.y + 50000);
	_time = 0;
	_isActive = false;

	delay = RANDOM->range(60, 300);
	return S_OK;
}

void Cloud::release()
{
}

void Cloud::update()
{
	_time++;
	if (_time % delay == 0) _isActive = true;

	if(_isActive)
	move();
}

void Cloud::render(HDC hdc)
{
	CAMERAMANAGER->ZorderAlphaRender(IMAGEMANAGER->findImage("큰구름"), 1000, _pos.x, _pos.y, 40);
	CAMERAMANAGER->ZorderAlphaRender( IMAGEMANAGER->findImage("작은구름"),1000, _pos.x + IMAGEMANAGER->findImage("큰구름")->getWidth() / 2,
		_pos.y - IMAGEMANAGER->findImage("큰구름")->getHeight() / 2, 40);
	
}

void Cloud::move()
{
	

	if (_pos.x <= -500 || _pos.y >= IMAGEMANAGER->findImage("townBack")->getHeight() + 500) {
		tagPosF tmp;
		tmp.x = RANDOM->range(400, IMAGEMANAGER->findImage("townBack")->getWidth() + 200);
		tmp.y = RANDOM->range(-1800, -3500);

		_pos = tmp;
	}

	

	_pos.x += cosf(_angle) * _speed;
	_pos.y += -sinf(_angle) * _speed / 2;
}
