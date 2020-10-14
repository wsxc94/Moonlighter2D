#include "stdafx.h"
#include "gotoDungeon.h"
#include "mapSaveLoad.h"

HRESULT gotoDungeon::init()
{
	
	this->loadTile();
	PLAYER->setX(1200);
	PLAYER->setY(1900);
	CAMERAMANAGER->init(PLAYER->getX(), PLAYER->getY(), 2380, 1995,0,0, WINSIZEX / 2, WINSIZEY / 2);

	_playerGotoAnim = new animation;
	_playerGotoAnim->init(IMAGEMANAGER->findImage("플레이어던전입장") , 0 , 7);

	_DungeonDoorAnim = new animation;
	_DungeonDoorAnim->init(IMAGEMANAGER->findImage("던전가는길문") , 0 ,7 , false , false);

	_dungeonDoorCheck = false;
	_portalPosCheck = false;

	return S_OK;
}

void gotoDungeon::release()
{
	//SAFE_DELETE(_playerGotoAnim);
	//SAFE_DELETE(_DungeonDoorAnim);
}

void gotoDungeon::update()
{
	PLAYER->update();

	float x = PLAYER->getX();
	float y = PLAYER->getY();

	if (y > 1500) _portalPosCheck = true;
	else _portalPosCheck = false;

	if ((y < 1030) && _dungeonDoorCheck) {
		/*cout << "문열기" << endl;
		_DungeonDoorAnim = new animation;
		_DungeonDoorAnim->init(IMAGEMANAGER->findImage("던전가는길문"), 0, 7, false, false);
		_dungeonDoorCheck = false;*/
		if (_DungeonDoorAnim->getAniState() != ANIMATION_PLAY)
		{
			_DungeonDoorAnim->aniPlay();
			_dungeonDoorCheck = false;
		}
	}
	else if ((y > 1030) && !_dungeonDoorCheck) {
		
		if (_DungeonDoorAnim->getAniState() != ANIMATION_PLAY)
		{
			_DungeonDoorAnim->aniReverse();
			_dungeonDoorCheck = true;
		}

	}

	_DungeonDoorAnim->update();

	CAMERAMANAGER->update(x, y);
	CAMERAMANAGER->movePivot(x, y);
	collTile();

	//cout << x << " " << y << endl;
	//_playerGotoAnim->update();
	

}

void gotoDungeon::render()
{
	CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("gotoDungeon"), 0, 0);
	if (INPUT->GetToggleKey(VK_F1))
	{
		for (int i = 0; i < _vTile.size(); i++)
		{
			RECT temp;
			if (!IntersectRect(&temp, &_vTile[i].rc, &CAMERAMANAGER->getRect()))continue;
			CAMERAMANAGER->FrameRect(getMemDC(), _vTile[i].rc, RGB(255, 255, 255));
		}
	}
	PLAYER->render(getMemDC());

	//_playerGotoAnim->render(getMemDC() , WINSIZEX/2 , WINSIZEY/2);
	_DungeonDoorAnim->CameraRender(getMemDC() , 565 , 770);


	/*char str[256];
	POINT tmp = CAMERAMANAGER->getRelativeMouse(_ptMouse);

	wsprintf(str, "X: %d", tmp.x);
	TextOut(getMemDC(), 5, 150, str, strlen(str));
	wsprintf(str, "Y: %d", tmp.y);
	TextOut(getMemDC(), 5, 170, str, strlen(str));*/

	CAMERAMANAGER->ZorderTotalRender(getMemDC());
}

void gotoDungeon::loadTile()
{
	_vTile = mapSaveLoad::getSingleton()->loadTile("maptool/gotoDungeon", 68, 57);

}

void gotoDungeon::collTile()
{
	for (int i = 0; i < _vTile.size(); i++)
	{
		RECT temp;
		if (IntersectRect(&temp, &PLAYER->getShadowRect(), &_vTile[i].rc))
		{

			if (_vTile[i].tState == TS_PORTAL) {
				if (!_portalPosCheck) {
				SOUNDMANAGER->stop("마을브금");
				SCENEMANAGER->loadScene("던전로딩");
				}
				else {
					SCENEMANAGER->loadScene("타운화면");
					PLAYER->setX(650);
					PLAYER->setY(40);
				}
			}
			if (_vTile[i].tState == TS_MOVEBAN)
			{
				int wid = temp.right - temp.left;
				int hei = temp.bottom - temp.top;

				if (wid > hei) // 위아래
				{
					if (temp.top == PLAYER->getShadowRect().top) // 아래
					{
						PLAYER->setY(PLAYER->getY() + hei);
					}
					else  // 위
					{
						PLAYER->setY(PLAYER->getY() - hei);
					}
				}
				else  // 양옆
				{
					if (temp.left == PLAYER->getShadowRect().left) // 오른쪽
					{
						PLAYER->setX(PLAYER->getX() + wid);
					}
					else // 왼쪽
					{
						PLAYER->setX(PLAYER->getX() - wid);
					}
				}
			}
		}
	}
}
