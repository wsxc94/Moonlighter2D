#include "stdafx.h"
#include "shopLoading.h"

HRESULT shopLoading::init()
{
	_loading = make_unique<loading>();
	_loading->init();

	this->loadingImage();
	this->loadingSound();

	PLAYERDATA->setIsActivate(false);
	return S_OK;
}

void shopLoading::release()
{
	if (_loading)
	{
		_loading->release();
	}
}

void shopLoading::update()
{
	_loading->update();
	if (_loading->loadingDone())
	{
		SCENEMANAGER->loadScene("상점화면");
		PLAYERDATA->setIsActivate(true);
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

	_loading->loadFrameImage("상점문열어", "Images/상점/shopDoor.bmp", 492, 90, 6, 1); // 900 139

	_loading->loadFrameImage("상점길목", "Images/상점/상점길목.bmp", 80, 154, 1, 1);
	_loading->loadFrameImage("상점문", "Images/상점/상점문.bmp", 110, 110, 1, 1);
	_loading->loadFrameImage("상점좌판", "Images/상점/상점좌판.bmp", 98, 112, 1, 1);
	_loading->loadFrameImage("상점책상", "Images/상점/상점책상.bmp", 192, 66, 1, 1);
	_loading->loadFrameImage("상점침대", "Images/상점/상점침대.bmp", 102, 76, 1, 1);
	_loading->loadFrameImage("아이템그림자", "Images/상점/item_shadow.bmp", 24, 12, 1, 1);


	_loading->loadFrameImage("엄청싸다", "Images/npc/엄청싸다.bmp", 884, 52, 17, 1);
	_loading->loadFrameImage("싸다", "Images/npc/싸다.bmp", 416, 52, 8, 1);
	_loading->loadFrameImage("비싸다", "Images/npc/비싸다.bmp", 468, 52, 9, 1);
	_loading->loadFrameImage("엄청비싸다", "Images/npc/엄청비싸다.bmp", 676, 52, 13, 1);
	_loading->loadFrameImage("기다리는중", "Images/npc/npc_waiting.bmp", 104, 52, 2, 1);

	_loading->loadFrameImage("npc말풍선몸", "Images/상점/bubble_body.bmp", 24, 24, 1, 1);
	_loading->loadFrameImage("npc말풍선꼬리", "Images/상점/bubble_bottom.bmp", 10, 6, 1, 1);
	_loading->loadFrameImage("npc말풍선왼쪽", "Images/상점/bubble_left.bmp", 6, 24, 1, 1);
	_loading->loadFrameImage("npc말풍선오른쪽", "Images/상점/bubble_right.bmp", 6, 24, 1, 1);

	_loading->loadFrameImage("상점배치", "Images/상점/icon_display.bmp", 384, 56, 3, 1);
	_loading->loadFrameImage("책상금고", "Images/상점/shop_counter.bmp", 308, 46, 7, 1);
	_loading->loadFrameImage("판매버튼", "Images/상점/icon_sell.bmp", 768, 56, 6, 1);
	_loading->loadFrameImage("꿀단지", "Images/상점/shop_cauldron.bmp", 204, 32, 6, 1);

	_loading->loadFrameImage("원형아저씨", "Images/npc/원형아저씨8X4.bmp", 352, 304, 8, 4);
	_loading->loadFrameImage("도둑강아지", "Images/npc/AnimalThief2 8x4.bmp", 464, 308, 8, 4);
	_loading->loadFrameImage("배낭맨여자", "Images/npc/women.bmp", 630, 280, 9, 4);
	_loading->loadFrameImage("히어로", "Images/npc/hero.bmp", 512, 303, 8, 4);
	//_loading->loadFrameImage("상점배치", "Images/상점/ui_onDP.bmp", 310, 50, 3, 1);


}

void shopLoading::loadingSound()
{

	_loading->loadSound("엄청싸다", "Sound/shopSound/엄청싸다.wav");
	_loading->loadSound("싸다", "Sound/shopSound/싸다.wav");
	_loading->loadSound("비싸다", "Sound/shopSound/비싸다.wav");
	_loading->loadSound("엄청비싸다", "Sound/shopSound/엄청비싸다.wav");

	_loading->loadSound("아이템팔림", "Sound/shopSound/아이템팔림.wav");
}
