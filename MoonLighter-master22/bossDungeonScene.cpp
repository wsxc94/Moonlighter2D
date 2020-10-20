#include "stdafx.h"
#include "bossDungeonScene.h"

HRESULT bossDungeonScene::init()
{
	
	PLAYER->setX(1024);
	PLAYER->setY(839 * 2 - 150);

	//_golemKing = new bossGolemKing;
	//_golemKing->init(1067, 477);
	_golemKing = nullptr;
	_bsState = BS_INIT;

	CAMERAMANAGER->init(PLAYER->getX(), PLAYER->getY(), 2048, 839 * 2, 0, 0, WINSIZEX / 2, WINSIZEY / 2);
	CAMERAMANAGER->FadeInit(80, FADE_IN);
	CAMERAMANAGER->FadeStart();

	return S_OK;
}

void bossDungeonScene::release()
{
}

void bossDungeonScene::update()
{
	PLAYER->update();
	PLAYER->updateWeaponState();
	if(_golemKing)
	_golemKing->update();

	switch (_bsState)
	{
	case BS_INIT:
		if (!_golemKing && PLAYER->getY() < 1100)
		{
			_golemKing = new bossGolemKing;
			_golemKing->init(1067, 477);
		}
		break;
	case BS_UPDATE:
		break;
	default:
		break;
	}
	
	

	CAMERAMANAGER->update(PLAYER->getX(), PLAYER->getY());
	CAMERAMANAGER->movePivot(PLAYER->getX(), PLAYER->getY());
	
}

void bossDungeonScene::render()
{
	CAMERAMANAGER->StretchRender(getMemDC(), IMAGEMANAGER->findImage("bossRoomBack"), 1024, 839, 2.f);
	PLAYER->render(getMemDC());
	CAMERAMANAGER->ZorderTotalRender(getMemDC());

	if(_golemKing)
	_golemKing->render();

	POINT pt = CAMERAMANAGER->getRelativeMouse(_ptMouse);
	textOut(getMemDC(), 10, 120, to_string(pt.x).c_str(), to_string(pt.x).size());
	textOut(getMemDC(), 10, 150, to_string(pt.y).c_str(), to_string(pt.y).size());
}
