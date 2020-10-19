#include "stdafx.h"
#include "fadeManager.h"

HRESULT fadeManager::init()
{
	memset(&_fadeInfo, 0, sizeof(_fadeInfo));

	return S_OK;
}

void fadeManager::release()
{
}

void fadeManager::update()
{
	if (!_fadeInfo.isActive) return;

	if (_fadeInfo.kind == FADE_IN)
	{
		_fadeInfo.alpha -= _fadeInfo.fadeUnit;

		if (_fadeInfo.alpha <= _fadeInfo.destAlpha)
		{
			_fadeInfo.alpha = _fadeInfo.destAlpha;
			if (_fadeInfo.alpha == 0) _fadeInfo.isActive = false;
		}
	}
	else
	{
		_fadeInfo.alpha += _fadeInfo.fadeUnit;

		if (_fadeInfo.alpha >= _fadeInfo.destAlpha) _fadeInfo.alpha = _fadeInfo.destAlpha;
	}
}

void fadeManager::render(HDC hdc)
{
	if (!_fadeInfo.isActive) return;

	IMAGEMANAGER->findImage("fadeImg")->alphaRender(hdc, 0, 0, _fadeInfo.alpha);
}

void fadeManager::fadeInit(int fadeUnit, FADEKIND kind)
{
	_fadeInfo.kind = kind;
	_fadeInfo.fadeUnit = fadeUnit;

	//현재 활성화상태가 아닐 때는 알파 값을 초기화 
	if (!_fadeInfo.isActive) _fadeInfo.alpha = (_fadeInfo.kind == FADE_IN) ? 255 : 0;
	
	_fadeInfo.destAlpha = (_fadeInfo.kind == FADE_IN) ? 0 : 255;
	_fadeInfo.isActive = true; 
}

void fadeManager::fadeInit(int fadeUnit, FADEKIND kind, int destAlpha)
{
	_fadeInfo.kind = kind;
	_fadeInfo.fadeUnit = fadeUnit;

	//현재 활성화상태가 아닐 때는 알파 값을 초기화 
	if (!_fadeInfo.isActive) _fadeInfo.alpha = (_fadeInfo.kind == FADE_IN) ? 255 : 0;

	_fadeInfo.destAlpha = destAlpha;
	_fadeInfo.isActive = true;
}
