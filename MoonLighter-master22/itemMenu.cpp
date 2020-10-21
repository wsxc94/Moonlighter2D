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

	_fadeManager = new fadeManager;
	_fadeManager->init();

	//메뉴 위치 초기화 
	initMenuPos();
	_tagPos.x = 490;
	_tagPos.y = -TAGINITPOSY;

	//변수 초기화 
	_initIsDone = true; 
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
	_goToTown_Pentant = false; 
	_goToTown_Emblem = false;

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

	_fadeManager->release();
	SAFE_DELETE(_fadeManager);
}

void itemMenu::update()
{
	_fadeManager->update();

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
	_fadeManager->render(hdc);

	if (_menuOn)
	{
		menuRender(hdc);
		tagMenuRender(hdc);
	}

	//char str[128];
	//wsprintf(str, "ctrlState : %d", _ctrlState);
	//TextOut(hdc, 10, 70, str, strlen(str));
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

			_fadeManager->fadeInit(16, FADE_IN);
		}
		else
		{
			//컨트롤러 상태 초기화(인벤토리) 및 인벤토리 변수 초기화  
			setCtrlState(CTRL_INVEN);

			_tagIdx = CTRL_INVEN;
			_menuOn = true;
			_invenOn = true;
			_openMenu = true;
			_openTagMenu = true;

			_fadeManager->fadeInit(16, FADE_OUT, 205);
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

	//인벤토리 창을 닫을 때 혹시 잡고 있던 아이템이 있다면 다시 내려놓기 
	_inventory->putGrabbingItem();

	//아이템 창을 닫으면서 아이템을 잡지 않도록 변수 설정 
	_inventory->setCanGrab(false);
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
		_inventory->initInven();
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

void itemMenu::loadImages()
{
	////아이템메뉴 이미지 초기화 
	//_loading->loadImage("menu_calendar", "Images/ItemMenu/menu_calendar.bmp", 1002, 592, true, RGB(255, 0, 255));
	//_loading->loadImage("menu_wishlist", "Images/ItemMenu/menu_wishlist.bmp", 818, 518, true, RGB(255, 0, 255));
	//_loading->loadImage("menu_inventory", "Images/ItemMenu/menu_inventory.bmp", 1044, 598, true, RGB(255, 0, 255));
	//_loading->loadImage("menu_inventory_inDungeon", "Images/ItemMenu/menu_inventory_inDungeon.bmp", 1044, 598, true, RGB(255, 0, 255));
	//_loading->loadImage("menu_notebook", "Images/ItemMenu/menu_notebook.bmp", 827, 574, true, RGB(255, 0, 255));
	//_loading->loadImage("btn_qe", "Images/ItemMenu/btn_qe.bmp", 300, 38, true, RGB(255, 0, 255));
	//_loading->loadFrameImage("toggle_calendar", "Images/ItemMenu/toggle_calendar.bmp", 92, 50, 2, 1);
	//_loading->loadFrameImage("toggle_wishlist", "Images/ItemMenu/toggle_wishlist.bmp", 92, 50, 2, 1);
	//_loading->loadFrameImage("toggle_inventory", "Images/ItemMenu/toggle_inventory.bmp", 92, 50, 2, 1);
	//_loading->loadFrameImage("toggle_notebook", "Images/ItemMenu/toggle_notebook.bmp", 92, 50, 2, 1);
	//_loading->loadFrameImage("icon_euip", "Images/ItemMenu/icon_euip.bmp", 64, 30, 2, 1);
	//_loading->loadFrameImage("icon_equip_idx", "Images/ItemMenu/icon_equip_idx.bmp", 60, 30, 2, 1);
	//_loading->loadFrameImage("icon_equip_idx_empty", "Images/ItemMenu/icon_equip_idx_empty.bmp", 60, 30, 2, 1);
	//_loading->loadImage("nameBase", "Images/ItemMenu/nameBase.bmp", 310, 34, true, RGB(255, 0, 255));
	//_loading->loadImage("grabBase", "Images/ItemMenu/grabBase.bmp", 72, 72, true, RGB(255, 0, 255));
	//_loading->loadImage("item_slot", "Images/ItemMenu/item_slot.bmp", 60, 60, true, RGB(255, 0, 255));
	//_loading->loadImage("inventory_selectBubble", "Images/ItemMenu/inventory_selectBubble.bmp", 220, 144, true, RGB(255, 0, 255));
	//_loading->loadFrameImage("select_yes", "Images/ItemMenu/select_yes.bmp", 312, 78, 4, 1);
	//_loading->loadFrameImage("select_no", "Images/ItemMenu/select_no.bmp", 312, 78, 4, 1);
	//_loading->loadImage("bag_pendant", "Images/ItemMenu/bag_pendant.bmp", 70, 48, true, RGB(255, 0, 255));
	//_loading->loadImage("bag_emblem", "Images/ItemMenu/bag_emblem.bmp", 68, 66, true, RGB(255, 0, 255));

	////커서 이미지 초기화 
	//_loading->loadFrameImage("cursor_move", "Images/ItemMenu/cursor_move.bmp", 304, 76, 4, 1);
	//_loading->loadFrameImage("cursor_click", "Images/ItemMenu/cursor_click.bmp", 380, 76, 5, 1);
	//_loading->loadFrameImage("cursor_price", "Images/Shop/cursor_price.bmp", 408, 94, 2, 1);
	//_loading->loadFrameImage("cursor_select", "Images/ItemMenu/cursor_select.bmp", 510, 102, 5, 1);

	////아이템 이미지 초기화 
	//_loading->loadImage("풍부한 젤리", "Images/Item/item_richJelly.bmp", 48, 48, true, RGB(255, 0, 255));
	//_loading->loadImage("맹독성 젤리", "Images/Item/item_venomJelly.bmp", 48, 48, true, RGB(255, 0, 255));
	//_loading->loadImage("강화 수정", "Images/Item/item_crystal.bmp", 48, 48, true, RGB(255, 0, 255));
	//_loading->loadImage("덩굴", "Images/Item/item_vine.bmp", 48, 48, true, RGB(255, 0, 255));
	//_loading->loadImage("이빨석", "Images/Item/item_teethStone.bmp", 48, 48, true, RGB(255, 0, 255));
	//_loading->loadImage("뿌리", "Images/Item/item_root.bmp", 48, 48, true, RGB(255, 0, 255));
	//_loading->loadImage("쇠막대", "Images/Item/item_ironBar.bmp", 48, 48, true, RGB(255, 0, 255));
	//_loading->loadImage("주물 잔해", "Images/Item/item_foundryRests.bmp", 48, 48, true, RGB(255, 0, 255));
	//_loading->loadImage("망가진 검", "Images/Item/item_brokenSword.bmp", 48, 48, true, RGB(255, 0, 255));
	//_loading->loadImage("천", "Images/Item/item_fabric.bmp", 48, 48, true, RGB(255, 0, 255));
	//_loading->loadImage("경화 강철", "Images/Item/item_hardenedSteel.bmp", 48, 48, true, RGB(255, 0, 255));

	//_loading->loadImage("훈련용 단검", "Images/Item/item_trainingShortSword.bmp", 48, 48, true, RGB(255, 0, 255));
	//_loading->loadImage("훈련용 활", "Images/Item/item_trainingBow.bmp", 48, 48, true, RGB(255, 0, 255));

	//_loading->loadImage("HP 물약Ⅰ", "Images/Item/item_hpPotion1.bmp", 48, 48, true, RGB(255, 0, 255));
	//_loading->loadImage("HP 물약Ⅱ", "Images/Item/item_hpPotion2.bmp", 48, 48, true, RGB(255, 0, 255));

	////아이템 이름 이미지 초기화
	//_loading->loadImage("name_richJelly", "Images/Item/name_richJelly.bmp", 160, 24);
	//_loading->loadImage("name_venomJelly", "Images/Item/name_venomJelly.bmp", 160, 24);
	//_loading->loadImage("name_crystal", "Images/Item/name_crystal.bmp", 160, 24);
	//_loading->loadImage("name_vine", "Images/Item/name_vine.bmp", 160, 24);
	//_loading->loadImage("name_teethStone", "Images/Item/name_teethStone.bmp", 160, 24);
	//_loading->loadImage("name_root", "Images/Item/name_root.bmp", 160, 24);
	//_loading->loadImage("name_ironBar", "Images/Item/name_ironBar.bmp", 160, 24);
	//_loading->loadImage("name_foundryRests", "Images/Item/name_foundryRests.bmp", 160, 24);
	//_loading->loadImage("name_brokenSword", "Images/Item/name_brokenSword.bmp", 160, 24);
	//_loading->loadImage("name_fabric", "Images/Item/name_fabric.bmp", 160, 24);
	//_loading->loadImage("name_hardenedSteel", "Images/Item/name_hardenedSteel.bmp", 160, 24);

	//_loading->loadImage("name_trainingShortSword", "Images/Item/name_trainingShortSword.bmp", 160, 24);
	//_loading->loadImage("name_trainingBow", "Images/Item/name_trainingBow.bmp", 160, 24);

	//_loading->loadImage("name_hpPotion1", "Images/Item/name_hpPotion1.bmp", 160, 24);
	//_loading->loadImage("name_hpPotion2", "Images/Item/name_hpPotion2.bmp", 160, 24);

	////아이템 설명 이미지 초기화 
	//_loading->loadImage("des_richJelly", "Images/Item/des_richJelly.bmp", 304, 66);
	//_loading->loadImage("des_venomJelly", "Images/Item/des_venomJelly.bmp", 314, 42);
	//_loading->loadImage("des_crystal", "Images/Item/des_crystal.bmp", 280, 20);
	//_loading->loadImage("des_vine", "Images/Item/des_vine.bmp", 292, 42);
	//_loading->loadImage("des_teethStone", "Images/Item/des_teethStone.bmp", 310, 42);
	//_loading->loadImage("des_root", "Images/Item/des_root.bmp", 316, 42);
	//_loading->loadImage("des_ironBar", "Images/Item/des_ironBar.bmp", 312, 64);
	//_loading->loadImage("des_foundryRests", "Images/Item/des_foundryRests.bmp", 278, 64);
	//_loading->loadImage("des_brokenSword", "Images/Item/des_brokenSword.bmp", 276, 64);
	//_loading->loadImage("des_fabric", "Images/Item/des_fabric.bmp", 306, 64);
	//_loading->loadImage("des_hardenedSteel", "Images/Item/des_hardenedSteel.bmp", 270, 42);

	//_loading->loadImage("des_trainingShortSword", "Images/Item/des_trainingShortSword.bmp", 286, 64);
	//_loading->loadImage("des_trainingBow", "Images/Item/des_trainingBow.bmp", 306, 66);

	//_loading->loadImage("des_hpPotion1", "Images/Item/des_hpPotion1.bmp", 310, 42);
	//_loading->loadImage("des_hpPotion2", "Images/Item/des_hpPotion2.bmp", 304, 42);

	////인터페이스 이미지 초기화 
	//_loading->loadImage("interface_key", "Images/Interface/interface_key.bmp", 174, 152, true, RGB(255, 0, 255));
	//_loading->loadImage("icon_coin", "Images/Interface/icon_coin.bmp", 18, 18, true, RGB(255, 0, 255));
	//_loading->loadImage("icon_goldPouch", "Images/Interface/icon_goldPouch.bmp", 70, 70, true, RGB(255, 0, 255));
	//_loading->loadImage("icon_bag", "Images/Interface/icon_bag.bmp", 84, 84, true, RGB(255, 0, 255));
	//_loading->loadImage("icon_hp", "Images/Interface/icon_hp.bmp", 56, 40, true, RGB(255, 0, 255));
	//_loading->loadImage("hp_barBack", "Images/Interface/hp_barBack.bmp", 130, 24, true, RGB(255, 0, 255));
	//_loading->loadImage("hp_barFront", "Images/Interface/hp_barFront.bmp", 130, 24, true, RGB(255, 0, 255));
	//_loading->loadFrameImage("weapon_slot", "Images/Interface/weapon_slot.bmp", 212, 70, 2, 1);
	//_loading->loadFrameImage("weapon_frontIdx", "Images/Interface/weapon_frontIdx.bmp", 60, 30, 2, 1);
	//_loading->loadFrameImage("weapon_backIdx", "Images/Interface/weapon_backIdx.bmp", 60, 30, 2, 1);

	////상점 이미지 초기화 
	//_loading->loadImage("menu_shopInventory", "Images/Shop/menu_shopInventory.bmp", 482, 544, true, RGB(255, 0, 255));
	//_loading->loadImage("menu_display", "Images/Shop/menu_display.bmp", 500, 464, true, RGB(255, 0, 255));
	//_loading->loadImage("arrow_price", "Images/Shop/arrow_price.bmp", 14, 46, true, RGB(255, 0, 255));

	////포션상점 이미지 초기화 
	//_loading->loadImage("shop_nameBase", "Images/PotionShop/shop_nameBase.bmp", 402, 88, true, RGB(255, 0, 255));
	//_loading->loadImage("menu_potion", "Images/PotionShop/menu_potion.bmp", 1124, 680, true, RGB(255, 0, 255));
	//_loading->loadImage("icon_potionKeyE", "Images/PotionShop/icon_potionKeyE.bmp", 68, 144, true, RGB(255, 0, 255));
	//_loading->loadImage("slot_base", "Images/PotionShop/slot_base.bmp", 60, 60, true, RGB(255, 0, 255));
	//_loading->loadImage("shop_bubble", "Images/PotionShop/shop_bubble.bmp", 500, 160, true, RGB(255, 0, 255));
	//_loading->loadFrameImage("shop_ox", "Images/PotionShop/shop_ox.bmp", 76, 60, 2, 1);

	////숫자(흰색) 이미지 초기화 
	//_loading->loadImage("0", "Images/Number/0.bmp", 10, 14, true, RGB(255, 0, 255));
	//_loading->loadImage("1", "Images/Number/1.bmp", 10, 14, true, RGB(255, 0, 255));
	//_loading->loadImage("2", "Images/Number/2.bmp", 10, 14, true, RGB(255, 0, 255));
	//_loading->loadImage("3", "Images/Number/3.bmp", 10, 14, true, RGB(255, 0, 255));
	//_loading->loadImage("4", "Images/Number/4.bmp", 10, 14, true, RGB(255, 0, 255));
	//_loading->loadImage("5", "Images/Number/5.bmp", 10, 14, true, RGB(255, 0, 255));
	//_loading->loadImage("6", "Images/Number/6.bmp", 10, 14, true, RGB(255, 0, 255));
	//_loading->loadImage("7", "Images/Number/7.bmp", 10, 14, true, RGB(255, 0, 255));
	//_loading->loadImage("8", "Images/Number/8.bmp", 10, 14, true, RGB(255, 0, 255));
	//_loading->loadImage("9", "Images/Number/9.bmp", 10, 14, true, RGB(255, 0, 255));

	////숫자(검정) 이미지 초기화 
	//_loading->loadImage("0_black", "Images/Number/0_black.bmp", 10, 14, true, RGB(255, 0, 255));
	//_loading->loadImage("1_black", "Images/Number/1_black.bmp", 10, 14, true, RGB(255, 0, 255));
	//_loading->loadImage("2_black", "Images/Number/2_black.bmp", 10, 14, true, RGB(255, 0, 255));
	//_loading->loadImage("3_black", "Images/Number/3_black.bmp", 10, 14, true, RGB(255, 0, 255));
	//_loading->loadImage("4_black", "Images/Number/4_black.bmp", 10, 14, true, RGB(255, 0, 255));
	//_loading->loadImage("5_black", "Images/Number/5_black.bmp", 10, 14, true, RGB(255, 0, 255));
	//_loading->loadImage("6_black", "Images/Number/6_black.bmp", 10, 14, true, RGB(255, 0, 255));
	//_loading->loadImage("7_black", "Images/Number/7_black.bmp", 10, 14, true, RGB(255, 0, 255));
	//_loading->loadImage("8_black", "Images/Number/8_black.bmp", 10, 14, true, RGB(255, 0, 255));
	//_loading->loadImage("9_black", "Images/Number/9_black.bmp", 10, 14, true, RGB(255, 0, 255));

	////숫자(초록색) 이미지 초기화 
	//_loading->loadImage("0_green", "Images/Number/0_green.bmp", 10, 14, true, RGB(255, 0, 255));
	//_loading->loadImage("1_green", "Images/Number/1_green.bmp", 10, 14, true, RGB(255, 0, 255));
	//_loading->loadImage("2_green", "Images/Number/2_green.bmp", 10, 14, true, RGB(255, 0, 255));
	//_loading->loadImage("3_green", "Images/Number/3_green.bmp", 10, 14, true, RGB(255, 0, 255));
	//_loading->loadImage("4_green", "Images/Number/4_green.bmp", 10, 14, true, RGB(255, 0, 255));
	//_loading->loadImage("5_green", "Images/Number/5_green.bmp", 10, 14, true, RGB(255, 0, 255));
	//_loading->loadImage("6_green", "Images/Number/6_green.bmp", 10, 14, true, RGB(255, 0, 255));
	//_loading->loadImage("7_green", "Images/Number/7_green.bmp", 10, 14, true, RGB(255, 0, 255));
	//_loading->loadImage("8_green", "Images/Number/8_green.bmp", 10, 14, true, RGB(255, 0, 255));
	//_loading->loadImage("9_green", "Images/Number/9_green.bmp", 10, 14, true, RGB(255, 0, 255));
	//_loading->loadImage("x_green", "Images/Number/x_green.bmp", 10, 14, true, RGB(255, 0, 255));
	//_loading->loadImage("0_gray", "Images/Number/0_gray.bmp", 10, 14, true, RGB(255, 0, 255));

	////작은 숫자(흰색) 이미지 초기화 
	//_loading->loadImage("0_white_small", "Images/Number/0_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
	//_loading->loadImage("1_white_small", "Images/Number/1_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
	//_loading->loadImage("2_white_small", "Images/Number/2_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
	//_loading->loadImage("3_white_small", "Images/Number/3_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
	//_loading->loadImage("4_white_small", "Images/Number/4_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
	//_loading->loadImage("5_white_small", "Images/Number/5_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
	//_loading->loadImage("6_white_small", "Images/Number/6_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
	//_loading->loadImage("7_white_small", "Images/Number/7_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
	//_loading->loadImage("8_white_small", "Images/Number/8_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
	//_loading->loadImage("9_white_small", "Images/Number/9_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
	//_loading->loadImage("slash_white_small", "Images/Number/slash_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
}

