#pragma once
#include "singletonBase.h"
#include "image.h"

enum class CAMERAMANAGERSTATE
{
	CAMERAMANAGER_FOLLOWPIVOT,
	CAMERAMANAGER_CHANGEPIVOT,
	CAMARAMANAGER_SHAKE
};

struct tagShakeTime
{
	int current;
	int max;
	int cool;
};

enum ZORDERIMGKIND
{
	IMG_NOMAL, IMG_NOMAL_SOUR, IMG_FRAME, IMG_ALPHA, IMG_ALPHA_SOUR, IMG_ALPHA_FRAME
};

struct tagZoderRender
{
	ZORDERIMGKIND kind;	// 렌더방법
	image* img;
	float x, y;			// 렌더되는 xy 좌표
	float z;				// 정렬기준
	int frameX, frameY;	// 프레임이미지일경우 프레임값
	int sourX, sourY;
	int sourWid, sourHei;
	BYTE alpha;			// 알파값

	//생성과 동시에초기화
	tagZoderRender(ZORDERIMGKIND _kind, image* _img, int _z, int _x, int _y)
	{
		kind = _kind;
		img = _img;
		z = _z;
		x = _x;
		y = _y;
		frameX = frameY = sourX = sourY = sourWid = sourHei = alpha = 0;
	}

};

class cameraManager : public singletonBase<cameraManager>
{
private:
	CAMERAMANAGERSTATE _CMState;
	RECT _cameraRect;
	float _pivotX, _pivotY;
	float _maxX, _maxY;
	float _minX, _minY;
	float _distanceX, _distanceY;
	float _absDistanceX, _absDistanceY;
	float _changePivotX;
	float _changePivotY;
	float _changeSpeed;
	POINT _moveToPivot;
	POINT _shakePivot;
	POINT _savePivot;
	float _shakePower;
	tagShakeTime _shakeTime;
	bool _isShake;

	vector<tagZoderRender*> _vZoderRender;
public:
	cameraManager() {
		_cameraRect = { 0,0,0,0 };
		_pivotX = 0.f;
		_pivotY = 0.f;
		_maxX = 0.f;
		_maxY = 0.f;
		_minX = 0.f;
		_minY = 0.f;
		_distanceX = 0.f;
		_distanceY = 0.f;
		_isShake = false;
	}
	~cameraManager() {}

	HRESULT init(float pivotX, float pivotY, float maxX, float maxY, float minX, float minY, float disX, float disY);
	void update(float pivotX, float  pivotY);

	void Rectangle(HDC hdc, RECT rc);
	void Rectangle(HDC hdc, int left, int top, int width, int height);
	void FrameRect(HDC hdc, RECT rc, COLORREF color);
	void LineMake(HDC hdc, int startX, int startY, int endX, int endY);
	RECT RectMake(float x, float y, int width, int height);
	void Render(HDC hdc, image* ig, int destX, int destY);
	void Render(HDC hdc, image* ig, int destX, int destY, int sourX, int sourY, int sourWid, int sourHei);
	void FrameRender(HDC hdc, image* ig, int destX, int destY, int frameX = 0, int frameY = 0);
	void StretchRender(HDC hdc, image* ig, int destX, int destY, int sizeX, int sizeY);
	void StretchRender(HDC hdc, image* ig, int destX, int destY, float size);
	void StretchFrameRender(HDC hdc, image* ig, int destX, int destY, int frameX, int frameY, int sizeX, int sizeY);
	void StretchFrameRender(HDC hdc, image* ig, int destX, int destY, int frameX, int frameY, float size);
	void AlphaRender(HDC hdc, image* ig, int destX, int destY, BYTE alpha);
	void AlphaRender(HDC hdc, image* ig, int destX, int destY, int sourX, int sourY, int sourWid, int sourHei, BYTE alpha);
	void AlphaFrameRender(HDC hdc, image* ig, int destX, int destY, int frameX, int frameY, BYTE alpha);

	void ZorderRender(image* img, float z, int destX, int destY);
	void ZorderRender(image* img, float z, int destX, int destY, int sourX, int sourY, int sourWid, int sourHei);
	void ZorderFrameRender(image* img, float z, int destX, int destY, int frameX = 0, int frameY = 0);
	void ZorderAlphaRender(image* img, float z, int destX, int destY, BYTE alpha);
	void ZorderAlphaRender(image* img, float z, int destX, int destY, int sourX, int sourY, int sourWid, int sourHei, BYTE alpha);
	void ZorderAlphaFrameRender(image* img, float z, int destX, int destY, int frameX, int frameY, BYTE alpha);

	void ZorderSort();
	void ZorderTotalRender(HDC hdc);
	static bool ZordorCompare(const tagZoderRender* z1, const tagZoderRender* z2);
	void Sort(int left, int right);


	void textOut(HDC hdc, int x, int y, const char* text, COLORREF color = RGB(255, 0, 0));

	void movePivot(float x, float y);
	void ChangePivot(float x, float y, float speed);
	void setShake(float power, int time, int cool);
	int getRelativeX(float x);
	int getRelativeY(float y);
	float getPivotX() { return _pivotX; }
	float getPivotY() { return _pivotY; }
	float getDistanceX() { return _distanceX; }
	float getDistanceY() { return _distanceY; }

	RECT getRect() { return _cameraRect; }
	POINT getRelativeMouse(POINT ptMouse);
	RECT getRelativeRect(RECT rc);

	void setCMState(CAMERAMANAGERSTATE state) { _CMState = state; }
	CAMERAMANAGERSTATE getCMState() { return _CMState; }
};