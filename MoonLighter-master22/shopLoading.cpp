#include "stdafx.h"
#include "shopLoading.h"

HRESULT shopLoading::init()
{
	_loading = new loading;
	_loading->init();

	this->loadingImage();
	this->loadingSound();

	return S_OK;
}

void shopLoading::release()
{
	if (_loading)
	{
		_loading->release();
		SAFE_DELETE(_loading);
	}
}

void shopLoading::update()
{
	_loading->update();
	if (_loading->loadingDone())
	{
		SCENEMANAGER->loadScene("상점화면");
	}
}

void shopLoading::render()
{
	_loading->render();
}

void shopLoading::loadingImage()
{
	_loading->loadImage("상점맵", "Images/상점/myShop_noon.bmp", 670, 860, true, RGB(255, 0, 255));
	_loading->loadImage("상점픽셀", "Images/상점/myShop_pixel2.bmp", 670, 860, true, RGB(255, 0, 255));
	//_loading->loadImage("상점문기둥", "Images/상점/shop_door.bmp", 183, 118, true, RGB(255, 0, 255));
	//_loading->loadImage("상점문기둥", "Images/상점/shop_door.bmp", 91, 64, true, RGB(255, 0, 255));
	_loading->loadFrameImage("상점문열어", "Images/상점/shopDoor.bmp", 900, 139, 6, 1);

	_loading->loadFrameImage("상점길목", "Images/상점/상점길목.bmp", 80, 154, 1, 1);
	_loading->loadFrameImage("상점문", "Images/상점/상점문.bmp", 110, 110, 1, 1);
	_loading->loadFrameImage("상점좌판", "Images/상점/상점좌판.bmp", 98, 112, 1, 1);
	_loading->loadFrameImage("상점책상", "Images/상점/상점책상.bmp", 192, 66, 1, 1);
	_loading->loadFrameImage("상점침대", "Images/상점/상점침대.bmp", 102, 76, 1, 1);
	_loading->loadFrameImage("아이템그림자", "Images/상점/item_shadow.bmp", 24, 12, 1, 1);
	_loading->loadFrameImage("가격좋음", "Images/npc/visitor_face.bmp", 512, 64, 8, 1);
}

void shopLoading::loadingSound()
{
	
	_loading->loadSound("엄청싸다", "Sound/shopSound/엄청싸다.wav");
	_loading->loadSound("싸다", "Sound/shopSound/싸다.wav");
	_loading->loadSound("비싸다", "Sound/shopSound/비싸다.wav");
	_loading->loadSound("엄청비싸다", "Sound/shopSound/엄청비싸다.wav");
}
