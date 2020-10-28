#include "stdafx.h"
#include "loading.h"

HRESULT loadItem::init(string strKey, int width, int height)
{
	//로딩종류 초기화
	_kind = LOAD_KIND_IMAGE_0;

	//이미지 구조체 초기화
	_imageResource.keyName = strKey;
	_imageResource.width = width;
	_imageResource.height = height;

	return S_OK;
}

HRESULT loadItem::init(string strKey, const char * fileName, int width, int height, bool isTrans, COLORREF transColor)
{
	//로딩종류 초기화
	_kind = LOAD_KIND_IMAGE_1;

	//이미지 구조체 초기화
	_imageResource.keyName = strKey;
	_imageResource.fileName = fileName;
	_imageResource.width = width;
	_imageResource.height = height;
	_imageResource.isTrans = isTrans;
	_imageResource.transColor = transColor;

	return S_OK;
}

HRESULT loadItem::init(string strKey, const char * fileName, float x, float y, int width, int height, bool isTrans, COLORREF transColor)
{
	//로딩종류 초기화
	_kind = LOAD_KIND_IMAGE_2;

	//이미지 구조체 초기화
	_imageResource.keyName = strKey;
	_imageResource.fileName = fileName;
	_imageResource.x = x;
	_imageResource.y = y;
	_imageResource.width = width;
	_imageResource.height = height;
	_imageResource.isTrans = isTrans;
	_imageResource.transColor = transColor;

	return S_OK;
}

HRESULT loadItem::init(string strKey, const char * fileName, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	//로딩종류 초기화
	_kind = LOAD_KIND_FRAMEIMAGE_0;

	//이미지 구조체 초기화
	_imageResource.keyName = strKey;
	_imageResource.fileName = fileName;
	_imageResource.width = width;
	_imageResource.height = height;
	_imageResource.frameX = frameX;
	_imageResource.frameY = frameY;
	_imageResource.isTrans = isTrans;
	_imageResource.transColor = transColor;

	return S_OK;
}

HRESULT loadItem::init(string strKey, const char * fileName, float x, float y, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	//로딩종류 초기화
	_kind = LOAD_KIND_FRAMEIMAGE_1;

	//이미지 구조체 초기화
	_imageResource.keyName = strKey;
	_imageResource.fileName = fileName;
	_imageResource.x = x;
	_imageResource.y = y;
	_imageResource.width = width;
	_imageResource.height = height;
	_imageResource.frameX = frameX;
	_imageResource.frameY = frameY;
	_imageResource.isTrans = isTrans;
	_imageResource.transColor = transColor;

	return S_OK;
}

HRESULT loadItem::init(string strKey, const char * fileName, bool isBGM, bool isLoop)
{
	_kind = LOAD_KIND_SOUND;
	_soundResource.key = strKey;
	_soundResource.fileName = fileName;
	_soundResource.isBGM = isBGM;
	_soundResource.isLoop = isLoop;
	return S_OK;
}

//=============================================================
//	## loading ## (로딩클래스 - 로딩화면 구현하기, 로딩바, 백그라운드)
//=============================================================
HRESULT loading::init()
{
	//로딩화면 백그라운드 이미지 초기화
	//_background = IMAGEMANAGER->addImage("bgLoadingScene", "bgLoadingScene.bmp", WINSIZEX, WINSIZEY);
	//로딩바 이미지 초기화
	/*IMAGEMANAGER->addImage("loadingBarFront", "loadingBarFront.bmp", 600, 20);
	IMAGEMANAGER->addImage("loadingBarBack", "loadingBarBack.bmp", 600, 20);*/

	IMAGEMANAGER->addFrameImage("로딩아이콘", "Images/타이틀화면/load2배.bmp", 5896, 90, 67, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("로딩텍스트", "Images/타이틀화면/loading.bmp", 1440, 44, 4, 1, true, RGB(255, 0, 255));
	////로딩바 클래스 초기화
	//_loadingBar = new progressBar;
	//_loadingBar->init("loadingBarFront", "loadingBarBack");
	////로딩바 위치 초기화
	//_loadingBar->setPos(100, 500);

	//현재 게이지
	_currentGauge = 0;
	r_index = 0;
	_index = 0;
	_count = 0;

	return S_OK;
}

void loading::release()
{
	for (int i = 0; i < _vLoadItem.size(); i++)
	{
		SAFE_DELETE(_vLoadItem[i]);
	}
	_vLoadItem.clear();

}

void loading::update()
{
	//로딩바 클래스 업데이트
	//_loadingBar->update();
	loadingAnim();
}

void loading::render()
{
	//백그라운드 이미지 렌더
	//_background->render(getMemDC());
	//로딩바 클래스 렌더
	//_loadingBar->render();

	if (_currentGauge < _vLoadItem.size())
	{
		if (_vLoadItem[_currentGauge]->getLoadKind() >= 1 && _vLoadItem[_currentGauge]->getLoadKind() <= 4)
		{
			TextOut(getMemDC(), 10, 50, _vLoadItem[_currentGauge]->getImageResource().fileName, strlen(_vLoadItem[_currentGauge]->getImageResource().fileName));
		}
		else if (_vLoadItem[_currentGauge]->getLoadKind() == LOAD_KIND_SOUND)
		{
			TextOut(getMemDC(), 10, 50, _vLoadItem[_currentGauge]->getSoundResource().fileName, strlen(_vLoadItem[_currentGauge]->getSoundResource().fileName));
		}
	}
	float per = (float)_currentGauge / (float)_vLoadItem.size();


	IMAGEMANAGER->frameRender("로딩텍스트", getMemDC(), WINSIZEX / 2 - 200, WINSIZEY - 100);
	IMAGEMANAGER->frameRender("로딩아이콘", getMemDC(), WINSIZEX - 100, WINSIZEY - 100);

}

void loading::loadImage(string strKey, int width, int height)
{
	loadItem* item = new loadItem;
	item->init(strKey, width, height);
	_vLoadItem.push_back(item);
}

void loading::loadImage(string strKey, const char * fileName, int width, int height, bool isTrans, COLORREF transColor)
{
	loadItem* item = new loadItem;
	item->init(strKey, fileName, width, height, isTrans, transColor);
	_vLoadItem.push_back(item);
}

void loading::loadImage(string strKey, const char * fileName, float x, float y, int width, int height, bool isTrans, COLORREF transColor)
{
	loadItem* item = new loadItem;
	item->init(strKey, fileName, x, y, width, height, isTrans, transColor);
	_vLoadItem.push_back(item);
}

void loading::loadFrameImage(string strKey, const char * fileName, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	loadItem* item = new loadItem;
	item->init(strKey, fileName, width, height, frameX, frameY, isTrans, transColor);
	_vLoadItem.push_back(item);
}

void loading::loadFrameImage(string strKey, const char * fileName, float x, float y, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	loadItem* item = new loadItem;
	item->init(strKey, fileName, width, x, y, height, frameX, frameY, isTrans, transColor);
	_vLoadItem.push_back(item);
}

void loading::loadSound(string key, const char * fileName, bool isBGM, bool isLoop)
{
	loadItem* item = new loadItem;
	item->init(key, fileName, isBGM, isLoop);
	_vLoadItem.push_back(item);
}

void loading::loadingAnim()
{
	IMAGEMANAGER->findImage("로딩아이콘")->setFrameY(0);
	IMAGEMANAGER->findImage("로딩텍스트")->setFrameY(0);

	_count++;

	if (_count % 7 == 0)
	{
		r_index++;

		if (r_index > IMAGEMANAGER->findImage("로딩아이콘")->getMaxFrameX())
		{

			r_index = 0;
		}

		IMAGEMANAGER->findImage("로딩아이콘")->setFrameX(r_index);
	}

	if (_count % 15 == 0)
	{
		_index++;
		if (_index > IMAGEMANAGER->findImage("로딩텍스트")->getMaxFrameX())
		{
			_index = 0;
		}
		IMAGEMANAGER->findImage("로딩텍스트")->setFrameX(_index);
	}
}

bool loading::loadingDone()
{
	//로딩완료됨
	if (_currentGauge >= _vLoadItem.size())
	{
		return true;
	}

	loadItem* item = _vLoadItem[_currentGauge];
	switch (item->getLoadKind())
	{
		case LOAD_KIND_IMAGE_0:
		{
			tagImageResource img = item->getImageResource();
			IMAGEMANAGER->addImage(img.keyName, img.width, img.height);
		}
		break;
		
		case LOAD_KIND_IMAGE_1:
		{
			tagImageResource img = item->getImageResource();
			IMAGEMANAGER->addImage(img.keyName, img.fileName, img.width, img.height, img.isTrans, img.transColor);
		}
		break;
		
		case LOAD_KIND_IMAGE_2:
		{
			tagImageResource img = item->getImageResource();
			IMAGEMANAGER->addImage(img.keyName, img.fileName, img.x, img.y, img.width, img.height, img.isTrans, img.transColor);
		}
		break;
		
		case LOAD_KIND_FRAMEIMAGE_0:
		{
			tagImageResource img = item->getImageResource();
			IMAGEMANAGER->addFrameImage(img.keyName, img.fileName, img.width, img.height, img.frameX, img.frameY, img.isTrans, img.transColor);
		}
		break;
		
		case LOAD_KIND_FRAMEIMAGE_1:
		{
			tagImageResource img = item->getImageResource();
			IMAGEMANAGER->addFrameImage(img.keyName, img.fileName, img.x, img.y, img.width, img.height, img.frameX, img.frameY, img.isTrans, img.transColor);
		}
		break;

		//여러분들이 한번 만들어 보기
		case LOAD_KIND_SOUND:
		{
			tagSoundResource sound = item->getSoundResource();
			SOUNDMANAGER->addSound(sound.key, sound.fileName, sound.isBGM, sound.isLoop);
		}
		break;
	}
	
	//현재 게이지 증가
	_currentGauge++;

	//로딩바 이미지 변경
	//_loadingBar->setGauge(_vLoadItem.size(), _currentGauge);

	return false;
}
