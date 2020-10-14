#include "stdafx.h"
#include "cursor.h"

HRESULT cursor::init()
{
	//커서 초기화 
	_cursor.img = IMAGEMANAGER->findImage("cursor_move");
	_cursor.state = CURSOR_IDLE;
	_cursor.idx = 2;
	_cursor.slotIdx = 0;
	_cursor.clickTime = 0;

	_animTimer = 0;
	_moveFrameUnit = 4; 
	_clickFrameUnit = 3;

	_showCursor = false;

	return S_OK;
}

void cursor::release()
{
}

void cursor::update()
{
	animation();
}

void cursor::render(HDC hdc)
{
}

void cursor::setCursorState(CURSOR_STATE state)
{
	switch (state)
	{
		case CURSOR_IDLE:
			_cursor.img = IMAGEMANAGER->findImage("cursor_move");
			_cursor.idx = _cursor.img->getMaxFrameX() - 1;
			_cursor.state = state;
			break;

		case CURSOR_INACTIVE:
			_cursor.img = IMAGEMANAGER->findImage("cursor_move");
			_cursor.idx = _cursor.img->getMaxFrameX();
			_cursor.state = state;
			break;

		case CURSOR_MOVE:
			_cursor.img = IMAGEMANAGER->findImage("cursor_move");
			_cursor.idx = 0;
			_cursor.state = state;

			break;

		case CURSOR_CLICK:
			_cursor.img = IMAGEMANAGER->findImage("cursor_click");
			_cursor.idx = 0;
			_cursor.state = state;
			break;

		case CURSOR_PRICE_OFF:
			_cursor.img = IMAGEMANAGER->findImage("cursor_price");
			_cursor.idx = 0;
			_cursor.state = state;
			break;

		case CURSOR_PRICE_ON:
			_cursor.img = IMAGEMANAGER->findImage("cursor_price");
			_cursor.idx = 1;
			_cursor.state = state;
			break;

		case CURSOR_SELECT_IDLE:
			_cursor.img = IMAGEMANAGER->findImage("cursor_select");
			_cursor.idx = _cursor.img->getMaxFrameX();
			_cursor.state = state;
			break;

		case CURSOR_SELECT_MOVE:
			_cursor.img = IMAGEMANAGER->findImage("cursor_select");
			_cursor.idx = 0;
			_cursor.state = state;
			break;
	}
}

void cursor::animation()
{
	switch (_cursor.state)
	{
		case CURSOR_MOVE:
			moveAnim();
			break;

		case CURSOR_CLICK:
			clickAnim();
			break;

		case CURSOR_SELECT_MOVE:
			selectMoveAnim();
			break;
	}
}

void cursor::moveAnim()
{
	_animTimer++;

	if (_animTimer % _moveFrameUnit == 0)
	{
		if (_cursor.idx < _cursor.img->getMaxFrameX() - 1)
		{
			_cursor.idx++;
		}
		else
		{
			setCursorState(CURSOR_IDLE);
		}
	}
}

void cursor::clickAnim()
{
	_animTimer++;

	if (_animTimer % _clickFrameUnit == 0)
	{
		if (_cursor.idx < _cursor.img->getMaxFrameX())
		{
			_cursor.idx++;
		}
		else
		{
			setCursorState(CURSOR_IDLE);
		}
	}
}

void cursor::selectMoveAnim()
{
	_animTimer++;

	if (_animTimer % _clickFrameUnit == 0)
	{
		if (_cursor.idx < _cursor.img->getMaxFrameX())
		{
			_cursor.idx++;
		}
		else
		{
			setCursorState(CURSOR_SELECT_IDLE);
		}
	}
}
