#include "stdafx.h"
#include "maptoolScene.h"

char maptoolScene::fileName[128] = {0};

HRESULT maptoolScene::init()
{
	
	for (int i = 0; i < SAMPLEOBJECT; i++)
	{
		object_index[i] = 0;
	}
	for (int i = 0; i < SAMPLENPC; i++)
	{
		npc_index[i] = 0;
	}
	_count = 0;
	_index = 0;

	_alpha = 150;
	tile_X = 30;
	tile_Y = 30;

	_menuBarOn = true; //타일맵 메뉴바 생성
	_ctrlSelect = CTRL_SIZE;

	_eraserOn = false; //타일맵 지우개 on ,off
	_menuBar = RectMake(720, 115, 408, 490); // 타이맵 메뉴바 RECT

	for (int i = 0; i < tile_X * tile_Y; i++)
	{
		tagTile _tag;
		_vTiles.push_back(_tag);
		_vObject.push_back(_tag);
	}
	//맵툴세팅
	this->maptoolSetup();

	//현재타일 초기화 (지형 = 잔디)
	_currentTile.x = 0;
	_currentTile.y = 0;
	_currentTile.key = "잔디타일";

	

	//카메라 초기화
	_pivotX = WINSIZEX / 2;
	_pivotY = WINSIZEY / 2;
	CAMERAMANAGER->init(_pivotX, _pivotY, TILESIZE * tile_X + WINSIZEX / 2 , TILESIZE * tile_Y, 0, 0, WINSIZEX/2, WINSIZEY / 2);

	isleft = false;
	isleftdown = false;
	isleftUp = false;

	//드래그 포인트 
	_cmMouse.x = CAMERAMANAGER->getRect().left + _ptMouse.x;
	_cmMouse.y = CAMERAMANAGER->getRect().top + _ptMouse.y;
	_dragSizeX = 0;
	_dragSizeY = 0;
	_isAutoDrag = false;
	_alphaRect = new image;

	//스크롤바 초기화
	_curScrollY = 0.f;
	_maxScrollY = 110.f;
	_barLength = 47.f;
	_menuLength = 222.f;
	_startY = 0.f;

	//미니맵
	_miniMap = new image;
	_backImg = nullptr;

	_page = 0;

	this->miniMapRender();	

	return S_OK;
}

void maptoolScene::release()
{

}

void maptoolScene::update()
{
	if (!SOUNDMANAGER->isPlaySound("맵툴브금")) {
		SOUNDMANAGER->play("맵툴브금", 0.3f);
	}
	checkButton();
	toggleMenu();

	//objectHit = checkObject();

	if (isleft) {
		if (_eraserOn)
		{
			this->eraseMap();
		}
		else {
			if (_ctrlSelect == CTRL_TILESTATE) this->setTileState();
			else this->setMap();
		}
	}

	if (isleftUp) this->miniMapRender();

	if (isleftdown && _menuBarOn) // 메뉴바 버튼들
	{
		switch (_ctrlSelect)
		{
		case CTRL_SIZE:	//사이즈 변경
			clickTileSizeThirty();	
			clickTileSizeFifty();
			break;

		case CTRL_DATA: case CTRL_SAVE: case CTRL_LOAD:
			clickSaveBtn();
			clickLoadBtn();
			break;

		case CTRL_TERRAIN: case CTRL_OBJECT: case CTRL_NPC:
			clickCtrlType();
			toggleEraser();
			clickResetBtn();
			this->tileArrowUp();
			this->tileArrowDown();
			break;

		case CTRL_TILESTATE:
			clickCtrlType();
			toggleEraser();
			clickMoveBanBtn();
			clickPortalBtn();
			break;

		default:
			break;
		}//end of switch 

		ClickTagMenu();
	}


	if (INPUT->GetKeyDown(VK_RBUTTON)) // 드래그 시작
	{
		if (_ctrlSelect == CTRL_TERRAIN || _ctrlSelect == CTRL_TILESTATE)
			this->setDragStartPos(_cmMouse);
	}

	if (INPUT->GetKey(VK_RBUTTON))
	{
		if (_ctrlSelect == CTRL_TERRAIN || _ctrlSelect == CTRL_TILESTATE)
		{
			this->setDragEndPos(_cmMouse);
			this->dragCount();
			this->dragMouseFix();
		}
	}
	if (INPUT->GetKey(VK_LSHIFT) && _ctrlSelect == CTRL_TERRAIN)
	{
		_isAutoDrag = true;
	}
	else _isAutoDrag = false;
	if (INPUT->GetKeyUp(VK_RBUTTON))
	{
		if (_ctrlSelect == CTRL_TERRAIN)
		{
			this->setDragTerrain(_isAutoDrag);
		}
		if (_ctrlSelect == CTRL_TILESTATE)
		{
			this->autoSetTileState();
		}
		ClipCursor(NULL);					// 마우스 고정 해제
		_dragStartPos = { 0,0 };
		_dragEndPos = { 0,0 };
		this->miniMapRender();
	}

	if (INPUT->GetKeyDown('T'))  this->clickTileTown();
	if (INPUT->GetKeyDown('Y')) this->clickTileDungeon();
	if (INPUT->GetKeyDown('U')) this->clickTileBossDungeon();

	CAMERAMANAGER->update(_pivotX,_pivotY);
	moveRect();
	animation();
	objecrRectRender();

	this->cmMouseUpdate();
	this->changeSampletile();		//스크롤에 따른 페이지 변화

}

	void maptoolScene::render()
	{
		//if (_backImg != nullptr) CAMERAMANAGER->Render(getMemDC(), _backImg, 0, 0);
		//CAMERAMANAGER->StretchRender(getMemDC(), IMAGEMANAGER->findImage("bossDungeon"), IMAGEMANAGER->findImage("bossDungeon")->getWidth(), IMAGEMANAGER->findImage("bossDungeon")->getHeight(), 2.f);

		//타일맵 이미지 전체화면 우측상단에 배치하기

		//인게임 화면 지형을 그린다

		for (int i = 0; i < tile_X * tile_Y; i++)
		{
			if (this->checkTileMap(i))	// 클리핑
			{
				if (_vTiles[i].terrain == TR_NONE) continue;

				CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage(_vTiles[i].key), _vTiles[i].rc.left, _vTiles[i].rc.top,
					_vTiles[i].terrainFrameX, _vTiles[i].terrainFrameY);

			}
		}

		//인게임 화면 오브젝트 그린다
		for (int i = 0; i < tile_X * tile_Y; i++)
		{
			//if (this->checkTileMap(i))
			{
				if (_vObject[i].obj == OBJ_NONE || _vObject[i].key == "") continue;
				CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage(_vObject[i].key), _vObject[i].rc.left, _vObject[i].rc.top,
					IMAGEMANAGER->findImage(_vObject[i].key)->getFrameX(), 0);
			}
		}

		if (INPUT->GetKey(VK_RBUTTON))
		{
			if (_isAutoDrag) this->autoDragFrameRectRender();
			else this->dragFrameRectRender(RGB(0, 0, 255));
		}

		//if (INPUT->GetToggleKey('G')) IMAGEMANAGER->findImage("dungeonBack")->render(getMemDC(), 0, 0);

		//타일 속성에따른 색상 출력
		this->tileStateRender();

		//클릭했을때 마우스 따라다니는 이미지 그리기
		if (_currentTile.key != "" && !_eraserOn && (_ctrlSelect == CTRL_OBJECT || _ctrlSelect == CTRL_NPC))
		{
			if (objectHit) alphaRectangle(_currentTile.rc, 100, RGB(0, 255, 0),false);
			else  alphaRectangle(_currentTile.rc, 100, RGB(255, 0, 0),false);
		}

		switch (_ctrlSelect)
		{
		case CTRL_TERRAIN:
			if (!_eraserOn)
			{
				if (IMAGEMANAGER->findImage(_currentTile.key) != nullptr) 
				{
					IMAGEMANAGER->findImage(_currentTile.key)->alphaFrameRender(getMemDC(),
						_ptMouse.x - IMAGEMANAGER->findImage(_currentTile.key)->getFrameWidth() / 2,
						_ptMouse.y - IMAGEMANAGER->findImage(_currentTile.key)->getFrameHeight() / 2,
						_currentTile.x, _currentTile.y, _alpha
					);
				}
			}
			break;
		case CTRL_OBJECT:
				if (_currentTile.key != "")
				{
					IMAGEMANAGER->findImage(_currentTile.key)->alphaFrameRender(getMemDC(),
						_ptMouse.x,
						_ptMouse.y,
						0, 0, _alpha);

				}
			break;
		case CTRL_NPC:
			if (_currentTile.key != "")
			{
				IMAGEMANAGER->findImage(_currentTile.key)->alphaFrameRender(getMemDC(),
					_ptMouse.x - IMAGEMANAGER->findImage(_currentTile.key)->getFrameWidth() / 2,
					_ptMouse.y - IMAGEMANAGER->findImage(_currentTile.key)->getFrameHeight() / 2,
					0, 0, _alpha);
			}
			break;
		case CTRL_ERASER:
			break;
		}


		//왼쪽 게임화면 및 오른쪽 샘플타일 렉트 보여주기
		if (INPUT->GetToggleKey(VK_F1))
		{
			for (int i = 0; i < tile_X * tile_Y; i++)
			{
				if (!checkTileMap(i)) continue;
				//Rectangle(getMemDC)
				//FrameRect(getMemDC(), _vTiles[i].rc, RGB(255, 255, 0));
				CAMERAMANAGER->FrameRect(getMemDC(), _vTiles[i].rc, RGB(255, 255, 0));
				if (_vObject[i].key != "")
				{
					//FrameRect(getMemDC(), _vObject[i].rc, RGB(255, 0, 0));
					CAMERAMANAGER->FrameRect(getMemDC(), _vObject[i].rc, RGB(255, 0, 0));
				}
			}
			//for (int i = 0; i < SAMPLETILEX * SAMPLETILEY; i++)
			//{
			//	//Rectangle(getMemDC)
			//	FrameRect(getMemDC(), _sampleTile[i].rc, RGB(255, 255, 0));
			//}
			//for (int i = 0; i < 4; i++)
			//{
			//	//Rectangle(getMemDC)
			//	FrameRect(getMemDC(), _sampleObject[i].rc, RGB(255, 255, 0));
			//}
			//for (int i = 0; i < 4; i++)
			//{
			//	//Rectangle(getMemDC)
			//	FrameRect(getMemDC(), _sampleNpc[i].rc, RGB(255, 255, 0));
			//}
		}

		if (_menuBarOn)
		{
			menuBarRender();
			this->scrollBarRender();
		}
	
		if (INPUT->GetToggleKey('M'))
		{
			_miniMap->stretchRender(getMemDC(), 0, 0, 0.5f);
		}
		
	}
void maptoolScene::maptoolSetup()
{
	//왼쪽 인게임 화면 렉트 초기화
	for (int i = 0; i < tile_Y; i++)
	{
		for (int j = 0; j < tile_X; j++)
		{
			_vTiles[i * tile_X + j].rc = RectMake(j * TILESIZE, + i * TILESIZE, TILESIZE, TILESIZE);
		}
	}

	//오른쪽 샘플타일 렉트 초기화
	for (int i = 0; i < SAMPLETILEY; i++)
	{
		for (int j = 0; j < SAMPLETILEX; j++)
		{
			_sampleTile[i * SAMPLETILEX + j].rc = RectMake(788 + j * TILESIZE, (335) + i * TILESIZE, TILESIZE, TILESIZE);
			_sampleTile[i * SAMPLETILEX + j].terrainFrameX = j;
			_sampleTile[i * SAMPLETILEX + j].terrainFrameY = i;
			_sampleTile[i * SAMPLETILEX + j].key = "잔디타일";
		}
	}

	for (int i = 0,index = 0; i < SAMPLEOBJECT; i++)
	{
		_sampleObject[i].rc = RectMake(788 + (i % SAMPLEOBJECTX) * 125, (325) + index * 125, 125, 125);
		_sampleObject[i].terrainFrameX = i;
		_sampleObject[i].terrainFrameY = i;
		_sampleObject[i].key = "집샘플";
		if (i % 2 == 1) index++;
		if (index > 1) index = 0;
	}

	for (int i = 0,index = 0; i < SAMPLENPC; i++)
	{
		_sampleNpc[i].rc = RectMake(788 + (i % 2) * 125, (325) + index * 125, 125, 125);
		_sampleNpc[i].terrainFrameX = i;
		_sampleNpc[i].terrainFrameY = i;
		_sampleNpc[i].key = "NPC샘플";
		if (i % 2 == 1) index++;
		if (index > 1) index = 0;
	}

	//왼쪽 인게임 화면 모두 잔디가 기본 타일이 되도록 세팅하기
	for (int i = 0; i < tile_X * tile_Y; i++)
	{
		_vTiles[i].terrainFrameX = 1;
		_vTiles[i].terrainFrameY = 1;
		_vTiles[i].objFrameX = 0;
		_vTiles[i].objFrameY = 0;
		_vTiles[i].terrain = terrainSelect(_vTiles[i].terrainFrameX, _vTiles[i].terrainFrameY,"잔디타일");
		_vTiles[i].obj = OBJ_NONE;
		_vTiles[i].key = "잔디타일";
		_vTiles[i].tState = TS_EMPTY;

		_vObject[i].obj = OBJ_NONE;
		_vObject[i].key = "";
		_vObject[i].tState = TS_EMPTY;
	}

	//컨트롤 렉트 위치 초기화
	setCtrlBtns();
}

void maptoolScene::maptoolSetup(int x, int y)
{
	//왼쪽 인게임 화면 렉트 초기화
	for (int i = 0; i < tile_Y; i++)
	{
		for (int j = 0; j < tile_X; j++)
		{
			_vTiles[i * tile_X + j].rc = RectMake(j * TILESIZE + x, +i * TILESIZE + y, TILESIZE, TILESIZE);
		}
	}

	//오른쪽 샘플타일 렉트 초기화
	for (int i = 0; i < SAMPLETILEY; i++)
	{
		for (int j = 0; j < SAMPLETILEX; j++)
		{
			_sampleTile[i * SAMPLETILEX + j].rc = RectMake(788 + j * TILESIZE, (335) + i * TILESIZE, TILESIZE, TILESIZE);
			_sampleTile[i * SAMPLETILEX + j].terrainFrameX = j;
			_sampleTile[i * SAMPLETILEX + j].terrainFrameY = i;
			_sampleTile[i * SAMPLETILEX + j].key = "잔디타일";
		}
	}

	for (int i = 0, index = 0; i < SAMPLEOBJECT; i++)
	{
		_sampleObject[i].rc = RectMake(788 + (i % SAMPLEOBJECTX) * 125, (325) + index * 125, 125, 125);
		_sampleObject[i].terrainFrameX = i;
		_sampleObject[i].terrainFrameY = i;
		_sampleObject[i].key = "집샘플";
		if (i % 2 == 1) index++;
		if (index > 1) index = 0;
	}

	for (int i = 0, index = 0; i < SAMPLENPC; i++)
	{
		_sampleNpc[i].rc = RectMake(788 + (i % 2) * 125, (325) + index * 125, 125, 125);
		_sampleNpc[i].terrainFrameX = i;
		_sampleNpc[i].terrainFrameY = i;
		_sampleNpc[i].key = "NPC샘플";
		if (i % 2 == 1) index++;
		if (index > 1) index = 0;
	}

	//왼쪽 인게임 화면 모두 잔디가 기본 타일이 되도록 세팅하기
	for (int i = 0; i < tile_X * tile_Y; i++)
	{
		_vTiles[i].terrainFrameX = 1;
		_vTiles[i].terrainFrameY = 1;
		_vTiles[i].objFrameX = 0;
		_vTiles[i].objFrameY = 0;
		_vTiles[i].terrain = TR_NONE;
		_vTiles[i].obj = OBJ_NONE;
		_vTiles[i].key = "잔디타일";
		_vTiles[i].tState = TS_EMPTY;

		_vObject[i].obj = OBJ_NONE;
		_vObject[i].key = "";
		_vObject[i].tState = TS_EMPTY;
	}

	//컨트롤 렉트 위치 초기화
	setCtrlBtns();
}

void maptoolScene::setMap()
{
		for (int i = 0; i < SAMPLETILEX * SAMPLETILEY; i++)
		{
			if (PtInRect(&_sampleTile[i].rc, _ptMouse) && _menuBarOn && _ctrlSelect == CTRL_TERRAIN)
			{
				_currentTile.x = _sampleTile[i].terrainFrameX;
				_currentTile.y = _sampleTile[i].terrainFrameY;
				_currentTile.key = _sampleTile[i].key;
				break;
			}
		}
		for (int i = 4 * _page,max = i + 4; i <max; i++)
		{
			if (PtInRect(&_sampleObject[i].rc, _ptMouse) && _menuBarOn && (_ctrlSelect == CTRL_OBJECT && _ctrlSelect != CTRL_NPC))
			{
				_currentTile.x = _sampleObject[i].terrainFrameX;
				_currentTile.y = _sampleObject[i].terrainFrameY;
				_currentTile.key = objectStr[i];

				break;
			}
		}
		for (int i = 4 * _page, max = i + 4; i < max; i++)
		{
			if (PtInRect(&_sampleNpc[i].rc, _ptMouse) && _menuBarOn && _ctrlSelect == CTRL_NPC)
			{
				_currentTile.x = _sampleNpc[i].terrainFrameX;
				_currentTile.y = _sampleNpc[i].terrainFrameY;
				_currentTile.key = npcStr[i];
			
				break;
			}
		}
	
	//인게임화면 렉트와 충돌했냐?
	RECT menubar;
	menubar.left = _menuBar.left + CAMERAMANAGER->getRect().left;
	menubar.right = _menuBar.right + CAMERAMANAGER->getRect().left;
	menubar.top = _menuBar.top + CAMERAMANAGER->getRect().top;
	menubar.bottom = _menuBar.bottom + CAMERAMANAGER->getRect().top;

	for (int i = 0; i < tile_X * tile_Y; i++)
	{
		if (PtInRect(&_vTiles[i].rc, _cmMouse))
		{
			if ((!PtInRect(&menubar, _cmMouse) || !_menuBarOn)) {
				if (!checkTileMap(i)) continue;
				//현재버튼이 지형이냐?
				if (_ctrlSelect == CTRL_TERRAIN)
				{
					_vTiles[i].terrainFrameX = _currentTile.x;
					_vTiles[i].terrainFrameY = _currentTile.y;
					_vTiles[i].terrain = terrainSelect(_currentTile.x, _currentTile.y,_currentTile.key);
					_vTiles[i].key = _currentTile.key;
				}
				//현재버튼이 오브젝트냐?
				if (_ctrlSelect == CTRL_OBJECT && _ctrlSelect != CTRL_NPC && _currentTile.key != "" && objectHit)
				{
					_vObject[i].objFrameX = _currentTile.x;
					_vObject[i].objFrameY = _currentTile.y;
					_vObject[i].obj = objectSelect(_currentTile.x, _currentTile.y);
					_vObject[i].key = _currentTile.key;
					_currentTile.key.clear();
					_ctrlSelect = CTRL_OBJECT;
					//break;
				}
				if (_ctrlSelect == CTRL_NPC && _currentTile.key != "" && objectHit)
				{
					_vObject[i].objFrameX = _currentTile.x;
					_vObject[i].objFrameY = _currentTile.y;
					_vObject[i].obj = NPCSelect(_currentTile.x, _currentTile.y);
					_vObject[i].key = _currentTile.key;
					_currentTile.key.clear();
					_ctrlSelect = CTRL_NPC;
					//break;
				}
				//현재버튼이 지우개냐?
				if (_ctrlSelect == CTRL_ERASER)
				{
					_vTiles[i].objFrameX = 0;
					_vTiles[i].objFrameY = 0;
					_vTiles[i].obj = OBJ_NONE;

					_vObject[i].objFrameX = 0;
					_vObject[i].objFrameY = 0;
					_vObject[i].obj = OBJ_NONE;
				}
			}
		}
	}

}

void maptoolScene::save()
{
	HANDLE file;
	DWORD write;

	file = CreateFile("tile.map", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	tagTile* _tile = new tagTile[tile_X*tile_Y];
	for (int i = 0; i < _vTiles.size(); i++)
	{
			_tile[i] = _vTiles[i];
	}
	WriteFile(file, _tile, sizeof(tagTile) *  tile_X * tile_Y, &write, NULL);
	CloseHandle(file);
	SAFE_DELETE_ARRAY(_tile);

	file = CreateFile("object.map", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	tagTile* _obj = new tagTile[tile_X*tile_Y];
	for (int i = 0; i < _vTiles.size(); i++)
	{
		_obj[i] = _vObject[i];
	}
	WriteFile(file, _obj, sizeof(tagTile) *  tile_X * tile_Y, &write, NULL);
	CloseHandle(file);
	SAFE_DELETE_ARRAY(_obj);
}

void maptoolScene::save(string fileName)
{
	HANDLE file;
	DWORD write;

	string temp = fileName + "tile.map";

	file = CreateFile(temp.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	tagTile* _tile = new tagTile[tile_X*tile_Y];
	for (int i = 0; i < _vTiles.size(); i++)
	{
		_tile[i] = _vTiles[i];
	}
	WriteFile(file, _tile, sizeof(tagTile) *  tile_X * tile_Y, &write, NULL);
	CloseHandle(file);
	SAFE_DELETE_ARRAY(_tile);

	string temp2 = fileName + "object.map";

	file = CreateFile(temp2.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	tagTile* _obj = new tagTile[tile_X*tile_Y];
	for (int i = 0; i < _vTiles.size(); i++)
	{
		_obj[i] = _vObject[i];
	}
	WriteFile(file, _obj, sizeof(tagTile) *  tile_X * tile_Y, &write, NULL);
	CloseHandle(file);
	SAFE_DELETE_ARRAY(_obj);
}

void maptoolScene::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("tile.map", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	tagTile* _tile = new tagTile[tile_X * tile_Y];
	ReadFile(file, _tile, sizeof(tagTile) *  tile_X * tile_Y, &read, NULL);
	for (int i = 0; i < tile_X * tile_Y; i++)
	{
			_vTiles[i] = _tile[i];
	}
	CloseHandle(file);

	file = CreateFile("object.map", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	tagTile* _obj = new tagTile[tile_X * tile_Y];
	ReadFile(file, _obj, sizeof(tagTile) *  tile_X * tile_Y, &read, NULL);
	for (int i = 0; i < tile_X * tile_Y; i++)
	{
		_vObject[i] = _obj[i];
	}
	CloseHandle(file);
}

void maptoolScene::load(string fileName)
{
	HANDLE file;
	DWORD read;
	string temp = fileName + "tile.map";
	file = CreateFile(temp.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	tagTile* _tile = new tagTile[tile_X * tile_Y];
	ReadFile(file, _tile, sizeof(tagTile) *  tile_X * tile_Y, &read, NULL);
	for (int i = 0; i < tile_X * tile_Y; i++)
	{
		_vTiles[i] = _tile[i];
	}
	CloseHandle(file);
	string temp2 = fileName + "object.map";
	file = CreateFile(temp2.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	tagTile* _obj = new tagTile[tile_X * tile_Y];
	ReadFile(file, _obj, sizeof(tagTile) *  tile_X * tile_Y, &read, NULL);
	for (int i = 0; i < tile_X * tile_Y; i++)
	{
		_vObject[i] = _obj[i];
	}
	CloseHandle(file);
}

bool maptoolScene::checkTileMap(int arr)
{
	RECT rc;
	RECT temp;

	GetClientRect(_hWnd, &rc);

	rc.right = WINSIZEX;

	RECT tile = RectMake(_vTiles[arr].rc.left - CAMERAMANAGER->getRect().left, _vTiles[arr].rc.top - CAMERAMANAGER->getRect().top, TILESIZE, TILESIZE);

	if (IntersectRect(&temp, &rc, &tile))
	{
		return true;
	}

	return false;
}


void maptoolScene::moveRect()
{
	if ((INPUT->GetKey('W')) && (_pivotY > WINSIZEY / 2))
	{
		_pivotY -= 5;
	}
	if (INPUT->GetKey('A') && _pivotX > WINSIZEX / 2)
	{
		_pivotX -= 5;
	}
	if (INPUT->GetKey('S') && _pivotY < (WINSIZEY * 0.5) + ((tile_Y * TILESIZE) - WINSIZEY))
	{
		_pivotY += 5;
	}
	if (INPUT->GetKey('D') && _pivotX < (WINSIZEX * 0.5) + ((tile_X * TILESIZE) - WINSIZEX))
	{
		_pivotX += 5;
	}
	CAMERAMANAGER->movePivot(_pivotX, _pivotY);
}

void maptoolScene::checkButton()
{
	if (INPUT->GetKeyDown(VK_LBUTTON))
	{
		isleftdown = true;
	}
	else {
		isleftdown = false;
	}

	if (INPUT->GetKey(VK_LBUTTON))
	{
		isleft = true;
	}
	else {
		isleft = false;
	}

	if (INPUT->GetKeyUp(VK_LBUTTON))
	{
		isleftUp = true;
	}
	else {
		isleftUp = false;
	}
} //건들지마시오

void maptoolScene::animation()
{
	_count++;

	for (int i = 0; i < SAMPLENPC; i++)
	{
		if (_count % 7 == 0)
		{
			npc_index[i]++;

			if (npc_index[i] >= IMAGEMANAGER->findImage(npcStr[i])->getMaxFrameX())
			{
				npc_index[i] = 0;
			}
			IMAGEMANAGER->findImage(npcStr[i])->setFrameX(npc_index[i]);
		}

	}

	for (int i = 0; i < SAMPLEOBJECT; i++)
	{
		if (objectStr[i] == "") continue;
		if (_count % 7 == 0)
		{
			object_index[i]++;

			if (object_index[i] >= IMAGEMANAGER->findImage(objectStr[i])->getMaxFrameX())
			{
				object_index[i] = 0;
			}
			IMAGEMANAGER->findImage(objectStr[i])->setFrameX(object_index[i]);
		}

	}
	
}
//오브젝트 사각형그리기
void maptoolScene::objecrRectRender()
{
	for (int i = 0; i < tile_X * tile_Y; i++)
	{
		if (_vObject[i].key == "") continue;
		_vObject[i].rc = RectMake(_vTiles[i].rc.left, _vTiles[i].rc.top,
			IMAGEMANAGER->findImage(_vObject[i].key)->getFrameWidth(),
			IMAGEMANAGER->findImage(_vObject[i].key)->getFrameHeight());
	}
	//현재 짚고있는 오브젝트 렉트 그리기

	if (_currentTile.key != "")
	{
		if (_ctrlSelect == CTRL_OBJECT)
		{
			_currentTile.rc = RectMake(_ptMouse.x, _ptMouse.y,
				IMAGEMANAGER->findImage(_currentTile.key)->getFrameWidth(),
				IMAGEMANAGER->findImage(_currentTile.key)->getFrameHeight());
		}
		else if (_ctrlSelect == CTRL_NPC)
		{
			_currentTile.rc = RectMakeCenter(_ptMouse.x, _ptMouse.y,
				IMAGEMANAGER->findImage(_currentTile.key)->getFrameWidth(),
				IMAGEMANAGER->findImage(_currentTile.key)->getFrameHeight());
		}
	}
}

//오브젝트 충돌 검사하기
bool maptoolScene::checkObject()
{
	RECT tmp;
	RECT curRC = RectMake(_cmMouse.x, _cmMouse.y, _currentTile.rc.right - _currentTile.rc.left, _currentTile.rc.bottom - _currentTile.rc.top);
	for (int i = 0; i < tile_X * tile_Y; i++)
	{
		if (IntersectRect(&tmp, &_vObject[i].rc, &curRC))
		{
			if (_vObject[i].obj != OBJ_NONE && _vObject[i].key != "") return false;
		}
	}

	return true;
}

void maptoolScene::eraseMap()
{
	switch (_ctrlSelect)
	{
	case CTRL_TERRAIN:
		eraseTerrain();
		break;
	case CTRL_OBJECT:
		eraseObject();
		break;
	case CTRL_NPC:
		eraseNPC();
		break;
	case CTRL_TILESTATE:
		eraseState();
		break;
	}//end of switch
}

void maptoolScene::eraseTerrain()
{
	//인게임화면 렉트와 충돌했냐?
	POINT mouse;

	mouse.x = _ptMouse.x + CAMERAMANAGER->getRect().left;
	mouse.y = _ptMouse.y + CAMERAMANAGER->getRect().top;

	for (int i = 0; i < tile_X * tile_Y; i++)
	{
		if (PtInRect(&_vTiles[i].rc, mouse))
		{
			if ((!PtInRect(&_menuBar, mouse) || !_menuBarOn))
			{
				if (!checkTileMap(i)) continue;

				_vTiles[i].terrain = TR_NONE;
			
			}
		}
	}//end of for 
}

void maptoolScene::eraseObject()
{
	//인게임화면 렉트와 충돌했냐?
	POINT mouse;

	mouse.x = _ptMouse.x + CAMERAMANAGER->getRect().left;
	mouse.y = _ptMouse.y + CAMERAMANAGER->getRect().top;

	for (int i = 0; i < tile_X * tile_Y; i++)
	{
		if (PtInRect(&_vObject[i].rc, mouse))
		{
			if ((!PtInRect(&_menuBar, mouse) || !_menuBarOn))
			{
				if (!checkTileMap(i)) continue;

				if (_vObject[i].obj == OBJ_OBJECT) {
					_vObject[i].obj = OBJ_NONE;
					_vObject[i].key = "";
				}
			}
		}
	}//end of for 
}

void maptoolScene::eraseNPC()
{
	//인게임화면 렉트와 충돌했냐?

	for (int i = 0; i < tile_X * tile_Y; i++)
	{
		if (PtInRect(&_vObject[i].rc, _cmMouse))
		{
			if ((!PtInRect(&_menuBar, _cmMouse) || !_menuBarOn))
			{
				if (!checkTileMap(i)) continue;

				if (_vObject[i].obj == OBJ_NPC)
				{
					_vObject[i].obj = OBJ_NONE;
					_vObject[i].key = "";
				}
			}
		}
	}//end of for 
}

void maptoolScene::eraseState()
{
	
	for (int i = 0; i < tile_X * tile_Y; i++)
	{
		if (PtInRect(&_vTiles[i].rc, _cmMouse))
		{
			if ((!PtInRect(&_menuBar, _cmMouse) || !_menuBarOn))
			{
				if (!checkTileMap(i)) continue;

				_vTiles[i].tState = TS_EMPTY;

			}
		}
	}
}
void maptoolScene::toggleMenu()
{
	if (INPUT->GetKeyDown('F')) // F 키로 메뉴바 열고 닫기 가능
	{
		if (_menuBarOn)
		{
			_menuBarOn = false;
		}
		else {
			_menuBarOn = true;
			//_ctrlSelect = CTRL_SIZE;
		}
	}
}

void maptoolScene::toggleEraser()
{
	if (PtInRect(&_rcEraser, _ptMouse))
	{
		if (_eraserOn) _eraserOn = false;
		else _eraserOn = true;
	}
}

void maptoolScene::setCtrlBtns()
{
	//태그 랙트 위치 초기화 
	_tagSize = RectMake(720, 291, 44, 54);
	_tagTile = RectMake(720, 349, 44, 54);
	_tagData = RectMake(720, 407, 44, 54);

	//컨트롤 랙트 위치 초기화
	_button30 = RectMake(790, 335, 120, 48);
	_button50 = RectMake(790 + 140, 335, 120, 48);

	_rcSave = RectMake(790, 335, 120, 48);
	_rcLoad = RectMake(790 + 140, 335, 120, 48);

	_rcTerrain = RectMake(784, 139, 80, 32);
	_rcObject = RectMake(784 + 96, 139, 80, 32);
	_rcNpc = RectMake(784 + 192, 139, 80, 32);
	_rcState = RectMake(832, 187, 80, 32);
	_rcReset = RectMake(928, 187, 80, 32);

	_rcMoveBan = RectMake(790, 335, 120, 48);
	_rcPortal = RectMake(790 + 140, 335, 120, 48);

	_rcEraser = RectMake(1076, 151, 44, 54);
	_rcArrowUp = RectMake(1092, 335, 22, 14);
	_rcArrowDown = RectMake(1092, 467, 22, 14);
}

void maptoolScene::clickSaveBtn()
{
	if (PtInRect(&_rcSave, _ptMouse))
	{
		_ctrlSelect = CTRL_SAVE;
		if (DialogBox(_hInstance, MAKEINTRESOURCE(IDD_SAVE), _hWnd, DlgProc))
		{
			string temp = fileName;
			memset(fileName, 0, sizeof(fileName));
			this->save(temp);
		}
		_ctrlSelect = CTRL_SIZE;

		_menuBarOn = false;
	}
}

void maptoolScene::clickLoadBtn()
{
	if (PtInRect(&_rcLoad, _ptMouse))
	{
		_ctrlSelect = CTRL_LOAD;
		if (DialogBox(_hInstance, MAKEINTRESOURCE(IDD_LOAD), _hWnd, DlgProc))
		{
			string temp = fileName;
			memset(fileName, 0, sizeof(fileName));
			this->load(temp);
		}
		_ctrlSelect = CTRL_SIZE;
		
		_menuBarOn = false;
	}
}

void maptoolScene::clickCtrlType()
{
	if (PtInRect(&_rcTerrain, _ptMouse))
	{
		_ctrlSelect = CTRL_TERRAIN;
		_page = 0;
	}
	if (PtInRect(&_rcObject, _ptMouse))
	{
		_ctrlSelect = CTRL_OBJECT;
		_page = 0;
	}
	if (PtInRect(&_rcNpc, _ptMouse))
	{
		_ctrlSelect = CTRL_NPC;
		_page = 0;
	}
	if (PtInRect(&_rcState, _ptMouse))
	{
		_ctrlSelect = CTRL_TILESTATE;
	}
}

void maptoolScene::clickTileSizeThirty()
{
	if (PtInRect(&_button30, _ptMouse))
	{
		_backImg = nullptr;
		tile_X = 30;
		tile_Y = 30;
		_vTiles.clear();
		_vObject.clear();
		for (int i = 0; i < tile_X * tile_Y; i++)
		{
			tagTile _tag;
			_vTiles.push_back(_tag);
			_vObject.push_back(_tag);
		}
		this->maptoolSetup();

		_pivotX = WINSIZEX / 2;
		_pivotY = WINSIZEY / 2;
		CAMERAMANAGER->init(_pivotX, _pivotY, TILESIZE * tile_X + WINSIZEX / 2,
			TILESIZE * tile_Y, 0, 0, WINSIZEX / 2, WINSIZEY / 2);
	}
}

void maptoolScene::clickTileSizeFifty()
{
	if (PtInRect(&_button50, _ptMouse))
	{
		_backImg = nullptr;
		tile_X = 50;
		tile_Y = 50;
		_vTiles.clear();
		_vObject.clear();
		for (int i = 0; i < tile_X * tile_Y; i++)
		{
			tagTile _tag;
			_vTiles.push_back(_tag);
			_vObject.push_back(_tag);
		}
		this->maptoolSetup();
		_pivotX = WINSIZEX / 2;
		_pivotY = WINSIZEY / 2;
		CAMERAMANAGER->init(_pivotX, _pivotY, TILESIZE * tile_X, TILESIZE * tile_Y, 0, 0, WINSIZEX / 2, WINSIZEY / 2);
	}
}

void maptoolScene::clickTileTown()
{
	_backImg = IMAGEMANAGER->findImage("townBack");
	tile_X = 74;
	tile_Y = 60;
	_vTiles.clear();
	_vObject.clear();
	for (int i = 0; i < tile_X * tile_Y; i++)
	{
		tagTile _tag;
		_vTiles.push_back(_tag);
		_vObject.push_back(_tag);
	}
	this->maptoolSetup(0,0);

	_pivotX = WINSIZEX / 2;
	_pivotY = WINSIZEY / 2;
	CAMERAMANAGER->init(_pivotX, _pivotY, TILESIZE * tile_X + WINSIZEX / 2,
		TILESIZE * tile_Y, 0, 0, WINSIZEX / 2, WINSIZEY / 2);
	
}

void maptoolScene::clickTileDungeon()
{
	_backImg = IMAGEMANAGER->findImage("dungeonBack");
	tile_X = 37;
	tile_Y = 21;
	_vTiles.clear();
	_vObject.clear();
	for (int i = 0; i < tile_X * tile_Y; i++)
	{
		tagTile _tag;
		_vTiles.push_back(_tag);
		_vObject.push_back(_tag);
	}
	this->maptoolSetup(0,0);

	_pivotX = WINSIZEX / 2;
	_pivotY = WINSIZEY / 2;
	CAMERAMANAGER->init(_pivotX, _pivotY, TILESIZE * tile_X + WINSIZEX / 2,
		TILESIZE * tile_Y, 0, 0, WINSIZEX / 2, WINSIZEY / 2);
	
}

void maptoolScene::clickTileGotoDungeon()
{
	_backImg = IMAGEMANAGER->findImage("gotoDungeon");
	tile_X = 68;
	tile_Y = 57;
	_vTiles.clear();
	_vObject.clear();
	for (int i = 0; i < tile_X * tile_Y; i++)
	{
		tagTile _tag;
		_vTiles.push_back(_tag);
		_vObject.push_back(_tag);
	}
	this->maptoolSetup(0, 0);

	_pivotX = WINSIZEX / 2;
	_pivotY = WINSIZEY / 2;
	CAMERAMANAGER->init(_pivotX, _pivotY, TILESIZE * tile_X + WINSIZEX / 2,
		TILESIZE * tile_Y, 0, 0, WINSIZEX / 2, WINSIZEY / 2);
}

void maptoolScene::clickTileBossDungeon()
{
	_backImg = IMAGEMANAGER->findImage("bossDungeon");
	tile_X = 30*2;
	tile_Y = 24*2;
	_vTiles.clear();
	_vObject.clear();
	for (int i = 0; i < tile_X * tile_Y; i++)
	{
		tagTile _tag;
		_vTiles.push_back(_tag);
		_vObject.push_back(_tag);
	}
	this->maptoolSetup(0, 0);

	_pivotX = WINSIZEX / 2;
	_pivotY = WINSIZEY / 2;
	CAMERAMANAGER->init(_pivotX, _pivotY, TILESIZE * tile_X + WINSIZEX / 2,
		TILESIZE * tile_Y, 0, 0, WINSIZEX / 2, WINSIZEY / 2);
}

void maptoolScene::clickResetBtn()
{
	// 리셋버튼 클릭 시 모든 타일의 지형 및 오브젝트 타입을
	// NONE으로 설정하여 출력하지 않도록 함 
	if (PtInRect(&_rcReset, _ptMouse))
	{
		for (int i = 0; i < tile_X * tile_Y; i++)
		{
			_vTiles[i].terrain = TR_NONE;
			_vTiles[i].obj = OBJ_NONE;
			_vTiles[i].tState = TS_EMPTY;

			_vObject[i].terrain = TR_NONE;
			_vObject[i].obj = OBJ_NONE;
			_vObject[i].tState = TS_EMPTY;
		}
	}
}

void maptoolScene::clickMoveBanBtn()
{
	if (PtInRect(&_rcMoveBan, _ptMouse))
	{
		_ctrlSelect = CTRL_TILESTATE;
		_currentTile.tState = TS_MOVEBAN;
		_eraserOn = false;
	}
}

void maptoolScene::clickPortalBtn()
{
	if (PtInRect(&_rcPortal, _ptMouse))
	{
		_ctrlSelect = CTRL_TILESTATE;
		_currentTile.tState = TS_PORTAL;
		_eraserOn = false;
	}
}

void maptoolScene::eraserRender()
{
	if (_eraserOn)
	{
		IMAGEMANAGER->findImage("eraser_activate")->render(getMemDC(),
			WINSIZEX - 80, 400);
	}
	else
	{
		IMAGEMANAGER->findImage("eraser_inactivate")->render(getMemDC(),
			WINSIZEX - 80, 400);
	}
}

void maptoolScene::ClickTagMenu()
{
	if (PtInRect(&_tagSize, _ptMouse))
	{
		_ctrlSelect = CTRL_SIZE;
	}
	if (PtInRect(&_tagTile, _ptMouse))
	{
		_ctrlSelect = CTRL_TERRAIN;
	}
	if (PtInRect(&_tagData, _ptMouse))
	{
		_ctrlSelect = CTRL_DATA;
	}
}

void maptoolScene::menuBarRender()
{
	//컨트롤러 상태에 따라 맵툴메뉴 랜더 
	switch (_ctrlSelect)
	{
	case CTRL_SIZE:
		IMAGEMANAGER->render("menu_size", getMemDC(), 720, 115);
		break;

	case CTRL_TERRAIN: case CTRL_OBJECT: case CTRL_NPC:
		IMAGEMANAGER->render("menu_tile", getMemDC(), 720, 115);

		//지형, 오브젝트, NPC 프레임이미지 출력 
		if (_ctrlSelect == CTRL_TERRAIN)
		{
			if (_page == 0) IMAGEMANAGER->render("던전타일", getMemDC(), 788, 335);
			if (_page == 1) IMAGEMANAGER->render("잔디타일", getMemDC(), 788, 335);
			if (_page == 2) IMAGEMANAGER->render("쩌리타일", getMemDC(), 788, 335);
		}
		if (_ctrlSelect == CTRL_OBJECT)
		{
			if (_page == 0) IMAGEMANAGER->render("집샘플", getMemDC(), 788, 325);
			if (_page == 1) IMAGEMANAGER->render("건물샘플", getMemDC(), 788, 325);
			if (_page == 2) IMAGEMANAGER->render("나무샘플", getMemDC(), 788, 325);
			if (_page == 3) IMAGEMANAGER->render("던전샘플1", getMemDC(), 788, 325);
			if (_page == 4) IMAGEMANAGER->render("던전샘플2", getMemDC(), 788, 325);
			if (_page == 5) IMAGEMANAGER->render("던전샘플3", getMemDC(), 788, 325);
			if (_page == 6) IMAGEMANAGER->render("건물샘플2", getMemDC(), 788, 325);
			if (_page == 7) IMAGEMANAGER->render("건물샘플3", getMemDC(), 788, 325);
		}
		if (_ctrlSelect == CTRL_NPC)
		{
			if (_page == 0) IMAGEMANAGER->render("NPC샘플", getMemDC(), 788, 325);
			if (_page == 1) IMAGEMANAGER->render("NPC샘플2", getMemDC(), 788, 325);
			if (_page == 2) IMAGEMANAGER->render("몬스터샘플", getMemDC(), 788, 325);
			if (_page == 3) IMAGEMANAGER->render("몬스터샘플2", getMemDC(), 788, 325);
		}

		//지우개 모드 on/off에 따라 지우개 태그 toggle
		if (_eraserOn)
		{
			IMAGEMANAGER->frameRender("menu_eraser", getMemDC(), 1076, 151, 1, 0);
		}
		else
		{
			IMAGEMANAGER->frameRender("menu_eraser", getMemDC(), 1076, 151, 0, 0);
		}
		break;

	case CTRL_TILESTATE:
		IMAGEMANAGER->render("menu_tile_state", getMemDC(), 720, 115);
		if (_eraserOn)
		{
			IMAGEMANAGER->frameRender("menu_eraser", getMemDC(), 1076, 151, 1, 0);
		}
		else
		{
			IMAGEMANAGER->frameRender("menu_eraser", getMemDC(), 1076, 151, 0, 0);
		}

		break;

	case CTRL_DATA: case CTRL_SAVE: case CTRL_LOAD:
		IMAGEMANAGER->render("menu_data", getMemDC(), 720, 115);
		break;
	}

}

void maptoolScene::scrollBarRender()
{
	switch (_ctrlSelect)
	{
	case CTRL_TERRAIN:
	{
		if (_page > 0)
		{
			IMAGEMANAGER->findImage("arrow_up")->render(getMemDC(), 1094, 337);
		}
		if (_page < 2)
		{
			IMAGEMANAGER->findImage("arrow_down")->render(getMemDC(), 1094, 469);
		}
		float _curScrollY = (_maxScrollY / 3) * _page;

		IMAGEMANAGER->findImage("scrollBar")->render(getMemDC(), 1098, 353 + (int)_curScrollY);
		break;
	}
	case CTRL_OBJECT:
	{
		if (_page > 0)
		{
			IMAGEMANAGER->findImage("arrow_up")->render(getMemDC(), 1094, 337);
		}
		if (_page < 7)
		{
			IMAGEMANAGER->findImage("arrow_down")->render(getMemDC(), 1094, 469);
		}
		float _curScrollY = ((_maxScrollY-30) / 6) * _page;

		IMAGEMANAGER->findImage("scrollBar")->render(getMemDC(), 1098, 353 + (int)_curScrollY);
		break;
	}
	case CTRL_NPC:
	{
		if (_page > 0)
		{
			IMAGEMANAGER->findImage("arrow_up")->render(getMemDC(), 1094, 337);
		}
		if (_page < 3)
		{
			IMAGEMANAGER->findImage("arrow_down")->render(getMemDC(), 1094, 469);
		}
		float _curScrollY = (_maxScrollY / 4) * _page;

		IMAGEMANAGER->findImage("scrollBar")->render(getMemDC(), 1098, 353 + (int)_curScrollY);
		break;
	}
	}//end of switch 
}

void maptoolScene::tileArrowUp()
{
	if (PtInRect(&_rcArrowUp, _ptMouse))
	{
		if (_page > 0)
		{
			_page--;
		}
		
	}
}

void maptoolScene::tileArrowDown()
{
	if (PtInRect(&_rcArrowDown, _ptMouse))
	{
		switch (_ctrlSelect)
		{
		case CTRL_NPC:
			if (_page < 3) _page++;
			break;
		case CTRL_TERRAIN:
			if (_page < 2) _page++;
			break;
		case CTRL_OBJECT:
			if (_page < 7) _page++;
			break;
		}
	}
	
}

TERRAIN maptoolScene::terrainSelect(int frameX, int frameY, string key)
{
	//던전 벽
	if (key == "던전타일") return TR_DUNGEONWALL;
	if (key == "잔디타일" && frameX < 5 && frameY < 3) return TR_GRASS;
	if (key == "쩌리타일") return TR_HOLE;

	return TR_OTHERS;
}

OBJECT maptoolScene::objectSelect(int frameX, int frameY)
{
	return OBJ_OBJECT;
}

OBJECT maptoolScene::NPCSelect(int frameX, int frameY)
{
	return OBJ_NPC;
}

void maptoolScene::setDragStartPos(POINT ptMouse)
{
	_dragStartPos = ptMouse;
}

void maptoolScene::setDragEndPos(POINT ptMouse)
{
	_dragEndPos = ptMouse;
}

void maptoolScene::setDragTerrain(bool isAuto)
{
	RECT dragRC = { _dragStartPos.x,_dragStartPos.y,_dragEndPos.x,_dragEndPos.y };
	if (dragRC.left > dragRC.right) swap(dragRC.left, dragRC.right);
	if (dragRC.top > dragRC.bottom) swap(dragRC.top, dragRC.bottom);

	if (dragRC.left <= 0) dragRC.left = 0;
	if (dragRC.top <= 0) dragRC.top = 0;
	if (dragRC.right >= tile_X * TILESIZE) dragRC.right = tile_X * TILESIZE;
	if (dragRC.top >= tile_Y * TILESIZE) dragRC.bottom = tile_Y * TILESIZE;

	for (int i = 0; i < _vTiles.size(); i++)
	{
		if (isTileInDragRange(i))
		{
			if (_eraserOn)
			{
				_vTiles[i].terrain = TR_NONE;
				//타일을 지우고 난 후에 tState가 남아있어서 지워주기 
				_vTiles[i].tState = TS_EMPTY;
			}
			else
			{
				if (isAuto)
				{
					POINT size = { 0,0 };
					POINT frame = { 0,0 };
					switch (terrainSelect(_currentTile.x, _currentTile.y,_currentTile.key))
					{
					case TR_DUNGEONWALL:
						if (_dragSizeX < 5 || _dragSizeY < 5) return;
						frame = { 0,0 };
						this->autoDungeonSet(_vTiles, i, size, frame, dragRC);
						break;
					case TR_GRASS:
						if (_dragSizeX < 2 || _dragSizeY < 2) return;
						frame = { 0,0 };
						this->autoGrassSet(_vTiles, i, size, frame, dragRC);
						break;
					case TR_HOLE:
						frame = { 0,0 };
						this->autoHoleSet(_vTiles, i, size, frame, dragRC);
						break;
					}
					break;
				}
				else
				{
					_vTiles[i].terrainFrameX = _currentTile.x;
					_vTiles[i].terrainFrameY = _currentTile.y;
					_vTiles[i].key = _currentTile.key;
					_vTiles[i].terrain = this->terrainSelect(_currentTile.x, _currentTile.y,_currentTile.key);
				}
			}

		}
	}
	_dragStartPos = { 0,0 };
	_dragEndPos = { 0,0 };
}

bool maptoolScene::isTileInDragRange(int i)
{
	POINT _start;
	POINT _end;
	//아래 -> 위 / 오른쪽 -> 왼쪽 방향으로 드래그 할때 값 비교를 해서 시작지점 정해준다
	if (_dragStartPos.x < _dragEndPos.x)
	{
		_start.x = _dragStartPos.x;
		_end.x = _dragEndPos.x;
	}
	else
	{
		_start.x = _dragEndPos.x;
		_end.x = _dragStartPos.x;

	}
	if (_dragStartPos.y < _dragEndPos.y)
	{
		_start.y = _dragStartPos.y;
		_end.y = _dragEndPos.y;
	}
	else
	{
		_start.y = _dragEndPos.y;
		_end.y = _dragStartPos.y;
	}
	if (_vTiles[i].rc.bottom > _start.y && _vTiles[i].rc.top < _end.y &&
		_vTiles[i].rc.right > _start.x && _vTiles[i].rc.left < _end.x)
	{
		return true;
	}
	return false;
}

void maptoolScene::dragFrameRectRender(COLORREF color)
{
	for (int i = 0; i < tile_X * tile_Y; i++)
	{
		if (!checkTileMap(i)) continue;
		if (isTileInDragRange(i))
		{
			CAMERAMANAGER->FrameRect(getMemDC(), _vTiles[i].rc, color);
		}
	}
}

void maptoolScene::autoDragFrameRectRender()
{
	switch (terrainSelect(_currentTile.x,_currentTile.y,_currentTile.key))
	{
	case TR_DUNGEONWALL:
		if (_dragSizeX < 5 || _dragSizeY < 5) this->dragFrameRectRender(RGB(255, 0, 0));
		else this->dragFrameRectRender(RGB(0, 0, 255));
		break;
	case TR_GRASS:
		if (_dragSizeX < 2 || _dragSizeY < 2) this->dragFrameRectRender(RGB(255, 0, 0));
		else this->dragFrameRectRender(RGB(0, 0, 255));
		break;
	case TR_BLOCK:
		if (_dragSizeX < 3 || _dragSizeY < 3) this->dragFrameRectRender(RGB(255, 0, 0));
		else this->dragFrameRectRender(RGB(0, 0, 255));
		break;
	case TR_HOLE:
		this->dragFrameRectRender(RGB(0, 0, 255));
		break;
	case TR_OTHERS:
		//this->dragFrameRectRender(RGB(0, 0, 255));
		break;
	}
}

void maptoolScene::autoDungeonSet(vector<tagTile>& vTile, int index, POINT size, POINT frame, RECT range)
{
	if (index >= tile_X * tile_Y) return;
	RECT temp;
	if (IntersectRect(&temp, &_vTiles[index].rc, &range))
	{
		if (size.x == 0 || size.x == _dragSizeX-1 || size.y == 0 || size.y == _dragSizeY-1) _vTiles[index].tState = TS_MOVEBAN;
		else _vTiles[index].tState = TS_EMPTY;
		_vTiles[index].terrainFrameX = frame.x;
		_vTiles[index].terrainFrameY = frame.y;
		_vTiles[index].key = _currentTile.key;
		size.x++;
		_vTiles[index].terrain = terrainSelect(frame.x, frame.y,_currentTile.key);
		if (size.x == _dragSizeX)
		{
			if (size.y < 2 || size.y >= _dragSizeY - 3)
			{
				frame.y++;
			}
			frame.x = 0;
			index -= (size.x-1);
			index += tile_X;
			size.x = 0;
			size.y++;
			autoDungeonSet(vTile, index, size, frame, range);
		}
		else if (size.x <= 2 || size.x > _dragSizeX - 3)
		{
			frame.x++;
			index++;
			autoDungeonSet(vTile, index, size, frame, range);
		}
		else
		{
			index++;
			autoDungeonSet(vTile, index, size, frame, range);
		}
	}
}

void maptoolScene::autoGrassSet(vector<tagTile>& vTile, int index, POINT size, POINT frame, RECT range)
{
	if (index >= tile_X * tile_Y) return;
	RECT temp;
	if (IntersectRect(&temp, &_vTiles[index].rc, &range))
	{
		_vTiles[index].terrainFrameX = frame.x;
		_vTiles[index].terrainFrameY = frame.y;
		_vTiles[index].tState = TS_EMPTY;
		_vTiles[index].key = _currentTile.key;
		_vTiles[index].terrain = terrainSelect(frame.x, frame.y,_currentTile.key);
		size.x++;
		if (size.x == _dragSizeX)
		{
			if (size.y == _dragSizeY - 2)
			{
				frame.y = 2;
			}
			else frame.y = 1;
			frame.x = 0;
			index -= (size.x-1);
			index += tile_X;
			size.x = 0;
			size.y++;
			autoGrassSet(vTile, index, size, frame, range);
		}
		else if (size.x == _dragSizeX - 1)
		{
			frame.x = 4;
			index++;
			autoGrassSet(vTile, index, size, frame, range);
		}
		else
		{
			frame.x = RANDOM->range(1, 3);
			index++;
			autoGrassSet(vTile, index, size, frame, range);
		}
	}
}

void maptoolScene::autoHoleSet(vector<tagTile>& vTile, int index, POINT size, POINT frame, RECT range)
{
	if (index >= tile_X * tile_Y) return;
	RECT temp;
	if (IntersectRect(&temp, &_vTiles[index].rc, &range))
	{
		_vTiles[index].terrainFrameX = frame.x;
		_vTiles[index].terrainFrameY = frame.y;
		_vTiles[index].tState = TS_EMPTY;
		_vTiles[index].key = _currentTile.key;
		_vTiles[index].terrain = terrainSelect(frame.x, frame.y, _currentTile.key);
		size.x++;
		if (size.x == _dragSizeX)
		{
			if (size.y < 2)
			{
				frame.y++;
			}
			else if (size.y < _dragSizeY-2)
			{
				frame.y = RANDOM->range(2, 3);
			}
			else if (size.y == _dragSizeY-2)
			{
				frame.y = 4;
			}
			frame.x = 0;
			index -= (size.x - 1);
			index += tile_X;
			size.x = 0;
			size.y++;
			autoHoleSet(vTile, index, size, frame, range);
		}
		else if (size.x == _dragSizeX - 1)
		{
			frame.x = 4;
			index++;
			autoHoleSet(vTile, index, size, frame, range);
		}
		else
		{
			frame.x = RANDOM->range(1, 3);
			index++;
			autoHoleSet(vTile, index, size, frame, range);
		}
	}
}

void maptoolScene::dragCount()
{
	RECT dragRC = { _dragStartPos.x,_dragStartPos.y,_dragEndPos.x,_dragEndPos.y };
	if (dragRC.left > dragRC.right) swap(dragRC.left, dragRC.right);
	if (dragRC.top > dragRC.bottom) swap(dragRC.top, dragRC.bottom);

	if (dragRC.left <= 0) dragRC.left = 0;
	if (dragRC.top <= 0) dragRC.top = 0;
	if (dragRC.right >= tile_X * TILESIZE) dragRC.right = tile_X * TILESIZE;
	if (dragRC.top >= tile_Y * TILESIZE) dragRC.bottom = tile_Y * TILESIZE;

	int con[4] = { 0,0,0,0 };

	for (int i = 0, count = 0; i < _vTiles.size(); i++)
	{
		RECT temp;
		if (isTileInDragRange(i))
		{
			if (_vTiles[i].rc.left <= dragRC.left && _vTiles[i].rc.top <= dragRC.top) // LT 모서리
			{
				con[0] = i;
			}
			if (_vTiles[i].rc.left <= dragRC.left && _vTiles[i].rc.bottom >= dragRC.bottom) // LB 모서리
			{
				con[2] = i;
			}
			if (_vTiles[i].rc.right >= dragRC.right && _vTiles[i].rc.top <= dragRC.top) // RT 모서리
			{
				con[1] = i;
			}
			if (_vTiles[i].rc.right >= dragRC.right && _vTiles[i].rc.bottom >= dragRC.bottom) // RB 모서리
			{
				con[3] = i;
			}
		}
	}

	_dragSizeX = con[1] - con[0] + 1;
	_dragSizeY = (con[2] - con[0]) / tile_X + 1;
}

void maptoolScene::dragMouseFix()
{
	RECT rc;
	POINT lt;
	POINT rb;
	if (_menuBarOn)
	{
		rc = { 0,0,WINSIZEX - 400,WINSIZEY };
		lt = { rc.left,rc.top };
		rb = { rc.right,rc.bottom };
		ClientToScreen(_hWnd, &lt);
		ClientToScreen(_hWnd, &rb);
		rc = { lt.x,lt.y,rb.x,rb.y };
		ClipCursor(&rc);
		rc = { 0,0,WINSIZEX - 400,WINSIZEY };
	}
	else
	{
		GetClientRect(_hWnd, &rc);
		lt = { rc.left,rc.top };
		rb = { rc.right,rc.bottom };
		ClientToScreen(_hWnd, &lt);
		ClientToScreen(_hWnd, &rb);
		rc = { lt.x,lt.y,rb.x,rb.y };
		ClipCursor(&rc);
		GetClientRect(_hWnd, &rc);
	}
	
	if (_ptMouse.y <= rc.top + 4 && (_pivotY > WINSIZEY / 2))
	{
		_pivotY -= 5;
	}
	if (_ptMouse.x <= rc.left + 4 && _pivotX > WINSIZEX / 2)
	{
		_pivotX -= 5;
	}
	if (_ptMouse.y >= rc.bottom - 4 && _pivotY < (WINSIZEY * 0.5) + ((tile_Y * TILESIZE) - WINSIZEY))
	{
		_pivotY += 5;
	}
	if (_ptMouse.x >= rc.right - 4 && _pivotX < (WINSIZEX * 0.5) + ((tile_X * TILESIZE) - WINSIZEX))
	{
		_pivotX += 5;
	}
	CAMERAMANAGER->movePivot(_pivotX, _pivotY);

}

void maptoolScene::cmMouseUpdate()
{
	_cmMouse.x = CAMERAMANAGER->getRect().left + _ptMouse.x;
	_cmMouse.y = CAMERAMANAGER->getRect().top + _ptMouse.y;
}

void maptoolScene::alphaRectangle(RECT rc, BYTE alpha, COLORREF color, bool isCameraRender)
{
	_alphaRect->init(rc.right - rc.left, rc.bottom - rc.top);
	HBRUSH brush = CreateSolidBrush(color);
	HBRUSH obrush = (HBRUSH)SelectObject(_alphaRect->getMemDC(), brush);
	Rectangle(_alphaRect->getMemDC(), RectMake(0, 0, rc.right - rc.left, rc.bottom - rc.top));
	SelectObject(_alphaRect->getMemDC(), obrush);
	DeleteObject(brush);
	if(isCameraRender) CAMERAMANAGER->AlphaRender(getMemDC(), _alphaRect, rc.left, rc.top, alpha);
	else _alphaRect->alphaRender(getMemDC(), rc.left, rc.top, alpha);
}

void maptoolScene::setTileState()
{
	RECT menubar;
	menubar.left = _menuBar.left + CAMERAMANAGER->getRect().left;
	menubar.right = _menuBar.right + CAMERAMANAGER->getRect().left;
	menubar.top = _menuBar.top + CAMERAMANAGER->getRect().top;
	menubar.bottom = _menuBar.bottom + CAMERAMANAGER->getRect().top;
	if (PtInRect(&menubar, _cmMouse) && _menuBarOn) return;
	for (int i = 0; i < _vTiles.size(); i++)
	{
		if (PtInRect(&_vTiles[i].rc, _cmMouse))
		{
			_vTiles[i].tState = _currentTile.tState;
		}
	}
}

void maptoolScene::autoSetTileState()
{
	RECT menubar;
	menubar.left = _menuBar.left + CAMERAMANAGER->getRect().left;
	menubar.right = _menuBar.right + CAMERAMANAGER->getRect().left;
	menubar.top = _menuBar.top + CAMERAMANAGER->getRect().top;
	menubar.bottom = _menuBar.bottom + CAMERAMANAGER->getRect().top;
	if (PtInRect(&menubar, _cmMouse)) return;
	for (int i = 0; i < _vTiles.size(); i++)
	{
		if (isTileInDragRange(i))
		{
			if (_eraserOn) _vTiles[i].tState = TS_EMPTY;
			else _vTiles[i].tState = _currentTile.tState;
		}
	}
	_dragSizeX = 0;
	_dragSizeY = 0;
}

void maptoolScene::tileStateRender()
{
	for (int i = 0; i < _vTiles.size(); i++)
	{
		if (checkTileMap(i))
		{
			switch (_vTiles[i].tState)
			{
			case TS_MOVEBAN:
				this->alphaRectangle(_vTiles[i].rc, 100, RGB(255, 0, 0), true);
				break;
			case TS_PORTAL:
				this->alphaRectangle(_vTiles[i].rc, 100, RGB(0, 0, 255), true);
				break;
			case TS_EMPTY:
				break;
			}
		}
	}
}

void maptoolScene::changeSampletile()
{
	switch (_ctrlSelect)
	{
	case CTRL_TERRAIN:
		if (_page == 0)
		{
			for (int i = 0; i < SAMPLETILEX * SAMPLETILEY; i++)
			{
				_sampleTile[i].key = "던전타일";
			}
		}
		else if (_page == 1)
		{
			for (int i = 0; i < SAMPLETILEX * SAMPLETILEY; i++)
			{
				_sampleTile[i].key = "잔디타일";
			}
		}
		else if (_page == 2)
		{
			for (int i = 0; i < SAMPLETILEX * SAMPLETILEY; i++)
			{
				_sampleTile[i].key = "쩌리타일";
			}
		}
		break;
	}
}

void maptoolScene::miniMapRender()
{
	_miniMap->init(tile_X * TILESIZE, tile_Y * TILESIZE);
	for (int i = 0; i < tile_X * tile_Y; i++)
	{
		if (_vTiles[i].terrain == TR_NONE || _vTiles[i].key == "") continue;

		IMAGEMANAGER->findImage(_vTiles[i].key)->frameRender(_miniMap->getMemDC(), _vTiles[i].rc.left, _vTiles[i].rc.top,
			_vTiles[i].terrainFrameX, _vTiles[i].terrainFrameY);
	}
	for (int i = 0; i < tile_X * tile_Y; i++)
	{
		{
			if (_vObject[i].obj == OBJ_NONE || _vObject[i].key == "") continue;
			IMAGEMANAGER->findImage(_vObject[i].key)->frameRender(_miniMap->getMemDC(), _vObject[i].rc.left, _vObject[i].rc.top,
				IMAGEMANAGER->findImage(_vObject[i].key)->getFrameX(), 0);
		}
	}
}

void maptoolScene::saveImage()
{
	BITMAPFILEHEADER BMFH;
	BITMAPINFOHEADER BMIH;

	int nWidth = 0;
	int nHeight = 0;
	unsigned long dwQuadrupleWidth = 0;
	
	
}

INT_PTR maptoolScene::DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCLOSE:
			EndDialog(hWnd, IDCANCEL);
			return FALSE;
		case IDC_SAVE_BTN:
			GetDlgItemText(hWnd, IDC_EDIT1, fileName, 200);
			EndDialog(hWnd, IDCANCEL);
			return TRUE;
		case IDC_LOAD_BTN:
			GetDlgItemText(hWnd, IDC_EDIT1, fileName, 200);
			EndDialog(hWnd, IDCANCEL);
			return TRUE;
		}
		break;
	}

	return FALSE;
}

