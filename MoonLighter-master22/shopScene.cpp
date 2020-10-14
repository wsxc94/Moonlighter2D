#include "stdafx.h"
#include "shopScene.h"

HRESULT shopScene::init()
{
	//클래스 초기화
	_npc = new ShopNpcManager;
	_npc->init();

	_displayStand = new displayStand;
	_displayStand->init();

	_pivotX = WINSIZEX / 2;
	_pivotY = WINSIZEY / 2;

	PLAYER->setX(700);
	PLAYER->setY(850);

	CAMERAMANAGER->init(PLAYER->getX(), PLAYER->getY(), 2000, 1000, 0, 0, WINSIZEX / 2, WINSIZEY / 2);

	_npcTime = 0;
	//675 870
	GoTownPortal = RectMake(670, 1000, 80, 50);

	IMAGEMANAGER->addImage("temp", 2000, 1000);

	return S_OK;
}

void shopScene::release()
{
	//_npc->release();

	//_displayStand->release();
	//SAFE_DELETE(_displayStand);
}

void shopScene::update()
{
	if (!SOUNDMANAGER->isPlaySound("상점브금")) {
		SOUNDMANAGER->play("상점브금", 0.3f);
	}

	if (INPUT->GetKey(VK_LEFT) && _pivotX >= WINSIZEX / 2) _pivotX -= 10;
	if (INPUT->GetKey(VK_RIGHT) && _pivotX < 1000 - WINSIZEX / 2) _pivotX += 10;
	if (INPUT->GetKey(VK_UP) && _pivotY > WINSIZEY / 2) _pivotY -= 10;
	if (INPUT->GetKey(VK_DOWN) && _pivotY < 1000 - WINSIZEY / 2) _pivotY += 10;

	_npc->update();
	_displayStand->update();

	ITEMMENU->update();
	PLAYER->update();
	CAMERAMANAGER->movePivot(PLAYER->getX(), PLAYER->getY());
	CAMERAMANAGER->update(PLAYER->getX(), PLAYER->getY());

	PlayerCol();
	PortaltoTown();
}

void shopScene::render()
{
	
	IMAGEMANAGER->findImage("상점픽셀")->render(IMAGEMANAGER->findImage("temp")->getMemDC(), 304, 132);
	CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("상점맵"), 304, 132);
	//CAMERAMANAGER->Render(getMemDC() , IMAGEMANAGER->findImage("상점픽셀"), 304, 132);
	
	_npc->render();
	_displayStand->render();
	
	CAMERAMANAGER->ZorderFrameRender(IMAGEMANAGER->findImage("상점길목"), 458 + IMAGEMANAGER->findImage("상점길목")->getFrameHeight(), 654, 458);

	CAMERAMANAGER->ZorderFrameRender(IMAGEMANAGER->findImage("상점문"), 870 + IMAGEMANAGER->findImage("상점문")->getFrameHeight(), 638, 870);

	CAMERAMANAGER->ZorderFrameRender(IMAGEMANAGER->findImage("상점침대"), 302 + IMAGEMANAGER->findImage("상점침대")->getFrameHeight()/4, 752, 302);

	CAMERAMANAGER->ZorderFrameRender(IMAGEMANAGER->findImage("상점좌판"), 674 + IMAGEMANAGER->findImage("상점좌판")->getFrameHeight()/4, 472, 674);

	CAMERAMANAGER->ZorderFrameRender(IMAGEMANAGER->findImage("상점책상"), 670 + IMAGEMANAGER->findImage("상점책상")->getFrameHeight()/4, 646, 670);

	CAMERAMANAGER->ZorderTotalRender(getMemDC());

	/*char str[256];
	POINT tmp = CAMERAMANAGER->getRelativeMouse(_ptMouse);

	wsprintf(str, "X: %d", tmp.x);
	TextOut(getMemDC(), 5, 150, str, strlen(str));
	wsprintf(str, "Y: %d", tmp.y);
	TextOut(getMemDC(), 5, 170, str, strlen(str));*/

	PLAYER->render(getMemDC());
	ITEMMENU->render(getMemDC());
	//IMAGEMANAGER->findImage("상점맵")->render(getMemDC(), 304, 132);

	//CAMERAMANAGER->Rectangle(getMemDC(), GoTownPortal);
	//Rectangle(getMemDC(), GoTownPortal);

	//CAMERAMANAGER->Rectangle(getMemDC(), PLAYER->getShadowRect());
}

void shopScene::PortaltoTown()
{
	RECT tmp;
	if (IntersectRect(&tmp , &PLAYER->getShadowRect() , &GoTownPortal))
	{
		SOUNDMANAGER->stop("상점브금");
		SCENEMANAGER->loadScene("타운화면");

		PLAYER->setX(1680);
		PLAYER->setY(450);
	}
}

void shopScene::PlayerCol()
{
	if (INPUT->GetKey(VK_SPACE))
	{
		PLAYER->setPlayerState(PLAYER_STATE::PLAYER_IDLE);
	}
	if (GetPixel(IMAGEMANAGER->findImage("temp")->getMemDC(),
		(PLAYER->getShadowRect().left + PLAYER->getShadowRect().right) /2 , PLAYER->getShadowRect().top) == RGB(255, 0, 0) ||
		GetPixel(IMAGEMANAGER->findImage("temp")->getMemDC(),
		(PLAYER->getShadowRect().left + PLAYER->getShadowRect().right) / 2, PLAYER->getShadowRect().bottom) == RGB(255, 0, 0) ||
		GetPixel(IMAGEMANAGER->findImage("temp")->getMemDC(),
		PLAYER->getShadowRect().left, (PLAYER->getShadowRect().bottom + PLAYER->getShadowRect().top) /2) == RGB(255, 0, 0) ||
		GetPixel(IMAGEMANAGER->findImage("temp")->getMemDC(),
		PLAYER->getShadowRect().right, (PLAYER->getShadowRect().bottom + PLAYER->getShadowRect().top) / 2) == RGB(255, 0, 0)) {

		if (PLAYER->getLeft()) {
			PLAYER->setX(PLAYER->getX() + 4);
		}
		if (PLAYER->getRight()) {
			PLAYER->setX(PLAYER->getX() - 4);
		}
		if (PLAYER->getUp()) {
			PLAYER->setY(PLAYER->getY() + 4);
		}
		if (PLAYER->getDown()) {
			PLAYER->setY(PLAYER->getY() - 4);
		}
	}
}
