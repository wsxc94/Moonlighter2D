#include "stdafx.h"
#include "object.h"

HRESULT object::init(int x, int y, string key)
{
	_imgKey = key;

	if (IMAGEMANAGER->findImage(key)->getMaxFrameX() == 0 && IMAGEMANAGER->findImage(key)->getMaxFrameY() == 0) // 프레임 이미지가 아닐때
	{
		_rc = RectMake(x, y, IMAGEMANAGER->findImage(key)->getWidth(), IMAGEMANAGER->findImage(key)->getHeight());
	}
	else //프레임 이미지일떄
	{
		_rc = RectMake(x, y, IMAGEMANAGER->findImage(key)->getFrameWidth(), IMAGEMANAGER->findImage(key)->getFrameHeight());
	}

	return S_OK;
}

void object::release()
{
}

void object::update()
{
}

void object::render()
{
	if (IMAGEMANAGER->findImage(_imgKey)->getMaxFrameX() == 0 && IMAGEMANAGER->findImage(_imgKey)->getMaxFrameY() == 0) // 프레임 이미지가 아닐때
	{
		//IMAGEMANAGER->findImage(_imgKey)->render(getMemDC(), _rc.left, _rc.top);
		//CAMERAMANAGER->Render(getMemDC(),IMAGEMANAGER->findImage(_imgKey), _rc.left, _rc.top);
		CAMERAMANAGER->ZorderRender(IMAGEMANAGER->findImage(_imgKey), (_rc.top + _rc.bottom) / 2, _rc.left, _rc.top);
	}
	else													//프레임 이미지일떄
	{
		//IMAGEMANAGER->findImage(_imgKey)->frameRender(getMemDC(), _rc.left, _rc.top);
		//CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage(_imgKey), _rc.left, _rc.top);
		//CAMERAMANAGER->ZorderFrameRender(IMAGEMANAGER->findImage(_imgKey), (_rc.bottom + _rc.top) / 2, _rc.left, _rc.top);
	}
}

HRESULT dgDoor::init(int x, int y, string key,int frameY)
{
	_kind = DOOR_NOMAL;
	_imgKey = key;
	_rc = RectMake(x, y, IMAGEMANAGER->findImage(key)->getFrameWidth(), IMAGEMANAGER->findImage(key)->getFrameHeight());
	_close = new animation;
	_close->init(IMAGEMANAGER->findImage(key), frameY, 7);
	_close->aniStop();
	_open = new animation;
	vector<POINT> _temp;
	for (int i = IMAGEMANAGER->findImage(key)->getMaxFrameX(); i >= 0; i--)
	{
		_temp.push_back(PointMake(i, frameY));
	}
	_open->initArray(_temp, IMAGEMANAGER->findImage(key), 7);

	_isOpen = false;
	_isClose = false;

	return S_OK;
}

void dgDoor::release()
{
	SAFE_DELETE(_open);
	SAFE_DELETE(_close);
}

void dgDoor::update()
{
	if(_isOpen) _open->update();
	_close->update();
}

void dgDoor::render()
{
	//_close->CameraRender(getMemDC(), _rc.left, _rc.top);
	_close->ZoderRender(0, _rc.left, _rc.top);
	if(_isOpen) _open->ZoderRender(1, _rc.left, _rc.top);
}

void dgDoor::openPlay()
{
	_isOpen = true;
	_open->aniPlay();
}

void dgDoor::closePlay()
{
	_isClose = true;
	_close->aniPlay();
}

HRESULT dgChest::init(int x, int y, string key)
{
	return E_NOTIMPL;
}

void dgChest::release()
{
}

void dgChest::update()
{
}

void dgChest::render()
{
}

void dgChest::openPlay()
{
}

HRESULT dgBossDoor::init(int x, int y, string key, int frameY)
{
	_kind = DOOR_BOSS;
	_imgKey = key;
	_x = x + IMAGEMANAGER->findImage(key)->getFrameWidth();
	_y = y + IMAGEMANAGER->findImage(key)->getFrameHeight();

	if (IMAGEMANAGER->findImage(key)->getFrameHeight() > IMAGEMANAGER->findImage(key)->getFrameWidth())
	{
		if (frameY == 1)
			_rc = RectMake(x, y, 97, 192);
		else
			_rc = RectMake(x + 11, y, 97, 192);
	}
	else
	{
		_rc = RectMake(x, y, 192, 97);
	}

	

	_open = new animation;
	vector<POINT> _temp;
	for (int i = IMAGEMANAGER->findImage(key)->getMaxFrameX(); i >= 0; i--)
	{
		_temp.push_back(PointMake(i, frameY));
	}
	_open->initArray(_temp, IMAGEMANAGER->findImage(key), 7);

	_isOpen = false;
	_isClose = false;

	return S_OK;
}

void dgBossDoor::release()
{
	SAFE_DELETE(_open);
}

void dgBossDoor::update()
{
	if(_isOpen)
	_open->update();
}

void dgBossDoor::render()
{
	_open->ZorderStretchRender(2000, _x, _y,2.f);
	//FrameRect(getMemDC(), _rc,RGB(255,255,255));
}

void dgBossDoor::openPlay()
{
	_isOpen = true;
}

void dgBossDoor::closePlay()
{
}
