#include "stdafx.h"
#include "bossDungeonScene.h"

HRESULT bossDungeonScene::init()
{
	CAMERAMANAGER->init(PLAYER->getX(), PLAYER->getY(), 2048, 839*2, 0, 0, WINSIZEX / 2, WINSIZEY / 2);
	CAMERAMANAGER->FadeInit(80, FADE_IN);
	CAMERAMANAGER->FadeStart();



	_aniBossUp = new animation;
	_aniBossUp->init(IMAGEMANAGER->findImage("bossUp"), 0, 7, false);
	_aniBossDead1 = new animation;
	_aniBossDead1->init(IMAGEMANAGER->findImage("bossDead1"), 0, 7, false);
	_aniBossDead2 = new animation;
	_aniBossDead2->init(IMAGEMANAGER->findImage("bossDead2"), 0, 7, false);
	_aniBossFistShoot = new animation;
	_aniBossFistShoot->init(IMAGEMANAGER->findImage("bossFistShoot"), 0, 7, false);
	_aniBossHandShootStart = new animation;
	_aniBossHandShootStart->init(IMAGEMANAGER->findImage("bossHandShootStart"), 0, 7, false);
	_aniBossHandShootEnd = new animation;
	_aniBossHandShootEnd->init(IMAGEMANAGER->findImage("bossHandShootEnd"), 0, 7, false);

	_vAni.push_back(_aniBossUp);
	_vAni.push_back(_aniBossDead1);
	_vAni.push_back(_aniBossDead2);
	_vAni.push_back(_aniBossFistShoot);
	_vAni.push_back(_aniBossHandShootStart);
	_vAni.push_back(_aniBossHandShootEnd);

	_idx = 0;

	_bossX = 1087;
	_bossY = 477;



	return S_OK;
}

void bossDungeonScene::release()
{
}

void bossDungeonScene::update()
{
	PLAYER->update();
	CAMERAMANAGER->update(PLAYER->getX(), PLAYER->getY());
	CAMERAMANAGER->movePivot(PLAYER->getX(), PLAYER->getY());
	
	_vAni[_idx]->update();
	if (_vAni[_idx]->getAniState() == ANIMATION_END)
	{
		_idx++;
		if (_idx >= _vAni.size()) _idx = 0;
		_vAni[_idx]->aniRestart();
	}

	if (INPUT->GetKeyDown(VK_UP))
	{
		_idx++;
		if (_idx >= _vAni.size()) _idx = 0;
	}
	
}

void bossDungeonScene::render()
{
	CAMERAMANAGER->StretchRender(getMemDC(), IMAGEMANAGER->findImage("bossRoomBack"), 1024, 839, 2.f);
	PLAYER->render(getMemDC());
	CAMERAMANAGER->ZorderTotalRender(getMemDC());

	//IMAGEMANAGER->addImage("test", 2048, 839 * 2);
	//_aniBossDead1->stretchRender(IMAGEMANAGER->findImage("test")->getMemDC(), _ptMouse.x, _ptMouse.y, 3.f);
	//IMAGEMANAGER->findImage("test")->alphaRender(getMemDC(), 100);
	//_aniBossDead1->ZorderStretchRender(477, _bossX, _bossY, 3.f);

	if(_idx == 2)
	_vAni[_idx]->ZorderStretchRender(_bossY, _bossX, _bossY + 90, 3.f);
	else
	_vAni[_idx]->ZorderStretchRender(_bossY, _bossX, _bossY, 3.f);


	POINT pt = CAMERAMANAGER->getRelativeMouse(_ptMouse);
	textOut(getMemDC(), 10, 120, to_string(pt.x).c_str(), to_string(pt.x).size());
	textOut(getMemDC(), 10, 150, to_string(pt.y).c_str(), to_string(pt.y).size());
}
