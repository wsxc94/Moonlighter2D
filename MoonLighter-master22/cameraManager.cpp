#include "stdafx.h"
#include "cameraManager.h"

HRESULT cameraManager::init(float pivotX, float pivotY, float maxX, float maxY, float minX, float minY, float disX, float disY)
{
	_maxX = maxX;
	_maxY = maxY;
	_minX = minX;
	_minY = minY;
	_pivotX = pivotX;
	_pivotY = pivotY;
	_distanceX = disX;
	_distanceY = disY;
	_absDistanceX = disX;
	_absDistanceY = disY;

	_cameraRect = ::RectMake(_pivotX - disX, _pivotY - disY, WINSIZEX, WINSIZEY);

	_CMState = CAMERAMANAGERSTATE::CAMERAMANAGER_FOLLOWPIVOT;

	return S_OK;
}

void cameraManager::update(float pivotX, float  pivotY)
{
	float angle;
	float _x;
	float _y;
	switch (_CMState)
	{
	case CAMERAMANAGERSTATE::CAMERAMANAGER_FOLLOWPIVOT:

		_pivotX = _moveToPivot.x;
		_pivotY = _moveToPivot.y;
		_cameraRect = ::RectMake(_pivotX - _absDistanceX, _pivotY - _absDistanceY, WINSIZEX, WINSIZEY);
		_distanceX = _absDistanceX;
		_distanceY = _absDistanceY;
		_savePivot.x = _pivotX;
		_savePivot.y = _pivotY;

		if (_cameraRect.left <= _minX)
		{
			_cameraRect.left = _minX;
			_cameraRect.right = _minX + WINSIZEX;
			_distanceX = _pivotX - _cameraRect.left;
		}
		if (_cameraRect.right >= _maxX)
		{
			_cameraRect.right = _maxX;
			_cameraRect.left = _maxX - WINSIZEX;
			_distanceX = _pivotX - _cameraRect.left;
		}
		if (_cameraRect.top <= _minY)
		{
			_cameraRect.top = _minY;
			_cameraRect.bottom = _minY + WINSIZEY;
			_distanceY = _pivotY - _cameraRect.top;

		}
		if (_cameraRect.bottom >= _maxY)
		{
			_cameraRect.bottom = _maxY;
			_cameraRect.top = _maxY - WINSIZEY;
			_distanceY = _pivotY - _cameraRect.top;
		}

		break;
	case CAMERAMANAGERSTATE::CAMERAMANAGER_CHANGEPIVOT:
		angle = getAngle(_pivotX, _pivotY, _changePivotX, _changePivotY);
		if (_pivotX == _changePivotX && _pivotY == _changePivotY)
		{
			_CMState = CAMERAMANAGERSTATE::CAMERAMANAGER_FOLLOWPIVOT;
			break;
		}
		_x = _pivotX;
		_y = _pivotY;
		_savePivot.x = _pivotX;
		_savePivot.y = _pivotY;
		_x += cosf(angle) * _changeSpeed;
		_y -= sinf(angle) * _changeSpeed;
		_cameraRect = ::RectMake(_x - _distanceX, _y - _distanceY, WINSIZEX, WINSIZEY);

		_pivotX = _x;
		_pivotY = _y;

		if (getDistance(_x, _y, _changePivotX, _changePivotY) < _changeSpeed + 1.f)
		{
			_pivotX = _changePivotX;
			_pivotY = _changePivotY;
			pivotX = _pivotX;
			pivotY = _pivotY;
			_savePivot.x = _pivotX;
			_savePivot.y = _pivotY;
			_CMState = CAMERAMANAGERSTATE::CAMERAMANAGER_FOLLOWPIVOT;
		}

		break;
	}



	if (_isShake == true)
	{
		_shakeTime.current++;
		if (_shakeTime.current > _shakeTime.max)
		{
			_isShake = false;
			_cameraRect = ::RectMake(_savePivot.x - _distanceX, _savePivot.y - _distanceY, WINSIZEX, WINSIZEY);
		}
		if (_shakeTime.current % _shakeTime.cool == 0)
		{
			_shakePivot.x = RANDOM->range(_savePivot.x - _shakePower, _savePivot.x + _shakePower);
			_shakePivot.y = RANDOM->range(_savePivot.y - _shakePower, _savePivot.y + _shakePower);
			_cameraRect = ::RectMake(_shakePivot.x - _distanceX, _shakePivot.y - _distanceY, WINSIZEX, WINSIZEY);
		}
	}



}

void cameraManager::release()
{
	/*for (int i = 0; i < _vZoderRender.size(); i++) {
		SAFE_DELETE(_vZoderRender[i]);
	}*/
	_vZoderRender.clear();
}

void cameraManager::Rectangle(HDC hdc, RECT rc)
{
	::Rectangle(hdc, getRelativeX(rc.left), getRelativeY(rc.top), getRelativeX(rc.right), getRelativeY(rc.bottom));
}

void cameraManager::Rectangle(HDC hdc, int left, int top, int width, int height)
{
	::Rectangle(hdc, this->getRelativeX(left), this->getRelativeY(top), this->getRelativeX(left + width), this->getRelativeY(top + height));
}

void cameraManager::FrameRect(HDC hdc, RECT rc, COLORREF color)
{
	rc.left = this->getRelativeX(rc.left);
	rc.right = this->getRelativeX(rc.right);
	rc.top = this->getRelativeY(rc.top);
	rc.bottom = this->getRelativeY(rc.bottom);
	::FrameRect(hdc, rc, color);
}

void cameraManager::LineMake(HDC hdc, int startX, int startY, int endX, int endY)
{
	MoveToEx(hdc, this->getRelativeX(startX), this->getRelativeY(startY), NULL);
	LineTo(hdc, this->getRelativeX(endX), this->getRelativeY(endY));
}

RECT cameraManager::RectMake(float x, float y, int width, int height)
{
	RECT rc = { this->getRelativeX(x), this->getRelativeY(y), this->getRelativeX(x + width), this->getRelativeY(y + height) };

	return rc;
}

void cameraManager::Render(HDC hdc, image* ig, int destX, int destY)
{
	ig->render(hdc, getRelativeX(destX), getRelativeY(destY));
}

void cameraManager::Render(HDC hdc, image * ig, int destX, int destY, int sourX, int sourY, int sourWid, int sourHei)
{
	ig->render(hdc, getRelativeX(destX), getRelativeY(destY), sourX, sourY, sourWid, sourHei);
}

void cameraManager::FrameRender(HDC hdc, image * ig, int destX, int destY, int frameX, int frameY)
{
	ig->frameRender(hdc, this->getRelativeX(destX), this->getRelativeY(destY), frameX, frameY);
}

void cameraManager::StretchRender(HDC hdc, image * ig, int destX, int destY, float size)
{
	ig->stretchRender(hdc, this->getRelativeX(destX), this->getRelativeY(destY), size);
}

void cameraManager::StretchRender(HDC hdc, image * ig, int destX, int destY, float scaleX, float scaleY)
{
	ig->stretchRender(hdc, destX, getRelativeX(destX), getRelativeY(destY), scaleY);
}

void cameraManager::StretchFrameRender(HDC hdc, image * ig, int destX, int destY, int frameX, int frameY, float size)
{
	ig->stretchFrameRender(hdc, this->getRelativeX(destX), this->getRelativeY(destY), frameX, frameY, size);
}

void cameraManager::StretchFrameRender(HDC hdc, image * ig, int destX, int destY, int frameX, int frameY, float scaleX, float scaleY)
{
	ig->stretchFrameRender(hdc, getRelativeX(destX), getRelativeY(destY), frameX, frameY, scaleX, scaleY);
}

void cameraManager::AlphaRender(HDC hdc, image * ig, int destX, int destY, BYTE alpha)
{
	ig->alphaRender(hdc, this->getRelativeX(destX), this->getRelativeY(destY), alpha);
}

void cameraManager::AlphaRender(HDC hdc, image * ig, int destX, int destY, int sourX, int sourY, int sourWid, int sourHei, BYTE alpha)
{
	ig->alphaRender(hdc, getRelativeX(destX), getRelativeY(destY), sourX, sourY, sourWid, sourHei, alpha);
}

void cameraManager::AlphaFrameRender(HDC hdc, image * ig, int destX, int destY, int frameX, int frameY, BYTE alpha)
{
	ig->alphaFrameRender(hdc, this->getRelativeX(destX), this->getRelativeY(destY), frameX, frameY, alpha);
}

void cameraManager::RotateRender(HDC hdc, image * img, int centerX, int centerY, float angle)
{
	img->rotateRender(hdc, this->getRelativeX(centerX), this->getRelativeY(centerY), angle);
}

void cameraManager::RotateFrameRender(HDC hdc, image * img, int centerX, int centerY, float angle, int frameX, int frameY)
{
	img->rotateFrameRender(hdc, this->getRelativeX(centerX), this->getRelativeY(centerY), angle, frameX, frameY);
}

void cameraManager::RotateAlphaRender(HDC hdc, image * img, int centerX, int centerY, float angle, BYTE alpha)
{
	img->rotateAlphaRender(hdc, this->getRelativeX(centerX), this->getRelativeY(centerY), angle, alpha);
}

void cameraManager::RotateAlphaFrameRender(HDC hdc, image * img, int centerX, int centerY, float angle, int frameX, int frameY, BYTE alpha)
{
	img->rotateAlphaFrameRender(hdc, this->getRelativeX(centerX), this->getRelativeY(centerY), angle, frameX, frameY, alpha);
}

void cameraManager::ZorderRender(image * img, float z, int destX, int destY)
{
	//weak_ptr<tagZoderRender> _zo = make_shared<tagZoderRender>((IMG_NOMAL, img, z, getRelativeX(destX), getRelativeY(destY)));
	tagZoderRender _zo(IMG_NOMAL, img, z, getRelativeX(destX), getRelativeY(destY));
	_vZoderRender.push_back(_zo);
}

void cameraManager::ZorderRender(image * img, float z, int destX, int destY, int sourX, int sourY, int sourWid, int sourHei)
{
	tagZoderRender _zo(IMG_NOMAL_SOUR, img, z, getRelativeX(destX), getRelativeY(destY));
	_zo.sourX = sourX;
	_zo.sourY = sourY;
	_zo.sourWid = sourWid;
	_zo.sourHei = sourHei;
	_vZoderRender.push_back(_zo);
}

void cameraManager::ZorderFrameRender(image * img, float z, int destX, int destY, int frameX, int frameY)
{
	tagZoderRender _zo(IMG_FRAME, img, z, getRelativeX(destX), getRelativeY(destY));
	_zo.frameX = frameX;
	_zo.frameY = frameY;
	_vZoderRender.push_back(_zo);
}

void cameraManager::ZorderAlphaRender(image * img, float z, int destX, int destY, BYTE alpha)
{
	tagZoderRender _zo(IMG_ALPHA, img, z, getRelativeX(destX), getRelativeY(destY));
	_zo.alpha = alpha;
	_vZoderRender.push_back(_zo);
}

void cameraManager::ZorderAlphaRender(image * img, float z, int destX, int destY, int sourX, int sourY, int sourWid, int sourHei, BYTE alpha)
{
	tagZoderRender _zo(IMG_ALPHA_SOUR, img, z, getRelativeX(destX), getRelativeY(destY));
	_zo.sourX = sourX;
	_zo.sourY = sourY;
	_zo.sourWid = sourWid;
	_zo.sourHei = sourHei;
	_zo.alpha = alpha;
	_vZoderRender.push_back(_zo);
}

void cameraManager::ZorderAlphaFrameRender(image * img, float z, int destX, int destY, int frameX, int frameY, BYTE alpha)
{
	tagZoderRender _zo(IMG_ALPHA_FRAME, img, z, getRelativeX(destX), getRelativeY(destY));
	_zo.frameX = frameX;
	_zo.frameY = frameY;
	_zo.alpha = alpha;
	_vZoderRender.push_back(_zo);
}

void cameraManager::ZorderRotateRender(image * img, float z, int centerX, int centerY, float angle)
{
	tagZoderRender _zo(IMG_ROTATE_RENDER, img, z, getRelativeX(centerX), getRelativeY(centerY));
	_zo.angle = angle;
	_vZoderRender.push_back(_zo);
}

void cameraManager::ZorderRotateFrameRender(image * img, float z, int centerX, int centerY, float angle, int frameX, int frameY)
{
	tagZoderRender _zo(IMG_ROTATE_FRAME, img, z, getRelativeX(centerX), getRelativeY(centerY));
	_zo.frameX = frameX;
	_zo.frameY = frameY;
	_zo.angle = angle;
	_vZoderRender.push_back(_zo);
}

void cameraManager::ZorderRotateAlphaRender(image * img, float z, int centerX, int centerY, float angle, BYTE alpha)
{
	tagZoderRender _zo(IMG_ROTATE_ALPHA, img, z, getRelativeX(centerX), getRelativeY(centerY));
	_zo.alpha = alpha;
	_zo.angle = angle;
	_vZoderRender.push_back(_zo);
}

void cameraManager::ZorderRotateAlphaFrameRender(image * img, float z, int centerX, int centerY, float angle, int frameX, int frameY, BYTE alpha)
{
	tagZoderRender _zo(IMG_ROTATE_ALPHAFRAME, img, z, getRelativeX(centerX), getRelativeY(centerY));
	_zo.frameX = frameX;
	_zo.frameY = frameY;
	_zo.alpha = alpha;
	_zo.angle = angle;
	_vZoderRender.push_back(_zo);
}

void cameraManager::ZorderStretchRender(image * img, float z, int centerX, int centerY, float scale)
{
	tagZoderRender _zo(IMG_STRETCH_RENDER, img, z, getRelativeX(centerX), getRelativeY(centerY));
	_zo.scale = scale;
	_zo.stretchKind = STRETCH_WHOLE;
	_vZoderRender.push_back(_zo);
}

void cameraManager::ZorderStretchRender(image * img, float z, int centerX, int centerY, float scaleX, float scaleY)
{
	tagZoderRender _zo(IMG_STRETCH_RENDER, img, z, getRelativeX(centerX), getRelativeY(centerY));
	_zo.scaleX = scaleX;
	_zo.scaleY = scaleY;
	_zo.stretchKind = STRETCH_EACH;
	_vZoderRender.push_back(_zo);
}

void cameraManager::ZorderStretchFrameRender(image * img, float z, int centerX, int centerY, int frameX, int frameY, float scale)
{
	tagZoderRender _zo(IMG_STRETCH_FRAME, img, z, getRelativeX(centerX), getRelativeY(centerY));
	_zo.frameX = frameX;
	_zo.frameY = frameY;
	_zo.scale = scale;
	_zo.stretchKind = STRETCH_WHOLE;
	_vZoderRender.push_back(_zo);
}

void cameraManager::ZorderStretchFrameRender(image * img, float z, int centerX, int centerY, int frameX, int frameY, float scaleX, float scaleY)
{
	tagZoderRender _zo(IMG_STRETCH_FRAME, img, z, getRelativeX(centerX), getRelativeY(centerY));
	_zo.frameX = frameX;
	_zo.frameY = frameY;
	_zo.scaleX = scaleX;
	_zo.scaleY = scaleY;
	_zo.stretchKind = STRETCH_EACH;
	_vZoderRender.push_back(_zo);
}

void cameraManager::ZorderRotateStretchRender(image * img, float z, int centerX, int centerY, int frameX, int frameY, float angle, float scale)
{
	tagZoderRender _zo(IMG_ROTATESTRETCH, img, z, getRelativeX(centerX), getRelativeY(centerY));
	_zo.frameX = frameX;
	_zo.frameY = frameY;
	_zo.angle = angle;
	_zo.scale = scale;
	_vZoderRender.push_back(_zo);
}

void cameraManager::ZorderDrawText(string txt, float z, RECT txtRC, HFONT font, COLORREF color, UINT format)
{
	tagZoderRender _zo(IMG_TXT, nullptr, z, 0, 0);
	_zo.txt = txt;
	_zo.txtRC = { txtRC.left - _cameraRect.left,txtRC.top - _cameraRect.top,txtRC.right - _cameraRect.left,txtRC.bottom - _cameraRect.top };
	_zo.font = font;
	_zo.txtColor = color;
	_zo.format = format;
	_vZoderRender.push_back(_zo);
}

void cameraManager::ZorderTextOut(string txt, float z, int x, int y, int size, COLORREF color)
{
	tagZoderRender _zo(IMG_TXTOUT, nullptr, z, getRelativeX(x), getRelativeY(y));
	_zo.txt = txt;
	_zo.txtColor = color;
	_vZoderRender.push_back(_zo);
}

void cameraManager::FadeInit(int time, FADEKIND fadeKind)
{
	IMAGEMANAGER->addImage("fadeImg", WINSIZEX, WINSIZEY);
	_fadeInfo.minus = 255 / time;
	_fadeInfo.fadeKind = fadeKind;
	_fadeInfo.alpha = (fadeKind == FADE_IN) ? 255 : 0;
	_fadeInfo.isStart = false;
}

void cameraManager::FadeStart()
{
	_fadeInfo.isStart = true;
	_fadeInfo.alpha = (_fadeInfo.fadeKind == FADE_IN) ? 255 : 0;
}

void cameraManager::FadeUpdate()
{
	if (_fadeInfo.fadeKind == FADE_IN)
	{
		_fadeInfo.alpha -= _fadeInfo.minus;
		if (_fadeInfo.alpha <= 0) _fadeInfo.isStart = false;
	}
	else
	{
		_fadeInfo.alpha += _fadeInfo.minus;
		if (_fadeInfo.alpha >= 255) _fadeInfo.isStart = false;
	}
}

void cameraManager::FadeRender(HDC hdc)
{
	if(_fadeInfo.isStart)
	IMAGEMANAGER->findImage("fadeImg")->alphaRender(hdc, 0, 0, _fadeInfo.alpha);
}

void cameraManager::ZorderSort()
{

	stable_sort(_vZoderRender.begin(), _vZoderRender.end(), ZordorCompare);
}

void cameraManager::ZorderTotalRender(HDC hdc)
{

	Sort(0, _vZoderRender.size() - 1);

	//sort(_vZoderRender.begin(), _vZoderRender.end() , ZordorCompare);

	for (int i = _vZoderRender.size() - 1; i >= 0; i--)
	{
		switch (_vZoderRender[i].kind)
		{
		case IMG_NOMAL:
			_vZoderRender[i].img->render(hdc, _vZoderRender[i].x, _vZoderRender[i].y);
			break;
		case IMG_NOMAL_SOUR:
			_vZoderRender[i].img->render(hdc, _vZoderRender[i].x, _vZoderRender[i].y, _vZoderRender[i].sourX, _vZoderRender[i].sourY, _vZoderRender[i].sourWid, _vZoderRender[i].sourHei);
			break;
		case IMG_FRAME:
			_vZoderRender[i].img->frameRender(hdc, _vZoderRender[i].x, _vZoderRender[i].y, _vZoderRender[i].frameX, _vZoderRender[i].frameY);
			break;
		case IMG_ALPHA:
			_vZoderRender[i].img->alphaRender(hdc, _vZoderRender[i].x, _vZoderRender[i].y, _vZoderRender[i].alpha);
			break;
		case IMG_ALPHA_SOUR:
			_vZoderRender[i].img->alphaRender(hdc, _vZoderRender[i].x, _vZoderRender[i].y, _vZoderRender[i].sourX, _vZoderRender[i].sourY, _vZoderRender[i].sourWid, _vZoderRender[i].sourHei, _vZoderRender[i].alpha);
			break;
		case IMG_ALPHA_FRAME:
			_vZoderRender[i].img->alphaFrameRender(hdc, _vZoderRender[i].x, _vZoderRender[i].y, _vZoderRender[i].frameX, _vZoderRender[i].frameY, _vZoderRender[i].alpha);
			break;
		case IMG_ROTATE_RENDER:
			_vZoderRender[i].img->rotateRender(hdc, _vZoderRender[i].x, _vZoderRender[i].y, _vZoderRender[i].angle);
			break;
		case IMG_ROTATE_FRAME:
			_vZoderRender[i].img->rotateFrameRender(hdc, _vZoderRender[i].x, _vZoderRender[i].y, _vZoderRender[i].angle, _vZoderRender[i].frameX, _vZoderRender[i].frameY);
			break;
		case IMG_ROTATE_ALPHA:
			_vZoderRender[i].img->rotateAlphaRender(hdc, _vZoderRender[i].x, _vZoderRender[i].y, _vZoderRender[i].angle, _vZoderRender[i].alpha);
			break;
		case IMG_ROTATE_ALPHAFRAME:
			_vZoderRender[i].img->rotateAlphaFrameRender(hdc, _vZoderRender[i].x, _vZoderRender[i].y, _vZoderRender[i].angle, _vZoderRender[i].frameX, _vZoderRender[i].frameY, _vZoderRender[i].alpha);
			break;
		case IMG_STRETCH_RENDER:
			if(_vZoderRender[i].stretchKind == STRETCH_WHOLE)
			_vZoderRender[i].img->stretchRender(hdc, _vZoderRender[i].x, _vZoderRender[i].y, _vZoderRender[i].scale);
			if (_vZoderRender[i].stretchKind == STRETCH_EACH)
				_vZoderRender[i].img->stretchRender(hdc, _vZoderRender[i].x, _vZoderRender[i].y, _vZoderRender[i].scaleX,_vZoderRender[i].scaleY);
			break;
		case IMG_STRETCH_FRAME:
			if(_vZoderRender[i].stretchKind == STRETCH_WHOLE)
			_vZoderRender[i].img->stretchFrameRender(hdc, _vZoderRender[i].x, _vZoderRender[i].y, _vZoderRender[i].frameX, _vZoderRender[i].frameY, _vZoderRender[i].scale);
			if (_vZoderRender[i].stretchKind == STRETCH_EACH)
				_vZoderRender[i].img->stretchFrameRender(hdc, _vZoderRender[i].x, _vZoderRender[i].y, _vZoderRender[i].frameX, _vZoderRender[i].frameY, _vZoderRender[i].scaleX, _vZoderRender[i].scaleY);
			break;
		case IMG_ROTATESTRETCH:
			_vZoderRender[i].img->rotateStretchFrameRender(hdc, _vZoderRender[i].x, _vZoderRender[i].y, _vZoderRender[i].frameX, _vZoderRender[i].frameY, _vZoderRender[i].angle, _vZoderRender[i].scale);
			break;
		case IMG_TXT:
		{
			HFONT font = _vZoderRender[i].font;
			HFONT oFont = (HFONT)SelectObject(hdc, font);
			SetTextColor(hdc, _vZoderRender[i].txtColor);
			DrawText(hdc, _vZoderRender[i].txt.c_str(), -1, &_vZoderRender[i].txtRC, _vZoderRender[i].format);
			SelectObject(hdc, oFont);
			DeleteObject(font);
			SetTextColor(hdc, RGB(255, 255, 255));
		}
			break;
		case IMG_TXTOUT:
		{
			HFONT font = _vZoderRender[i].font;
			HFONT oFont = (HFONT)SelectObject(hdc, font);
			SetTextColor(hdc, _vZoderRender[i].txtColor);
			TextOut(hdc, _vZoderRender[i].x, _vZoderRender[i].y, _vZoderRender[i].txt.c_str(), _vZoderRender[i].txt.size());
			SelectObject(hdc, oFont);
			DeleteObject(font);
			SetTextColor(hdc, RGB(255, 255, 255));
		}
			break;
		}
	}
	_vZoderRender.clear();
}

bool cameraManager::ZordorCompare(const tagZoderRender& z1, const tagZoderRender& z2)
{
	if (z1.z < z2.z)
	{
		return false;
	}
	else if (z1.z == z2.z)
	{
		if (z1.y < z2.y)
		{
			return false;
		}
		else if (z1.y == z2.y)
		{
			if (z1.x < z2.x)
			{
				return false;
			}
			else if (z1.x == z2.x)
			{
				if (&z1 < &z2) return false;
				else if (&z1 == &z2)
				{
					if (z1.kind < z2.kind) return false;
				}
			}
		}
	}
}

void cameraManager::Sort(int i, int j)
{
	if (i >= j) return;

	float pivot = _vZoderRender[(i + j) / 2].z;
	int left = i;
	int right = j;

	while (left <= right)
	{
		while (_vZoderRender[left].z > pivot) left++;
		while (_vZoderRender[right].z < pivot) right--;
		if (left <= right)
		{
			swap(_vZoderRender[left], _vZoderRender[right]);
			left++; right--;
		}
	}

	Sort(i, right);
	Sort(left, j);
}

void cameraManager::textOut(HDC hdc, int x, int y, const char * text, COLORREF color)
{
	SetTextColor(hdc, color);
	TextOut(hdc, this->getRelativeX(x), this->getRelativeY(y), text, strlen(text));
}

void cameraManager::movePivot(float x, float y)
{
	_moveToPivot.x = x;
	_moveToPivot.y = y;
}


void cameraManager::ChangePivot(float x, float y, float speed)
{
	_CMState = CAMERAMANAGERSTATE::CAMERAMANAGER_CHANGEPIVOT;
	_changePivotX = x;
	_changePivotY = y;
	_changeSpeed = speed;
}

void cameraManager::setShake(float power, int time, int cool)
{
	_isShake = true;
	_shakePower = power;
	_shakeTime.current = 0;
	_shakeTime.max = time;
	_shakeTime.cool = cool;
}

int cameraManager::getRelativeX(float x)
{
	return x - _cameraRect.left;
}

int cameraManager::getRelativeY(float y)
{
	return y - _cameraRect.top;
}

POINT cameraManager::getRelativeMouse(POINT ptMouse)
{
	ptMouse.x = ptMouse.x + _cameraRect.left;
	ptMouse.y = ptMouse.y + _cameraRect.top;
	return ptMouse;
}

RECT cameraManager::getRelativeRect(RECT rc)
{
	rc.left = rc.left - _cameraRect.left;
	rc.right = rc.right - _cameraRect.right;
	rc.top = rc.top - _cameraRect.top;
	rc.bottom = rc.bottom - _cameraRect.bottom;
	return rc;
}
