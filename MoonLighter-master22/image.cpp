#include "stdafx.h"
#include "image.h"
image::image() : _imageInfo(NULL), _fileName(NULL), _blendImage(NULL), _stretchImage(NULL), _minimapImage(NULL), _rotateImage(NULL)
{
}
image::~image()
{
}

//빈 비트맵 초기화
HRESULT image::init(int width, int height)
{
	//재초기화 방지용, 이미지 정보의 값이 들어 있다면 릴리즈해서 깨끗하게 초기화 하자
	if (_imageInfo != NULL) this->release();

	//DC 가져오기
	HDC hdc = GetDC(_hWnd);

	//이미지 정보 구조체 새로 생성후 초기화 하기
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_EMPTY;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;

	//파일이름
	_fileName = NULL;
	//투명키 컬러 세팅
	_isTrans = false;
	_transColor = RGB(0, 0, 0);

	//이미지 정보를 얻는데 실패했을 시
	if (_imageInfo->hBit == NULL)
	{
		this->release();
		return E_FAIL;
	}

	//DC 해제하기
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

//이미지 파일로 초기화
HRESULT image::init(const char * fileName, int width, int height, bool isTrans, COLORREF transColor)
{
	//재초기화 방지용, 이미지 정보의 값이 들어 있다면 릴리즈해서 깨끗하게 초기화 하자
	if (_imageInfo != NULL) this->release();

	//DC 가져오기
	HDC hdc = GetDC(_hWnd);

	//이미지 정보 구조체 새로 생성후 초기화 하기
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;

	//파일이름
	int len = strlen(fileName);
	_fileName = new char[len + 1];
	strcpy(_fileName, fileName);

	//투명키 컬러 세팅
	_isTrans = isTrans;
	_transColor = transColor;

	//이미지 정보를 얻는데 실패했을 시
	if (_imageInfo->hBit == NULL)
	{
		this->release();
		return E_FAIL;
	}

	//DC 해제하기
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}
HRESULT image::init(const char * fileName, float x, float y, int width, int height, bool isTrans, COLORREF transColor)
{
	//재초기화 방지용, 이미지 정보의 값이 들어 있다면 릴리즈해서 깨끗하게 초기화 하자
	if (_imageInfo != NULL) this->release();

	//DC 가져오기
	HDC hdc = GetDC(_hWnd);

	//이미지 정보 구조체 새로 생성후 초기화 하기
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->x = x;
	_imageInfo->y = y;
	_imageInfo->width = width;
	_imageInfo->height = height;

	//파일이름
	int len = strlen(fileName);
	_fileName = new char[len + 1];
	strcpy(_fileName, fileName);

	//투명키 컬러 세팅
	_isTrans = isTrans;
	_transColor = transColor;

	//이미지 정보를 얻는데 실패했을 시
	if (_imageInfo->hBit == NULL)
	{
		this->release();
		return E_FAIL;
	}

	//DC 해제하기
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}
HRESULT image::init(const char * fileName, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	//재초기화 방지용, 이미지 정보의 값이 들어 있다면 릴리즈해서 깨끗하게 초기화 하자
	if (_imageInfo != NULL) this->release();

	//DC 가져오기
	HDC hdc = GetDC(_hWnd);

	//이미지 정보 구조체 새로 생성후 초기화 하기
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;
	_imageInfo->maxFrameX = frameX - 1;
	_imageInfo->maxFrameY = frameY - 1;
	_imageInfo->frameWidth = width / frameX;
	_imageInfo->frameHeight = height / frameY;

	//파일이름
	int len = strlen(fileName);
	_fileName = new char[len + 1];
	strcpy(_fileName, fileName);

	//투명키 컬러 세팅
	_isTrans = isTrans;
	_transColor = transColor;

	//이미지 정보를 얻는데 실패했을 시
	if (_imageInfo->hBit == NULL)
	{
		this->release();
		return E_FAIL;
	}

	//DC 해제하기
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}
HRESULT image::init(const char * fileName, float x, float y, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	//재초기화 방지용, 이미지 정보의 값이 들어 있다면 릴리즈해서 깨끗하게 초기화 하자
	if (_imageInfo != NULL) this->release();

	//DC 가져오기
	HDC hdc = GetDC(_hWnd);

	//이미지 정보 구조체 새로 생성후 초기화 하기
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->x = x;
	_imageInfo->y = y;
	_imageInfo->width = width;
	_imageInfo->height = height;
	_imageInfo->maxFrameX = frameX - 1;
	_imageInfo->maxFrameY = frameY - 1;
	_imageInfo->frameWidth = width / frameX;
	_imageInfo->frameHeight = height / frameY;

	//파일이름
	int len = strlen(fileName);
	_fileName = new char[len + 1];
	strcpy(_fileName, fileName);

	//투명키 컬러 세팅
	_isTrans = isTrans;
	_transColor = transColor;

	//이미지 정보를 얻는데 실패했을 시
	if (_imageInfo->hBit == NULL)
	{
		this->release();
		return E_FAIL;
	}

	//DC 해제하기
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}
//알파블렌드 초기화
HRESULT image::initForAlphaBlend()
{
	//DC 가져오기
	HDC hdc = GetDC(_hWnd);

	//알파블렌드 옵션 초기화
	//BlendOp => 현재는 AC_SRC_OVER만 사용가능
	//BlendFlags => 그냥 0대입, 나도 잘 모름, 예전 OS에서 사용했다고 함
	//AlphaFormat => 24bit사용시 0, 32bit사용시 AC_SRC_ALPHA 사용하면 되지만 별차이가 없어서 우린 그냥 0 
	_blendFunc.BlendOp = AC_SRC_OVER;
	_blendFunc.BlendFlags = 0;
	_blendFunc.AlphaFormat = 0;
	//0 ~ 255 => 255일때 원본색상
	//_blendFunc.SourceConstantAlpha = 255;

	int size;
	(_imageInfo->width > _imageInfo->height ? size = _imageInfo->width : size = _imageInfo->height);

	//알파블렌드 이미지 정보 구조체 새로 생성후 초기화 하기
	_blendImage = new IMAGE_INFO;
	_blendImage->loadType = LOAD_EMPTY;
	_blendImage->hMemDC = CreateCompatibleDC(hdc);
	_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, size, size);
	_blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
	_blendImage->width = size;
	_blendImage->height = size;

	//DC 해제하기
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

HRESULT image::initForStretch()
{
	HDC hdc = GetDC(_hWnd);

	_stretchImage = new IMAGE_INFO;
	_stretchImage->loadType = LOAD_EMPTY;
	_stretchImage->hMemDC = CreateCompatibleDC(hdc);
	_stretchImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
	_stretchImage->hOBit = (HBITMAP)SelectObject(_stretchImage->hMemDC, _stretchImage->hBit);
	_stretchImage->width = WINSIZEX;
	_stretchImage->height = WINSIZEY;

	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

HRESULT image::initForStretch(int x, int y)
{
	HDC hdc = GetDC(_hWnd);

	_stretchImage = new IMAGE_INFO;
	_stretchImage->loadType = LOAD_EMPTY;
	_stretchImage->hMemDC = CreateCompatibleDC(hdc);
	_stretchImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc,x,y);
	_stretchImage->hOBit = (HBITMAP)SelectObject(_stretchImage->hMemDC, _stretchImage->hBit);
	_stretchImage->width = x;
	_stretchImage->height = y;

	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

HRESULT image::initForMinimap(int sizeX, int sizeY)
{
	HDC hdc = GetDC(_hWnd);

	_minimapImage = new IMAGE_INFO;
	_minimapImage->loadType = LOAD_EMPTY;
	_minimapImage->hMemDC = CreateCompatibleDC(hdc);
	_minimapImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, sizeX, sizeY);
	_minimapImage->hOBit = (HBITMAP)SelectObject(_minimapImage->hMemDC, _minimapImage->hBit);
	_minimapImage->width = WINSIZEX;
	_minimapImage->height = WINSIZEY;

	ReleaseDC(_hWnd, hdc);
	return S_OK;
}

HRESULT image::initForRotate()
{
	HDC hdc = GetDC(_hWnd);
	int size;
	(_imageInfo->width > _imageInfo->height ? size = _imageInfo->width : size = _imageInfo->height);
	_rotateImage = new IMAGE_INFO;
	_rotateImage->loadType = LOAD_EMPTY;
	_rotateImage->hMemDC = CreateCompatibleDC(hdc);
	_rotateImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, size, size);
	_rotateImage->hOBit = (HBITMAP)SelectObject(_rotateImage->hMemDC, _rotateImage->hBit);
	_rotateImage->width = size;
	_rotateImage->height = size;

	ReleaseDC(_hWnd, hdc);
	return S_OK;
}

//해제
void image::release()
{
	//이미지 정보가 남아 있다면 해제 시켜라
	if (_imageInfo)
	{
		//이미지 삭제
		SelectObject(_imageInfo->hMemDC, _imageInfo->hOBit);
		DeleteObject(_imageInfo->hBit);
		DeleteDC(_imageInfo->hMemDC);

		//포인터 삭제
		SAFE_DELETE(_imageInfo);
		SAFE_DELETE_ARRAY(_fileName);

		//투명 컬러키 초기화
		_isTrans = false;
		_transColor = RGB(0, 0, 0);
	}

	//알파블렌드 이미지 정보가 남아 있다면 해제 시켜라
	if (_blendImage)
	{
		//이미지 삭제
		SelectObject(_blendImage->hMemDC, _blendImage->hOBit);
		DeleteObject(_blendImage->hBit);
		DeleteDC(_blendImage->hMemDC);

		//포인터 삭제
		SAFE_DELETE(_blendImage);
	}
}

//렌더(내가 원하는 좌표에 출력해라, 일반적으로 우리는 RECT의 LT좌표에 이미지를 붙인다)
void image::render(HDC hdc, int destX, int destY)
{
	if (_isTrans)//배경색 없애고 출력
	{
		//GdiTransparentBlt : 비트맵 이미지의 특정색상을 제외하고 복사 해주는 함수
		GdiTransparentBlt(
			hdc,				//복사할 장소의 DC
			destX,				//복사할 좌표 시작X
			destY,				//복사할 좌표 시작Y
			_imageInfo->width,	//복사할 이미지 가로크기
			_imageInfo->height,	//복사할 이미지 세로크기
			_imageInfo->hMemDC,	//복사될 대상 DC
			0, 0,				//복사될 대상의 시작지점
			_imageInfo->width,	//복사 영역 가로크기
			_imageInfo->height,	//복사 영역 세로크기
			_transColor);		//복사할때 제외할 색상 (일반적으로 마젠타 색상을 사용함)
	}
	else//원본 이미지 그대로 출력
	{
		//BitBlt : DC간의 영역끼리 고속복사 해주는 함수
		//메모리DC => 화면DC로 복사한다
		BitBlt(hdc, destX, destY, _imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, 0, 0, SRCCOPY);
	}
}

//렌더(원하는 좌표에 이미지를 잘라서 붙이기)
void image::render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight)
{
	if (_isTrans)//배경색 없애고 출력
	{
		//GdiTransparentBlt : 비트맵 이미지의 특정색상을 제외하고 복사 해주는 함수
		GdiTransparentBlt(
			hdc,				//복사할 장소의 DC
			destX,				//복사할 좌표 시작X
			destY,				//복사할 좌표 시작Y
			sourWidth,			//복사할 이미지 가로크기
			sourHeight,			//복사할 이미지 세로크기
			_imageInfo->hMemDC,	//복사될 대상 DC
			sourX, sourY,		//복사될 대상의 시작지점
			sourWidth,			//복사 영역 가로크기
			sourHeight,			//복사 영역 세로크기
			_transColor);		//복사할때 제외할 색상 (일반적으로 마젠타 색상을 사용함)
	}
	else//원본 이미지 그대로 출력
	{
		//BitBlt : DC간의 영역끼리 고속복사 해주는 함수
		//메모리DC => 화면DC로 복사한다
		BitBlt(hdc, destX, destY, sourWidth, sourHeight,
			_imageInfo->hMemDC, sourX, sourY, SRCCOPY);
	}
}

void image::alphaRender(HDC hdc, BYTE alpha)
{
	//알파블렌드 사용할 수 있도록 초기화 해라
	if (!_blendImage) this->initForAlphaBlend();
	//알파값 초기화
	_blendFunc.SourceConstantAlpha = alpha;

	if (_isTrans)//배경색 없애고 출력
	{
		//1. 현재화면DC의 내용을 => 블렌드이미지에 복사
		//2. 이미지메모리DC의 배경색을 없앤 후 => 블렌드이미지에 복사
		//3. 블렌드이미지를 화면DC에 복사

		//1
		BitBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height,
			hdc, 0, 0, SRCCOPY);

		//2
		//GdiTransparentBlt : 비트맵 이미지의 특정색상을 제외하고 복사 해주는 함수
		GdiTransparentBlt(
			_blendImage->hMemDC,	//복사할 장소의 DC
			0,						//복사할 좌표 시작X
			0,						//복사할 좌표 시작Y
			_imageInfo->width,		//복사할 이미지 가로크기
			_imageInfo->height,		//복사할 이미지 세로크기
			_imageInfo->hMemDC,		//복사될 대상 DC
			0, 0,					//복사될 대상의 시작지점
			_imageInfo->width,		//복사 영역 가로크기
			_imageInfo->height,		//복사 영역 세로크기
			_transColor);			//복사할때 제외할 색상 (일반적으로 마젠타 색상을 사용함)

		//3
		GdiAlphaBlend(hdc, 0, 0, _imageInfo->width, _imageInfo->height,
			_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}
	else//원본 이미지 그대로 출력
	{
		GdiAlphaBlend(hdc, 0, 0, _imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}
}

void image::alphaRender(HDC hdc, int destX, int destY, BYTE alpha)
{

	if (!_blendImage) this->initForAlphaBlend();
	_blendFunc.SourceConstantAlpha = alpha;

	if (_isTrans)//배경색 없애고 출력
	{
		BitBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height,
			hdc, destX, destY, SRCCOPY);

		GdiTransparentBlt(
			_blendImage->hMemDC,	//복사할 장소의 DC
			0,						//복사할 좌표 시작X
			0,						//복사할 좌표 시작Y
			_imageInfo->width,		//복사할 이미지 가로크기
			_imageInfo->height,		//복사할 이미지 세로크기
			_imageInfo->hMemDC,		//복사될 대상 DC
			0, 0,					//복사될 대상의 시작지점
			_imageInfo->width,		//복사 영역 가로크기
			_imageInfo->height,		//복사 영역 세로크기
			_transColor);			//복사할때 제외할 색상 (일반적으로 마젠타 색상을 사용함)

		GdiAlphaBlend(hdc, destX, destY, _imageInfo->width, _imageInfo->height,
			_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}
	else//원본 이미지 그대로 출력
	{
		GdiAlphaBlend(hdc, destX, destY, _imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}
}

void image::alphaFrameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, BYTE alpha)
{
	if (!_blendImage) this->initForAlphaBlend();

	_blendFunc.SourceConstantAlpha = alpha;

	if (_isTrans)
	{
		BitBlt(
			_blendImage->hMemDC, 0, 0,
			_imageInfo->frameWidth, _imageInfo->frameHeight,
			hdc,
			destX, destY, SRCCOPY);

		GdiTransparentBlt(
			_blendImage->hMemDC, 0, 0,
			_imageInfo->frameWidth, _imageInfo->frameHeight, _imageInfo->hMemDC,
			_imageInfo->frameWidth * currentFrameX, _imageInfo->frameHeight * currentFrameY, _imageInfo->frameWidth,
			_imageInfo->frameHeight, _transColor);

		GdiAlphaBlend(
			hdc, destX, destY,
			_imageInfo->frameWidth, _imageInfo->frameHeight, _blendImage->hMemDC, 0, 0, _imageInfo->frameWidth, _imageInfo->frameHeight, _blendFunc);

	}
	else
	{
		GdiAlphaBlend(hdc, destX, destY, _imageInfo->frameWidth, _imageInfo->frameHeight, _imageInfo->hMemDC, _imageInfo->frameWidth * currentFrameX, _imageInfo->frameHeight * currentFrameY, _imageInfo->frameWidth, _imageInfo->frameHeight, _blendFunc);
	}
}

void image::alphaRender(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha)
{
	if (!_blendImage) this->initForAlphaBlend();
	_blendFunc.SourceConstantAlpha = alpha;

	if (_isTrans)//배경색 없애고 출력
	{
		BitBlt(_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height,
			hdc, destX, destY, SRCCOPY);

		GdiTransparentBlt(
			_blendImage->hMemDC,	//복사할 장소의 DC
			0,						//복사할 좌표 시작X
			0,						//복사할 좌표 시작Y
			sourWidth,				//복사할 이미지 가로크기
			sourHeight,				//복사할 이미지 세로크기
			_imageInfo->hMemDC,		//복사될 대상 DC
			sourX, sourY,			//복사될 대상의 시작지점
			sourWidth,				//복사 영역 가로크기
			sourHeight,				//복사 영역 세로크기
			_transColor);			//복사할때 제외할 색상 (일반적으로 마젠타 색상을 사용함)

		GdiAlphaBlend(hdc, destX, destY, sourWidth, sourHeight,
			_blendImage->hMemDC, 0, 0, sourWidth, sourHeight, _blendFunc);
	}
	else//원본 이미지 그대로 출력
	{
		GdiAlphaBlend(hdc, destX, destY, sourWidth, sourHeight,
			_imageInfo->hMemDC, sourX, sourY, sourWidth, sourHeight, _blendFunc);
	}
}

void image::stretchRender(HDC hdc, int centerX, int centerY, float size)
{
	//스트레치이미지 처음 사용하냐?
		//이미지 스케일링을 사용할 수 있도록 초기화 해라
	if (!_stretchImage) this->initForStretch();

	_stretchImage->width = _imageInfo->width * size;
	_stretchImage->height = _imageInfo->height * size;

	if (_isTrans) //배경색 없앨꺼냐?
	{
		//원본이미지를 Scale값 만큼 확대/축소시켜서 그려준다
		SetStretchBltMode(hdc, COLORONCOLOR);
		StretchBlt(_stretchImage->hMemDC, 0, 0, _stretchImage->width, _stretchImage->height,
			_imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, SRCCOPY);

		//GdiTransparentBlt : 비트맵의 특정색상을 제외하고 고속복사 해주는 함수
		GdiTransparentBlt(
			hdc,					//복사할 장소의 DC
			centerX - _stretchImage->width / 2,					//복사될 좌표 시작점 X
			centerY - _stretchImage->height /2,					//복사될 좌표 시작점 Y
			_stretchImage->width,	//복사될 이미지 가로크기
			_stretchImage->height,	//복사될 이미지 세로크기
			_stretchImage->hMemDC,	//복사될 대상 DC
			0, 0,					//복사 시작지점
			_stretchImage->width,	//복사 영역 가로크기
			_stretchImage->height,	//복사 영역 세로크기
			_transColor);			//복사할때 제외할 색상 (마젠타)

	}
	else //원본 이미지 그래도 출력할꺼냐?
	{
		//원본 이미지의 크기를 확대/축소 해서 렌더 시킨다
		StretchBlt(hdc, centerX - _stretchImage->width / 2, centerY - _stretchImage->height / 2, _stretchImage->width, _stretchImage->height,
			_imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, SRCCOPY);
	}
}

void image::stretchRender(HDC hdc, int centerX, int centerY, int scaleX, int scaleY)
{
	//스트레치이미지 처음 사용하냐?
		//이미지 스케일링을 사용할 수 있도록 초기화 해라
	if (!_stretchImage) this->initForStretch();

	_stretchImage->width = scaleX;
	_stretchImage->height = scaleY;

	if (_isTrans) //배경색 없앨꺼냐?
	{
		//원본이미지를 Scale값 만큼 확대/축소시켜서 그려준다
		SetStretchBltMode(hdc, COLORONCOLOR);
		StretchBlt(_stretchImage->hMemDC, 0, 0, _stretchImage->width, _stretchImage->height,
			_imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, SRCCOPY);

		//GdiTransparentBlt : 비트맵의 특정색상을 제외하고 고속복사 해주는 함수
		GdiTransparentBlt(
			hdc,					//복사할 장소의 DC
			centerX - _stretchImage->width / 2,					//복사될 좌표 시작점 X
			centerY - _stretchImage->height / 2,					//복사될 좌표 시작점 Y
			_stretchImage->width,	//복사될 이미지 가로크기
			_stretchImage->height,	//복사될 이미지 세로크기
			_stretchImage->hMemDC,	//복사될 대상 DC
			0, 0,					//복사 시작지점
			_stretchImage->width,	//복사 영역 가로크기
			_stretchImage->height,	//복사 영역 세로크기
			_transColor);			//복사할때 제외할 색상 (마젠타)

	}
	else //원본 이미지 그래도 출력할꺼냐?
	{
		//원본 이미지의 크기를 확대/축소 해서 렌더 시킨다
		StretchBlt(hdc, centerX - _stretchImage->width / 2, centerY - _stretchImage->height / 2, _stretchImage->width, _stretchImage->height,
			_imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, SRCCOPY);
	}
}

void image::stretchFrameRender(HDC hdc, int centerX, int centerY, int currentFrameX, int currentFrameY, float size)
{
	//스트레치이미지 처음 사용하냐?
	//이미지 스케일링을 사용할 수 있도록 초기화 해라
	if (!_stretchImage) this->initForStretch(_imageInfo->width * size, _imageInfo->height * size);

	_stretchImage->width = _imageInfo->width * size;
	_stretchImage->height = _imageInfo->height * size;
	_stretchImage->frameWidth = _stretchImage->width / (_imageInfo->maxFrameX + 1);
	_stretchImage->frameHeight = _stretchImage->height / (_imageInfo->maxFrameY + 1);

	if (_isTrans) //배경색 없앨꺼냐?
	{
		//원본이미지를 Scale값 만큼 확대/축소시켜서 그려준다
		SetStretchBltMode(getMemDC(), COLORONCOLOR);
		StretchBlt(_stretchImage->hMemDC, 0, 0, _stretchImage->frameWidth, _stretchImage->frameHeight,
			_imageInfo->hMemDC,
			currentFrameX * _imageInfo->frameWidth,
			currentFrameY * _imageInfo->frameHeight,
			_imageInfo->frameWidth,
			_imageInfo->frameHeight, SRCCOPY);

		//GdiTransparentBlt : 비트맵의 특정색상을 제외하고 고속복사 해주는 함수
		GdiTransparentBlt(
			hdc,							//복사할 장소의 DC
			centerX - _stretchImage->frameWidth /2,							//복사될 좌표 시작점 X
			centerY - _stretchImage->frameHeight / 2,							//복사될 좌표 시작점 Y
			_stretchImage->frameWidth,		//복사될 이미지 가로크기
			_stretchImage->frameHeight,		//복사될 이미지 세로크기
			_stretchImage->hMemDC,			//복사될 대상 DC
			0, 0,							//복사 시작지점
			_stretchImage->frameWidth,		//복사 영역 가로크기
			_stretchImage->frameHeight,		//복사 영역 세로크기
			_transColor);					//복사할때 제외할 색상 (마젠타)
	}
	else //원본 이미지 그래도 출력할꺼냐?
	{
		//원본이미지를 Scale값 만큼 확대/축소시켜서 그려준다
		StretchBlt(hdc, centerX - _stretchImage->frameWidth / 2, centerY - _stretchImage->frameHeight / 2, _stretchImage->frameWidth, _stretchImage->frameHeight,
			_imageInfo->hMemDC,
			currentFrameX * _imageInfo->frameWidth,
			currentFrameY * _imageInfo->frameHeight,
			_imageInfo->frameWidth,
			_imageInfo->frameHeight, SRCCOPY);
	}
}

void image::stretchFrameRender(HDC hdc, int centerX, int centerY, int currentFrameX, int currentFrameY, int scaleX, int scaleY)
{
	//스트레치이미지 처음 사용하냐?
	//이미지 스케일링을 사용할 수 있도록 초기화 해라
	if (!_stretchImage) this->initForStretch();

	_stretchImage->frameWidth = scaleX;
	_stretchImage->frameHeight = scaleY;
	_stretchImage->width = scaleX * _imageInfo->maxFrameX;
	_stretchImage->height = scaleY * _imageInfo->maxFrameY;

	if (_isTrans) //배경색 없앨꺼냐?
	{
		//원본이미지를 Scale값 만큼 확대/축소시켜서 그려준다
		SetStretchBltMode(hdc, COLORONCOLOR);
		StretchBlt(_stretchImage->hMemDC, 0, 0, _stretchImage->frameWidth, _stretchImage->frameHeight,
			_imageInfo->hMemDC,
			currentFrameX * _imageInfo->frameWidth,
			currentFrameY * _imageInfo->frameHeight,
			_imageInfo->frameWidth,
			_imageInfo->frameHeight, SRCCOPY);

		//GdiTransparentBlt : 비트맵의 특정색상을 제외하고 고속복사 해주는 함수
		GdiTransparentBlt(
			hdc,							//복사할 장소의 DC
			centerX - _stretchImage->frameWidth / 2,							//복사될 좌표 시작점 X
			centerY - _stretchImage->frameHeight / 2,							//복사될 좌표 시작점 Y
			_stretchImage->frameWidth,		//복사될 이미지 가로크기
			_stretchImage->frameHeight,		//복사될 이미지 세로크기
			_stretchImage->hMemDC,			//복사될 대상 DC
			0, 0,							//복사 시작지점
			_stretchImage->frameWidth,		//복사 영역 가로크기
			_stretchImage->frameHeight,		//복사 영역 세로크기
			_transColor);					//복사할때 제외할 색상 (마젠타)
	}
	else //원본 이미지 그래도 출력할꺼냐?
	{
		//원본이미지를 Scale값 만큼 확대/축소시켜서 그려준다
		StretchBlt(hdc, centerX - _stretchImage->frameWidth / 2, centerY - _stretchImage->frameHeight / 2, _stretchImage->frameWidth, _stretchImage->frameHeight,
			_imageInfo->hMemDC,
			currentFrameX * _imageInfo->frameWidth,
			currentFrameY * _imageInfo->frameHeight,
			_imageInfo->frameWidth,
			_imageInfo->frameHeight, SRCCOPY);
	}
}

void image::frameRender(HDC hdc, int destX, int destY)
{
	if (_isTrans)//배경색 없애고 출력
	{
		GdiTransparentBlt(
			hdc,						//복사할 장소의 DC
			destX,						//복사할 좌표 시작X
			destY,						//복사할 좌표 시작Y
			_imageInfo->frameWidth,		//복사할 이미지 가로크기
			_imageInfo->frameHeight,	//복사할 이미지 세로크기
			_imageInfo->hMemDC,			//복사될 대상 DC
			_imageInfo->currentFrameX * _imageInfo->frameWidth,		//복사될 대상의 시작지점
			_imageInfo->currentFrameY * _imageInfo->frameHeight,	//복사될 대상의 시작지점			
			_imageInfo->frameWidth,		//복사 영역 가로크기
			_imageInfo->frameHeight,	//복사 영역 세로크기
			_transColor);				//복사할때 제외할 색상 (일반적으로 마젠타 색상을 사용함)
	}
	else//원본 이미지 그대로 출력
	{
		BitBlt(hdc, destX, destY, _imageInfo->frameWidth, _imageInfo->frameHeight,
			_imageInfo->hMemDC,
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight, SRCCOPY);
	}
}

void image::frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY)
{
	//이미지 예외처리
	_imageInfo->currentFrameX = currentFrameX;
	_imageInfo->currentFrameY = currentFrameY;
	if (currentFrameX > _imageInfo->maxFrameX)
	{
		_imageInfo->currentFrameX = _imageInfo->maxFrameX;
	}
	if (currentFrameY > _imageInfo->maxFrameY)
	{
		_imageInfo->currentFrameY = _imageInfo->maxFrameX;
	}

	if (_isTrans)//배경색 없애고 출력
	{
		GdiTransparentBlt(
			hdc,						//복사할 장소의 DC
			destX,						//복사할 좌표 시작X
			destY,						//복사할 좌표 시작Y
			_imageInfo->frameWidth,		//복사할 이미지 가로크기
			_imageInfo->frameHeight,	//복사할 이미지 세로크기
			_imageInfo->hMemDC,			//복사될 대상 DC
			_imageInfo->currentFrameX * _imageInfo->frameWidth,		//복사될 대상의 시작지점
			_imageInfo->currentFrameY * _imageInfo->frameHeight,	//복사될 대상의 시작지점			
			_imageInfo->frameWidth,		//복사 영역 가로크기
			_imageInfo->frameHeight,	//복사 영역 세로크기
			_transColor);				//복사할때 제외할 색상 (일반적으로 마젠타 색상을 사용함)
	}
	else//원본 이미지 그대로 출력
	{
		BitBlt(hdc, destX, destY, _imageInfo->frameWidth, _imageInfo->frameHeight,
			_imageInfo->hMemDC,
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight, SRCCOPY);
	}
}

//루프렌더
void image::loopRender(HDC hdc, const LPRECT drawArea, int offsetX, int offsetY)
{
	//오프셋(offset)은 일반적으로 동일 오브젝트 안에서 오브젝트 처음부터 주어진 요소나 특정 지점까지의 변위차
		//offset은 어떤 특정한 값에서 약간 차이가 나는 값 또는 그 차이 자체를 의미(차이에 따른 보정값)

		//offset 값이 음수인 경우 보정하기
	if (offsetX < 0) offsetX = _imageInfo->width + (offsetX % _imageInfo->width);
	if (offsetY < 0) offsetY = _imageInfo->height + (offsetY % _imageInfo->height);

	//그려지는 영역 세팅
	RECT rcSour;
	int sourWidth;
	int sourHeight;

	//그려지는 DC 영역 (화면크기)
	RECT rcDest;

	//그려야 할 전체 영역
	int drawAreaX = drawArea->left;
	int drawAreaY = drawArea->top;
	int drawAreaW = drawArea->right - drawArea->left;
	int drawAreaH = drawArea->bottom - drawArea->top;

	//세로 루프영역
	for (int y = 0; y < drawAreaH; y += sourHeight)
	{
		//소스 영역의 높이 계산
		rcSour.top = (y + offsetY) % _imageInfo->height;
		rcSour.bottom = _imageInfo->height;
		sourHeight = rcSour.bottom - rcSour.top;

		//소스 영역이 그리는 화면을 넘어갔다면(화면밖으로 나갔을때)
		if (y + sourHeight > drawAreaH)
		{
			//넘어간 그림의 값만큼 바텀값을 올려준다
			rcSour.bottom -= (y + sourHeight) - drawAreaH;
			sourHeight = rcSour.bottom - rcSour.top;
		}

		//그려지는 영역
		rcDest.top = y + drawAreaY;
		rcDest.bottom = rcDest.top + sourHeight;

		//가로 루프영역
		for (int x = 0; x < drawAreaW; x += sourWidth)
		{
			//소스 영역의 높이 계산
			rcSour.left = (x + offsetX) % _imageInfo->width;
			rcSour.right = _imageInfo->width;
			sourWidth = rcSour.right - rcSour.left;

			//소스 영역이 그리는 화면을 넘어갔다면(화면밖으로 나갔을때)
			if (x + sourWidth > drawAreaW)
			{
				//넘어간 그림의 값만큼 바텀값을 올려준다
				rcSour.right -= (x + sourWidth) - drawAreaW;
				sourWidth = rcSour.right - rcSour.left;
			}

			//그려지는 영역
			rcDest.left = x + drawAreaX;
			rcDest.right = rcDest.left + sourWidth;

			//그려주자(일반렌더-이미지잘라서붙이기)
			render(hdc, rcDest.left, rcDest.top, rcSour.left, rcSour.top, sourWidth, sourHeight);
		}
	}
}

void image::loopAlphaRender(HDC hdc, const LPRECT drawArea, int offsetX, int offsetY, BYTE alpha)
{
	//offset 값이 음수인 경우 보정하기
	if (offsetX < 0) offsetX = _imageInfo->width + (offsetX % _imageInfo->width);
	if (offsetY < 0) offsetY = _imageInfo->height + (offsetY % _imageInfo->height);

	//그려지는 영역 세팅
	RECT rcSour;
	int sourWidth;
	int sourHeight;

	//그려지는 DC 영역 (화면크기)
	RECT rcDest;

	//그려야 할 전체 영역
	int drawAreaX = drawArea->left;
	int drawAreaY = drawArea->top;
	int drawAreaW = drawArea->right - drawArea->left;
	int drawAreaH = drawArea->bottom - drawArea->top;

	//세로 루프영역
	for (int y = 0; y < drawAreaH; y += sourHeight)
	{
		//소스 영역의 높이 계산
		rcSour.top = (y + offsetY) % _imageInfo->height;
		rcSour.bottom = _imageInfo->height;
		sourHeight = rcSour.bottom - rcSour.top;

		//소스 영역이 그리는 화면을 넘어갔다면(화면밖으로 나갔을때)
		if (y + sourHeight > drawAreaH)
		{
			//넘어간 그림의 값만큼 바텀값을 올려준다
			rcSour.bottom -= (y + sourHeight) - drawAreaH;
			sourHeight = rcSour.bottom - rcSour.top;
		}

		//그려지는 영역
		rcDest.top = y + drawAreaY;
		rcDest.bottom = rcDest.top + sourHeight;

		//가로 루프영역
		for (int x = 0; x < drawAreaW; x += sourWidth)
		{
			//소스 영역의 높이 계산
			rcSour.left = (x + offsetX) % _imageInfo->width;
			rcSour.right = _imageInfo->width;
			sourWidth = rcSour.right - rcSour.left;

			//소스 영역이 그리는 화면을 넘어갔다면(화면밖으로 나갔을때)
			if (x + sourWidth > drawAreaW)
			{
				//넘어간 그림의 값만큼 바텀값을 올려준다
				rcSour.right -= (x + sourWidth) - drawAreaW;
				sourWidth = rcSour.right - rcSour.left;
			}

			//그려지는 영역
			rcDest.left = x + drawAreaX;
			rcDest.right = rcDest.left + sourWidth;

			//그려주자(알파렌더-이미지잘라서붙이기)
			alphaRender(hdc, rcDest.left, rcDest.top, rcSour.left, rcSour.top, sourWidth, sourHeight, alpha);
		}
	}
}

void image::rotateRender(HDC hdc, float centerX, float centerY, float angle)
{
	if (!_rotateImage) this->initForRotate();
	POINT rPoint[3];
	int dist = sqrt((_imageInfo->width / 2) * (_imageInfo->width / 2) + (_imageInfo->height / 2) * (_imageInfo->height / 2));
	float baseAngle[3];
	baseAngle[0] = PI - atanf(((float)_imageInfo->height / 2) / ((float)_imageInfo->width / 2));
	baseAngle[1] = atanf(((float)_imageInfo->height / 2) / ((float)_imageInfo->width / 2));
	baseAngle[2] = PI + atanf(((float)_imageInfo->height / 2) / ((float)_imageInfo->width / 2));

	for (int i = 0; i < 3; i++)
	{
		rPoint[i].x = (_rotateImage->width / 2 + cosf(baseAngle[i] + angle) * dist);
		rPoint[i].y = (_rotateImage->height / 2 + -sinf(baseAngle[i] + angle) * dist);
	}

	if (_isTrans)
	{
		BitBlt(_rotateImage->hMemDC, 0, 0, _rotateImage->width, _rotateImage->height, hdc, 0, 0, BLACKNESS);
		HBRUSH hBrush = CreateSolidBrush(_transColor);
		HBRUSH oBrush = (HBRUSH)SelectObject(_rotateImage->hMemDC, hBrush);
		ExtFloodFill(_rotateImage->hMemDC, 1, 1, RGB(0, 0, 0), FLOODFILLSURFACE);
		DeleteObject(hBrush);

		PlgBlt(_rotateImage->hMemDC, rPoint, _imageInfo->hMemDC,
			0, 0,
			_imageInfo->width,
			_imageInfo->height,
			NULL, 0, 0);
		GdiTransparentBlt(hdc,
			centerX - _rotateImage->width / 2,
			centerY - _rotateImage->height / 2,
			_rotateImage->width,
			_rotateImage->height,
			_rotateImage->hMemDC,
			0, 0,
			_rotateImage->width,
			_rotateImage->height,
			_transColor);
	}
	else
	{
		PlgBlt(hdc, rPoint, _imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, NULL, 0, 0);
	}
}

void image::rotateFrameRender(HDC hdc, float centerX, float centerY, float angle, int frameX, int frameY)
{
	if (!_rotateImage) this->initForRotate();
	POINT rPoint[3];
	int dist = sqrt((_imageInfo->frameWidth / 2) * (_imageInfo->frameWidth / 2) + (_imageInfo->frameHeight / 2) * (_imageInfo->frameHeight / 2));
	float baseAngle[3];
	baseAngle[0] = PI - atanf(((float)_imageInfo->frameHeight / 2) / ((float)_imageInfo->frameWidth / 2));
	baseAngle[1] = atanf(((float)_imageInfo->frameHeight / 2) / ((float)_imageInfo->frameWidth / 2));
	baseAngle[2] = PI + atanf(((float)_imageInfo->frameHeight / 2) / ((float)_imageInfo->frameWidth / 2));

	for (int i = 0; i < 3; i++)
	{
		rPoint[i].x = (_rotateImage->width / 2 + cosf(baseAngle[i] + angle) * dist);
		rPoint[i].y = (_rotateImage->height / 2 + -sinf(baseAngle[i] + angle) * dist);
	}

	if (_isTrans)
	{
		BitBlt(_rotateImage->hMemDC, 0, 0, _rotateImage->width, _rotateImage->height, hdc, 0, 0, BLACKNESS);
		HBRUSH hBrush = CreateSolidBrush(_transColor);
		HBRUSH oBrush = (HBRUSH)SelectObject(_rotateImage->hMemDC, hBrush);
		ExtFloodFill(_rotateImage->hMemDC, 1, 1, RGB(0, 0, 0), FLOODFILLSURFACE);
		DeleteObject(hBrush);

		PlgBlt(_rotateImage->hMemDC, rPoint, _imageInfo->hMemDC,
			_imageInfo->frameWidth * frameX, _imageInfo->frameHeight * frameY,
			_imageInfo->frameWidth,
			_imageInfo->frameHeight,
			NULL, 0, 0);

		GdiTransparentBlt(hdc,
			centerX - _rotateImage->width / 2,
			centerY - _rotateImage->height / 2,
			_rotateImage->width,
			_rotateImage->height,
			_rotateImage->hMemDC,
			0, 0,
			_rotateImage->width,
			_rotateImage->height,
			_transColor);
	}
	else
	{
		PlgBlt(hdc, rPoint, _imageInfo->hMemDC, _imageInfo->frameWidth * frameX, _imageInfo->frameHeight * frameY, _imageInfo->frameWidth, _imageInfo->frameHeight, NULL, 0, 0);
	}
}

void image::rotateAlphaRender(HDC hdc, float centerX, float centerY, float angle, BYTE alpha)
{
	if (!_blendImage) this->initForAlphaBlend();
	if (!_rotateImage) this->initForRotate();

	_blendFunc.SourceConstantAlpha = alpha;

	POINT rPoint[3];
	int dist = sqrt((_imageInfo->width / 2) * (_imageInfo->width / 2) + (_imageInfo->height / 2) * (_imageInfo->height / 2));
	float baseAnlge[3];
	baseAnlge[0] = PI - atanf(((float)_imageInfo->height / 2) / ((float)_imageInfo->width / 2));
	baseAnlge[1] = atanf(((float)_imageInfo->height / 2) / ((float)_imageInfo->width / 2));
	baseAnlge[2] = PI + atanf(((float)_imageInfo->height / 2) / ((float)_imageInfo->width / 2));

	for (int i = 0; i < 3; i++)
	{
		rPoint[i].x = (_rotateImage->width / 2 + cosf(baseAnlge[i] + angle) * dist);
		rPoint[i].y = (_rotateImage->height / 2 - sinf(baseAnlge[i] + angle) * dist);
	}

	if (_isTrans)
	{
		BitBlt(_rotateImage->hMemDC, 0, 0, _rotateImage->width, _rotateImage->height, hdc, 0, 0, BLACKNESS);
		BitBlt(_blendImage->hMemDC, 0, 0, _rotateImage->width, _rotateImage->height, hdc, centerX - _rotateImage->width / 2, centerY - _rotateImage->height / 2, SRCCOPY);

		HBRUSH brush = CreateSolidBrush(_transColor);
		HBRUSH oBrush = (HBRUSH)SelectObject(_rotateImage->hMemDC, brush);
		ExtFloodFill(_rotateImage->hMemDC, 1, 1, RGB(0, 0, 0), FLOODFILLSURFACE);
		DeleteObject(brush);

		PlgBlt(_rotateImage->hMemDC, rPoint, _imageInfo->hMemDC,
			0, 0,
			_imageInfo->width,
			_imageInfo->height,
			NULL, 0, 0);
		GdiTransparentBlt(_blendImage->hMemDC,
			0, 0,
			_rotateImage->width,
			_rotateImage->height,
			_rotateImage->hMemDC,
			0, 0,
			_rotateImage->width,
			_rotateImage->height,
			_transColor);
		GdiAlphaBlend(hdc,
			centerX - _rotateImage->width / 2,
			centerY - _rotateImage->height / 2,
			_rotateImage->width,
			_rotateImage->height,
			_blendImage->hMemDC,
			0, 0,
			_rotateImage->width,
			_rotateImage->height,
			_blendFunc);
	}
	else
	{
		PlgBlt(hdc, rPoint, _imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, NULL, 0, 0);
	}

}

void image::rotateAlphaFrameRender(HDC hdc, float centerX, float centerY, float angle, int frameX, int frameY, BYTE alpha)
{
	if (!_rotateImage) this->initForRotate();
	if (!_blendImage) this->initForAlphaBlend();
	_blendFunc.SourceConstantAlpha = alpha;
	POINT rPoint[3];
	int dist = sqrt((_imageInfo->frameWidth / 2) * (_imageInfo->frameWidth / 2) + (_imageInfo->frameHeight / 2) * (_imageInfo->frameHeight / 2));
	float baseAngle[3];
	baseAngle[0] = PI - atanf(((float)_imageInfo->frameHeight / 2) / ((float)_imageInfo->frameWidth / 2));
	baseAngle[1] = atanf(((float)_imageInfo->frameHeight / 2) / ((float)_imageInfo->frameWidth / 2));
	baseAngle[2] = PI + atanf(((float)_imageInfo->frameHeight / 2) / ((float)_imageInfo->frameWidth / 2));

	for (int i = 0; i < 3; i++)
	{
		rPoint[i].x = (_rotateImage->width / 2 + cosf(baseAngle[i] + angle) * dist);
		rPoint[i].y = (_rotateImage->height / 2 + -sinf(baseAngle[i] + angle) * dist);
	}

	if (_isTrans)
	{
		BitBlt(_rotateImage->hMemDC, 0, 0, _rotateImage->width, _rotateImage->height, hdc, 0, 0, BLACKNESS);
		BitBlt(_blendImage->hMemDC, 0, 0, _rotateImage->width, _rotateImage->height, hdc, centerX - _rotateImage->width / 2, centerY - _rotateImage->height / 2, SRCCOPY);
		HBRUSH hBrush = CreateSolidBrush(_transColor);
		HBRUSH oBrush = (HBRUSH)SelectObject(_rotateImage->hMemDC, hBrush);
		ExtFloodFill(_rotateImage->hMemDC, 1, 1, RGB(0, 0, 0), FLOODFILLSURFACE);
		DeleteObject(hBrush);

		PlgBlt(_rotateImage->hMemDC, rPoint, _imageInfo->hMemDC,
			_imageInfo->frameWidth * frameX, _imageInfo->frameHeight * frameY,
			_imageInfo->frameWidth,
			_imageInfo->frameHeight,
			NULL, 0, 0);

		GdiTransparentBlt(_blendImage->hMemDC,
			0, 0,
			_rotateImage->width,
			_rotateImage->height,
			_rotateImage->hMemDC,
			0, 0,
			_rotateImage->width,
			_rotateImage->height,
			_transColor);
		GdiAlphaBlend(hdc,
			centerX - _rotateImage->width / 2,
			centerY - _rotateImage->height / 2,
			_rotateImage->width,
			_rotateImage->height,
			_blendImage->hMemDC,
			0, 0,
			_rotateImage->width,
			_rotateImage->height,
			_blendFunc);
	}
	else
	{
		PlgBlt(hdc, rPoint, _imageInfo->hMemDC, _imageInfo->frameWidth * frameX, _imageInfo->frameHeight * frameY, _imageInfo->frameWidth, _imageInfo->frameHeight, NULL, 0, 0);
	}
}

void image::rotateStretchFrameRender(HDC hdc, float centerX, float centerY, int frameX, int frameY, float angle, float scale)
{
	if (!_rotateImage) this->initForRotate();

	POINT rPoint[3];
	int dist = sqrt((_imageInfo->frameWidth / 2) * (_imageInfo->frameWidth / 2) + (_imageInfo->frameHeight / 2) * (_imageInfo->frameHeight / 2));
	float baseAngle[3];
	baseAngle[0] = PI - atanf(((float)_imageInfo->frameHeight / 2) / ((float)_imageInfo->frameWidth / 2));
	baseAngle[1] = atanf(((float)_imageInfo->frameHeight / 2) / ((float)_imageInfo->frameWidth / 2));
	baseAngle[2] = PI + atanf(((float)_imageInfo->frameHeight / 2) / ((float)_imageInfo->frameWidth / 2));

	for (int i = 0; i < 3; i++)
	{
		rPoint[i].x = (_rotateImage->width / 2 + cosf(baseAngle[i] + angle) * dist);
		rPoint[i].y = (_rotateImage->height / 2 + -sinf(baseAngle[i] + angle) * dist);
	}

	if (!_stretchImage) this->initForStretch(_rotateImage->width * scale, _rotateImage->height * scale);

	if (_isTrans)
	{
		BitBlt(_rotateImage->hMemDC, 0, 0, _rotateImage->width, _rotateImage->height, hdc, 0, 0, BLACKNESS);
		HBRUSH hBrush = CreateSolidBrush(_transColor);
		HBRUSH oBrush = (HBRUSH)SelectObject(_rotateImage->hMemDC, hBrush);
		ExtFloodFill(_rotateImage->hMemDC, 1, 1, RGB(0, 0, 0), FLOODFILLSURFACE);
		DeleteObject(hBrush);

		SetStretchBltMode(hdc, COLORONCOLOR);

		PlgBlt(_rotateImage->hMemDC, rPoint, _imageInfo->hMemDC,
			_imageInfo->frameWidth * frameX, _imageInfo->frameHeight * frameY,
			_imageInfo->frameWidth,
			_imageInfo->frameHeight,
			NULL, 0, 0);

		StretchBlt(_stretchImage->hMemDC,
			0, 0, _stretchImage->width, _stretchImage->height, _rotateImage->hMemDC,
			0, 0, _rotateImage->width, _rotateImage->height, SRCCOPY);

		GdiTransparentBlt(hdc,
			centerX - _stretchImage->width / 2,
			centerY - _stretchImage->height / 2,
			_stretchImage->width,
			_stretchImage->height,
			_stretchImage->hMemDC,
			0, 0,
			_stretchImage->width,
			_stretchImage->height,
			_transColor);
	
	
	}
	else
	{
		PlgBlt(hdc, rPoint, _imageInfo->hMemDC, _imageInfo->frameWidth * frameX, _imageInfo->frameHeight * frameY, _imageInfo->frameWidth, _imageInfo->frameHeight, NULL, 0, 0);
	}
	delete _stretchImage;
	_stretchImage = nullptr;
}
