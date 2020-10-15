#include "stdafx.h"
#include "nomalDungeonScene.h"
#include "mapSaveLoad.h"

HRESULT nomalDungeonScene::init()
{
	//던전 업데이트 종류
	_dState = DS_UPDATE;
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
	PLAYERDATA->setIsInDungeon(true);
	//에너미 테스트


	PLAYER->setX(WINSIZEX / 2);
	PLAYER->setY(WINSIZEY / 2);

	SOUNDMANAGER->play("dungeonBGM",0.4f);


	//카메라 초기화
	CAMERAMANAGER->init(WINSIZEX / 2, WINSIZEY / 2, WINSIZEX, WINSIZEY, 0, 0, WINSIZEX / 2, WINSIZEY / 2);
	CAMERAMANAGER->FadeInit(80, FADE_IN);
	CAMERAMANAGER->FadeStart();

	this->initItemSlot();
	this->getInvenItem();
	_resultKind = RESULT_PLAYERDIE;

	// 에너미 테스트
	RESULTENEMY em;
	em.attack = new animation;
	em.attack->init(IMAGEMANAGER->findImage("해골공격"), 0, 7, true);
	em.frameY = 2;
	PLAYERDATA->pushVEnemy(em);
	em.attack = new animation;
	em.attack->init(IMAGEMANAGER->findImage("해골공격"), 0, 7, true);
	em.frameY = 2;
	PLAYERDATA->pushVEnemy(em);
	em.attack = new animation;
	em.attack->init(IMAGEMANAGER->findImage("해골공격"), 0, 7, true);
	em.frameY = 2;
	PLAYERDATA->pushVEnemy(em);
	for (int i = 0; i < 20; i++)
	{
		em.attack = new animation;
		em.attack->init(IMAGEMANAGER->findImage("해골공격"), 0, 7, true);
		em.frameY = 2;
		PLAYERDATA->pushVEnemy(em);
	}

	_potal = new animation;
	_potal->init(IMAGEMANAGER->findImage("potalPlayer"), 0, 5, true);
	
	return S_OK;
}

void nomalDungeonScene::release()
{
}

void nomalDungeonScene::update()
{
	if (INPUT->GetKeyDown(VK_LEFT)) _dState = DS_UPDATE;
	if (INPUT->GetKeyDown(VK_RIGHT)) _dState = DS_RESULT;
	_potal->update();//포탈테스트
	
	this->soundUpdate();

	switch (_dState)
	{
	case DS_UPDATE:
		PLAYERDATA->setIsActivate(true);

		if (PLAYER->getPlayerState() == PLAYER_DIE)
		{
			_dState = DS_RESULT;
			_vEnemy = PLAYERDATA->getVEnemy();
			_killEnemy = PLAYERDATA->getKillEnemy();
		}
		this->dungeonUpdate();
		PLAYER->update();
		ITEMMENU->update();
		break;
	case DS_RESULT:
		PLAYERDATA->setIsActivate(false);
		for (int i = 0; i < _vEnemy.size(); i++)
		{
			_vEnemy[i].attack->update();
		}
		if (_killEnemy)
			_killEnemy->attack->update();

		if (INPUT->GetKeyDown('J'))
		{
			SCENEMANAGER->loadScene("타운로딩");
		}
		break;
	default:
		break;
	}

	//에너미 받아오고 업데이트
	
	//카메라 업뎃
	CAMERAMANAGER->update(PLAYER->getX(),PLAYER->getY());
	CAMERAMANAGER->movePivot(PLAYER->getX(), PLAYER->getY());
}

void nomalDungeonScene::render()
{
	_currentDungeon->render();
	CAMERAMANAGER->ZorderTotalRender(getMemDC());
	_potal->ZorderStretchRender(WINSIZEY/2,WINSIZEX/2,WINSIZEY/2,2.f);//포탈테스트
	switch (_dState)
	{
	case DS_UPDATE: case DS_RETURN:
		this->golemScrollRender();
		
		PLAYER->render(getMemDC());
		if (INPUT->GetKey(VK_TAB))
		{
			this->minimapRender();
		}
		ITEMMENU->render(getMemDC());
		break;
	case DS_RESULT:
		IMAGEMANAGER->addImage("dark", WINSIZEX, WINSIZEY)->alphaRender(getMemDC(), 0, 0, 170);
		IMAGEMANAGER->findImage("resultBack")->render(getMemDC(), 40, 22);
		this->resultRender();
		
		this->itemResultRender();
		break;
	
	}

	
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

void nomalDungeonScene::soundUpdate()
{
	switch (_currentDungeon->getDungeonKind())
	{
	case DG_NOMAL:
		if (!SOUNDMANAGER->isPlaySound("dungeonBGM"))
			SOUNDMANAGER->play("dungeonBGM", 0.4f);
		SOUNDMANAGER->pause("spaRoomBGM");
		SOUNDMANAGER->pause("bossRoomBGM");
		break;
	case DG_SEMIBOSS:
		if (!SOUNDMANAGER->isPlaySound("bossRoomBGM"))
			SOUNDMANAGER->play("bossRoomBGM", 0.4f);
		SOUNDMANAGER->pause("spaRoomBGM");
		SOUNDMANAGER->pause("dungeonBGM");
		break;
	case DG_SPA:
		if (!SOUNDMANAGER->isPlaySound("spaRoomBGM"))
			SOUNDMANAGER->play("spaRoomBGM", 0.4f);
		SOUNDMANAGER->pause("dungeonBGM");
		SOUNDMANAGER->pause("bossRoomBGM");
		break;
	default:
		break;
	}
}

void nomalDungeonScene::dungeonUpdate()
{
	if (_currentDungeon->moveDungeon(PLAYER->getShadowRect()) != nullptr && _currentDungeon->getDungeonDoorState() == DUNGEONDOOR::DOOR_OPEN
		&& _currentDungeon->getDungeonKind() != DG_SEMIBOSS)
	{
		//플레이어 이동
		switch (_currentDungeon->moveDungeonDirection(PLAYER->getShadowRect()))
		{
		case 1:
			_currentDungeon = _currentDungeon->moveDungeon(PLAYER->getShadowRect());
			PLAYER->setX(1085 + 17);
			PLAYER->setY(350 + 17);
			break;
		case 2:
			_currentDungeon = _currentDungeon->moveDungeon(PLAYER->getShadowRect());
			PLAYER->setX(140 + 17);
			PLAYER->setY(350 + 17);
			break;
		case 3:
			_currentDungeon = _currentDungeon->moveDungeon(PLAYER->getShadowRect());
			PLAYER->setX(595 + 17);
			PLAYER->setY(595 + 17);
			break;
		case 4:
			_currentDungeon = _currentDungeon->moveDungeon(PLAYER->getShadowRect());
			PLAYER->setX(595 + 17);
			PLAYER->setY(105 + 17);
			break;
		}
		//던전 이동


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
}

void nomalDungeonScene::resultUpdate()
{

}

void nomalDungeonScene::initItemSlot()
{
	for (int i = 0; i < 28; i++)
	{
		_dungeonSlot[i].slotIdx = i;
		_dungeonSlot[i].isEmpty = true;
	}
}

void nomalDungeonScene::getInvenItem()
{	
	//원래 샵 인벤토리 벡터에 있던 아이템은 전부 지우기 
	
	deleteInvenItems();

	for (int i = 0; i < ITEMMENU->getInventory()->getItem().size(); i++)
	{
		//장착 슬롯에 있는 아이템을 제외하고 인벤토리 슬롯에 있던 아이템 가져오기 
		switch (ITEMMENU->getInventory()->getItem()[i]->getInvenPosIdx())
		{
			case 5: case 6: case 12: case 13:
			case 19: case 20: case 26: case 27:
				break;

			default:
				gameItem *item = new gameItem;
				item->init(ITEMMENU->getInventory()->getItem()[i]);
				_vItem.push_back(item);
				_dungeonSlot[item->getInvenPosIdx()].isEmpty = false;
				break;
		}
	}
}

void nomalDungeonScene::deleteInvenItems()
{
	for (int i = 0; i < _vItem.size(); )
	{
		switch (_vItem[i]->getInvenPosIdx())
		{
			case 5: case 6: case 12: case 13:
			case 19: case 20: case 26: case 27:
				i++;
				break;

			default:
				SAFE_DELETE(_vItem[i]);
				_vItem.erase(_vItem.begin() + i);
				break;
		}
	}
}

void nomalDungeonScene::itemResultRender()
{
	//IMAGEMANAGER->render("menu_shopInventory", getMemDC(), 100, 86);
	for (int i = 0; i < 28; i++)
	{
		//비어있는 슬롯은 건너뛰기 
		if (_dungeonSlot[i].isEmpty) continue;

		//비어있지 않은 슬롯은 인벤토리 벡터에서 해당 슬롯의 인덱스와 일치하는 아이템을 출력 
		for (int j = 0; j < _vItem.size(); j++)
		{
			//인벤토리 벡터에서 슬롯의 인덱스와 일치하지 않는 아이템은 건너뛰기 
			if (_vItem[j]->getInvenPosIdx() != i) continue;

			int columnIdx = _vItem[j]->getInvenPosIdx() % 7;
			int rowIdx = _vItem[j]->getInvenPosIdx() / 7;

			switch (_vItem[j]->getInvenPosIdx())
			{
			case 0: case 1: case 2: case 3: case 4:
				_vItem[j]->getItemImg()->render(getMemDC(),
					140 + (columnIdx * 72), 172 + 152 + 6);
				countRender(_vItem[j]->getCount(),
					180 + (columnIdx * 72), 208 + 152 + 6);
				break;

			case 7: case 8: case 9: case 10: case 11:
			case 14: case 15: case 16: case 17: case 18:
			case 21: case 22: case 23: case 24: case 25:
				_vItem[j]->getItemImg()->render(getMemDC(),
					140 + (columnIdx * 72), 258 + 152 + 6 + ((rowIdx - 1) * 72));
				countRender(_vItem[j]->getCount(),
					180 + (columnIdx * 72), 294 + 152 + 6 + ((rowIdx - 1) * 72));
				break;

			}//end of switch
		}//end of for(j)
	}//end of for(i)
}

void nomalDungeonScene::countRender(int count, int destX, int destY)
{
	if (count == 0)
	{
		IMAGEMANAGER->render("0_black", getMemDC(), destX, destY);
		return;
	}

	for (int i = 1, distance = 0; i <= count; i *= 10)
	{
		int number = (count / i) % 10;

		char keyName[16];

		wsprintf(keyName, "%d_black", number);
		IMAGEMANAGER->render(keyName, getMemDC(), destX - (distance * 12), destY);
		distance++;

	}//end of for
}

void nomalDungeonScene::golemScrollRender()
{
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
}

void nomalDungeonScene::updateRender()
{
}

void nomalDungeonScene::resultRender()
{
	RECT txtRC = RectMake(1198, 584, 82, 20);
	HFONT hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
		0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("JejuGothic"));
	HFONT oFont = (HFONT)SelectObject(getMemDC(), hFont);
	SetTextColor(getMemDC(), RGB(255, 255, 255));
	string str = " X " + to_string(_vEnemy.size());
	DrawText(getMemDC(), str.c_str(), -1, &txtRC, DT_LEFT | DT_VCENTER);
	txtRC = RectMake(486, 21, 311, 34);
	str = "Golem Dungeon " + to_string(_dgFloor);
	DrawText(getMemDC(), str.c_str(), -1, &txtRC, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	
	switch (_resultKind)
	{
	case RESULT_PLAYERDIE:
	{
		int cx = 630 - IMAGEMANAGER->findImage("죽음")->getFrameWidth() / 2;
		int cy = 170 - IMAGEMANAGER->findImage("죽음")->getFrameHeight() / 2;
		IMAGEMANAGER->findImage("죽음")->frameRender(getMemDC(), cx, cy, 9, 0);
		_killEnemy->attack->setFrameY(_killEnemy->frameY);
		_killEnemy->attack->centerRender(getMemDC(), 762, 240);
	}
		break;
	case RESULT_RETURN:
		break;
	default:
		break;
	}

	int destY = 70 / (_vEnemy.size() / 2 / 9);
	for (int i = 0; i < _vEnemy.size() / 2; i++)
	{
		_vEnemy[i].attack->setFrameY(_vEnemy[i].frameY);
		_vEnemy[i].attack->centerRender(getMemDC(), 724 + ((i % 9) * 50), 320 + (i / 9 * destY));
	}
	for (int i = _vEnemy.size() / 2, j = 0; i < _vEnemy.size(); i++)
	{
		_vEnemy[i].attack->setFrameY(_vEnemy[i].frameY);
		_vEnemy[i].attack->centerRender(getMemDC(), 724 + ((j % 9) * 50), 480 + (j / 9 * destY));
		j++;
	}

}
