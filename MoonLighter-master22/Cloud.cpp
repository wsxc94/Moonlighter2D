#include "stdafx.h"
#include "Cloud.h"


HRESULT Cloud::init(tagPosF pos)
{
	_pos = pos;
	_speed = 0.8f;
	_angle = getAngle(_pos.x, _pos.y, _pos.x - RANDOM->range(300,2000), _pos.y + 50000);
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
	CAMERAMANAGER->AlphaRender(hdc, IMAGEMANAGER->findImage("큰구름"), _pos.x, _pos.y, 40);
	CAMERAMANAGER->AlphaRender(hdc, IMAGEMANAGER->findImage("작은구름"), _pos.x + IMAGEMANAGER->findImage("큰구름")->getWidth() / 2,
		_pos.y - IMAGEMANAGER->findImage("큰구름")->getHeight() / 2, 40);
	
}

void Cloud::move()
{
	

	if (_pos.y >= IMAGEMANAGER->findImage("townBack")->getHeight() + 500) {
		tagPosF tmp;
		tmp.x = RANDOM->range(100, IMAGEMANAGER->findImage("townBack")->getWidth() + 200);
		tmp.y = RANDOM->range(-2000, -3500);

		_pos = tmp;
	}

	

	_pos.x += cosf(_angle) * _speed;
	_pos.y += -sinf(_angle) * _speed / 2;
}
