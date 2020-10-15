#include "stdafx.h"
#include "selectMenu.h"

HRESULT selectMenu::init()
{
	//선택메뉴 초기화 
	_selectMenu.animTimer = 0;
	_selectMenu.frameUnit = 6;
	_selectMenu.idx = 0;
	_selectMenu.selectIdx = 0;
	_selectMenu.img = IMAGEMANAGER->findImage("select_no");
	
	return S_OK;
}

void selectMenu::release()
{
}

void selectMenu::update()
{
	animation();
}

void selectMenu::render(HDC hdc)
{
}

void selectMenu::setMenuState(int selectIdx)
{
	switch (selectIdx)
	{
		case SELECT_YES:
			_selectMenu.img = IMAGEMANAGER->findImage("select_yes");
			_selectMenu.idx = 0;
			break;

		case SELECT_NO:
			_selectMenu.img = IMAGEMANAGER->findImage("select_no");
			_selectMenu.idx = 0;
			break;
	}
}

void selectMenu::animation()
{
	switch (_selectMenu.selectIdx)
	{
		case SELECT_YES:
			yesAnim();
			break;

		case SELECT_NO:
			noAnim();
			break; 
	}
}

void selectMenu::yesAnim()
{
	_selectMenu.animTimer++;

	if (_selectMenu.animTimer % _selectMenu.frameUnit == 0)
	{
		if (_selectMenu.idx < _selectMenu.img->getMaxFrameX())
		{
			_selectMenu.idx++;
		}
		else
		{
			_selectMenu.idx = 0;
		}
	}
}

void selectMenu::noAnim()
{
	_selectMenu.animTimer++;

	if (_selectMenu.animTimer % _selectMenu.frameUnit == 0)
	{
		if (_selectMenu.idx < _selectMenu.img->getMaxFrameX())
		{
			_selectMenu.idx++;
		}
		else
		{
			_selectMenu.idx = 0;
		}
	}
}
