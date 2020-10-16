#include "stdafx.h"
#include "bossLoading.h"

HRESULT bossLoading::init()
{
	_loading = new loading;
	_loading->init();

	this->loadImage();
	this->loadSound();

	PLAYERDATA->setIsActivate(false);
	return S_OK;
}

void bossLoading::release()
{
	if (_loading)
	{
		_loading->release();
		SAFE_DELETE(_loading);
	}
}

void bossLoading::update()
{
	_loading->update();
	if (_loading->loadingDone())
	{
		SCENEMANAGER->loadScene("보스던전");
		PLAYERDATA->setIsActivate(true);
	}
}

void bossLoading::render()
{
	_loading->render();
}

void bossLoading::loadImage()
{
	//보스방 배경
	_loading->loadImage("bossRoomBack", "Images/보스/bossRoom .bmp", 1024, 839);

	//보스이미지
	_loading->loadFrameImage("bossUp", "Images/보스/BossUp32 .bmp", 5600, 175, 32, 1);
	_loading->loadFrameImage("bossDead1", "Images/보스/dead1 .bmp", 7175, 175, 41, 1);
	_loading->loadFrameImage("bossDead2", "Images/보스/dead2 .bmp", 7175, 175, 41, 1);
	_loading->loadFrameImage("bossFistShoot", "Images/보스/FistShoot .bmp", 3850, 525, 22, 3);
	_loading->loadFrameImage("bossHandShootStart", "Images/보스/Hand_Shoot_First .bmp", 3500, 175, 20, 1);
	_loading->loadFrameImage("bossHandShootEnd", "Images/보스/Hand_Shoot_Last .bmp", 1925, 175, 11, 1);
	_loading->loadFrameImage("bossIdle", "Images/보스/idleBoss .bmp", 350, 175, 2, 1);

	//보스오브젝트
	_loading->loadImage("bossRock0", "Images/보스/Boss_Rock0.bmp", 100, 108, true, RGB(255, 0, 255));
	_loading->loadImage("bossRock1", "Images/보스/Boss_Rock1.bmp", 100, 108, true, RGB(255, 0, 255));
	_loading->loadImage("bossRock2", "Images/보스/Boss_Rock2.bmp", 100, 108, true, RGB(255, 0, 255));
	_loading->loadImage("bossRock3", "Images/보스/Boss_Rock3.bmp", 100, 108, true, RGB(255, 0, 255));

}

void bossLoading::loadSound()
{

}
