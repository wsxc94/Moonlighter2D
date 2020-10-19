#include "stdafx.h"
#include "movingObject.h"

HRESULT movingObject::init()
{
	memset(&_movingObj, 0, sizeof(_movingObj));

	return S_OK;
}

HRESULT movingObject::init(image * img, int posX, int posY, float speed)
{
	_movingObj.img = img;
	_movingObj.pos.x = posX;
	_movingObj.pos.y = posY;
	_movingObj.movingSpeed = speed;
	_movingObj.isArrived = false; 

	return S_OK;
}

void movingObject::release()
{
}

void movingObject::update()
{
}

void movingObject::render(HDC hdc)
{
	_movingObj.img->render(hdc, _movingObj.pos.x, _movingObj.pos.y);
}

void movingObject::initPos(int posX, int posY)
{
	_movingObj.pos.x = posX;
	_movingObj.pos.y = posY;
}

void movingObject::moveLeft(const int destPos)
{
	_movingObj.pos.x -= _movingObj.movingSpeed;

	if (_movingObj.pos.x < destPos)
	{
		_movingObj.pos.x = destPos;
		_movingObj.isArrived = true; 
	}
}

void movingObject::moveRight(const int destPos)
{
	_movingObj.pos.x += _movingObj.movingSpeed;

	if (_movingObj.pos.x > destPos)
	{
		_movingObj.pos.x = destPos;
		_movingObj.isArrived = true;
	}
}

void movingObject::moveUp(const int destPos)
{
	_movingObj.pos.y -= _movingObj.movingSpeed;

	if (_movingObj.pos.y < destPos)
	{
		_movingObj.pos.y = destPos;
		_movingObj.isArrived = true;
	}
}

void movingObject::moveDown(const int destPos)
{
	_movingObj.pos.y += _movingObj.movingSpeed;

	if (_movingObj.pos.y > destPos)
	{
		_movingObj.pos.y = destPos;
		_movingObj.isArrived = true;
	}
}

