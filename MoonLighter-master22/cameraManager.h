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
	IMG_NOMAL, IMG_NOMAL_SOUR, IMG_FRAME, IMG_ALPHA, IMG_ALPHA_SOUR, IMG_ALPHA_FRAME,
	IMG_ROTATE_RENDER, IMG_ROTATE_FRAME, IMG_ROTATE_ALPHA, IMG_ROTATE_ALPHAFRAME,
	IMG_STRETCH_RENDER,IMG_STRETCH_FRAME, IMG_TXT, IMG_TXTOUT
};

enum STRETCHRENDERKIND
{
	STRETCH_WHOLE, STRETCH_EACH
};

enum FADEKIND
{
	FADE_IN,FADE_OUT
};

struct tagFadeInfo
{
	FADEKIND fadeKind;
	bool isStart;
	int minus;
	int alpha;
};

struct tagZoderRender
{
	ZORDERIMGKIND kind;	// 렌더방법
	STRETCHRENDERKIND stretchKind;
	image* img;
	float x, y;			// 렌더되는 xy 좌표
	float z;				// 정렬기준
	int frameX, frameY;	// 프레임이미지일경우 프레임값
	int sourX, sourY;
	int sourWid, sourHei;
	float angle;			// 회전각
	float scale;			// 확대비율
	float scaleX;
	float scaleY;
	BYTE alpha;			// 알파값
	string txt;			// 텍스트
	HFONT font;			//글자폰트
	RECT txtRC;			//글자를 출력할 위치
	UINT format;			//drawText 포맷
	COLORREF txtColor;		//글자색

	//생성과 동시에초기화
	tagZoderRender(ZORDERIMGKIND _kind, image* _img, int _z, int _x, int _y)
	{
		kind = _kind;
		img = _img;
		z = _z;
		x = _x;
		y = _y;
		frameX = frameY = sourX = sourY = sourWid = sourHei = alpha = angle = scale = alpha = format = NULL;
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

	tagFadeInfo _fadeInfo;

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
	void StretchRender(HDC hdc, image* ig, int destX, int destY, float size);
	void StretchRender(HDC hdc, image* ig, int destX, int destY, float scaleX, float scaleY);
	void StretchFrameRender(HDC hdc, image* ig, int destX, int destY, int frameX, int frameY, float size);
	void StretchFrameRender(HDC hdc, image* ig, int destX, int destY, int frameX, int frameY, float scaleX, float scaleY);
	void AlphaRender(HDC hdc, image* ig, int destX, int destY, BYTE alpha);
	void AlphaRender(HDC hdc, image* ig, int destX, int destY, int sourX, int sourY, int sourWid, int sourHei, BYTE alpha);
	void AlphaFrameRender(HDC hdc, image* ig, int destX, int destY, int frameX, int frameY, BYTE alpha);
	void RotateRender(HDC hdc, image* img, int centerX, int centerY, float angle);
	void RotateFrameRender(HDC hdc, image* img, int centerX, int centerY, float angle, int frameX = 0, int frameY = 0);
	void RotateAlphaRender(HDC hdc, image* img, int centerX, int centerY, float angle, BYTE alpha);
	void RotateAlphaFrameRender(HDC hdc, image* img, int centerX, int centerY, float angle, int frameX, int frameY, BYTE alpha);

	void ZorderRender(image* img, float z, int destX, int destY);
	void ZorderRender(image* img, float z, int destX, int destY, int sourX, int sourY, int sourWid, int sourHei);
	void ZorderFrameRender(image* img, float z, int destX, int destY, int frameX = 0, int frameY = 0);
	void ZorderAlphaRender(image* img, float z, int destX, int destY, BYTE alpha);
	void ZorderAlphaRender(image* img, float z, int destX, int destY, int sourX, int sourY, int sourWid, int sourHei, BYTE alpha);
	void ZorderAlphaFrameRender(image* img, float z, int destX, int destY, int frameX, int frameY, BYTE alpha);
	void ZorderRotateRender(image* img, float z, int centerX, int centerY, float angle);
	void ZorderRotateFrameRender(image* img, float z, int centerX, int centerY, float angle, int frameX = 0, int frameY = 0);
	void ZorderRotateAlphaRender(image* img, float z, int centerX, int centerY, float angle, BYTE alpha);
	void ZorderRotateAlphaFrameRender(image* img, float z, int centerX, int centerY, float angle, int frameX, int frameY, BYTE alpha);
	void ZorderStretchRender(image* img, float z, int centerX, int centerY, float scale);
	void ZorderStretchRender(image* img, float z, int centerX, int centerY, float scaleX, float scaleY);
	void ZorderStretchFrameRender(image* img, float z, int centerX, int centerY, int frameX, int frameY, float scale);
	void ZorderStretchFrameRender(image* img, float z, int centerX, int centerY, int frameX, int frameY, float scaleX, float scaleY);
	void ZorderDrawText(string txt, float z, RECT txtRC, HFONT font, COLORREF color, UINT format);
	void ZorderTextOut(string txt, float z, int x, int y, int size, COLORREF color);


	void FadeInit(int time, FADEKIND fadeKind);
	void FadeStart();
	void FadeUpdate();
	void FadeRender(HDC hdc);
	bool& getFadeIsStart() { return _fadeInfo.isStart; }

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

	bool getIsShake() { return _isShake; }

	RECT getRect() { return _cameraRect; }
	POINT getRelativeMouse(POINT ptMouse);
	RECT getRelativeRect(RECT rc);

	void setCMState(CAMERAMANAGERSTATE state) { _CMState = state; }
	CAMERAMANAGERSTATE getCMState() { return _CMState; }
};