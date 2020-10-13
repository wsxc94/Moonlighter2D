#include "stdafx.h"
#include "animation.h"

HRESULT animation::init(image * img,int frameY, int aniFrame, bool isLoop, bool isReverse)
{
	_img = img;
	_isLoop = isLoop;
	_isReverse = isReverse;
	_aniFrame = aniFrame;
	_aniState = ANIMATION_PLAY;
	_curIndex = 0;
	_aniCount = 0;
	_maxFrameX = _img->getMaxFrameX();
	_maxFrameY = _img->getMaxFrameY();
	_frameY = frameY;
	_isArray = false;
	for (int i = 0; i <= _maxFrameX; i++)
	{
		POINT temp = { i,frameY };
		_aniList.push_back(temp);
	}
	if (_isReverse)
	{
		for (int i = _maxFrameX; i >= 0; i--)
		{
			POINT temp = { i,frameY };
			_aniList.push_back(temp);
		}
	}
	
	return S_OK;
}

HRESULT animation::init(image * img, int frameY, int aniFrame, RECT rc, string key, bool isLoop, bool isReverse)
{
	_img = img;
	_isLoop = isLoop;
	_isReverse = isReverse;
	_aniFrame = aniFrame;
	_aniState = ANIMATION_PLAY;
	_curIndex = 0;
	_aniCount = 0;
	_maxFrameX = _img->getMaxFrameX();
	_maxFrameY = _img->getMaxFrameY();
	_frameY = frameY;
	_isArray = false;
	_rc = rc;
	_key = key;

	for (int i = 0; i <= _maxFrameX; i++)
	{
		POINT temp = { i,frameY };
		_aniList.push_back(temp);
	}
	if (_isReverse)
	{
		for (int i = _maxFrameX; i >= 0; i--)
		{
			POINT temp = { i,frameY };
			_aniList.push_back(temp);
		}
	}

	return S_OK;
}

HRESULT animation::initArray(vector<POINT> aniList,image* img,int aniFrame, bool isLoop, bool isReverse)
{
	_img = img;
	_aniList = aniList;
	_isLoop = isLoop;
	_isReverse = isReverse;
	_aniFrame = aniFrame;
	_aniState = ANIMATION_PLAY;
	_curIndex = 0;
	_aniCount = 0;
	_maxFrameX = _img->getMaxFrameX();
	_maxFrameY = _img->getMaxFrameY();
	_isArray = true;
	if (isReverse)
	{
		for (int i = aniList.size() - 1; i >= 0; i--)
		{
			_aniList.push_back(aniList[i]);
		}
	}

	return S_OK;
}

void animation::update()
{
	_aniCount++;
	if (_aniCount % _aniFrame != 0) return;

	switch (_aniState)
	{
	case ANIMATION_PLAY:
		_aniCount = 0;
		_curIndex++;
		if (_curIndex > _aniList.size() - 1)
		{
			if (_isLoop)
			{
				_curIndex = 0;
			}
			else
			{
				_curIndex--;
				_aniState = ANIMATION_END;
			}
		}
		break;
	case ANIMATION_PAUSE:

		break;
	case ANIMATION_END:
		break;
	}
	
}

void animation::render(HDC hdc,int destX, int destY)
{
	if(!_isArray)	_img->frameRender(hdc, destX, destY, _aniList[_curIndex].x, _frameY	);
	if (_isArray) _img->frameRender(hdc, destX, destY, _aniList[_curIndex].x, _aniList[_curIndex].y);
}

void animation::CameraRender(HDC hdc, int destX, int destY)
{
	if (!_isArray)	CAMERAMANAGER->FrameRender(hdc, _img, destX, destY, _aniList[_curIndex].x, _frameY);
	if (_isArray) CAMERAMANAGER->FrameRender(hdc, _img, destX, destY, _aniList[_curIndex].x, _aniList[_curIndex].y);
}

void animation::ZoderRender(int z, int destX, int destY)
{
	if (!_isArray)	CAMERAMANAGER->ZorderFrameRender(_img,z, destX, destY, _aniList[_curIndex].x, _frameY);
	if (_isArray) CAMERAMANAGER->ZorderFrameRender(_img,z, destX, destY, _aniList[_curIndex].x, _aniList[_curIndex].y);
}

void animation::ZoderAlphaRender(int z, int destX, int destY, BYTE alpha)
{
	if (!_isArray)	CAMERAMANAGER->ZorderAlphaFrameRender(_img, z, destX, destY, _aniList[_curIndex].x, _frameY , alpha);
	if (_isArray) CAMERAMANAGER->ZorderAlphaFrameRender(_img, z, destX, destY, _aniList[_curIndex].x, _aniList[_curIndex].y, alpha);
}


void animation::aniPlay()
{
	_aniState = ANIMATION_PLAY;
}

void animation::aniPause()
{
	_aniState = ANIMATION_PAUSE;
}

void animation::aniStop()
{
	_aniState = ANIMATION_END;
	_curIndex = 0;
	_aniCount = 0;
}

void animation::aniRestart()
{
	_curIndex = 0;
	_aniCount = 0;
	_aniState = ANIMATION_PLAY;
}
