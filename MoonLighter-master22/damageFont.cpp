#include "stdafx.h"
#include "damageFont.h"

HRESULT damageFont::init(int x, int y, int damage)
{
	dFont* font = new dFont;
	font->init(x, y, damage);
	_vFont.push_back(font);
	return S_OK;
}

void damageFont::release()
{
	for (int i = 0; i < _vFont.size(); i++)
	{
		SAFE_DELETE(_vFont[i]);
	}
	_vFont.clear();
}

void damageFont::update()
{
	for (int i = 0; i < _vFont.size();)
	{
		if (!_vFont[i]->update())
		{
			_vFont[i]->release();
			SAFE_DELETE(_vFont[i]);
			_vFont.erase(_vFont.begin() + i);
		}
		else i++;
	}
}

void damageFont::render(HDC hdc)
{
	for (int i = 0; i < _vFont.size(); i++)
	{
		_vFont[i]->render(hdc);
	}
}


HRESULT dFont::init(int x, int y, int damage)
{
	_x = x;
	_y = y;
	_damage = damage;
	_time = 0;
	_back = new image;
	_back->init(100, 100);
	_back->setIsTrans(true);
	_back->setTransColor(RGB(0, 0, 0));
	_alpha = 255;



	return S_OK;
}

void dFont::release()
{
	SAFE_DELETE(_back);
}

bool dFont::update()
{
	_time++;
	if (_time > 60) return false;

	_y -= 0.5f;
	_alpha -= 3;
	if (_alpha <= 0) _alpha = 0;
	return true;
}

void dFont::render(HDC hdc)
{
	HFONT hFont = CreateFont(22, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
		0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("JejuGothic"));
	HFONT oFont = (HFONT)SelectObject(_back->getMemDC(), hFont);
	SelectObject(_back->getMemDC(), hFont);
	SetTextColor(_back->getMemDC(), RGB(255, 0, 0));
	SetBkColor(_back->getMemDC(), 0);
	TextOut(_back->getMemDC(), 0, 0, to_string(_damage).c_str(), to_string(_damage).size());
	CAMERAMANAGER->AlphaRender(hdc,_back, _x, _y,_alpha);
	SelectObject(_back->getMemDC(), oFont);
	DeleteObject(hFont);

}
