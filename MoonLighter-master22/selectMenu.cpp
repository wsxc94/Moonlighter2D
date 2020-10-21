#include "stdafx.h"
#include "selectMenu.h"

HRESULT selectMenu::init()
{
	//선택메뉴 초기화 
	_selectMenu.animTimer = 0;
	_selectMenu.frameUnit = 8;
	_selectMenu.idx = 1;
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

void selectMenu::render(HDC hdc, int destX, int destY)
{
	_selectMenu.img->frameRender(hdc, destX, destY, _selectMenu.idx, 0);

	if (_selectMenu.selectIdx == SELECT_YES)
	{
		RECT txtRC = RectMake(destX + 18, destY + 82, 42, 26);
		HFONT font = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
			0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("JejuGothic"));
		HFONT oFont = (HFONT)SelectObject(hdc, font);
		SetTextColor(hdc, RGB(227, 212, 184));
		DrawText(hdc, "네", -1, &txtRC, DT_CENTER | DT_WORDBREAK | DT_VCENTER);
		SelectObject(hdc, oFont);
		DeleteObject(font);
	}
	else
	{
		RECT txtRC = RectMake(destX + 10, destY + 82, 58, 26);
		HFONT font = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
			0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("JejuGothic"));
		HFONT oFont = (HFONT)SelectObject(hdc, font);
		SetTextColor(hdc, RGB(227, 212, 184));
		DrawText(hdc, "아니요", -1, &txtRC, DT_CENTER | DT_WORDBREAK | DT_VCENTER);
		SelectObject(hdc, oFont);
		DeleteObject(font);
	}
}

void selectMenu::setMenuState(int selectIdx)
{
	switch (selectIdx)
	{
		case SELECT_YES:
			_selectMenu.selectIdx = SELECT_YES;
			_selectMenu.img = IMAGEMANAGER->findImage("select_yes");
			_selectMenu.idx = 1;
			break;

		case SELECT_NO:
			_selectMenu.selectIdx = SELECT_NO;
			_selectMenu.img = IMAGEMANAGER->findImage("select_no");
			_selectMenu.idx = 1;
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
			_selectMenu.idx = 1;
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
			_selectMenu.idx = 1;
		}
	}
}
