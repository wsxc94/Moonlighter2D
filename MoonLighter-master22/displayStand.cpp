#include "stdafx.h"
#include "displayStand.h"

HRESULT displayStand::init()
{
	//클래스 초기화 
	_fadeManager = new fadeManager;
	_fadeManager->init();

	_cursor = new cursor;
	_cursor->init();

	//메뉴 위치 초기화 
	initMenuPos();

	//아이템 구조체 초기화 
	memset(&_itemEmpty, 0, sizeof(_itemEmpty));
	memset(&_itemGrabbed, 0, sizeof(_itemGrabbed));
	memset(&_displayItem, 0, sizeof(_displayItem));
	memset(_lastPrice, 0, sizeof(_lastPrice));

	//가격 커서 구조체 초기화 
	_priceCursor.img = IMAGEMANAGER->findImage("arrow_price");
	_priceCursor.posIdx = 0;
	_priceCursor.digit[0] = 1;
	_priceCursor.digit[1] = 10;
	_priceCursor.digit[2] = 100;
	_priceCursor.digit[3] = 1000;
	_priceCursor.digit[4] = 10000;
	_priceCursor.digit[5] = 100000;
	_priceCursor.digit[6] = 1000000;

	//상점 슬롯 초기화 
	initShopSlot();
	_shopCtrl = CTRL_INVENTORY;

	//메뉴 열기 및 닫기 관련 변수 
	_menuMoveSpeed = 90.f;
	_menuOn = false;
	_openMenu = false;
	_closeMenu = false;

	//아이템 잡기 관련 변수 
	_canGrab = true;
	_isGrabbingItem = false;
	_isPuttingItem = false;
	_grabSoundPlayed = false;
	_grabTime = 12;

	return S_OK;
}

void displayStand::release()
{
	for (int i = 0; i < _vShopInven.size(); i++)
	{
		SAFE_DELETE(_vShopInven[i]);
	}

	_fadeManager->release();
	SAFE_DELETE(_fadeManager);

	_cursor->release();
	SAFE_DELETE(_cursor);
}

void displayStand::update()
{
	_fadeManager->update();

	//아이템 메뉴가 열려있지 않을 때만 키값 받기 
	if (!ITEMMENU->getOpenMenu())
	{
		//메뉴 열고 닫기 함수 
		//toggleMenu();
		openMenu();
		closeMenu();

		if (_menuOn)
		{
			//메뉴 닫기 함수 
			closeDisplayStand();

			//키 입력 함수 
			keyInput();

			//현재 디스플레이중인 아이템 업데이트 
			//getDisplayItem();

			//커서 애니메이션 함수 
			_cursor->update();
		}
	}
}

void displayStand::render()
{
	_fadeManager->render(getMemDC());

	if (_menuOn)
	{
		menuRender();

		if (_cursor->getShowCursor())
		{
			itemRender();
			itemNameRender();

			if (_itemGrabbed.getType() != ITEM_EMPTY) itemGrabbedRender();

			displayCountRender();
			unitPriceRender();
			totalPriceRender();
			cursorRender();

			//현재 가격책정 모드라면 가격 커서 렌더 
			if (_shopCtrl == CTRL_PRICE) priceCursorRender();
		}
	}

	//char str[100];

	//wsprintf(str, "itemMenuCanKeyInput : %d", ITEMMENU->getCanKeyInput());
	//TextOut(getMemDC(), 10, 90, str, strlen(str));

	/*wsprintf(str, "invenSize : %d", _vShopInven.size());
	TextOut(getMemDC(), 10, 90, str, strlen(str));*/

	//for (int i = 0; i < 4; i++)
	//{
	//	if (_displayItem[i].getType() == ITEM_EMPTY) continue;

	//	/*wsprintf(str, "displayItem[%d] : %d", i, _displayItem[i].getType());
	//	TextOut(getMemDC(), 10, 130 + (i * 25), str, strlen(str));*/

	//	_displayItem[i].getItemImg()->render(getMemDC(), 10, 130 + (i * 25));
	//}
}

void displayStand::openDisplayStand()
{
	//메뉴가 현재 열려있는 상태거나 열기 및 닫기 중이라면 함수 종료 
	if (_menuOn || _openMenu || _closeMenu) return;

	_menuOn = true;
	_openMenu = true;
	PLAYER->setDisplayOn(true);

	//인벤토리 슬롯값 초기화 
	initInvenSlot();

	//인벤토리 아이템 초기화 
	initInvenItem();

	//아이템메뉴의 키입력을 받지 않는다.(메뉴의 키값이 서로 충돌하기 때문)
	ITEMMENU->setCanKeyInput(false);
	_fadeManager->fadeInit(16, FADE_OUT, 205);
	SOUNDMANAGER->play("openInven", 0.4f);
}

void displayStand::closeDisplayStand()
{
	if (INPUT->GetKeyDown('I'))
	{
		//현재 메뉴가 닫힌 상태거나 열기 및 닫기 중이라면 함수 종료 
		if (!_menuOn || _openMenu || _closeMenu) return;

		_closeMenu = true;
		_cursor->setShowCursor(false);
		PLAYER->setDisplayOn(false);

		//잡고있던 아이템이 있다면 제자리에 내려놓기
		putGrabbingItem();

		//인벤토리의 아이템 슬롯과 동기화시키기
		ITEMMENU->getInventory()->syncWithShopInven(_vShopInven);

		_fadeManager->fadeInit(16, FADE_IN);

		getDisplayItem();
	}
}

void displayStand::toggleMenu()
{
	if (INPUT->GetKeyDown('O'))
	{
		//현재 메뉴를 열고 있거나 닫고 있는 중이라면 함수 종료 
		if (_openMenu || _closeMenu) return;

		if (_menuOn)
		{
			_closeMenu = true;
			_cursor->setShowCursor(false);
			PLAYER->setDisplayOn(false);

			//인벤토리의 아이템 슬롯과 동기화시키기
			ITEMMENU->getInventory()->syncWithShopInven(_vShopInven);
		}
		else
		{
			_menuOn = true;
			_openMenu = true;
			PLAYER->setDisplayOn(true);

			//인벤토리 슬롯값 초기화 
			initInvenSlot();

			//인벤토리 아이템 초기화 
			initInvenItem();

			//아이템메뉴의 키입력을 받지 않는다.(메뉴의 키값이 서로 충돌하기 때문)
			ITEMMENU->setCanKeyInput(false);
			SOUNDMANAGER->play("openInven", 0.4f);
		}
	}
}

void displayStand::initMenuPos()
{
	_shopInvenPos.x = SHOPINVENPOSX;
	_shopInvenPos.y = WINSIZEY;

	_displayPos.x = WINSIZEX;
	_displayPos.y = DISPLAYPOSY;
}

void displayStand::openMenu()
{
	if (!_openMenu) return;

	menuMoveUp(&_shopInvenPos, SHOPINVENPOSY);
	menuMoveLeft(&_displayPos, DISPLAYPOSX);

	if (_shopInvenPos.y == SHOPINVENPOSY && _displayPos.x == DISPLAYPOSX)
	{
		_openMenu = false;
		_cursor->setShowCursor(true);
		_cursor->setCursorState(CURSOR_MOVE);
		initShopInven();
	}
}

void displayStand::closeMenu()
{
	if (!_closeMenu) return;

	menuMoveDown(&_shopInvenPos, WINSIZEY);
	menuMoveRight(&_displayPos, WINSIZEX);

	if (_shopInvenPos.y == WINSIZEY && _displayPos.x == WINSIZEX)
	{
		_closeMenu = false;
		_menuOn = false;

		//다시 아이템 메뉴의 키값을 입력받도록 함. 
		ITEMMENU->setCanKeyInput(true);
	}
}

void displayStand::menuMoveLeft(POINT * pos, const int destPos)
{
	pos->x -= _menuMoveSpeed;

	if (pos->x < destPos) pos->x = destPos;
}

void displayStand::menuMoveRight(POINT * pos, const int destPos)
{
	pos->x += _menuMoveSpeed;

	if (pos->x > destPos) pos->x = destPos;
}

void displayStand::menuMoveUp(POINT * pos, const int destPos)
{
	pos->y -= _menuMoveSpeed;

	if (pos->y < destPos) pos->y = destPos;
}

void displayStand::menuMoveDown(POINT * pos, const int destPos)
{
	pos->y += _menuMoveSpeed;

	if (pos->y > destPos) pos->y = destPos;
}

void displayStand::initShopInven()
{
	_isGrabbingItem = false;
	_isPuttingItem = false;
	_grabSoundPlayed = false;

	initInvenSlot();
	initInvenItem();

	_cursor->setSlotIdx(0);
	setShopCtrl(CTRL_INVENTORY);
	putGrabbingItem();
}

void displayStand::initShopSlot()
{
	memset(_shopSlot, 0, sizeof(_shopSlot));

	for (int i = 0; i < MAXSHOPSLOT; i++)
	{
		_shopSlot[i].slotIdx = i;
		_shopSlot[i].isEmpty = true;

		switch (_shopSlot[i].slotType)
		{
		case 0: case 1: case 2: case 3: case 4:
		case 7: case 8: case 9: case 10: case 11:
		case 14: case 15: case 16: case 17: case 18:
		case 21: case 22: case 23: case 24: case 25:
			_shopSlot[i].slotType = SLOT_INVEN;
			break;

		case 5: case 6: case 19: case 20:
			_shopSlot[i].slotType = SLOT_DISPLAY;

		case 12: case 13: case 26: case 27:
			_shopSlot[i].slotType = SLOT_PRICE;
		}
	}//end of for 
}

void displayStand::initInvenSlot()
{
	for (int i = 0; i < MAXSHOPSLOT; i++)
	{
		switch (i)
		{
			//진열 아이템 슬롯
			case 5: case 6: case 19: case 20:
				break;

			//진열 아이템 가격 슬롯 
			case 12: case 13: case 26: case 27:
				break;

			//인벤토리 슬롯 
			default:
				_shopSlot[i].isEmpty = true;
				break;
		}
	}//end of for 
}

void displayStand::initInvenItem()
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
			case 28: case 29: case 30:
				break;

			default:
				gameItem *item = new gameItem;
				item->init(ITEMMENU->getInventory()->getItem()[i]);
				_vShopInven.push_back(item);

				_shopSlot[item->getInvenPosIdx()].isEmpty = false;
				break;
		}
	}//end of for 
}

void displayStand::deleteInvenItems()
{
	for (int i = 0; i < _vShopInven.size(); )
	{
		switch (_vShopInven[i]->getInvenPosIdx())
		{
			case 5: case 6: case 12: case 13:
			case 19: case 20: case 26: case 27:
				i++;
				break;

			default:
				SAFE_DELETE(_vShopInven[i]);
				_vShopInven.erase(_vShopInven.begin() + i);
				break;
		}
	}
}

gameItem displayStand::findItemByIdx(int index)
{
	for (int i = 0; i < _vShopInven.size(); i++)
	{
		if (_vShopInven[i]->getInvenPosIdx() != index) continue;

		if (_vShopInven[i]->getPrice() == 0) return _itemEmpty;
		else
		{
			return *_vShopInven[i];
		}
	}

	return _itemEmpty;
}

gameItem * displayStand::getDisplayItem()
{
	for (int i = 0; i < MAXSHOPSLOT; i++)
	{
		switch (_shopSlot[i].slotIdx)
		{
			case 5:
				_displayItem[0] = findItemByIdx(_shopSlot[i].slotIdx);
				break;

			case 6:
				_displayItem[1] = findItemByIdx(_shopSlot[i].slotIdx);
				break;

			case 19:
				_displayItem[2] = findItemByIdx(_shopSlot[i].slotIdx);
				break;

			case 20:
				_displayItem[3] = findItemByIdx(_shopSlot[i].slotIdx);
				break;
		}//end of switch 
	}//end of for 

	return _displayItem;
}

void displayStand::deleteDisplayItem(int index)
{
	switch (index)
	{
		case 0:
			deleteItemByIdx(5);
			_shopSlot[5].isEmpty = true; 
			break;

		case 1:
			deleteItemByIdx(6);
			_shopSlot[6].isEmpty = true;

			break;

		case 2:
			deleteItemByIdx(19);
			_shopSlot[19].isEmpty = true;
			break;

		case 3:
			deleteItemByIdx(20);
			_shopSlot[20].isEmpty = true;
			break; 
	}
}

void displayStand::deleteItemByIdx(int index)
{
	for (int i = 0; i < _vShopInven.size(); i++)
	{
		if (_vShopInven[i]->getInvenPosIdx() != index) continue; 

		SAFE_DELETE(_vShopInven[i]);
		_vShopInven.erase(_vShopInven.begin() + i);
		return; 
	}
}

void displayStand::setShopCtrl(SHOP_CTRL state)
{
	switch (_shopCtrl)
	{
		case CTRL_INVENTORY:
			_shopCtrl = state;
			break;

		case CTRL_PRICE:
			_priceCursor.posIdx = 0;
			_shopCtrl = state;
			break;
	}
}

void displayStand::keyInput()
{
	switch (_shopCtrl)
	{
		case CTRL_INVENTORY:
			invenKeyInput();
			break;

		case CTRL_PRICE:
			priceKeyInput();
			break;
	}//end of for 

	switch (_cursor->getSlotIdx())
	{
		case 12: case 13: case 26: case 27:
			if (_shopCtrl == CTRL_INVENTORY) _cursor->setCursorState(CURSOR_PRICE_OFF);
			else _cursor->setCursorState(CURSOR_PRICE_ON);
			break;

	}//end of switch 
}

void displayStand::invenKeyInput()
{
	//상하좌우 키 입력 받아서 커서 움직이기 
	//커서가 움직일 때마다 커서 애니메이션 실행 
	if (INPUT->GetKeyDown('W'))
	{
		upKeyDown();
		_cursor->setCursorState(CURSOR_MOVE);
	}
	if (INPUT->GetKeyDown('S'))
	{
		downKeyDown();
		_cursor->setCursorState(CURSOR_MOVE);
	}
	if (INPUT->GetKeyDown('A'))
	{
		leftKeyDown();
		_cursor->setCursorState(CURSOR_MOVE);
	}
	if (INPUT->GetKeyDown('D'))
	{
		rightKeyDown();
		_cursor->setCursorState(CURSOR_MOVE);
	}

	//커서의 위치가 가격조정판에 있냐 아니냐에 따라서 키 입력 달리 받기 
	switch (_cursor->getSlotIdx())
	{
		//커서의 위치가 가격조정판에 있을 때 j키 입력시 가격 입력 컨트롤러로 변경 
		case 12: case 13: case 26: case 27:
			if (INPUT->GetKeyDown('J'))
			{
				setShopCtrl(CTRL_PRICE);
			}
			break;

			//그외 모든 상황에서는 아이템 잡기 및 놓기 실행
		default:
			if (INPUT->GetKey('J'))
			{
				_cursor->setClickTime(_cursor->getClickTime() + 1);

				//꾹 누르고 있으면 한꺼번에 잡기 실행 
				if (_cursor->getClickTime() >= _grabTime && !_isPuttingItem)
				{
					grabItem();
				}
			}
			if (INPUT->GetKeyDown('J'))
			{
				_cursor->setCursorState(CURSOR_CLICK);
				putItem();
			}
			if (INPUT->GetKeyUp('J'))
			{
				//한꺼번에 잡기를 실행할 정도로 길게 누르지 않고 손을 뗄 경우
				//1개씩 아이템을 잡을 수 있도록 함수 실행 
				if (!_isPuttingItem) grabItem();

				_canGrab = true;
				_isPuttingItem = false;
				_grabSoundPlayed = false;
				_cursor->setClickTime(0);
			}
			break;

	}//end of switch 
}

void displayStand::priceKeyInput()
{
	//좌우 키 입력으로 가격 커서 좌우로 이동하기 
	if (INPUT->GetKeyDown('A'))
	{
		if (_priceCursor.posIdx < 6) _priceCursor.posIdx++;
		SOUNDMANAGER->play("cursor_move", 0.2f);
	}
	if (INPUT->GetKeyDown('D'))
	{
		if (_priceCursor.posIdx > 0) _priceCursor.posIdx--;
		SOUNDMANAGER->play("cursor_move", 0.2f);
	}

	//상하 키 입력으로 가격 변경하기  
	if (INPUT->GetKeyDown('W'))
	{
		setPriceUp();
		SOUNDMANAGER->play("cursor_move", 0.2f);
	}
	if (INPUT->GetKeyDown('S'))
	{
		setPriceDown();
		SOUNDMANAGER->play("cursor_move", 0.2f);
	}

	//가격 확정하기(확정 후 컨트롤러 인벤토리로 변경)
	if (INPUT->GetKeyDown('J'))
	{
		saveLastPrice();
		setShopCtrl(CTRL_INVENTORY);
		SOUNDMANAGER->play("cursor_move", 0.2f);
	}
}

void displayStand::leftKeyDown()
{
	//왼쪽 키 입력을 받을 때 커서의 인덱스 변경
	//슬롯의 맨 왼쪽에 있을 경우 왼쪽 키를 입력하면 슬롯의 맨 오른쪽으로 이동하므로 
	//switch문을 통해 해당 케이스에 별도의 명령을 실행. 

	switch (_cursor->getSlotIdx())
	{
		case 0:
			_cursor->setSlotIdx(6);
			break;

		case 7:
			//현재 아이템을 잡고있지 않고, 6번 슬롯에 아이템이 있을 때 
			if (!_isGrabbingItem && !_shopSlot[6].isEmpty) _cursor->setSlotIdx(13);
			else _cursor->setSlotIdx(6);
			break;

		case 13:
			//현재 아이템을 잡고있지 않고, 5번 슬롯에 아이템이 있을 때 
			if (!_isGrabbingItem && !_shopSlot[5].isEmpty) _cursor->setSlotIdx(12);
			else _cursor->setSlotIdx(5);
			break;

		case 14:
			_cursor->setSlotIdx(20);
			break;

		case 21:
			//현재 아이템을 잡고있지 않고, 20번 슬롯에 아이템이 있을 때 
			if (!_isGrabbingItem && !_shopSlot[20].isEmpty) _cursor->setSlotIdx(27);
			else _cursor->setSlotIdx(20);
			break;

		case 27:
			//현재 아이템을 잡고있지 않고, 19번 슬롯에 아이템이 있을 때 
			if (!_isGrabbingItem && !_shopSlot[19].isEmpty) _cursor->setSlotIdx(26);
			else _cursor->setSlotIdx(19);
			break;

		default:
			_cursor->setSlotIdx(_cursor->getSlotIdx() - 1);
			break;
	}
}

void displayStand::rightKeyDown()
{
	switch (_cursor->getSlotIdx())
	{
		case 6:
			_cursor->setSlotIdx(0);
			break;

		case 11:
			//현재 아이템을 잡고있지 않고, 5번 슬롯에 아이템이 있을 때 
			if (!_isGrabbingItem && !_shopSlot[5].isEmpty) _cursor->setSlotIdx(12);
			else _cursor->setSlotIdx(5);
			break;

		case 12:
			//현재 아이템을 잡고있지 않고, 6번 슬롯에 아이템이 있을 때 
			if (!_isGrabbingItem && !_shopSlot[6].isEmpty) _cursor->setSlotIdx(13);
			else _cursor->setSlotIdx(6);
			break;

		case 13:
			_cursor->setSlotIdx(7);
			break;

		case 20:
			_cursor->setSlotIdx(14);
			break;

		case 25:
			//현재 아이템을 잡고있지 않고, 19번 슬롯에 아이템이 있을 때 
			if (!_isGrabbingItem && !_shopSlot[19].isEmpty) _cursor->setSlotIdx(26);
			else _cursor->setSlotIdx(19);
			break;

		case 26:
			//현재 아이템을 잡고있지 않고, 20번 슬롯에 아이템이 있을 때 
			if (!_isGrabbingItem && !_shopSlot[20].isEmpty) _cursor->setSlotIdx(27);
			else _cursor->setSlotIdx(20);
			break;

		case 27:
			_cursor->setSlotIdx(21);
			break;

		default:
			_cursor->setSlotIdx(_cursor->getSlotIdx() + 1);
			break;
	}
}

void displayStand::upKeyDown()
{
	switch (_cursor->getSlotIdx())
	{
		case 0: case 1: case 2: case 3: case 4:
			_cursor->setSlotIdx(_cursor->getSlotIdx() + 21);
			break;

		case 5:
			//현재 아이템을 잡고있지 않고, 19번 슬롯에 아이템이 있을 때 
			if (!_isGrabbingItem && !_shopSlot[19].isEmpty) _cursor->setSlotIdx(26);
			else _cursor->setSlotIdx(19);
			break;

		case 6:
			//현재 아이템을 잡고있지 않고, 20번 슬롯에 아이템이 있을 때 
			if (!_isGrabbingItem && !_shopSlot[20].isEmpty) _cursor->setSlotIdx(27);
			else _cursor->setSlotIdx(20);
			break;

		case 19:
			//현재 아이템을 잡고있지 않고, 5번 슬롯에 아이템이 있을 때 
			if (!_isGrabbingItem && !_shopSlot[5].isEmpty) _cursor->setSlotIdx(12);
			else _cursor->setSlotIdx(5);
			break;

		case 20:
			//현재 아이템을 잡고있지 않고, 6번 슬롯에 아이템이 있을 때 
			if (!_isGrabbingItem && !_shopSlot[6].isEmpty) _cursor->setSlotIdx(13);
			else _cursor->setSlotIdx(6);
			break;

		default:
			_cursor->setSlotIdx(_cursor->getSlotIdx() - 7);
			break;
	}
}

void displayStand::downKeyDown()
{
	switch (_cursor->getSlotIdx())
	{
		case 5:
			//현재 아이템을 잡고있지 않고, 5번 슬롯에 아이템이 있을 때 
			if (!_isGrabbingItem && !_shopSlot[5].isEmpty) _cursor->setSlotIdx(12);
			else _cursor->setSlotIdx(19);
			break;

		case 6:
			//현재 아이템을 잡고있지 않고, 6번 슬롯에 아이템이 있을 때 
			if (!_isGrabbingItem && !_shopSlot[6].isEmpty) _cursor->setSlotIdx(13);
			else _cursor->setSlotIdx(20);
			break;

		case 19:
			//현재 아이템을 잡고있지 않고, 19번 슬롯에 아이템이 있을 때 
			if (!_isGrabbingItem && !_shopSlot[19].isEmpty) _cursor->setSlotIdx(26);
			else _cursor->setSlotIdx(5);
			break;

		case 20:
			//현재 아이템을 잡고있지 않고, 20번 슬롯에 아이템이 있을 때 
			if (!_isGrabbingItem && !_shopSlot[20].isEmpty) _cursor->setSlotIdx(27);
			else _cursor->setSlotIdx(6);
			break;

		case 21: case 22: case 23: case 24: case 25: case 26: case 27:
			_cursor->setSlotIdx(_cursor->getSlotIdx() - 21);
			break;

		default:
			_cursor->setSlotIdx(_cursor->getSlotIdx() + 7);
			break;
	}
}

void displayStand::grabItem()
{
	if (!_canGrab) return; 

	//현재 잡고 있는 아이템이 있거나 빈슬롯일 경우 함수 종료 
	if (_shopSlot[_cursor->getSlotIdx()].isEmpty && !_isGrabbingItem)
	{
		if (!_grabSoundPlayed)
		{
			_grabSoundPlayed = true;
			SOUNDMANAGER->play("cursor_move", 0.2f);
		}
		return;
	}
	if (_isGrabbingItem) return;

	//클릭하고 있는 시간이 길면 한꺼번에 잡기 
	if (_cursor->getClickTime() >= _grabTime)
	{
		_isGrabbingItem = true;
		grabAllItem();
		SOUNDMANAGER->play("cursor_pick", 0.4f);
	}
	else //짧다면 1개씩 잡기 
	{
		_isGrabbingItem = true;
		grabOneItem();
		SOUNDMANAGER->play("cursor_pick", 0.4f);
	}
}

void displayStand::grabOneItem()
{
	//커서가 가리키는 인벤토리 인덱스와 일치하는 아이템을 itemGrabbed에 담는다.
	for (int i = 0; i < _vShopInven.size(); i++)
	{
		if (_vShopInven[i]->getInvenPosIdx() != _cursor->getSlotIdx()) continue;

		//아이템을 한 개만 집을테니까 아이템 클래스를 복사해주되, 개수는 1개로 수정한다.
		_itemGrabbed = *(_vShopInven[i]);
		_itemGrabbed.setCount(1);

		//담은 아이템의 개수만큼 인벤토리 인덱스의 아이템 카운트를 빼준다. 
		_vShopInven[i]->setCount(_vShopInven[i]->getCount() - 1);

		//슬롯에 있는 아이템이 0보다 작아질 경우 해당 데이터는 삭제해준다. 
		if (_vShopInven[i]->getCount() <= 0)
		{
			_shopSlot[_cursor->getSlotIdx()].isEmpty = true;
			SAFE_DELETE(_vShopInven[i]);
			_vShopInven.erase(_vShopInven.begin() + i);
			return;
		}
	}
}

void displayStand::grabAllItem()
{
	//커서가 가리키는 인벤토리 인덱스와 일치하는 아이템을 itemGrabbed에 담는다.
	for (int i = 0; i < _vShopInven.size(); i++)
	{
		if (_vShopInven[i]->getInvenPosIdx() != _cursor->getSlotIdx()) continue;

		//아이템을 한꺼번에 잡을 것이므로 개수 그대로 아이템 클래스를 복사해준다. 
		_itemGrabbed = *(_vShopInven[i]);

		//아이템을 한꺼번에 잡은 것이므로 슬롯에 있던 아이템은 삭제 
		_shopSlot[_cursor->getSlotIdx()].isEmpty = true;
		SAFE_DELETE(_vShopInven[i]);
		_vShopInven.erase(_vShopInven.begin() + i);
		return;
	}
}

void displayStand::grabOneMoreItem()
{
	//담은 아이템의 개수만큼 인벤토리 인덱스의 아이템 카운트를 빼준다. 
	for (int i = 0; i < _vShopInven.size(); i++)
	{
		if (_vShopInven[i]->getInvenPosIdx() != _cursor->getSlotIdx()) continue;

		//만약 잡은 아이템과 슬롯의 아이템이 다르다면 바꿔치기하기 
		if (_itemGrabbed.getItemIdx() != _vShopInven[i]->getItemIdx())
		{
			SOUNDMANAGER->play("cursor_drop", 0.4f);

			gameItem *item = new gameItem;
			item->init(&_itemGrabbed);
			item->setInvenPosIdx(_cursor->getSlotIdx());

			_itemGrabbed = *(_vShopInven[i]);
			SAFE_DELETE(_vShopInven[i]);
			_vShopInven.erase(_vShopInven.begin() + i);
			_vShopInven.push_back(item);
			return;
		}
		else
		{
			//이미 슬롯의 소지개수가 최대개수이거나, 둘을 더한 값이 최대 개수를 초과할 때 스위칭
			if (_vShopInven[i]->getCount() == _vShopInven[i]->getMaxCount() ||
				_vShopInven[i]->getCount() + _itemGrabbed.getCount() > _vShopInven[i]->getMaxCount())
			{
				SOUNDMANAGER->play("cursor_drop", 0.4f);

				gameItem *item = new gameItem;
				item->init(&_itemGrabbed);
				item->setInvenPosIdx(_cursor->getSlotIdx());

				_itemGrabbed = *(_vShopInven[i]);
				SAFE_DELETE(_vShopInven[i]);
				_vShopInven.erase(_vShopInven.begin() + i);
				_vShopInven.push_back(item);
				return;
			}

			SOUNDMANAGER->play("cursor_pick", 0.4f);

			//기존에 쥐고 있는 아이템과 같은 종류의 아이템을 하나 더 잡는다. 
			_itemGrabbed.plusOneCount();
			_vShopInven[i]->minusOneCount();

			//슬롯에 있는 아이템이 0보다 작아질 경우 해당 데이터는 삭제해준다. 
			if (_vShopInven[i]->getCount() <= 0)
			{
				//기존에 선택했던 슬롯이 사라졌으므로 이제 잡고있는 아이템의 위치값은
				//현재 가리키고 있는 슬롯의 위치값으로 수정한다.

				_itemGrabbed.setInvenPosIdx(_cursor->getSlotIdx());
				_shopSlot[_cursor->getSlotIdx()].isEmpty = true;
				SAFE_DELETE(_vShopInven[i]);
				_vShopInven.erase(_vShopInven.begin() + i);
			}
			return;
		}
	}//end of for 
}

void displayStand::putItem()
{
	//잡고 있는 아이템이 없는 경우 함수 종료 
	if (!_isGrabbingItem) return;

	//빈 슬롯은 클릭했을 때 해당 슬롯에 잡고있던 아이템을 놓는다.
	if (_shopSlot[_cursor->getSlotIdx()].isEmpty)
	{
		_isPuttingItem = true;
		putItemOnEmptySlot();
		loadLastPrice(_cursor->getSlotIdx());
		_isGrabbingItem = false;
		SOUNDMANAGER->play("cursor_drop", 0.4f);
	}
	else //슬롯이 비어있지 않을 때 
	{
		//1.처음 아이템을 잡았던 위치를 다시 클릭했을 때 아이템을 한 개씩 잡는다.  
		if (_itemGrabbed.getInvenPosIdx() == _cursor->getSlotIdx())
		{
			_isPuttingItem = true;
			grabOneMoreItem();
		}
		else //2.처음 아이템을 잡은 위치가 아닐 때 
		{
			_isPuttingItem = true;
			putItemOnOccupiedSlot();
		}
	}
}

void displayStand::putItemOnEmptySlot()
{
	//새 게임아이템을 생성해서 현재 잡고 있는 아이템을 복사하고 
	//해당 게임아이템을 현재 커서가 가리키고 있는 위치로 설정하여 푸시백한다. 
	gameItem *item = new gameItem;
	item->init(&_itemGrabbed);

	item->setInvenPosIdx(_cursor->getSlotIdx());
	_vShopInven.push_back(item);
	_shopSlot[_cursor->getSlotIdx()].isEmpty = false;

	_itemGrabbed = _itemEmpty;
}

void displayStand::putItemOnOccupiedSlot()
{
	for (int i = 0; i < _vShopInven.size(); i++)
	{
		if (_vShopInven[i]->getInvenPosIdx() != _cursor->getSlotIdx()) continue;

		//1.내가 현재 잡고 있는 아이템과 슬롯에 있는 아이템의 종류가 같을 때 
		if (_itemGrabbed.getItemIdx() == _vShopInven[i]->getItemIdx())
		{
			SOUNDMANAGER->play("cursor_drop", 0.4f);

			//이미 슬롯의 소지개수가 최대개수일 때는 스위칭
			if (_vShopInven[i]->getCount() == _vShopInven[i]->getMaxCount())
			{
				gameItem *item = new gameItem;
				item->init(&_itemGrabbed);
				item->setInvenPosIdx(_cursor->getSlotIdx());

				_itemGrabbed = *(_vShopInven[i]);
				SAFE_DELETE(_vShopInven[i]);
				_vShopInven.erase(_vShopInven.begin() + i);
				_vShopInven.push_back(item);
				return;
			}

			//현재 잡고 있는 아이템을 슬롯에 합치는데, 그 과정에서 슬롯에 소지 가능한
			//최대 아이템 개수를 넘길 경우 최대 개수까지 슬롯에 저장하고, 나머지는 다시 잡는다. 
			int maxCount = _itemGrabbed.getMaxCount();
			int sum = _itemGrabbed.getCount() + _vShopInven[i]->getCount();

			if (sum > maxCount) //최대 소지 개수를 넘겼을 때 
			{
				_itemGrabbed.setCount(_itemGrabbed.getCount() - (maxCount - _vShopInven[i]->getCount()));
				_vShopInven[i]->setCount(maxCount);
				_itemGrabbed.setInvenPosIdx(i);
				return;
			}
			else //최대 소지 개수를 넘기지 않았을 때 
			{
				_vShopInven[i]->setCount(sum);
				_itemGrabbed = _itemEmpty;
				_isGrabbingItem = false;
				return;
			}
		}
		else //2.내가 현재 잡고 있는 아이템과 슬롯에 있는 아이템 종류가 다를 떄 
		{
			SOUNDMANAGER->play("cursor_drop", 0.4f);

			//내가 아이템을 잡았던 슬롯자리가 비어있는 경우
			if (_shopSlot[_itemGrabbed.getInvenPosIdx()].isEmpty)
			{
				//현재 잡고있는 아이템은 슬롯자리로 보내고, 
				//슬롯자리에 있던 아이템은 현재 잡고있는 아이템이 가리키는 인덱스로 보낸다. 
				gameItem *item = new gameItem;
				item->init(&_itemGrabbed);
				item->setInvenPosIdx(_cursor->getSlotIdx());
				_vShopInven.push_back(item);

				_vShopInven[i]->setInvenPosIdx(_itemGrabbed.getInvenPosIdx());
				_shopSlot[_itemGrabbed.getInvenPosIdx()].isEmpty = false;

				_itemGrabbed = _itemEmpty;
				_isGrabbingItem = false;
				loadLastPrice(_cursor->getSlotIdx());
				return;
			}
			else //비어있지 않은 경우 현재 들고있는 아이템과 슬롯에 있는 아이템 바꿔치기 
			{
				gameItem *item = new gameItem;
				item->init(&_itemGrabbed);
				item->setInvenPosIdx(_cursor->getSlotIdx());

				_itemGrabbed = *(_vShopInven[i]);
				SAFE_DELETE(_vShopInven[i]);
				_vShopInven.erase(_vShopInven.begin() + i);
				_vShopInven.push_back(item);
				loadLastPrice(_cursor->getSlotIdx());
				//push_back을 해주고나서 또 for문을 돌면 중복실행이 될 수 있으므로
				//반드시 return하여 함수를 종료시켜준다. 
				return;
			}
		}
	}//end of for 
}

void displayStand::putGrabbingItem()
{
	//잡고 있는 아이템이 있다면 원래 자리로 돌려놓기 
	if (!_isGrabbingItem) return;

	//내가 아이템을 잡은 슬롯의 자리가 비어있다면
	if (_shopSlot[_itemGrabbed.getInvenPosIdx()].isEmpty)
	{
		//잡고있는 아이템 복사해서 벡터에 푸시하고 잡은 아이템은 없애기 
		gameItem *item = new gameItem;
		item->init(&_itemGrabbed);
		_vShopInven.push_back(item);

		_shopSlot[item->getInvenPosIdx()].isEmpty = false;
		_itemGrabbed = _itemEmpty;
		_isGrabbingItem = false;
	}
	else // 비어있지 않다면 잡고 있는 아이템의 카운트를 슬롯에 더하기 
	{
		for (int i = 0; i < _vShopInven.size(); i++)
		{
			//현재 잡고 있는 아이템이 원래 있던 자리의 인덱스 찾기 
			if (_vShopInven[i]->getInvenPosIdx() != _itemGrabbed.getInvenPosIdx()) continue;

			_vShopInven[i]->setCount(_vShopInven[i]->getCount() + _itemGrabbed.getCount());

			_itemGrabbed = _itemEmpty;
			_isGrabbingItem = false;
			return;

		}//end of for 
	}
}

void displayStand::setPriceUp()
{
	//현재 커서의 위치가 가격 책정 위치일 때 
	switch (_cursor->getSlotIdx())
	{
		case 12: case 13: case 26: case 27:
			for (int i = 0; i < _vShopInven.size(); i++)
			{
				//현재 커서 위치가 가리키는 아이템 인덱스가 아니면 건너뛰기 
				if (_vShopInven[i]->getInvenPosIdx() != (_cursor->getSlotIdx() - 7)) continue;

				//현재 가격이 맥스가격보다 낮을 때 가격 올리기 
				if (_vShopInven[i]->getPrice() < MAXPRICE)
				{
					_vShopInven[i]->addPrice(_priceCursor.digit[_priceCursor.posIdx]);

					if (_vShopInven[i]->getPrice() > MAXPRICE) _vShopInven[i]->setPrice(MAXPRICE);
				}
			}
			return;
	}//end of switch 
}

void displayStand::setPriceDown()
{
	//현재 커서의 위치가 가격 책정 위치일 때 
	switch (_cursor->getSlotIdx())
	{
		case 12: case 13: case 26: case 27:
			for (int i = 0; i < _vShopInven.size(); i++)
			{
				//현재 커서 위치가 가리키는 아이템 인덱스가 아니면 건너뛰기 
				if (_vShopInven[i]->getInvenPosIdx() != (_cursor->getSlotIdx() - 7)) continue;

				//현재 가격이 맥스가격보다 낮을 때 가격 올리기 
				if (_vShopInven[i]->getPrice() > 0)
				{
					_vShopInven[i]->subPrice(_priceCursor.digit[_priceCursor.posIdx]);

					if (_vShopInven[i]->getPrice() < 0) _vShopInven[i]->setPrice(0);
				}
			}
			return;
	}//end of switch 
}

void displayStand::saveLastPrice()
{
	switch (_cursor->getSlotIdx())
	{
		case 12: case 13: case 26: case 27:
			for (int i = 0; i < _vShopInven.size(); i++)
			{
				//현재 커서 위치가 가리키는 아이템 인덱스가 아니면 건너뛰기 
				if (_vShopInven[i]->getInvenPosIdx() != (_cursor->getSlotIdx() - 7)) continue;

				//현재 책정한 가격을 마지막 가격으로 저장하기 
				_lastPrice[_vShopInven[i]->getItemIdx()] = _vShopInven[i]->getPrice();
			}
			return;
	}//end of switch 
}

void displayStand::loadLastPrice(int index)
{
	for (int i = 0; i < _vShopInven.size(); i++)
	{
		if (_vShopInven[i]->getInvenPosIdx() != index) continue;

		_vShopInven[i]->setPrice(_lastPrice[_vShopInven[i]->getItemIdx()]);
	}
}

void displayStand::menuRender()
{
	//인벤토리 메뉴 렌더 
	IMAGEMANAGER->render("menu_shopInventory", getMemDC(), _shopInvenPos.x, _shopInvenPos.y);

	//가판대 메뉴 렌더 
	IMAGEMANAGER->render("menu_display", getMemDC(), _displayPos.x, _displayPos.y);
}

void displayStand::cursorRender()
{
	for (int i = 0; i < MAXSHOPSLOT; i++)
	{
		int columnIdx = _cursor->getSlotIdx() % 7;
		int rowIdx = _cursor->getSlotIdx() / 7;

		switch (_cursor->getSlotIdx())
		{
			case 0: case 1: case 2: case 3: case 4:
				_cursor->getImg()->frameRender(getMemDC(),
					186 + (columnIdx * 72), 158, _cursor->getIdx(), 0);
				break;

			case 7: case 8: case 9: case 10: case 11:
			case 14: case 15: case 16: case 17: case 18:
			case 21: case 22: case 23: case 24: case 25:
				_cursor->getImg()->frameRender(getMemDC(),
					186 + (columnIdx * 72), 244 + ((rowIdx - 1) * 72), _cursor->getIdx(), 0);
				break;

			case 5:
				_cursor->getImg()->frameRender(getMemDC(), 742, 158, _cursor->getIdx(), 0);
				break;

			case 6:
				_cursor->getImg()->frameRender(getMemDC(), 998, 158, _cursor->getIdx(), 0);
				break;

			case 12:
				_cursor->getImg()->frameRender(getMemDC(), 678, 246, _cursor->getIdx(), 0);
				break;

			case 13:
				_cursor->getImg()->frameRender(getMemDC(), 934, 246, _cursor->getIdx(), 0);
				break;

			case 19:
				_cursor->getImg()->frameRender(getMemDC(), 742, 384, _cursor->getIdx(), 0);
				break;

			case 20:
				_cursor->getImg()->frameRender(getMemDC(), 998, 384, _cursor->getIdx(), 0);
				break;

			case 26:
				_cursor->getImg()->frameRender(getMemDC(), 678, 472, _cursor->getIdx(), 0);
				break;

			case 27:
				_cursor->getImg()->frameRender(getMemDC(), 934, 472, _cursor->getIdx(), 0);
				break;
		}
	}
}

void displayStand::priceCursorRender()
{
	switch (_cursor->getSlotIdx())
	{
		case 12:
			_priceCursor.img->render(getMemDC(), 808 - (_priceCursor.posIdx * 12), 250);
			break;

		case 13:
			_priceCursor.img->render(getMemDC(), 1064 - (_priceCursor.posIdx * 12), 250);
			break;

		case 26:
			_priceCursor.img->render(getMemDC(), 808 - (_priceCursor.posIdx * 12), 476);
			break;

		case 27:
			_priceCursor.img->render(getMemDC(), 1064 - (_priceCursor.posIdx * 12), 476);
			break;
	}
}

void displayStand::itemRender()
{
	for (int i = 0; i < MAXSHOPSLOT; i++)
	{
		//비어있는 슬롯은 건너뛰기 
		if (_shopSlot[i].isEmpty) continue;

		//비어있지 않은 슬롯은 인벤토리 벡터에서 해당 슬롯의 인덱스와 일치하는 아이템을 출력 
		for (int j = 0; j < _vShopInven.size(); j++)
		{
			//인벤토리 벡터에서 슬롯의 인덱스와 일치하지 않는 아이템은 건너뛰기 
			if (_vShopInven[j]->getInvenPosIdx() != i) continue;

			int columnIdx = _vShopInven[j]->getInvenPosIdx() % 7;
			int rowIdx = _vShopInven[j]->getInvenPosIdx() / 7;

			switch (_vShopInven[j]->getInvenPosIdx())
			{
				case 0: case 1: case 2: case 3: case 4:
					_vShopInven[j]->getItemImg()->render(getMemDC(),
						200 + (columnIdx * 72), 172);
					countRender(_vShopInven[j]->getCount(),
						240 + (columnIdx * 72), 208, COLOR_BLACK);
					break;

				case 7: case 8: case 9: case 10: case 11:
				case 14: case 15: case 16: case 17: case 18:
				case 21: case 22: case 23: case 24: case 25:
					_vShopInven[j]->getItemImg()->render(getMemDC(),
						200 + (columnIdx * 72), 258 + ((rowIdx - 1) * 72));
					countRender(_vShopInven[j]->getCount(),
						240 + (columnIdx * 72), 294 + ((rowIdx - 1) * 72), COLOR_BLACK);
					break;

				case 5:
					_vShopInven[j]->getItemImg()->render(getMemDC(), 756, 172);
					countRender(_vShopInven[j]->getCount(), 796, 208, COLOR_WHITE);
					break;

				case 6:
					_vShopInven[j]->getItemImg()->render(getMemDC(), 1012, 172);
					countRender(_vShopInven[j]->getCount(), 1052, 208, COLOR_WHITE);
					break;

				case 19:
					_vShopInven[j]->getItemImg()->render(getMemDC(), 756, 398);
					countRender(_vShopInven[j]->getCount(), 796, 434, COLOR_WHITE);
					break;

				case 20:
					_vShopInven[j]->getItemImg()->render(getMemDC(), 1012, 398);
					countRender(_vShopInven[j]->getCount(), 1052, 434, COLOR_WHITE);
					break;

			}//end of switch
		}//end of for(j)
	}//end of for(i)
}

void displayStand::itemGrabbedRender()
{
	//잡은 아이템 출력(커서에 기반)
	for (int i = 0; i < MAXSHOPSLOT; i++)
	{
		int columnIdx = _cursor->getSlotIdx() % 7;
		int rowIdx = _cursor->getSlotIdx() / 7;

		switch (_cursor->getSlotIdx())
		{
			case 0: case 1: case 2: case 3: case 4:
				IMAGEMANAGER->render("grabBase", getMemDC(),
					188 + (columnIdx * 72), 90);
				_itemGrabbed.getItemImg()->render(getMemDC(),
					200 + (columnIdx * 72), 100);
				countRender(_itemGrabbed.getCount(), 240 + (columnIdx * 72), 136, COLOR_BLACK);
				break;

			case 7: case 8: case 9: case 10: case 11:
			case 14: case 15: case 16: case 17: case 18:
			case 21: case 22: case 23: case 24: case 25:
				IMAGEMANAGER->render("grabBase", getMemDC(),
					188 + (columnIdx * 72), 176 + ((rowIdx - 1) * 72));
				_itemGrabbed.getItemImg()->render(getMemDC(),
					200 + (columnIdx * 72), 186 + ((rowIdx - 1) * 72));
				countRender(_itemGrabbed.getCount(),
					240 + (columnIdx * 72), 222 + ((rowIdx - 1) * 72), COLOR_BLACK);
				break;

			case 5:
				IMAGEMANAGER->render("grabBase", getMemDC(), 744, 90);
				_itemGrabbed.getItemImg()->render(getMemDC(), 756, 100);
				countRender(_itemGrabbed.getCount(), 796, 136, COLOR_BLACK);
				break;

			case 6:
				IMAGEMANAGER->render("grabBase", getMemDC(), 1000, 90);
				_itemGrabbed.getItemImg()->render(getMemDC(), 1012, 100);
				countRender(_itemGrabbed.getCount(), 1052, 136, COLOR_BLACK);
				break;

			case 19:
				IMAGEMANAGER->render("grabBase", getMemDC(), 744, 308);
				_itemGrabbed.getItemImg()->render(getMemDC(), 756, 318);
				countRender(_itemGrabbed.getCount(), 796, 354, COLOR_BLACK);
				break;

			case 20:
				IMAGEMANAGER->render("grabBase", getMemDC(), 1000, 308);
				_itemGrabbed.getItemImg()->render(getMemDC(), 1012, 318);
				countRender(_itemGrabbed.getCount(), 1052, 354, COLOR_BLACK);
				break;
		}
	}
}

void displayStand::itemNameRender()
{
	//커서가 가리키는 인벤토리의 슬롯이 비어있는 경우 함수를 종료 
	if (_shopSlot[_cursor->getSlotIdx()].isEmpty) return;

	//커서가 가리키는 인벤토리 인덱스와 일치하는 아이템을 찾는다. 
	for (int i = 0; i < _vShopInven.size(); i++)
	{
		if (_vShopInven[i]->getInvenPosIdx() != _cursor->getSlotIdx()) continue;

		IMAGEMANAGER->render("nameBase", getMemDC(), 468, 630);
		_vShopInven[i]->getNameImg()->render(getMemDC(), 544, 635);
	}
}

void displayStand::displayCountRender()
{
	//비어있을 경우 0개로 출력 
	for (int i = 0; i < MAXSHOPSLOT; i++)
	{
		if (!_shopSlot[i].isEmpty) continue;

		switch (_shopSlot[i].slotIdx)
		{
			case 5:
				greenCountRender(0, 842, 286);
				break;

			case 6:
				greenCountRender(0, 1098, 286);
				break;

			case 19:
				greenCountRender(0, 842, 512);
				break;

			case 20:
				greenCountRender(0, 1098, 512);
				break;
		}
	}

	//비어있지 않은 경우 해당 아이템의 카운트를 출력 
	for (int i = 0; i < _vShopInven.size(); i++)
	{
		switch (_vShopInven[i]->getInvenPosIdx())
		{
		case 5:
			greenCountRender(_vShopInven[i]->getCount(), 842, 286);
			break;

		case 6:
			greenCountRender(_vShopInven[i]->getCount(), 1098, 286);
			break;

		case 19:
			greenCountRender(_vShopInven[i]->getCount(), 842, 512);
			break;

		case 20:
			greenCountRender(_vShopInven[i]->getCount(), 1098, 512);
			break;

		}//end of switch
	}//end of for 
}

void displayStand::unitPriceRender()
{
	//비어있을 경우 0원으로 출력 
	for (int i = 0; i < MAXSHOPSLOT; i++)
	{
		if (!_shopSlot[i].isEmpty) continue;

		switch (_shopSlot[i].slotIdx)
		{
			case 5:
				priceCountRender(0, 810, 266);
				break;

			case 6:
				priceCountRender(0, 1066, 266);
				break;

			case 19:
				priceCountRender(0, 810, 492);
				break;

			case 20:
				priceCountRender(0, 1066, 492);
				break;
		}
	}

	//비어있지 않을 경우 아이템 한 개당 가격을 출력 
	for (int i = 0; i < _vShopInven.size(); i++)
	{
		switch (_vShopInven[i]->getInvenPosIdx())
		{
		case 5:
			priceCountRender(_vShopInven[i]->getPrice(), 810, 266);
			break;

		case 6:
			priceCountRender(_vShopInven[i]->getPrice(), 1066, 266);
			break;

		case 19:
			priceCountRender(_vShopInven[i]->getPrice(), 810, 492);
			break;

		case 20:
			priceCountRender(_vShopInven[i]->getPrice(), 1066, 492);
			break;

		}//end of switch
	}//end of for 
}

void displayStand::totalPriceRender()
{
	//비어있을 경우 0원으로 출력 
	for (int i = 0; i < MAXSHOPSLOT; i++)
	{
		if (!_shopSlot[i].isEmpty) continue;

		switch (_shopSlot[i].slotIdx)
		{
			case 5:
				countRender(0, 842, 316, COLOR_WHITE);
				break;

			case 6:
				countRender(0, 1098, 316, COLOR_WHITE);
				break;

			case 19:
				countRender(0, 842, 542, COLOR_WHITE);
				break;

			case 20:
				countRender(0, 1098, 542, COLOR_WHITE);
				break;
		}
	}

	//비어있지 않은 경우 해당 아이템의 개수와 가격을 곱한 총 가격을 출력 
	for (int i = 0; i < _vShopInven.size(); i++)
	{
		switch (_vShopInven[i]->getInvenPosIdx())
		{
		case 5:
			countRender(_vShopInven[i]->getCount() * _vShopInven[i]->getPrice(),
				842, 316, COLOR_WHITE);
			break;

		case 6:
			countRender(_vShopInven[i]->getCount() * _vShopInven[i]->getPrice(),
				1098, 316, COLOR_WHITE);
			break;

		case 19:
			countRender(_vShopInven[i]->getCount() * _vShopInven[i]->getPrice(),
				842, 542, COLOR_WHITE);
			break;

		case 20:
			countRender(_vShopInven[i]->getCount() * _vShopInven[i]->getPrice(),
				1098, 542, COLOR_WHITE);
			break;

		}//end of switch
	}//end of for 
}

void displayStand::countRender(int count, int destX, int destY, COLOR_TYPE colorIdx)
{
	if (count == 0)
	{
		switch (colorIdx)
		{
			case COLOR_BLACK:
				IMAGEMANAGER->render("0_black", getMemDC(), destX, destY);
				return;

			case COLOR_WHITE:
				IMAGEMANAGER->render("0", getMemDC(), destX, destY);
				return;
		}
	}

	for (int i = 1, distance = 0; i <= count; i *= 10)
	{
		int number = (count / i) % 10;

		char keyName[16];

		switch (colorIdx)
		{
			case COLOR_BLACK:
				wsprintf(keyName, "%d_black", number);
				IMAGEMANAGER->render(keyName, getMemDC(), destX - (distance * 12), destY);
				break;

			case COLOR_WHITE:
				wsprintf(keyName, "%d", number);
				IMAGEMANAGER->render(keyName, getMemDC(), destX - (distance * 12), destY);
				break;
		}

		distance++;

	}//end of for 
}

void displayStand::greenCountRender(int count, int destX, int destY)
{
	if (count == 0)
	{
		IMAGEMANAGER->render("0_green", getMemDC(), destX, destY);
		IMAGEMANAGER->findImage("x_green")->render(getMemDC(), (destX - 12), destY);
		return;
	}

	for (int i = 1, distance = 0; i <= count; i *= 10)
	{
		int number = (count / i) % 10;

		char keyName[16];

		wsprintf(keyName, "%d_green", number);
		IMAGEMANAGER->render(keyName, getMemDC(), destX - (distance * 12), destY);

		distance++;

		if (i >= count) IMAGEMANAGER->findImage("x_green")->render(getMemDC(),
			destX - (distance * 12), destY);

	}//end of for 
}

void displayStand::priceCountRender(int count, int destX, int destY)
{
	//count가 0일 때 출력되는 자리수(7자리)만큼 0 표기 
	if (count == 0)
	{
		for (int i = 1, distance = 0; i <= 1000000; i *= 10)
		{
			IMAGEMANAGER->render("0_gray", getMemDC(), destX - (distance * 12), destY);
			distance++;
		}
		return;
	}

	for (int i = 1, distance = 0; i <= 1000000; i *= 10)
	{
		int number = (count / i) % 10;

		char keyName[16];

		//출력하고자 하는 숫자보다 큰 자리수는 회색의 0으로 채우기 
		if (i > count && number == 0)
		{
			IMAGEMANAGER->render("0_gray", getMemDC(), destX - (distance * 12), destY);
		}
		else
		{
			wsprintf(keyName, "%d_black", number);
			IMAGEMANAGER->render(keyName, getMemDC(), destX - (distance * 12), destY);
		}

		distance++;

	}//end of for 
}

