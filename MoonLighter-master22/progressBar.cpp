#include "stdafx.h"
#include "progressBar.h"

HRESULT progressBar::init(const char * frontImg, const char * backImg, float maxGauge)
{
	//체력바 이미지 초기화
	_progressBarFront = IMAGEMANAGER->findImage(frontImg);
	_progressBarBack = IMAGEMANAGER->findImage(backImg);

	//체력바 가로, 세로길이 초기화
	_width = _progressBarFront->getWidth();
	_height = _progressBarFront->getHeight();
	
	_maxGauge = maxGauge;

	return S_OK;
}

void progressBar::release()
{
}

void progressBar::update(float currentHp)
{
	//체력바 렉트 업데이트
	_width = (currentHp / _maxGauge) * _progressBarFront->getWidth();
}

void progressBar::render(int x, int y)
{
	//렌더링 되는 순서에 의해서 렌더가 되니까 피통부터 렌더 시킨다
	RECT back = RectMakeCenter(x, y, _progressBarBack->getWidth(), _progressBarBack->getHeight());
	_progressBarBack->render(getMemDC(), back.left, back.top);
	//앞에 보여지는 체력바 이미지
	RECT front = RectMakeCenter(x, y, _progressBarFront->getWidth(), _progressBarFront->getHeight());
	_progressBarFront->render(getMemDC(), front.left, front.top, 0, 0, _width, _height);
}

void progressBar::alphaRender(int x, int y, BYTE alpha)
{
	RECT back = RectMakeCenter(x, y, _progressBarBack->getWidth(), _progressBarBack->getHeight());
	_progressBarBack->alphaRender(getMemDC(), back.left, back.top, alpha);
	//앞에 보여지는 체력바 이미지
	RECT front = RectMakeCenter(x, y, _progressBarFront->getWidth(), _progressBarFront->getHeight());
	_progressBarFront->alphaRender(getMemDC(), front.left, front.top, 0, 0, _width, _height,alpha);
}

void progressBar::cameraAlphaRender(int x, int y, BYTE alpha)
{
	RECT back = RectMakeCenter(x, y, _progressBarBack->getWidth(), _progressBarBack->getHeight());
	CAMERAMANAGER->AlphaRender(getMemDC(), _progressBarBack, back.left, back.top,alpha);
	CAMERAMANAGER->ZorderAlphaRender(_progressBarBack, 2000, back.left, back.top, alpha);
	//앞에 보여지는 체력바 이미지
	RECT front = RectMakeCenter(x, y, _progressBarFront->getWidth(), _progressBarFront->getHeight());
	CAMERAMANAGER->AlphaRender(getMemDC(), _progressBarFront, front.left, front.top, 0, 0, _width, _height,alpha);
	CAMERAMANAGER->ZorderAlphaRender(_progressBarFront, 2001, front.left, front.top,0,0,_width,_height, alpha);
}

void progressBar::ZorderRender(float z, int x, int y)
{

}

void progressBar::ZorderAlphaRender(float z, int x, int y)
{
}

void progressBar::cameraRender(int x, int y)
{
	//렌더링 되는 순서에 의해서 렌더가 되니까 피통부터 렌더 시킨다
	RECT back = RectMakeCenter(x, y, _progressBarBack->getWidth(), _progressBarBack->getHeight());
	 CAMERAMANAGER->ZorderRender(_progressBarBack,2001, back.left, back.top);
	//앞에 보여지는 체력바 이미지
	RECT front = RectMakeCenter(x, y, _progressBarFront->getWidth(), _progressBarFront->getHeight());
	CAMERAMANAGER->ZorderRender(_progressBarFront,2001, front.left, front.top, 0, 0, _width, _height);
}

