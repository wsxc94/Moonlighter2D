#include "stdafx.h"
#include "itemMenu.h"

HRESULT itemMenu::init()
{
	// 'I' 버튼을 눌러서 메뉴 on/off 
	// 'Q(좌)' 'E(우)' 버튼을 눌러서 메뉴 이동 

	//클래스 초기화 
	_calendar = new calendar;
	_calendar->init();

	_wishList = new wishList;
	_wishList->init();

	_inventory = new inventory;
	_inventory->init();

	_noteBook = new noteBook;
	_noteBook->init();

	//메뉴 위치 초기화 
	initMenuPos();
	_tagPos.x = 490;
	_tagPos.y = -TAGINITPOSY;

	//변수 초기화 
	_ctrlState = CTRL_INVEN;
	_tagIdx = CTRL_INVEN;

	_menuMoveSpeed = 90.f;
	_tagMoveSpeed = 10.f;

	_menuOn = false;
	_canKeyInput = true; 
	_calendarOn = false;
	_wishListOn = false;
	_invenOn = false;
	_noteBookOn = false;

	_openMenu = false;
	_closeMenu = false;
	_openTagMenu = false;
	_closeTagMenu = false;
	_movingLeft = false;
	_movingRight = false;
	_goToTown = false; 

	return S_OK;
}

void itemMenu::release()
{
	_calendar->release();
	SAFE_DELETE(_calendar);

	_wishList->release();
	SAFE_DELETE(_wishList);

	_inventory->release();
	SAFE_DELETE(_inventory);

	_noteBook->release();
	SAFE_DELETE(_noteBook);
}

void itemMenu::update()
{
	//메뉴 on/off 관련 함수 
	if(_canKeyInput) toggleMenu();
	openMenu();
	closeMenu();
	openTagMenu();
	closeTagMenu();

	//키입력 함수 
	keyInput();

	//플레이어 데이터 업데이트 함수  
	setCurItemCount();
	setPotionEquipped();
	setWeaponIdx();
	setWeaponEquipped();

	if (_menuOn)
	{
		switch (_ctrlState)
		{
			case CTRL_CALENDAR:
				break;

			case CTRL_WISHLIST:
				break;

			case CTRL_INVEN:
				_inventory->update();
				break;

			case CTRL_NOTEBOOK:
				break;
		}
	}
}

void itemMenu::render(HDC hdc)
{
	if (_menuOn)
	{
		menuRender(hdc);
		tagMenuRender(hdc);
	}

	//char str[128];
	//wsprintf(str, "ctrlState : %d", _ctrlState);
	//TextOut(hdc, 10, 70, str, strlen(str));

	//wsprintf(str, "calendarOn : %d", _calendarOn);
	//TextOut(getMemDC(), 10, 90, str, strlen(str));

	//wsprintf(str, "noteOn : %d", _noteBookOn);
	//TextOut(getMemDC(), 10, 110, str, strlen(str));

	//wsprintf(str, "wishlistOn : %d", _wishListOn);
	//TextOut(getMemDC(), 10, 130, str, strlen(str));

	//wsprintf(str, "invenOn : %d", _invenOn);
	//TextOut(hdc, 10, 150, str, strlen(str));

	//wsprintf(str, "tagIdx : %d", _tagIdx);
	//TextOut(getMemDC(), 10, 170, str, strlen(str));
}

void itemMenu::setCurItemCount()
{
	int itemCount = _inventory->getCurItemCount();

	PLAYERDATA->setItemCount(itemCount);
}

void itemMenu::setPotionEquipped()
{
	PLAYERDATA->setPotionEquipped(_inventory->getPotionEquipped());
}

void itemMenu::setWeaponIdx()
{
	PLAYERDATA->setWeaponIdx(_inventory->getWeaponIdx());
}

void itemMenu::setWeaponEquipped()
{
	PLAYERDATA->setWeaponEquipped(_inventory->getWeaponEquipped());
}

void itemMenu::toggleMenu()
{
	if (!_canKeyInput) return; 

	if (INPUT->GetKeyDown('I'))
	{
		if (_menuOn)
		{
			DoCloseMenu();
		}
		else
		{
			//컨트롤러 상태 초기화(인벤토리) 및 인벤토리 변수 초기화  
			setCtrlState(CTRL_INVEN);
			_inventory->initInven();

			_tagIdx = CTRL_INVEN;
			_menuOn = true;
			_invenOn = true;
			_openMenu = true;
			_openTagMenu = true;

			SOUNDMANAGER->play("openInven", 0.4f);
		}
	}
}

void itemMenu::DoCloseMenu()
{
	_closeMenu = true;
	_closeTagMenu = true;

	//인벤토리 창을 닫으므로 커서를 꺼준다.
	_inventory->getCursor()->setShowCursor(false);
}

void itemMenu::openMenu()
{
	if (!_openMenu) return;

	_inventory->setInvenPosY(_inventory->getInvenPos().y - _menuMoveSpeed);

	if (_inventory->getInvenPos().y <= INVENPOSY)
	{
		_inventory->setInvenPosY(INVENPOSY);
		_openMenu = false;
		_inventory->getCursor()->setShowCursor(true);	//인벤토리 창을 열었으므로 커서를 켜준다.
	}
}

void itemMenu::closeMenu()
{
	if (!_closeMenu) return;

	//현재 열려있는 메뉴 상태에 따라서 메뉴를 닫을 때 이동하는 메뉴가 다르므로 
	//현재의 컨트롤러 상태(열려있는 메뉴 상태)에 맞추어 그에 알맞은 동작 실행 
	switch (_ctrlState)
	{
		case CTRL_CALENDAR:
			_calendar->moveCalendarLeft(-IMAGEMANAGER->findImage("menu_calendar")->getWidth());

			if (_calendar->getCalendarPos().x == -IMAGEMANAGER->findImage("menu_calendar")->getWidth())
			{
				_closeMenu = false;
				_calendarOn = false;
				_menuOn = false;
			}
			break;

		case CTRL_WISHLIST:
			_wishList->moveWishListLeft(-IMAGEMANAGER->findImage("menu_wishlist")->getWidth());

			if (_wishList->getWishListPos().x == -IMAGEMANAGER->findImage("menu_wishlist")->getWidth())
			{
				_closeMenu = false;
				_wishListOn = false;
				_menuOn = false;
			}
			break;

		case CTRL_INVEN:
			_inventory->setInvenPosY(_inventory->getInvenPos().y + _menuMoveSpeed);

			if (_inventory->getInvenPos().y >= WINSIZEY)
			{
				_inventory->setInvenPosY(WINSIZEY);
				_closeMenu = false;
				_invenOn = false;
				_menuOn = false;
			}
			break;

		case CTRL_NOTEBOOK:
			_noteBook->moveNoteBookRight(WINSIZEX);

			if (_noteBook->getNoteBookPos().x == WINSIZEX)
			{
				_closeMenu = false;
				_noteBookOn = false;
				_menuOn = false;
			}
			break;
	}

	//메뉴를 닫는 일련의 동작들이 다 끝났을 때, 모든 메뉴의 위치를 초기화
	//메뉴를 닫으면서 이벤토리 창의 커서도 함께 꺼준다. 
	if (!_closeMenu) initMenuPos();
}

void itemMenu::openTagMenu()
{
	if (!_openTagMenu) return;

	_tagPos.y += _tagMoveSpeed;

	if (_tagPos.y >= TAGPOSY)
	{
		_tagPos.y = TAGPOSY;
		_openTagMenu = false;
	}
}

void itemMenu::closeTagMenu()
{
	if (!_closeTagMenu) return;

	_tagPos.y -= _tagMoveSpeed;

	if (_tagPos.y <= -TAGINITPOSY)
	{
		_tagPos.y = -TAGINITPOSY;
		_closeTagMenu = false;
	}
}

void itemMenu::initMenuPos()
{
	_calendar->initPos();
	_wishList->initPos();
	_inventory->initPos();
	_noteBook->initPos();
}

void itemMenu::setCtrlState(CTRL_MENU state)
{
	_movingLeft = false;
	_movingRight = false;

	switch (state)
	{
		case CTRL_CALENDAR:
			_ctrlState = state;
			break;

		case CTRL_WISHLIST:
			_ctrlState = state;
			break;

		case CTRL_INVEN:
			_ctrlState = state;
			break;

		case CTRL_NOTEBOOK:
			_ctrlState = state;
			break;
	}
}

void itemMenu::keyInput()
{
	//현재 메뉴가 열려있지 않거나, 메뉴열기 및 메뉴닫기 동작이 실행중이라면 키입력을 받지 않음.
	if (!_menuOn || _openMenu || _closeMenu) return;

	//좌우(Q,E) 키 입력을 받는다.
	leftKeyDown();
	rightKeyDown();

	//현재 컨트롤러 상태에 따라 키 입력에 따른 동작(메뉴 이동)을 다르게 처리한다. 
	switch (_ctrlState)
	{
		case CTRL_CALENDAR:
			ctrlCalendarInput();
			break;

		case CTRL_WISHLIST:
			ctrlWishListInput();
			break;

		case CTRL_INVEN:
			ctrlInvenInput();
			break;

		case CTRL_NOTEBOOK:
			ctrlNoteBookInput();
			break;
	}
}

void itemMenu::leftKeyDown()
{
	//캘린더메뉴가 왼쪽 끝 메뉴이므로 캘린더메뉴에서 왼쪽 키 입력은 받지 않도록 함. 
	if (_ctrlState == CTRL_CALENDAR) return;

	if (INPUT->GetKeyDown('Q'))
	{
		if (!_movingLeft && !_movingRight)
		{
			_movingLeft = true;
			_movingRight = false;
			_tagIdx--;

			SOUNDMANAGER->play("moveMenu", 0.3f);
		}
	}
}

void itemMenu::rightKeyDown()
{
	//노트북메뉴가 오른쪽 끝 메뉴이므로 노트북메뉴에서 오른쪽 키 입력은 받지 않도록 함. 
	if (_ctrlState == CTRL_NOTEBOOK) return;

	if (INPUT->GetKeyDown('E'))
	{
		if (!_movingRight && !_movingLeft)
		{
			_movingRight = true;
			_movingLeft = false;
			_tagIdx++;

			SOUNDMANAGER->play("moveMenu", 0.3f);
		}
	}
}

void itemMenu::ctrlCalendarInput()
{
	if (_movingRight)
	{
		if (!_wishListOn) _wishListOn = true;

		//각 메뉴의 position 값을 destination 값까지 이동시킨다.  
		_calendar->moveCalendarLeft(-IMAGEMANAGER->findImage("menu_calendar")->getWidth());
		_wishList->moveWishListLeft(WISHLISTPOSX);

		//각 메뉴가 destination 위치까지 도달했을 경우 랜더용 변수 변경 및 컨트롤러상태 변경 
		if (_calendar->getCalendarPos().x == -IMAGEMANAGER->findImage("menu_calendar")->getWidth() &&
			_wishList->getWishListPos().x == WISHLISTPOSX)
		{
			_calendarOn = false;
			setCtrlState(CTRL_WISHLIST);
		}
	}
}

void itemMenu::ctrlWishListInput()
{
	if (_movingLeft)
	{
		if (!_calendarOn) _calendarOn = true;

		_wishList->moveWishListRight(WINSIZEX);
		_calendar->moveCalendarRight(CALENDARPOSX);

		if (_wishList->getWishListPos().x == WINSIZEX && _calendar->getCalendarPos().x == CALENDARPOSX)
		{
			_wishListOn = false;
			setCtrlState(CTRL_CALENDAR);
		}
	}
	if (_movingRight)
	{
		if (!_invenOn) _invenOn = true;

		//각 메뉴의 position 값을 destination 값까지 이동시킨다.  
		_wishList->moveWishListLeft(-IMAGEMANAGER->findImage("menu_wishlist")->getWidth());
		_inventory->moveInvenLeft(INVENPOSX);

		//각 메뉴가 destination 위치까지 도달했을 경우 랜더용 변수 변경 및 컨트롤러상태 변경 
		if (_wishList->getWishListPos().x == -IMAGEMANAGER->findImage("menu_wishlist")->getWidth() &&
			_inventory->getInvenPos().x == INVENPOSX)
		{
			_wishListOn = false;
			setCtrlState(CTRL_INVEN);

			//인벤토리 창으로 넘어왔으므로 커서를 켜준다.
			_inventory->getCursor()->setShowCursor(true);	
		}
	}
}

void itemMenu::ctrlInvenInput()
{
	if (_movingLeft)
	{
		//인벤토리를 이동시킬 것이므로 커서를 꺼준다. 
		if (_inventory->getCursor()->getShowCursor())
		{
			_inventory->getCursor()->setShowCursor(false);
		}
		if (!_wishListOn) _wishListOn = true;

		_inventory->moveInvenRight(WINSIZEX);
		_wishList->moveWishListRight(WISHLISTPOSX);

		if (_inventory->getInvenPos().x == WINSIZEX && _wishList->getWishListPos().x == WISHLISTPOSX)
		{
			_invenOn = false;
			setCtrlState(CTRL_WISHLIST);
		}
	}
	if (_movingRight)
	{
		//인벤토리를 이동시킬 것이므로 커서를 꺼준다. 
		if (_inventory->getCursor()->getShowCursor())
		{
			_inventory->getCursor()->setShowCursor(false);
		}
		if (!_noteBookOn) _noteBookOn = true;

		//각 메뉴의 position 값을 destination 값까지 이동시킨다.  
		_inventory->moveInvenLeft(-IMAGEMANAGER->findImage("menu_inventory")->getWidth());
		_noteBook->moveNoteBookLeft(NOTEBOOKPOSX);

		//각 메뉴가 destination 위치까지 도달했을 경우 랜더용 변수 변경 및 컨트롤러상태 변경 
		if (_inventory->getInvenPos().x == -IMAGEMANAGER->findImage("menu_inventory")->getWidth() &&
			_noteBook->getNoteBookPos().x == NOTEBOOKPOSX)
		{
			_invenOn = false;
			setCtrlState(CTRL_NOTEBOOK);
		}
	}
}

void itemMenu::ctrlNoteBookInput()
{
	if (_movingLeft)
	{
		if (!_invenOn) _invenOn = true;

		_noteBook->moveNoteBookRight(WINSIZEX);
		_inventory->moveInvenRight(INVENPOSX);

		if (_inventory->getInvenPos().x == INVENPOSX && _noteBook->getNoteBookPos().x == WINSIZEX)
		{
			_noteBookOn = false;
			setCtrlState(CTRL_INVEN);
			
			//인벤토리 창으로 넘어왔으므로 커서를 켜준다. 
			_inventory->getCursor()->setShowCursor(true);	
		}
	}
}

void itemMenu::menuMoveLeft(POINT *pos, const int destPos)
{
	pos->x -= _menuMoveSpeed;

	if (pos->x <= destPos)
	{
		pos->x = destPos;
	}
}

void itemMenu::menuMoveRight(POINT *pos, const int destPos)
{
	pos->x += _menuMoveSpeed;

	if (pos->x >= destPos)
	{
		pos->x = destPos;
	}
}

void itemMenu::menuRender(HDC hdc)
{
	if (_calendarOn)
	{
		_calendar->render(hdc);
	}
	if (_wishListOn)
	{
		_wishList->render(hdc);
	}
	if (_invenOn)
	{
		_inventory->render(hdc);
	}
	if (_noteBookOn)
	{
		_noteBook->render(hdc);
	}
}

void itemMenu::tagMenuRender(HDC hdc)
{
	IMAGEMANAGER->findImage("btn_qe")->render(hdc, 490, _tagPos.y);

	//캘린더태그 출력 
	if (_tagIdx == CTRL_CALENDAR)
	{
		IMAGEMANAGER->findImage("toggle_calendar")->frameRender(
			hdc, 542, _tagPos.y - 6, 1, 0);
	}
	else
	{
		IMAGEMANAGER->findImage("toggle_calendar")->frameRender(
			hdc, 542, _tagPos.y - 6, 0, 0);
	}

	//위시리스트태그 출력 
	if (_tagIdx == CTRL_WISHLIST)
	{
		IMAGEMANAGER->findImage("toggle_wishlist")->frameRender(
			hdc, 592, _tagPos.y - 6, 1, 0);
	}
	else
	{
		IMAGEMANAGER->findImage("toggle_wishlist")->frameRender(
			hdc, 592, _tagPos.y - 6, 0, 0);
	}

	//인벤토리태그 출력 
	if (_tagIdx == CTRL_INVEN)
	{
		IMAGEMANAGER->findImage("toggle_inventory")->frameRender(
			hdc, 642, _tagPos.y - 6, 1, 0);
	}
	else
	{
		IMAGEMANAGER->findImage("toggle_inventory")->frameRender(
			hdc, 642, _tagPos.y - 6, 0, 0);
	}

	//노트북태그 출력 
	if (_tagIdx == CTRL_NOTEBOOK)
	{
		IMAGEMANAGER->findImage("toggle_notebook")->frameRender(
			hdc, 692, _tagPos.y - 6, 1, 0);
	}
	else
	{
		IMAGEMANAGER->findImage("toggle_notebook")->frameRender(
			hdc, 692, _tagPos.y - 6, 0, 0);
	}
}

