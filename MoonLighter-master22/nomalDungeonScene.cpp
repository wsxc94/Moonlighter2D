#include "stdafx.h"
#include "nomalDungeonScene.h"
#include "mapSaveLoad.h"

HRESULT nomalDungeonScene::init()
{
	if (PLAYERDATA->getIsEmblemReturn())
	{
		this->initFromSave();
		return S_OK;
	}

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
	PLAYERDATA->setIsInDungeon(true);
	//에너미 테스트

	//플레이어 초기화 
	PLAYER->setX(WINSIZEX / 2);
	PLAYER->setY(WINSIZEY / 2);

	SOUNDMANAGER->play("dungeonBGM",0.3f);
	PLAYERDATA->initDungeonHp();

	//카메라 초기화
	CAMERAMANAGER->init(WINSIZEX / 2, WINSIZEY / 2, WINSIZEX, WINSIZEY, 0, 0, WINSIZEX / 2, WINSIZEY / 2);
	CAMERAMANAGER->FadeInit(80, FADE_IN);
	CAMERAMANAGER->FadeStart();

	this->initItemSlot();
	this->getInvenItem();
	_resultKind = RESULT_PLAYERDIE;


	//포탈애니메이션 초기화
	_potal = nullptr;

	_aniBefore = new animation;

	_aniCenter = new animation;
	
	return S_OK;
}

HRESULT nomalDungeonScene::initFromSave()
{
	_currentDungeon = PLAYERDATA->getMapData();
	_dgFloor = PLAYERDATA->getDungeonFloor();

	_golemScroll = new animation;
	_golemScroll->init(IMAGEMANAGER->findImage("golemScroll"), 0, 7);
	PLAYERDATA->setIsInDungeon(true);
	PLAYERDATA->initDungeonHp();
	PLAYER->setX(_currentDungeon->getPotal()->getX());
	PLAYER->setY(_currentDungeon->getPotal()->getY());
	SOUNDMANAGER->play("dungeonBGM", 0.4f);
	CAMERAMANAGER->init(WINSIZEX / 2, WINSIZEY / 2, WINSIZEX, WINSIZEY, 0, 0, WINSIZEX / 2, WINSIZEY / 2);
	CAMERAMANAGER->FadeInit(80, FADE_IN);
	CAMERAMANAGER->FadeStart();
	this->initItemSlot();
	this->getInvenItem();
	_resultKind = RESULT_PLAYERDIE;
	_dState = DS_RETURN;
	_potal = _currentDungeon->getPotal();
	_potal->setPotalState(POTAL_INIT);
	_aniBefore = new animation;
	_aniCenter = new animation;
	_playerClone = new animation;
	_playerClone->init(IMAGEMANAGER->findImage("던전구르기"), 0, 4);
	_playerClone->aniStop();
	return S_OK;
}

void nomalDungeonScene::release()
{
	
}

void nomalDungeonScene::update()
{
	if (INPUT->GetKeyDown(VK_LEFT)) _dState = DS_UPDATE;
	if (INPUT->GetKeyDown(VK_RIGHT)) _dState = DS_RESULT;
	this->soundUpdate();

	switch (_dState)
	{
	case DS_UPDATE:
		PLAYERDATA->setIsActivate(true);
		//죽으면 결과창띄워라
		if (PLAYER->getPlayerState() == PLAYER_DIE)
		{
			_aniBefore->init(IMAGEMANAGER->findImage("죽음"), 0, 5);
			_aniCenter->init(IMAGEMANAGER->findImage("죽음"), 0, 5);
			_dState = DS_RESULT;
			_vEnemy = PLAYERDATA->getVEnemy();
			_killEnemy = PLAYERDATA->getKillEnemy();
			_resultKind = RESULT_PLAYERDIE;
			PLAYERDATA->setIsPendantReturn(true);
		}
		// 팬던트 사용했냐?? 사용했으면 에니메이션 띄우고 결과창 띄워라
		else if (ITEMMENU->getGoToTownPendant())
		{
			_aniBefore->init(IMAGEMANAGER->findImage("플레이어팬던트사용"), 0, 5);
			_aniCenter->init(IMAGEMANAGER->findImage("potalUpdate"), 0, 5,true);
			_dState = DS_RESULT;
			_vEnemy = PLAYERDATA->getVEnemy();
			_killEnemy = PLAYERDATA->getKillEnemy();
			_resultKind = RESULT_PENDANT;
			PLAYERDATA->setIsPendantReturn(true);
		}
		//엠블렘 사용했냐?? 사용했으면 포탈을 만들어라
		else if (ITEMMENU->getGoToTownEmblem())
		{
			_currentDungeon->initPotal();
			ITEMMENU->SetGoToTownEmblem(false);
		}
		this->emblemUpdate();
		this->dungeonUpdate();
		PLAYER->update();
		ITEMMENU->update();
		break;
	case DS_RESULT:
		//ui창 끄기
		PLAYERDATA->setIsActivate(false);
		_aniBefore->update();
		if (_aniBefore->getAniState() == ANIMATION_END)
		_aniCenter->update();
		if (_resultKind == RESULT_PLAYERDIE && _aniCenter->getCurIndex() >= _aniCenter->getImage()->getMaxFrameX())
			_aniCenter->aniPause();
		for (int i = 0; i < _vEnemy.size(); i++)
		{
			_vEnemy[i].attack->update();
		}
		if (_killEnemy)
			_killEnemy->attack->update();
		if (INPUT->GetKeyDown('J') && _aniBefore->getAniState() == ANIMATION_END)
		{
			SCENEMANAGER->loadScene("타운로딩");
			SOUNDMANAGER->stop("dungeonBGM");
			SOUNDMANAGER->stop("spaRoomBGM");
			SOUNDMANAGER->stop("bossRoomBGM");
			PLAYERDATA->vEnemyClear();
			
			if (_resultKind == RESULT_EMBLEM)
			{
				//엠블렘 사용 시 
				PLAYERDATA->saveDungeonMap(_currentDungeon);
				PLAYERDATA->setIsEmblemReturn(true);
				PLAYERDATA->setDungeonFloor(_dgFloor);
				PLAYERDATA->setIsInDungeon(false);
			}
			else
			{
				PLAYERDATA->setIsEmblemReturn(false);
				PLAYERDATA->setIsInDungeon(false);
			}
			this->release();
		}

		break;
	case DS_RETURN:
		_currentDungeon->update();
		_golemScroll->update();
		if (_potal->getPotalState() == POTAL_UPDATE)
		{
			_potal->setPotalState(POTAL_BREAK);
			PLAYER->setPlayerState(PLAYER_IDLE);
			PLAYER->setPlayerDirection(0);
		}
		if (_potal->getPotalState() == POTAL_BREAK && _potal->getAnimation()->getAniState() == ANIMATION_END)
		{
			_currentDungeon->releasePotal();
			_dState = DS_UPDATE;
		}
		break;
	default:
		break;
	}

	//카메라 업뎃
	CAMERAMANAGER->update(PLAYER->getX(),PLAYER->getY());
	CAMERAMANAGER->movePivot(PLAYER->getX(), PLAYER->getY());
}

void nomalDungeonScene::render()
{
	_currentDungeon->render();
	CAMERAMANAGER->ZorderTotalRender(getMemDC());
	switch (_dState)
	{
	case DS_UPDATE:
		this->golemScrollRender();
		PLAYER->render(getMemDC());
		if (INPUT->GetKey(VK_TAB))
		{
			this->minimapRender();
		}
		ITEMMENU->render(getMemDC());
		break;
	case DS_RESULT:
		switch (_resultKind)
		{
		case RESULT_PLAYERDIE:
			_aniBefore->centerRender(getMemDC(), PLAYER->getX(), PLAYER->getY() - 8);
			break;
		case RESULT_PENDANT:
			_aniBefore->centerRender(getMemDC(), PLAYER->getX() - 12, PLAYER->getY() - 8);
			break;
		case RESULT_EMBLEM:
			_aniBefore->stretchRender(getMemDC(), _potal->getX(), _potal->getY(),2.f);
			break;
		}
		if (_aniBefore->getAniState() == ANIMATION_END)
		{
			IMAGEMANAGER->addImage("dark", WINSIZEX, WINSIZEY)->alphaRender(getMemDC(), 0, 0, 170);
			IMAGEMANAGER->findImage("resultBack")->render(getMemDC(), 40, 22);
			this->resultRender();
			this->itemResultRender();
		}
		break;
	case DS_RETURN:
		this->golemScrollRender();
		
		if (_potal->getPotalState() == POTAL_BREAK)
		{
			PLAYER->render(getMemDC());
		}
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
		{
			SOUNDMANAGER->play("dungeonBGM", 0.3f);
		}
		else
		{
			if (ITEMMENU->getOpenMenu()) SOUNDMANAGER->setVolumn("dungeonBGM", 0.08f);
			else SOUNDMANAGER->setVolumn("dungeonBGM", 0.3f);
		}
		SOUNDMANAGER->stop("spaRoomBGM");
		SOUNDMANAGER->stop("bossRoomBGM");

		break;
	case DG_SEMIBOSS:
		if (!SOUNDMANAGER->isPlaySound("bossRoomBGM"))
		{
			SOUNDMANAGER->play("bossRoomBGM", 0.3f);
		}
		else
		{
			if (ITEMMENU->getOpenMenu()) SOUNDMANAGER->setVolumn("bossRoomBGM", 0.08f);
			else SOUNDMANAGER->setVolumn("bossRoomBGM", 0.3f);
		}
		SOUNDMANAGER->stop("spaRoomBGM");
		SOUNDMANAGER->stop("dungeonBGM");
		break;
	case DG_SPA:
		if (!SOUNDMANAGER->isPlaySound("spaRoomBGM"))
		{
			SOUNDMANAGER->play("spaRoomBGM", 0.3f);
		}
		else
		{
			if (ITEMMENU->getOpenMenu()) SOUNDMANAGER->setVolumn("spaRoomBGM", 0.08f);
			else SOUNDMANAGER->setVolumn("spaRoomBGM", 0.3f);
		}
		SOUNDMANAGER->stop("dungeonBGM");
		SOUNDMANAGER->stop("bossRoomBGM");
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
		if (_dgFloor == 1)
			this->setNewFloor();
		else if (_dgFloor == 2)
			SCENEMANAGER->loadScene("보스로딩");
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
		_aniCenter->centerRender(getMemDC(), 630, 170);
		_killEnemy->attack->setFrameY(_killEnemy->frameY);
		_killEnemy->attack->stretchRender(getMemDC(), 762, 240, _killEnemy->scale);
	}
	break;
	case RESULT_PENDANT:
	{
		_aniCenter->stretchRender(getMemDC(), 640, 170, 2.f);
		int cx = 762 - IMAGEMANAGER->findImage("bag_pendant")->getWidth() / 2;
		int cy = 240 - IMAGEMANAGER->findImage("bag_pendant")->getHeight() / 2;
		IMAGEMANAGER->findImage("bag_pendant")->render(getMemDC(), cx, cy);
	}
	break;
	case RESULT_EMBLEM:
	{
		_aniCenter->stretchRender(getMemDC(), 640, 170, 2.f);
		int cx = 762 - IMAGEMANAGER->findImage("bag_emblem")->getWidth() / 2;
		int cy = 240 - IMAGEMANAGER->findImage("bag_emblem")->getHeight() / 2;
		IMAGEMANAGER->findImage("bag_emblem")->render(getMemDC(), cx, cy);
	}
	break;
	}

	int destY;
	if (_vEnemy.size()/2/9 > 0)  destY = 70 / (_vEnemy.size() / 2 / 9);
	else destY = 70;
	for (int i = 0; i < _vEnemy.size() / 2; i++)
	{
		_vEnemy[i].attack->setFrameY(_vEnemy[i].frameY);
		_vEnemy[i].attack->stretchRender(getMemDC(), 724 + ((i % 9) * 50), 320 + (i / 9 * destY), _vEnemy[i].scale);
	}
	for (int i = _vEnemy.size() / 2, j = 0; i < _vEnemy.size(); i++)
	{
		_vEnemy[i].attack->setFrameY(_vEnemy[i].frameY);
		_vEnemy[i].attack->stretchRender(getMemDC(), 724 + ((j % 9) * 50), 480 + (j / 9 * destY) , _vEnemy[i].scale);
		j++;
	}

}

void nomalDungeonScene::emblemUpdate()
{
	if (!_currentDungeon->getPotal()) return;

	_potal = _currentDungeon->getPotal();

	if (INPUT->GetKeyDown('J') && _potal->getIsInRange())
	{
		_aniBefore->init(IMAGEMANAGER->findImage("potalPlayer"), 0, 5);
		_aniCenter->init(IMAGEMANAGER->findImage("potalUpdate"), 0, 5, true);
		_dState = DS_RESULT;
		_vEnemy = PLAYERDATA->getVEnemy();
		_killEnemy = PLAYERDATA->getKillEnemy();
		_resultKind = RESULT_EMBLEM;
	}

}
