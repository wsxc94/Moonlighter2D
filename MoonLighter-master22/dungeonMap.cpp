#include "stdafx.h"
#include "dungeonMap.h"
#include "mapSaveLoad.h"

DungeonMap::DungeonMap(int x, int y)
{
	_itemManager = new itemManager;
	_itemManager->init();
	_x = x;
	_y = y;
	_fileName = "maptool/dungeon" + to_string(RANDOM->range(14) + 1);
	_backImg = "dungeonBack";
	_isCheck = false;
	_isClear = false;
	_leftDg = nullptr;
	_rightDg = nullptr;
	_topDg = nullptr;
	_bottomDg = nullptr;
	_leftDoor = nullptr;
	_rightDoor = nullptr;
	_topDoor = nullptr;
	_bottomDoor = nullptr;
	_checkLoadTile = false;
	_doorState = DOOR_OPEN;
	_dgKind = DG_NOMAL;
	_test = RANDOM->range(1000);
}

HRESULT DungeonMap::init()
{
	this->setStartDungeon();

	return S_OK;
}

void DungeonMap::release()
{

}

void DungeonMap::update()
{
	this->dgDoorUpdate();
	if (_vEnemy.size() > 0 && _doorState == DUNGEONDOOR::DOOR_OPEN)
	{
		_doorState = DUNGEONDOOR::DOOR_CLOSE;
		this->dgDoorClose();			// 적이있으면 문을 닫는다
	}
	if (_vEnemy.size() == 0 && _doorState == DUNGEONDOOR::DOOR_CLOSE)
	{
		_doorState = DUNGEONDOOR::DOOR_OPEN;
		this->dgDoorOpen();			// 적이 다 죽었으면 문을 닫는다
	}

	// 플레이어랑 충돌
	this->checkColiMoveBen();
	this->checkColiHole();
	this->checkCollisionSpa();

	//무기충돌
	this->checkColiArrow();
	//에너미 업뎃
	this->enemyUpdate();
	// 아이템 업뎃
	this->itemUpdate();

}

void DungeonMap::render()
{
	CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage(_backImg), 0, 0);
	for (int i = 0; i < _vTile.size(); i++)
	{
		if (_vTile[i].terrain == TR_NONE || _vTile[i].key == "") continue;
		//IMAGEMANAGER->findImage(_vTile[i].key)->frameRender(getMemDC(), _vTile[i].rc.left, _vTile[i].rc.top, _vTile[i].terrainFrameX, _vTile[i].terrainFrameY);
		CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage(_vTile[i].key), _vTile[i].rc.left, _vTile[i].rc.top, _vTile[i].terrainFrameX, _vTile[i].terrainFrameY);
	}
	for (int i = 0; i < _vDGObject.size(); i++)
	{
		_vDGObject[i]->render();
	}
	//에너미 렌더
	for (int i = 0; i < _vEnemy.size(); i++)
	{
		_vEnemy[i]->render();
	}
	//아이템 렌더
	for (int i = 0; i < _vItem.size(); i++)
	{
		_vItem[i]->render(getMemDC());
	}
	for (int i = 0; i < _vTile.size(); i++)
	{
		if (_vTile[i].tState == TS_PORTAL) FrameRect(getMemDC(), _vTile[i].rc, RGB(255, 255, 255));
	}
	this->dgDoorRender();

}

void DungeonMap::setStartDungeon()
{
	_fileName = "maptool/dungeonStart";
	_vMaps.push_back(this);
	_mapSize = 3;

	_leftDg = new DungeonMap(-1, 0);
	_vMaps.push_back(_leftDg);
	this->setRandomMap(&_leftDg);

	_rightDg = new DungeonMap(1, 0);
	_vMaps.push_back( _rightDg);
	this->setRandomMap(&_rightDg);

	_topDg = new DungeonMap(0, -1);
	_vMaps.push_back( _topDg);
	this->setRandomMap(&_topDg);

	this->bossMapDfs();
	DungeonMap* temp = this;
	this->setBossMap(&temp, 100);
	this->connectMap(&_leftDg);
	this->connectMap(&_rightDg);
	this->connectMap(&_topDg);
	this->loadTiles(&temp);


}

void DungeonMap::setRandomMap(DungeonMap ** map)
{
	if (_mapSize <= 0) return;
	DungeonMap* cur = *map;
	int left = RANDOM->range(2);
	int right = RANDOM->range(2);
	int top = RANDOM->range(2);
	int bottom = RANDOM->range(2);

	if (left == 1 && !isFindMap(PointMake(cur->_x - 1, cur->_y)))
	{
		cur->_leftDg = new DungeonMap(cur->_x - 1, cur->_y);
		_vMaps.push_back(cur->_leftDg);
		_mapSize--;
		this->setRandomMap(&cur->_leftDg);
	}
	if (right == 1 && !isFindMap(PointMake(cur->_x + 1, cur->_y)))
	{
		cur->_rightDg = new DungeonMap(cur->_x + 1, cur->_y);
		_vMaps.push_back(cur->_rightDg);
		_mapSize--;
		this->setRandomMap(&cur->_rightDg);
	}
	if (top == 1 && !isFindMap(PointMake(cur->_x, cur->_y - 1)))
	{
		cur->_topDg = new DungeonMap(cur->_x, cur->_y - 1);
		_vMaps.push_back(cur->_topDg);
		_mapSize--;
		this->setRandomMap(&cur->_topDg);
	}
	if (bottom == 1 && !isFindMap(PointMake(cur->_x, cur->_y + 1)))
	{
		cur->_bottomDg = new DungeonMap(cur->_x, cur->_y + 1);
		_vMaps.push_back(cur->_bottomDg);
		_mapSize--;
		this->setRandomMap(&cur->_bottomDg);
	}
}

void DungeonMap::connectMap(DungeonMap ** map)
{
	DungeonMap* cur = *map;

	if (cur->_leftDg != nullptr) this->connectMap(&cur->_leftDg);
	if (cur->_rightDg != nullptr) this->connectMap(&cur->_rightDg);
	if (cur->_topDg != nullptr) this->connectMap(&cur->_topDg);
	if (cur->_bottomDg != nullptr) this->connectMap(&cur->_bottomDg);

	if (isFindMap(PointMake(cur->_x - 1, cur->_y)) && cur->_leftDg == nullptr) // 왼
	{
		cur->_leftDg = findMap(PointMake(cur->_x - 1, cur->_y));
	}
	if (isFindMap(PointMake(cur->_x + 1, cur->_y)) && cur->_rightDg == nullptr) // 오
	{
		cur->_rightDg = findMap(PointMake(cur->_x + 1, cur->_y));
	}
	if (isFindMap(PointMake(cur->_x, cur->_y - 1)) && cur->_topDg == nullptr && findMap(PointMake(cur->_x, cur->_y - 1)) != this) // 위 위에가 시작방인거 예외처리
	{
		cur->_topDg = findMap(PointMake(cur->_x, cur->_y - 1));
	}
	if (isFindMap(PointMake(cur->_x, cur->_y + 1)) && cur->_bottomDg == nullptr) // 아래
	{
		cur->_bottomDg = findMap(PointMake(cur->_x, cur->_y + 1));
	}
}

void DungeonMap::bossMapDfs()
{
	DungeonMap* temp = this;
	temp->_isCheck = true;
	_qTemp.push(temp);

	while (!_qTemp.empty())
	{
		if (_qTemp.front()->_leftDg != nullptr && _qTemp.front()->_leftDg->_isCheck == false)
		{
			_qTemp.front()->_leftDg->_isCheck = true;
			_qTemp.push(_qTemp.front()->_leftDg);
		}
		if (_qTemp.front()->_rightDg != nullptr && _qTemp.front()->_rightDg->_isCheck == false)
		{
			_qTemp.front()->_rightDg->_isCheck = true;
			_qTemp.push(_qTemp.front()->_rightDg);
		}
		if (_qTemp.front()->_topDg != nullptr && _qTemp.front()->_topDg->_isCheck == false)
		{
			_qTemp.front()->_topDg->_isCheck = true;
			_qTemp.push(_qTemp.front()->_topDg);
		}
		if (_qTemp.front()->_bottomDg != nullptr && _qTemp.front()->_bottomDg->_isCheck == false)
		{
			_qTemp.front()->_bottomDg->_isCheck = true;
			_qTemp.push(_qTemp.front()->_bottomDg);
		}
		_qDfs.push(_qTemp.front());
		_qTemp.pop();
	}
}

void DungeonMap::setBossMap(DungeonMap ** map, int percent)
{
	if (_qDfs.empty()) return;
	DungeonMap* cur = *map;
	if (percent == 0) percent = 1;
	if (cur->_x != _qDfs.front()->_x || cur->_y != _qDfs.front()->_y)
	{
		if (cur->_leftDg != nullptr) this->setBossMap(&cur->_leftDg, percent / 10);
		if (cur->_rightDg != nullptr) this->setBossMap(&cur->_rightDg, percent / 10);
		if (cur->_topDg != nullptr) this->setBossMap(&cur->_topDg, percent / 10);
		if (cur->_bottomDg != nullptr)this->setBossMap(&cur->_bottomDg, percent / 10);
	}
	else if (cur->_x == _qDfs.front()->_x && cur->_y == _qDfs.front()->_y)
	{
		if (!isFindMap(PointMake(cur->_x, cur->_y - 1)) && cur->_topDg == nullptr && RANDOM->range(percent) == 0)
		{
			cur->_topDg = new DungeonMap(cur->_x, cur->_y - 1);
			cur->_topDg->_backImg = "dungeonSpa";
			cur->_topDg->_fileName = "maptool/dungeonStart";
			_vMaps.push_back(cur->_topDg);
			cur->_topDg->_topDg = new DungeonMap(cur->_x, cur->_y - 2);
			cur->_topDg->_topDg->_backImg = "dungeonBack";
			cur->_topDg->_topDg->_fileName = "maptool/dungeonStart";
			cur->_topDg->_topDg->_dgKind = DG_SEMIBOSS;
			_vMaps.push_back(cur->_topDg->_topDg);
			while (!_qDfs.empty())
			{
				_qDfs.pop();
			}
			return;
		}
		else
		{
			_qDfs.pop();
			DungeonMap* temp = this;
			if (cur->_leftDg != nullptr) this->setBossMap(&temp, percent);
			if (cur->_rightDg != nullptr) this->setBossMap(&temp, percent);
			if (cur->_topDg != nullptr) this->setBossMap(&temp, percent);
			if (cur->_bottomDg != nullptr)this->setBossMap(&temp, percent);
		}
	}
}

void DungeonMap::loadTiles(DungeonMap ** map)
{
	if ((*map)->_checkLoadTile == true) return;
	(*map)->_vTile = mapSaveLoad::getSingleton()->loadTile((*map)->_fileName, 37, 21);
	(*map)->_vObject = mapSaveLoad::getSingleton()->loadObject((*map)->_fileName, 37, 21);
	if ((*map)->_fileName != "maptool/dungeonStart")
	{
		//오브젝트 객체 추가
		for (int i = 0; i < (*map)->_vObject.size(); i++)
		{
			if ((*map)->_vObject[i].obj != OBJ_OBJECT || (*map)->_vObject[i].key == "") continue;
			object* obj = new object;
			obj->init((*map)->_vObject[i].rc.left, (*map)->_vObject[i].rc.top, (*map)->_vObject[i].key);
			(*map)->_vDGObject.push_back(obj);
		}
		//구멍상태 추가
		for (int i = 0; i < (*map)->_vTile.size(); i++)
		{
			if ((*map)->_vTile[i].terrain == TR_HOLE) (*map)->_vTile[i].tState = TS_HOLE;
		}
		// 에너미 추가
		for (int i = 0; i < (*map)->_vObject.size(); i++)
		{
			if ((*map)->_vObject[i].obj != OBJ_NPC || (*map)->_vObject[i].key == "") continue;
			if ((*map)->_vObject[i].key == "골렘전사")
			{
				enemy* _em;
				if (RANDOM->range(2) == 0)
				{
					_em = new golem;
				}
				else _em = new redGolem;
				 
				_em->init(((*map)->_vObject[i].rc.left + (*map)->_vObject[i].rc.right) / 2, ((*map)->_vObject[i].rc.top + (*map)->_vObject[i].rc.bottom) /2);
				_em->initTileSize(28, 15);
				_em->setWallTile((*map)->_vTile);
				(*map)->_vEnemy.push_back(_em);
			}
			if ((*map)->_vObject[i].key == "항아리")
			{
				enemy* _em = new potGolem;
				_em->init(((*map)->_vObject[i].rc.left + (*map)->_vObject[i].rc.right) / 2, ((*map)->_vObject[i].rc.top + (*map)->_vObject[i].rc.bottom) / 2);
				_em->initTileSize(28, 15);
				_em->setWallTile((*map)->_vTile);
				(*map)->_vEnemy.push_back(_em);
			}
			if ((*map)->_vObject[i].key == "해골전사")
			{
				enemy* _em = new skullHammer;
				_em->init(((*map)->_vObject[i].rc.left + (*map)->_vObject[i].rc.right) / 2, ((*map)->_vObject[i].rc.top + (*map)->_vObject[i].rc.bottom) / 2);
				_em->initTileSize(28, 15);
				_em->setWallTile((*map)->_vTile);
				(*map)->_vEnemy.push_back(_em);
			}
			if ((*map)->_vObject[i].key == "babySlime")
			{
				enemy* _em = new gasMan;
				_em->init(((*map)->_vObject[i].rc.left + (*map)->_vObject[i].rc.right) / 2, ((*map)->_vObject[i].rc.top + (*map)->_vObject[i].rc.bottom) / 2);
				_em->initTileSize(28, 15);
				_em->setWallTile((*map)->_vTile);
				(*map)->_vEnemy.push_back(_em);
			}
		}
	}
	
	
	//던전 문들 생성

	if ((*map)->_leftDg != nullptr)
	{
		(*map)->_leftDoor = new dgDoor;
		(*map)->_leftDoor->init(35, 280, "던전문(양옆)", 1);
	}
	if ((*map)->_rightDg != nullptr)
	{
		(*map)->_rightDoor = new dgDoor;
		(*map)->_rightDoor->init(1120, 280, "던전문(양옆)", 0);
	}
	if ((*map)->_topDg != nullptr)
	{
		(*map)->_topDoor = new dgDoor;
		(*map)->_topDoor->init(525, 0, "던전문(위아래)", 0);
	}
	if ((*map)->_bottomDg != nullptr)
	{
		(*map)->_bottomDoor = new dgDoor;
		(*map)->_bottomDoor->init(525, 630, "던전문(위아래)", 1);
	}

	if ((*map)->_dgKind == DG_SEMIBOSS)
	{
		switch (RANDOM->range(3))
		{
		case 0: // 왼쪽에 보스문 생성
			(*map)->_leftDoor = new dgBossDoor;
			(*map)->_leftDoor->init(43, 235, "보스문(양옆)", 1);
			break;
		case 1: // 오른쪽에 보스문 생성
			(*map)->_rightDoor = new dgBossDoor;
			(*map)->_rightDoor->init(1109, 235, "보스문(양옆)", 0);
			break;
		case 2: // 위에 보스문 생성
			(*map)->_topDoor = new dgBossDoor;
			(*map)->_topDoor->init(515, 8, "보스문(위아래)", 0);
			break;
		}

		enemy* _em = new bossSkeleton;
		_em->init(WINSIZEX/2,WINSIZEY/2);
		_em->initTileSize(28, 15);
		_em->setWallTile((*map)->_vTile);
		(*map)->_vEnemy.push_back(_em);
	}

	//던전문에 따른 포탈타일 생성
	for (int i = 0, left = 0, right = 0, top = 0, bottom = 0; i < (*map)->_vTile.size(); i++)
	{
		RECT rc;
		if ((*map)->_leftDoor != nullptr)
		{
			if (IntersectRect(&rc, &(*map)->_vTile[i].rc, &(*map)->_leftDoor->getRect()))
			{
				if ((*map)->_vTile[i].rc.right == (*map)->_leftDoor->getRect().right &&
					(*map)->_vTile[i].rc.top != (*map)->_leftDoor->getRect().top &&
					(*map)->_vTile[i].rc.bottom != (*map)->_leftDoor->getRect().bottom)
				{
					if (left == 1 && (*map)->_dgKind == DG_NOMAL) (*map)->_vTile[i].tState = TS_PORTAL;
					if (left == 3 && (*map)->_dgKind == DG_SEMIBOSS) (*map)->_vTile[i].tState = TS_PORTAL;

					left++;
				}
			}
		}

		if ((*map)->_rightDoor != nullptr)
		{
			if (IntersectRect(&rc, &(*map)->_vTile[i].rc, &(*map)->_rightDoor->getRect()))
			{
				if ((*map)->_vTile[i].rc.left == (*map)->_rightDoor->getRect().left &&
					(*map)->_vTile[i].rc.top != (*map)->_rightDoor->getRect().top &&
					(*map)->_vTile[i].rc.bottom != (*map)->_rightDoor->getRect().bottom)
				{
					if (right == 1 && (*map)->_dgKind == DG_NOMAL) (*map)->_vTile[i].tState = TS_PORTAL;
					if (right == 3 && (*map)->_dgKind == DG_SEMIBOSS) (*map)->_vTile[i].tState = TS_PORTAL;
					right++;
				}
			}
		}

		if ((*map)->_topDoor != nullptr)
		{
			if (IntersectRect(&rc, &(*map)->_vTile[i].rc, &(*map)->_topDoor->getRect()))
			{
				if (((*map)->_vTile[i].rc.bottom + 1 == (*map)->_topDoor->getRect().bottom || (*map)->_vTile[i].rc.bottom == (*map)->_topDoor->getRect().bottom) &&
					(*map)->_vTile[i].rc.left != (*map)->_topDoor->getRect().left &&
					(*map)->_vTile[i].rc.right != (*map)->_topDoor->getRect().right)
				{
					if (top == 1 && (*map)->_dgKind == DG_NOMAL) (*map)->_vTile[i].tState = TS_PORTAL;
					if (top == 3 && (*map)->_dgKind == DG_SEMIBOSS) (*map)->_vTile[i].tState = TS_PORTAL;
					top++;
				}
			}
		}

		if ((*map)->_bottomDoor != nullptr)
		{
			if (IntersectRect(&rc, &(*map)->_vTile[i].rc, &(*map)->_bottomDoor->getRect()))
			{
				if ((*map)->_vTile[i].rc.top == (*map)->_bottomDoor->getRect().top &&
					(*map)->_vTile[i].rc.left != (*map)->_bottomDoor->getRect().left &&
					(*map)->_vTile[i].rc.right != (*map)->_bottomDoor->getRect().right)
				{
					if (bottom == 1) (*map)->_vTile[i].tState = TS_PORTAL;
					bottom++;
				}
			}
		}

	}

	(*map)->_checkLoadTile = true;

	if ((*map)->_leftDg != nullptr) this->loadTiles(&(*map)->_leftDg);
	if ((*map)->_rightDg != nullptr) this->loadTiles(&(*map)->_rightDg);
	if ((*map)->_topDg != nullptr) this->loadTiles(&(*map)->_topDg);
	if ((*map)->_bottomDg != nullptr) this->loadTiles(&(*map)->_bottomDg);
}

void DungeonMap::dgDoorRender()
{
	if (_leftDoor != nullptr)
	{
		_leftDoor->render();
	}
	if (_rightDoor != nullptr)
	{
		_rightDoor->render();
	}
	if (_topDoor != nullptr)
	{
		_topDoor->render();
	}
	if (_bottomDoor != nullptr)
	{
		_bottomDoor->render();
	}
}

void DungeonMap::dgDoorUpdate()
{
	if (_leftDoor)	 _leftDoor->update();
	if (_rightDoor)	 _rightDoor->update();
	if (_topDoor)	 _topDoor->update();
	if (_bottomDoor)	 _bottomDoor->update();
}

void DungeonMap::dgDoorOpen()
{
	if (_dgKind == DG_NOMAL)
	{
		if (!SOUNDMANAGER->isPlaySound("DungeonDoor"))
			SOUNDMANAGER->play("DungeonDoor");
	}
	else
	{
		if (!SOUNDMANAGER->isPlaySound("bossDoor"))
			SOUNDMANAGER->play("bossDoor");
	}
	if (_leftDoor)	_leftDoor->openPlay();
	if (_rightDoor)	_rightDoor->openPlay();
	if (_topDoor)	_topDoor->openPlay();
	if (_bottomDoor)	_bottomDoor->openPlay();
}

void DungeonMap::dgDoorClose()
{
	if (!SOUNDMANAGER->isPlaySound("DungeonDoor"))
		SOUNDMANAGER->play("DungeonDoor");
	if (_leftDoor)	_leftDoor->closePlay();
	if (_rightDoor)	_rightDoor->closePlay();
	if (_topDoor)	_topDoor->closePlay();
	if (_bottomDoor)	_bottomDoor->closePlay();
}

void DungeonMap::enemyUpdate()
{
	for (int i = 0; i < _vEnemy.size(); i++)
	{
		_vEnemy[i]->isAttackRange(PLAYER->getRect());
		_vEnemy[i]->update();
		_vEnemy[i]->setEndTile((PLAYER->getRect().right + PLAYER->getRect().left) / 2, (PLAYER->getRect().bottom + PLAYER->getRect().top) / 2);
		// 에너미 마지막까지 돌았을때 플레이어 공격박스 비활성화 ( 공격한번에 체력한번 깍기위해 하는거임 )
		if (i == _vEnemy.size() - 1)
		{
			if (PLAYER->getPlayerState() == PLAYER_ATTACK_SWORD) PLAYER->setPlayerAttackBoxHit(true);
			if (PLAYER->getPlayerState() == PLAYER_ATTACK_SWORD_SECOND) PLAYER->setPlayerAttackTwoBoxHit(true);
		}
	}

	for (int i = 0; i < _vEnemy.size(); i++)
	{
		if (_vEnemy[i]->getEmHp() <= 0	&& _vEnemy[i]->getIsItemDrop() == false)
		{
			//아이템 드랍
			for (int j = 0; j < _vEnemy[i]->getDropItemSize(); j++)
			{
				gameItem* item = new gameItem;

				for (int k = 0; k < _itemManager->getItem().size(); k++)
				{
					if (_itemManager->getItem()[k]->getItemIdx() != _vEnemy[i]->getDropItemIndex()[j]) continue;
					item->init(_itemManager->getItem()[k]);
					item->setItemPos(PointMake(_vEnemy[i]->getX(), _vEnemy[i]->getY()));
					_vItem.push_back(item);
				}
			}
			_vEnemy[i]->setIsItemDrop(true);
		}
		if (_vEnemy[i]->getIsActivate() == false)
		{
			_vEnemy[i]->release();
			SAFE_DELETE(_vEnemy[i]);
			_vEnemy.erase(_vEnemy.begin() + i);
		}
	}
}

void DungeonMap::popEnemy(int arr)
{
	_vEnemy[arr]->release();
	SAFE_DELETE(_vEnemy[arr]);
	_vEnemy.erase(_vEnemy.begin() + arr);
}

void DungeonMap::itemUpdate()
{
	// 아이템 업뎃
	for (int i = 0; i < _vItem.size(); i++)
	{
		_vItem[i]->moveUpdate(PLAYER->getX(), PLAYER->getY());

		//아이템 랙트와 플레이어 랙트가 충돌할 경우 
		//획득한 아이템은 인벤토리 벡터로 이동하고 현재 벡터에서는 해당 아이템 삭제 
		if (_vItem[i]->checkCollision(PLAYER->getRect()) && _vItem[i]->getItem().moveState != ITEM_INIT)
		{
			if (ITEMMENU->getInventory()->addItemToInven(*_vItem[i]))
			{
				_vItem.erase(_vItem.begin() + i);
			}
		}
	}

	// 떨어지다가 아이템 삭제
	for (int i = 0; i < _vItem.size();)
	{
		if (_vItem[i]->getItem().isPop == true)
		{
			_vItem.erase(_vItem.begin() + i);
		}
		else i++;
	}

	int _wid = 0;
	int _hei = 0;

	// 구멍으로 떨어졌는지 확인
	for (int i = 0; i < _vItem.size(); i++)
	{
		if (_vItem[i]->getItem().moveState != ITEM_CHECKSTATE) continue;
		RECT temp;
		for (int j = 0; j < _vTile.size(); j++)
		{
			if (IntersectRect(&temp, &_vItem[i]->getRect(), &_vTile[j].rc))
			{
				if (_vTile[j].tState == TS_HOLE)
				{
					_vItem[i]->setItemMoveState(ITEM_FALL);
				}
			}
		}
		if(_vItem[i]->getItem().moveState != ITEM_FALL)
		_vItem[i]->setItemMoveState(ITEM_IDLE);
	}

}

void DungeonMap::checkColiMoveBen()
{
	for (int i = 0; i < _vTile.size(); i++)
	{
		RECT temp;
		if (IntersectRect(&temp, &PLAYER->getShadowRect(), &_vTile[i].rc))
		{
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

void DungeonMap::checkColiHole()
{
	// 달리는중 밀어내주기
	for (int i = 0; i < _vTile.size(); i++)
	{
		RECT temp;
		if (IntersectRect(&temp,&PLAYER->getShadowRect(),&_vTile[i].rc))
		{
			if (_vTile[i].tState == TS_HOLE)
			{
				if (PLAYER->getPlayerState() == PLAYER_RUN)
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
	// 던전문 열리기 전이면 충돌처리 해놓기
	for (int i = 0; i < _vTile.size(); i++)
	{
		RECT temp;
		if (IntersectRect(&temp, &PLAYER->getShadowRect(), &_vTile[i].rc))
		{
			if (_vTile[i].tState == TS_PORTAL && _doorState == DOOR_CLOSE)
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

	// 떨구기
	int _wid = 0;
	int _hei = 0;

	for (int i = 0; i < _vTile.size(); i++)
	{
		RECT temp;
		if (IntersectRect(&temp, &PLAYER->getShadowRect(), &_vTile[i].rc))
		{
			if (_vTile[i].tState == TS_HOLE)
			{
				_wid += temp.right - temp.left;
				_hei += temp.bottom - temp.top;
			}
		}
	}

	for (int i = 0; i < _vTile.size(); i++)
	{
		RECT temp;
		if (IntersectRect(&temp, &PLAYER->getShadowRect(), &_vTile[i].rc))
		{
			if (_vTile[i].tState == TS_HOLE)
			{
				if (PLAYER->getPlayerState() == PLAYER_IDLE)
				{
					if (_wid >= 34 && _hei >= 34)
					{
						PLAYER->setPlayerState(PLAYER_FALL);
					}
					else
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
	}


}

void DungeonMap::checkCollisionSpa()
{
	if (_backImg != "dungeonSpa") return;
	COLORREF color = GetPixel(IMAGEMANAGER->findImage("dungeonSpaPixel")->getMemDC(),PLAYER->getX(),PLAYER->getY());
	int r = GetRValue(color);
	int g = GetGValue(color);
	int b = GetBValue(color);
	if (!(r == 255 && g == 0 && b == 255))
	{
		cout << "aa" << endl;
		PLAYER->setPlayerState(PLAYER_SWIM);
		if (PLAYERDATA->getInDungeonHp() < PLAYERDATA->getHp())
		{
			PLAYERDATA->setInDungeonHp(PLAYERDATA->getInDungeonHp() + 1);

			if (PLAYERDATA->getInDungeonHp() > PLAYERDATA->getHp())
			{
				PLAYERDATA->setInDungeonHp(PLAYERDATA->getHp());
			}
		}
	}
	else if (r == 255 && g == 0 && b == 255 && PLAYER->getPlayerState() == PLAYER_SWIM)
	{
		PLAYER->setPlayerState(PLAYER_IDLE);
	}
	
}

void DungeonMap::checkColiArrow()
{
	RECT temp;
	float x, y;
	for (int i = 0; i < _vTile.size(); i++)
	{
		if (_vTile[i].tState == TS_MOVEBAN)
		{
			if (IntersectRect(&temp, &_vTile[i].rc, &PLAYER->getArrow()->getRect()))
			{
				PLAYER->setShoot(false);
			}
		}
	}
}

DungeonMap * DungeonMap::findMap(POINT pos)
{
	for (int i = 0; i < _vMaps.size(); i++)
	{
		if (_vMaps[i]->_x == pos.x && _vMaps[i]->_y == pos.y) return _vMaps[i];
	}
	return nullptr;
}

bool DungeonMap::isFindMap(POINT pos)
{
	for (int i = 0; i < _vMaps.size(); i++)
	{
		if (_vMaps[i]->_x == pos.x && _vMaps[i]->_y == pos.y) return true;
	}
	return false;
}

DungeonMap * DungeonMap::moveDungeon(RECT rc)
{
	RECT temp;
	for (int i = 0; i < _vTile.size(); i++)
	{
		if (IntersectRect(&temp, &rc, &_vTile[i].rc))
		{
			if (_leftDoor != nullptr)
			{
				if (_vTile[i].tState == TS_PORTAL && IntersectRect(&temp, &rc, &_leftDoor->getRect()))
				{
					return _leftDg;
				}
			}
			if (_rightDoor != nullptr)
			{
				if (_vTile[i].tState == TS_PORTAL && IntersectRect(&temp, &rc, &_rightDoor->getRect()))
				{
					return _rightDg;
				}
			}
			if (_topDoor != nullptr)
			{
				if (_vTile[i].tState == TS_PORTAL && IntersectRect(&temp, &rc, &_topDoor->getRect()))
				{
					return _topDg;
				}
			}
			if (_bottomDoor != nullptr)
			{
				if (_vTile[i].tState == TS_PORTAL && IntersectRect(&temp, &rc, &_bottomDoor->getRect()))
				{
					return _bottomDg;
				}
			}
		}
	}
	return nullptr;
}

int DungeonMap::moveDungeonDirection(RECT rc)
{
	RECT temp;
	for (int i = 0; i < _vTile.size(); i++)
	{
		if (IntersectRect(&temp, &rc, &_vTile[i].rc))
		{
			if (_leftDoor != nullptr)
			{
				if (_vTile[i].tState == TS_PORTAL && IntersectRect(&temp, &rc, &_leftDoor->getRect()))
				{
					if (_dgKind == DG_SEMIBOSS)
						return 5;
					else 
						return 1;
				}
			}
			if (_rightDoor != nullptr)
			{
				if (_vTile[i].tState == TS_PORTAL && IntersectRect(&temp, &rc, &_rightDoor->getRect()))
				{
					if (_dgKind == DG_SEMIBOSS)
						return 5;
					else
					return 2;
				}
			}
			if (_topDoor != nullptr)
			{
				if (_vTile[i].tState == TS_PORTAL && IntersectRect(&temp, &rc, &_topDoor->getRect()))
				{
					if (_dgKind == DG_SEMIBOSS)
						return 5;
					else
					return 3;
				}
			}
			if (_bottomDoor != nullptr)
			{
				if (_vTile[i].tState == TS_PORTAL && IntersectRect(&temp, &rc, &_bottomDoor->getRect()))
				{
					if (_dgKind == DG_SEMIBOSS)
						return 5;
					else
					return 4;
				}
			}
		}
	}



	return 0;
}

bool DungeonMap::isMoveBen(RECT rc)
{
	RECT temp;
	for (int i = 0; i < _vTile.size(); i++)
	{
		if (IntersectRect(&temp, &rc, &_vTile[i].rc))
		{
			if (_vTile[i].tState == TS_MOVEBAN) return true;
		}
	}
	return false;
}

bool DungeonMap::isTSHole(RECT rc)
{
	RECT temp;
	for (int i = 0; i < _vTile.size(); i++)
	{
		if (IntersectRect(&temp, &rc, &_vTile[i].rc))
		{
			if (_vTile[i].tState == TS_HOLE) return true;
		}
	}
	return false;
}

RECT DungeonMap::getMoveBenTile(RECT rc)
{
	RECT temp;
	for (int i = 0; i < _vTile.size(); i++)
	{
		if (IntersectRect(&temp, &rc, &_vTile[i].rc))
		{
			if (_vTile[i].tState == TS_MOVEBAN) return _vTile[i].rc;
		}
	}
	return RectMake(0, 0, 0, 0);
}

RECT DungeonMap::getHoleTile(RECT rc)
{
	RECT temp;
	for (int i = 0; i < _vTile.size(); i++)
	{
		if (IntersectRect(&temp, &rc, &_vTile[i].rc))
		{
			if (_vTile[i].tState == TS_HOLE) return _vTile[i].rc;
		}
	}
	return RectMake(0, 0, 0, 0);
}
