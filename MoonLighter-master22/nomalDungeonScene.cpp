#include "stdafx.h"
#include "nomalDungeonScene.h"
#include "mapSaveLoad.h"

HRESULT nomalDungeonScene::init()
{
	//던전층수
	_dgFloor = 1;

	_startDungeon = new DungeonMap(0, 0);
	_startDungeon->setCurrentFloor(_dgFloor);
	_startDungeon->init();
	_currentDungeon = _startDungeon;

	_golemScroll = new animation;
	_golemScroll->init(IMAGEMANAGER->findImage("golemScroll"), 0, 7);

	//플레이어 테스트
	_player = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, 40, 40);
	//에너미 테스트


	PLAYER->setX(WINSIZEX / 2);
	PLAYER->setY(WINSIZEY / 2);


	//카메라 초기화
	CAMERAMANAGER->init(WINSIZEX / 2, WINSIZEY / 2, WINSIZEX, WINSIZEY, 0, 0, WINSIZEX / 2, WINSIZEY / 2);
	CAMERAMANAGER->FadeInit(80, FADE_IN);
	CAMERAMANAGER->FadeStart();

	return S_OK;
}

void nomalDungeonScene::release()
{
}

void nomalDungeonScene::update()
{
	// 던전 배경음 
	if (!SOUNDMANAGER->isPlaySound("dungeonBGM"))
	{
		SOUNDMANAGER->play("dungeonBGM",0.2f);
	}

	if (_currentDungeon->moveDungeon(PLAYER->getShadowRect()) != nullptr && _currentDungeon->getDungeonDoorState() == DUNGEONDOOR::DOOR_OPEN)
	{
		if (_currentDungeon->getDungeonKind() == DG_NOMAL)
		{
			//플레이어 이동
			switch (_currentDungeon->moveDungeonDirection(PLAYER->getShadowRect()))
			{
			case 1:
				PLAYER->setX(1085 + 17);
				PLAYER->setY(350 + 17);
				break;
			case 2:
				PLAYER->setX(140 + 17);
				PLAYER->setY(350 + 17);
				break;
			case 3:
				PLAYER->setX(595 + 17);
				PLAYER->setY(595 + 17);
				break;
			case 4:
				PLAYER->setX(595 + 17);
				PLAYER->setY(105 + 17);
				break;
			}
			//던전 이동
			_currentDungeon = _currentDungeon->moveDungeon(PLAYER->getShadowRect());
		}
	}
	else if (_currentDungeon->moveDungeonDirection(PLAYER->getShadowRect()) == 5 && _currentDungeon->getDungeonDoorState() == DUNGEONDOOR::DOOR_OPEN)
	{
		this->setNewFloor();
	}


	//골렘스크롤 업뎃
	_golemScroll->update();
	//미니맵 안간부분 추가해줌 (건들필요없음)
	if (this->minimapPush(_currentDungeon->getDungeonXY()))
	{
		_vMinimap.push_back(make_pair(_currentDungeon->getDungeonXY(), _currentDungeon));
	}


	//던전 업뎃
	_currentDungeon->update();
	//카메라 업뎃
	CAMERAMANAGER->update(PLAYER->getX(),PLAYER->getY());
	CAMERAMANAGER->movePivot(PLAYER->getX(), PLAYER->getY());


	PLAYER->update();
	ITEMMENU->update();
}

void nomalDungeonScene::render()
{

	PLAYER->render(getMemDC());
	_currentDungeon->render();

	//골렘 스크롤 렌더
	if (_golemScroll->getAniState() == ANIMATION_PLAY)
	{
		_golemScroll->ZorderStretchRender(2000, WINSIZEX / 2, WINSIZEY - 150, 2.f);
		RECT txtRC = RectMakeCenter(WINSIZEX / 2, WINSIZEY - 70, 300, 40);
		HFONT hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
			0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("JejuGothic"));
		CAMERAMANAGER->ZorderDrawText("골렘   던전", WINSIZEY / 2, txtRC, hFont, RGB(255, 255, 255), DT_CENTER | DT_WORDBREAK | DT_VCENTER);
		txtRC = RectMakeCenter(WINSIZEX / 2, WINSIZEY - 50, 300, 40);
		CAMERAMANAGER->ZorderDrawText(to_string(_dgFloor), WINSIZEY / 2, txtRC, hFont, RGB(255, 255, 255), DT_CENTER | DT_WORDBREAK | DT_VCENTER);
	}

	if (INPUT->GetKey(VK_TAB))
	{
		this->minimapRender();
	}

	CAMERAMANAGER->ZorderTotalRender(getMemDC());
	ITEMMENU->render(getMemDC());
}
bool nomalDungeonScene::minimapPush(POINT pt)
{
	for (int i = 0; i < _vMinimap.size(); i++)
	{
		if (_vMinimap[i].first.x == pt.x && _vMinimap[i].first.y == pt.y) return false;
	}
	return true;
}
void nomalDungeonScene::minimapRender()
{
	IMAGEMANAGER->addImage("minimapBack", WINSIZEX, WINSIZEY)->alphaRender(getMemDC(), 0, 0, 150);

	for (int i = 0; i < _vMinimap.size(); i++)
	{
		RECT rc = RectMakeCenter(WINSIZEX / 2 + (_vMinimap[i].first.x * 100), WINSIZEY / 2 + (_vMinimap[i].first.y * 65), 86, 48);
		//Rectangle(getMemDC(), rc);
		IMAGEMANAGER->findImage("minimap")->frameRender(getMemDC(), rc.left, rc.top, 0, 0);

		if (_vMinimap[i].second->getLeftDG() != nullptr)        IMAGEMANAGER->findImage("minimapEntranceW")->frameRender(getMemDC(), rc.left - 6, rc.bottom - 28, 0, 0);
		else                                            IMAGEMANAGER->findImage("minimapEntranceW")->frameRender(getMemDC(), rc.left, rc.bottom - 28, 0, 1);
		if (_vMinimap[i].second->getRightDG() != nullptr)    IMAGEMANAGER->findImage("minimapEntranceW")->frameRender(getMemDC(), rc.right - 2, rc.bottom - 28, 0, 0);
		else                                            IMAGEMANAGER->findImage("minimapEntranceW")->frameRender(getMemDC(), rc.right - 2, rc.bottom - 28, 0, 1);
		if (_vMinimap[i].second->getTopDG() != nullptr)        IMAGEMANAGER->findImage("minimapEntranceH")->frameRender(getMemDC(), rc.left + 38, rc.top - 6, 0, 0);
		else                                            IMAGEMANAGER->findImage("minimapEntranceH")->frameRender(getMemDC(), rc.left + 38, rc.top - 4, 0, 1);
		if (_vMinimap[i].second->getBottomDG() != nullptr)    IMAGEMANAGER->findImage("minimapEntranceH")->frameRender(getMemDC(), rc.left + 38, rc.bottom, 0, 0);
		else                                            IMAGEMANAGER->findImage("minimapEntranceH")->frameRender(getMemDC(), rc.left + 38, rc.bottom - 6, 0, 1);
	}

	RECT rc = RectMakeCenter(WINSIZEX / 2 + (_currentDungeon->getDungeonXY().x * 100), WINSIZEY / 2 + (_currentDungeon->getDungeonXY().y * 65), 86, 48);
	IMAGEMANAGER->findImage("minimap")->frameRender(getMemDC(), rc.left, rc.top, 1, 0);

	if (_currentDungeon->getLeftDG() != nullptr)        IMAGEMANAGER->findImage("minimapEntranceW")->frameRender(getMemDC(), rc.left - 6, rc.bottom - 28, 1, 0);
	else                                        IMAGEMANAGER->findImage("minimapEntranceW")->frameRender(getMemDC(), rc.left, rc.bottom - 28, 1, 1);
	if (_currentDungeon->getRightDG() != nullptr)    IMAGEMANAGER->findImage("minimapEntranceW")->frameRender(getMemDC(), rc.right - 2, rc.bottom - 28, 1, 0);
	else                                        IMAGEMANAGER->findImage("minimapEntranceW")->frameRender(getMemDC(), rc.right - 2, rc.bottom - 28, 1, 1);
	if (_currentDungeon->getTopDG() != nullptr)        IMAGEMANAGER->findImage("minimapEntranceH")->frameRender(getMemDC(), rc.left + 38, rc.top - 6, 1, 0);
	else                                        IMAGEMANAGER->findImage("minimapEntranceH")->frameRender(getMemDC(), rc.left + 38, rc.top - 4, 1, 1);
	if (_currentDungeon->getBottomDG() != nullptr)    IMAGEMANAGER->findImage("minimapEntranceH")->frameRender(getMemDC(), rc.left + 38, rc.bottom, 1, 0);
	else                                        IMAGEMANAGER->findImage("minimapEntranceH")->frameRender(getMemDC(), rc.left + 38, rc.bottom - 6, 1, 1);
	
}

void nomalDungeonScene::setNewFloor()
{
	SAFE_DELETE(_startDungeon);
	//던전층수
	_dgFloor++;

	_startDungeon = new DungeonMap(0, 0);
	_startDungeon->setCurrentFloor(_dgFloor);
	_startDungeon->init();
	_currentDungeon = _startDungeon;

	_golemScroll->aniRestart();

	//플레이어 테스트
	_player = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, 40, 40);
	//에너미 테스트

	PLAYER->setX(WINSIZEX / 2);
	PLAYER->setY(WINSIZEY / 2);


	CAMERAMANAGER->FadeInit(80, FADE_IN);
	CAMERAMANAGER->FadeStart();
}
