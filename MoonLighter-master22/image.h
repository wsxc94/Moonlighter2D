#pragma once
//=============================================================
//	## image ## (앞으로 계속 업데이트 된다)
//=============================================================

class image
{
public:
	enum IMAGE_LOAD_KIND
	{
		LOAD_EMPTY,		//빈비트맵 로딩
		LOAD_FILE		//파일로 로딩
	};

	typedef struct tagImage
	{
		HDC hMemDC;			//메모리 DC
		HBITMAP hBit;		//비트맵
		HBITMAP hOBit;		//올드비트맵
		float x;				//이미지 x좌표
		float y;				//이미지 y좌표
		int width;			//이미지 가로길이
		int height;			//이미지 세로길이
		int currentFrameX;	//현재 프레임X
		int currentFrameY;	//현재 프레임Y
		int maxFrameX;		//최대 프레임X 갯수
		int maxFrameY;		//최대 프레임Y 갯수
		int frameWidth;		//1프레임 가로길이
		int frameHeight;	//1프레임 세로길이
		BYTE loadType;		//이미지 로드타입
		tagImage()
		{
			hMemDC = NULL;
			hBit = NULL;
			hOBit = NULL;
			x = 0;
			y = 0;
			width = 0;
			height = 0;
			currentFrameX = 0;
			currentFrameY = 0;
			maxFrameX = 0;
			maxFrameY = 0;
			frameWidth = 0;
			frameHeight = 0;
			loadType = LOAD_EMPTY;
		}
	}IMAGE_INFO, *LPIMAGE_INFO;

private:
	LPIMAGE_INFO	_imageInfo;		//이미지 정보
	char*			_fileName;		//이미지 파일이름
	bool			_isTrans;		//배경색 없앨거냐?
	COLORREF		_transColor;	//배경색 없앨 RGB (마젠타 = RGB(255, 0, 255))

	LPIMAGE_INFO	_blendImage;	//알파블렌드 이미지
	BLENDFUNCTION	_blendFunc;		//알파블렌드 기능

	LPIMAGE_INFO	_stretchImage;  //스트레치 이미지
	LPIMAGE_INFO	_minimapImage;	//미니맵 이미지
	LPIMAGE_INFO	_rotateImage;	//회 전이미지

public:
	image();
	~image();

	//빈 비트맵 초기화
	HRESULT init(int width, int height);
	//이미지 파일로 초기화
	HRESULT init(const char* fileName, int width, int height, bool isTrans = false, COLORREF transColor = RGB(0, 0, 0));
	HRESULT init(const char* fileName, float x, float y, int width, int height, bool isTrans = false, COLORREF transColor = RGB(0, 0, 0));
	//프레임 이미지 파일로 초기화
	HRESULT init(const char* fileName, int width, int height, int frameX, int frameY, bool isTrans = true, COLORREF transColor = RGB(255, 0, 255));
	HRESULT init(const char* fileName, float x, float y, int width, int height, int frameX, int frameY, bool isTrans = true, COLORREF transColor = RGB(255, 0, 255));

	//알파블렌드 초기화
	HRESULT initForAlphaBlend();
	HRESULT initForStretch();
	HRESULT initForStretch(int x, int y);
	HRESULT initForMinimap(int sizeX, int sizeY);
	HRESULT initForRotate();


	//해제
	void release();

	//렌더(내가 원하는 좌표에 출력해라, 일반적으로 우리는 RECT의 LT좌표에 이미지를 붙인다)
	void render(HDC hdc, int destX = 0, int destY = 0);
	//렌더(원하는 좌표에 이미지를 잘라서 붙이기)
	void render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight);
	//알파렌더(이미지를 알파값만큼 투명화 시켜준다)
	void alphaRender(HDC hdc, BYTE alpha);
	void alphaRender(HDC hdc, int destX, int destY, BYTE alpha);
	void alphaFrameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, BYTE alpha);
	void alphaRender(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha);
	//스트레치 렌더
	void stretchRender(HDC hdc, int centerX, int centerY, float size);
	void stretchRender(HDC hdc, int centerX, int centerY, int scaleX , int scaleY );
	void stretchFrameRender(HDC hdc, int centerX, int centerY, int currentFrameX, int currentFrameY, float size);
	void stretchFrameRender(HDC hdc, int centerX, int centerY, int currentFrameX, int currentFrameY, int scaleX , int scaleY );
	//프레임렌더
	void frameRender(HDC hdc, int destX, int destY);
	void frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY);

	//void minimapRender(HDC hdc, int destX, int destY, int sizeX, int sizeY,bool isAlpha, BYTE alpha = 255);

	//루프렌더
	void loopRender(HDC hdc, const LPRECT drawArea, int offsetX, int offsetY);
	void loopAlphaRender(HDC hdc, const LPRECT drawArea, int offsetX, int offsetY, BYTE alpha);

	//회전렌더 기본적으로 선터점렌더다
	void rotateRender(HDC hdc, float centerX, float centerY, float angle);
	void rotateFrameRender(HDC hdc, float centerX, float centerY, float angle, int frameX = 0, int frameY = 0);
	void rotateAlphaRender(HDC hdc, float centerX, float centerY, float angle, BYTE alpha);
	void rotateAlphaFrameRender(HDC hdc, float centerX, float centerY, float angle, int frameX = 0, int frameY = 0, BYTE alpha = 255);
	void rotateStretchFrameRender(HDC hdc, float centerX, float centerY, int frameX, int frameY, float angle, float scale);

	//DC 얻기
	inline HDC getMemDC() { return _imageInfo->hMemDC; }

	//이미지 x, y좌표
	inline float getX() { return _imageInfo->x; }
	inline float getY() { return _imageInfo->y; }
	inline void setX(float x) { _imageInfo->x = x; }
	inline void setY(float y) { _imageInfo->y = y; }
	//이미지 센터좌표
	inline void setCenter(float x, float y)
	{
		_imageInfo->x = x + (_imageInfo->width / 2);
		_imageInfo->y = y + (_imageInfo->height / 2);
	}
	//이미지 가로, 세로길이 가져오기
	inline int getWidth() { return _imageInfo->width; }
	inline int getHeight() { return _imageInfo->height; }
	//바운딩 박스(충돌용 렉트)
	inline RECT boundingBox()
	{
		RECT rc = { (int)_imageInfo->x, (int)_imageInfo->y,
		(int)_imageInfo->x + _imageInfo->width,
		(int)_imageInfo->y + _imageInfo->height };
		return rc;
	}
	inline RECT boudingBoxWithFrame()
	{
		RECT rc = { (int)_imageInfo->x, (int)_imageInfo->y,
		(int)_imageInfo->x + _imageInfo->frameWidth,
		(int)_imageInfo->y + _imageInfo->frameHeight };
		return rc;
	}
	//프레임 X, Y
	inline int getFrameX() { return _imageInfo->currentFrameX; }
	inline int getFrameY() { return _imageInfo->currentFrameY; }
	inline void setFrameX(int frameX)
	{
		_imageInfo->currentFrameX = frameX;
		if (frameX > _imageInfo->maxFrameX)
		{
			_imageInfo->currentFrameX = _imageInfo->maxFrameX;
		}
	}
	inline void setFrameY(int frameY)
	{
		_imageInfo->currentFrameY = frameY;
		if (frameY > _imageInfo->maxFrameY)
		{
			_imageInfo->currentFrameY = _imageInfo->maxFrameY;
		}
	}
	//1프레임 가로, 세로길이 가져오기
	inline int getFrameWidth() { return _imageInfo->frameWidth; }
	inline int getFrameHeight() { return _imageInfo->frameHeight; }
	//맥스 프레임 가져오기
	inline int getMaxFrameX() { return _imageInfo->maxFrameX; }
	inline int getMaxFrameY() { return _imageInfo->maxFrameY; }

	//이미지 트랜스 설정
	inline void setIsTrans(bool trans) { _isTrans = trans; }
	inline void setTransColor(COLORREF color) { _transColor = color; }

};

