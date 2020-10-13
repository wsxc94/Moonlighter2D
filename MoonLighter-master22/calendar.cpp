#include "stdafx.h"
#include "calendar.h"

HRESULT calendar::init()
{
	//캘린더메뉴 위치 초기화 
	initPos();

	_menuMoveSpeed = 90.f;

	return S_OK;
}

void calendar::release()
{
}

void calendar::update()
{
}

void calendar::render(HDC hdc)
{
	IMAGEMANAGER->findImage("menu_calendar")->render(hdc, _calendarPos.x, _calendarPos.y);
}

void calendar::initPos()
{
	_calendarPos.x = -IMAGEMANAGER->findImage("menu_calendar")->getWidth();
	_calendarPos.y = CALENDARPOSY;
}

void calendar::moveCalendarLeft(int destPos)
{
	_calendarPos.x -= _menuMoveSpeed;

	if (_calendarPos.x <= destPos)
	{
		_calendarPos.x = destPos;
	}
}

void calendar::moveCalendarRight(int destPos)
{
	_calendarPos.x += _menuMoveSpeed;

	if (_calendarPos.x >= destPos)
	{
		_calendarPos.x = destPos;
	}
}
