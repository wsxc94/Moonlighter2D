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
	_loading->loadFrameImage("bossRock0", "Images/보스/Boss_Rock0 .bmp", 100, 108,1,1);
	_loading->loadFrameImage("bossRock1", "Images/보스/Boss_Rock1 .bmp", 100, 108, 1, 1);
	_loading->loadFrameImage("bossRock2", "Images/보스/Boss_Rock2 .bmp", 100, 108, 1, 1);
	_loading->loadFrameImage("bossRock3", "Images/보스/Boss_Rock3 .bmp", 100, 108, 1, 1);
	//_loading->loadImage("bossFist", "Images/보스/fist .bmp", 104, 373, true, RGB(255, 0, 255));
	_loading->loadFrameImage("bossFist", "Images/보스/fist2 .bmp", 623, 746, 6, 1);
	_loading->loadFrameImage("bossHand", "Images/보스/hand.bmp", 1140, 60, 19, 1);


	//던전로딩에있지만 테스트로 하는것
	_loading->loadFrameImage("golemScroll", "Images/던전/골렘던전스크롤11.bmp", 2277, 60, 11, 1);
	_loading->loadImage("semiBossHpBarFront(red)", "Images/몬스터/semiBossHpFront(red).bmp", 927, 17, true, RGB(255, 0, 255));
	_loading->loadImage("semiBossHpBarFront(white)", "Images/몬스터/semiBossHpFront(white).bmp", 927, 17, true, RGB(255, 0, 255));
	_loading->loadImage("semiBossHpBarBack", "Images/몬스터/semiBossHpBack.bmp", 947, 23, true, RGB(255, 0, 255));
	_loading->loadFrameImage("potalUpdate", "Images/던전/potalUpdate7.bmp", 917, 108, 7, 1);
	_loading->loadFrameImage("potalInit", "Images/던전/potalInit26.bmp", 3406, 108, 26, 1);
	_loading->loadFrameImage("potalPlayer", "Images/던전/potalPlayer25.bmp", 3275, 108, 25, 1);
	_loading->loadFrameImage("플레이어팬던트사용", "Images/플레이어/playerUsePendant31.bmp", 4464, 120, 31, 1);
	_loading->loadImage("messegeBox_potal", "Images/던전/messegeBox_potal.bmp", 183, 50, true, RGB(255, 0, 255));
	_loading->loadFrameImage("dungeonStageBar", "Images/던전결과창/dungeonProgress1X3.bmp", 85, 45, 1, 3);
	_loading->loadImage("resultBack", "Images/던전결과창/ui_dungeonResult4022.bmp", 1158, 656, true, RGB(255, 0, 255));
	
}

void bossLoading::loadSound()
{
	_loading->loadSound("bossBGM", "Sound/던전/bossGolemBGM.wav", true, true);


	_loading->loadSound("bossInit", "Sound/에너미/보스/보스처음등장.wav" );
	_loading->loadSound("bossRock", "Sound/에너미/보스/보스 돌던지기전.wav" );
	_loading->loadSound("bossHit", "Sound/에너미/보스/보스맞음.wav" );
	_loading->loadSound("bossDead", "Sound/에너미/보스/보스죽음.wav" );

}
