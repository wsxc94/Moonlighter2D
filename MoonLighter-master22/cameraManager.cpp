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

void cameraManager::StretchRender(HDC hdc, image * ig, int destX, int destY, int sizeX, int sizeY)
{
	ig->stretchRender(hdc, this->getRelativeX(destX), this->getRelativeY(destY), sizeX, sizeY);
}

void cameraManager::StretchRender(HDC hdc, image * ig, int destX, int destY, float size)
{
	ig->stretchRender(hdc, this->getRelativeX(destX), this->getRelativeY(destY), size);
}

void cameraManager::StretchFrameRender(HDC hdc, image * ig, int destX, int destY, int frameX, int frameY, int sizeX, int sizeY)
{
	ig->stretchFrameRender(hdc, this->getRelativeX(destX), this->getRelativeY(destY), frameX, frameY, sizeX, sizeY);
}

void cameraManager::StretchFrameRender(HDC hdc, image * ig, int destX, int destY, int frameX, int frameY, float size)
{
	ig->stretchFrameRender(hdc, this->getRelativeX(destX), this->getRelativeY(destY), frameX, frameY, size);
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

void cameraManager::ZorderRender(image * img, float z, int destX, int destY)
{
	tagZoderRender* _zo = new tagZoderRender(IMG_NOMAL, img, z, getRelativeX(destX), getRelativeY(destY));
	_vZoderRender.push_back(_zo);
}

void cameraManager::ZorderRender(image * img, float z, int destX, int destY, int sourX, int sourY, int sourWid, int sourHei)
{
	tagZoderRender* _zo = new tagZoderRender(IMG_NOMAL_SOUR, img, z, getRelativeX(destX), getRelativeY(destY));
	_zo->sourX = sourX;
	_zo->sourY = sourY;
	_zo->sourWid = sourWid;
	_zo->sourHei = sourHei;
	_vZoderRender.push_back(_zo);
}

void cameraManager::ZorderFrameRender(image * img, float z, int destX, int destY, int frameX, int frameY)
{
	tagZoderRender* _zo = new tagZoderRender(IMG_FRAME, img, z, getRelativeX(destX), getRelativeY(destY));
	_zo->frameX = frameX;
	_zo->frameY = frameY;
	_vZoderRender.push_back(_zo);
}

void cameraManager::ZorderAlphaRender(image * img, float z, int destX, int destY, BYTE alpha)
{
	tagZoderRender* _zo = new tagZoderRender(IMG_ALPHA, img, z, getRelativeX(destX), getRelativeY(destY));
	_zo->alpha = alpha;
	_vZoderRender.push_back(_zo);
}

void cameraManager::ZorderAlphaRender(image * img, float z, int destX, int destY, int sourX, int sourY, int sourWid, int sourHei, BYTE alpha)
{
	tagZoderRender* _zo = new tagZoderRender(IMG_ALPHA_SOUR, img, z, getRelativeX(destX), getRelativeY(destY));
	_zo->sourX = sourX;
	_zo->sourY = sourY;
	_zo->sourWid = sourWid;
	_zo->sourHei = sourHei;
	_zo->alpha = alpha;
	_vZoderRender.push_back(_zo);
}

void cameraManager::ZorderAlphaFrameRender(image * img, float z, int destX, int destY, int frameX, int frameY, BYTE alpha)
{
	tagZoderRender* _zo = new tagZoderRender(IMG_ALPHA_FRAME, img, z, getRelativeX(destX), getRelativeY(destY));
	_zo->frameX = frameX;
	_zo->frameY = frameY;
	_zo->alpha = alpha;
	_vZoderRender.push_back(_zo);
}

void cameraManager::ZorderSort()
{

	stable_sort(_vZoderRender.begin(), _vZoderRender.end(), ZordorCompare);
}

void cameraManager::ZorderTotalRender(HDC hdc)
{
	Sort(0,_vZoderRender.size() -1);
	for (int i = _vZoderRender.size() - 1; i >= 0; i--)
	{
		switch (_vZoderRender[i]->kind)
		{
		case IMG_NOMAL:
			_vZoderRender[i]->img->render(hdc, _vZoderRender[i]->x, _vZoderRender[i]->y);
			break;
		case IMG_NOMAL_SOUR:
			_vZoderRender[i]->img->render(hdc, _vZoderRender[i]->x, _vZoderRender[i]->y, _vZoderRender[i]->sourX, _vZoderRender[i]->sourY, _vZoderRender[i]->sourWid, _vZoderRender[i]->sourHei);
			break;
		case IMG_FRAME:
			_vZoderRender[i]->img->frameRender(hdc, _vZoderRender[i]->x, _vZoderRender[i]->y, _vZoderRender[i]->frameX, _vZoderRender[i]->frameY);
			break;
		case IMG_ALPHA:
			_vZoderRender[i]->img->alphaRender(hdc, _vZoderRender[i]->x, _vZoderRender[i]->y, _vZoderRender[i]->alpha);
			break;
		case IMG_ALPHA_SOUR:
			_vZoderRender[i]->img->alphaRender(hdc, _vZoderRender[i]->x, _vZoderRender[i]->y, _vZoderRender[i]->sourX, _vZoderRender[i]->sourY, _vZoderRender[i]->sourWid, _vZoderRender[i]->sourHei, _vZoderRender[i]->alpha);
			break;
		case IMG_ALPHA_FRAME:
			_vZoderRender[i]->img->alphaFrameRender(hdc, _vZoderRender[i]->x, _vZoderRender[i]->y, _vZoderRender[i]->frameX, _vZoderRender[i]->frameY, _vZoderRender[i]->alpha);
			break;
		}
	}
	_vZoderRender.clear();
}

bool cameraManager::ZordorCompare(const tagZoderRender * z1, const tagZoderRender *z2)
{
	if (z1->z < z2->z)
	{
		return false;
	}
	else if (z1->z == z2->z)
	{
		if (z1->y < z2->y)
		{
			return false;
		}
		else if (z1->y == z2->y)
		{
			if (z1->x < z2->x)
			{
				return false;
			}
			else if (z1->x == z2->x)
			{
				if (&z1 < &z2) return false;
				else if (&z1 == &z2)
				{
					if (z1->kind < z2->kind) return false;
				}
			}
		}
	}
}

void cameraManager::Sort(int i, int j)
{
	if (i >= j) return;

	float pivot = _vZoderRender[(i + j) / 2]->z;
	int left = i;
	int right = j;

	while (left <= right)
	{
		while (_vZoderRender[left]->z > pivot) left++;
		while (_vZoderRender[right]->z < pivot) right--;
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
