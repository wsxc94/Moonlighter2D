#include "stdafx.h"
#include "noteBook.h"

HRESULT noteBook::init()
{
	//노트북메뉴 위치 초기화 
	initPos();

	_menuMoveSpeed = 90.f;

	return S_OK;
}

void noteBook::release()
{
}

void noteBook::update()
{
}

void noteBook::render(HDC hdc)
{
	IMAGEMANAGER->findImage("menu_notebook")->render(hdc, _noteBookPos.x, _noteBookPos.y);
}

void noteBook::initPos()
{
	_noteBookPos.x = WINSIZEX;
	_noteBookPos.y = NOTEBOOKPOSY;
}

void noteBook::moveNoteBookLeft(int destPos)
{
	_noteBookPos.x -= _menuMoveSpeed;

	if (_noteBookPos.x <= destPos)
	{
		_noteBookPos.x = destPos;
	}
}

void noteBook::moveNoteBookRight(int destPos)
{
	_noteBookPos.x += _menuMoveSpeed;

	if (_noteBookPos.x >= destPos)
	{
		_noteBookPos.x = destPos;
	}
}
