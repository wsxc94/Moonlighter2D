#include "stdafx.h"
#include "displayStand.h"

displayStand::~displayStand()
{
	for (int i = 0; i < _vShopInven.size(); i++)
	{
		SAFE_DELETE(_vShopInven[i]);
	}

	_fadeManager->release();
	//SAFE_DELETE(_fadeManager);

	_cursor->release();
	//SAFE_DELETE(_cursor);

}

HRESULT displayStand::init()
{
	//Ŭ���� �ʱ�ȭ 
	//_fadeManager = new fadeManager;
	_fadeManager = make_unique<fadeManager>();
	_fadeManager->init();

	//_cursor = new cursor;
	_cursor = make_unique<cursor>();
	_cursor->init();

	//�޴� ��ġ �ʱ�ȭ 
	initMenuPos();

	//������ ����ü �ʱ�ȭ 
	memset(&_itemEmpty, 0, sizeof(_itemEmpty));
	memset(&_itemGrabbed, 0, sizeof(_itemGrabbed));
	memset(&_displayItem, 0, sizeof(_displayItem));
	memset(_lastPrice, 0, sizeof(_lastPrice));

	//���� Ŀ�� ����ü �ʱ�ȭ 
	_priceCursor.img = IMAGEMANAGER->findImage("arrow_price");
	_priceCursor.posIdx = 0;
	_priceCursor.digit[0] = 1;
	_priceCursor.digit[1] = 10;
	_priceCursor.digit[2] = 100;
	_priceCursor.digit[3] = 1000;
	_priceCursor.digit[4] = 10000;
	_priceCursor.digit[5] = 100000;
	_priceCursor.digit[6] = 1000000;

	//���� ���� �ʱ�ȭ 
	initShopSlot();
	_shopCtrl = CTRL_INVENTORY;

	//�޴� ���� �� �ݱ� ���� ���� 
	_menuMoveSpeed = 90.f;
	_menuOn = false;
	_openMenu = false;
	_closeMenu = false;

	//������ ��� ���� ���� 
	_canGrab = true;
	_isGrabbingItem = false;
	_isPuttingItem = false;
	_grabSoundPlayed = false;
	_grabTime = 12;

	return S_OK;
}

void displayStand::release()
{
	//for (int i = 0; i < _vShopInven.size(); i++)
	//{
	//	SAFE_DELETE(_vShopInven[i]);
	//}

	//_fadeManager->release();
 //    //SAFE_DELETE(_fadeManager);

	//_cursor->release();
	////SAFE_DELETE(_cursor);
}

void displayStand::update()
{
	_fadeManager->update();

	//������ �޴��� �������� ���� ���� Ű�� �ޱ� 
	if (!ITEMMENU->getOpenMenu())
	{
		//�޴� ���� �ݱ� �Լ� 
		//toggleMenu();
		openMenu();
		closeMenu();

		if (_menuOn)
		{
			//�޴� �ݱ� �Լ� 
			closeDisplayStand();

			//Ű �Է� �Լ� 
			keyInput();

			//Ŀ�� �ִϸ��̼� �Լ� 
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

			//���� ����å�� ����� ���� Ŀ�� ���� 
			if (_shopCtrl == CTRL_PRICE) priceCursorRender();
		}
	}

	//char str[100];

	//wsprintf(str, "itemMenuCanKeyInput : %d", ITEMMENU->getCanKeyInput());
	//TextOut(getMemDC(), 10, 90, str, strlen(str));
}

void displayStand::openDisplayStand()
{
	//�޴��� ���� �����ִ� ���°ų� ���� �� �ݱ� ���̶�� �Լ� ���� 
	if (_menuOn || _openMenu || _closeMenu) return;

	_menuOn = true;
	_openMenu = true;
	PLAYER->setDisplayOn(true);

	//�κ��丮 ���԰� �ʱ�ȭ 
	initInvenSlot();

	//�κ��丮 ������ �ʱ�ȭ 
	initInvenItem();

	//�����۸޴��� Ű�Է��� ���� �ʴ´�.(�޴��� Ű���� ���� �浹�ϱ� ����)
	ITEMMENU->setCanKeyInput(false);
	_fadeManager->fadeInit(16, FADE_OUT, 205);
	SOUNDMANAGER->play("openInven", 0.4f);

	getDisplayItem();
	ITEMMENU->setInitDisplayStand(true);
}

void displayStand::closeDisplayStand()
{
	if (INPUT->GetKeyDown('I'))
	{
		//���� �޴��� ���� ���°ų� ���� �� �ݱ� ���̶�� �Լ� ���� 
		if (!_menuOn || _openMenu || _closeMenu) return;

		_closeMenu = true;
		_cursor->setShowCursor(false);
		PLAYER->setDisplayOn(false);

		//����ִ� �������� �ִٸ� ���ڸ��� ��������
		putGrabbingItem();

		//�κ��丮�� ������ ���԰� ����ȭ��Ű��
		ITEMMENU->getInventory()->syncWithShopInven(_vShopInven);

		_fadeManager->fadeInit(16, FADE_IN);

		getDisplayItem();
	}
}

void displayStand::toggleMenu()
{
	if (INPUT->GetKeyDown('O'))
	{
		//���� �޴��� ���� �ְų� �ݰ� �ִ� ���̶�� �Լ� ���� 
		if (_openMenu || _closeMenu) return;

		if (_menuOn)
		{
			_closeMenu = true;
			_cursor->setShowCursor(false);
			PLAYER->setDisplayOn(false);

			//�κ��丮�� ������ ���԰� ����ȭ��Ű��
			ITEMMENU->getInventory()->syncWithShopInven(_vShopInven);
		}
		else
		{
			_menuOn = true;
			_openMenu = true;
			PLAYER->setDisplayOn(true);

			//�κ��丮 ���԰� �ʱ�ȭ 
			initInvenSlot();

			//�κ��丮 ������ �ʱ�ȭ 
			initInvenItem();

			//�����۸޴��� Ű�Է��� ���� �ʴ´�.(�޴��� Ű���� ���� �浹�ϱ� ����)
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

		//�ٽ� ������ �޴��� Ű���� �Է¹޵��� ��. 
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

void displayStand::initDisplayStand()
{

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
			//���� ������ ����
			case 5: case 6: case 19: case 20:
				break;

			//���� ������ ���� ���� 
			case 12: case 13: case 26: case 27:
				break;

			//�κ��丮 ���� 
			default:
				_shopSlot[i].isEmpty = true;
				break;
		}
	}//end of for 
}

void displayStand::initInvenItem()
{
	//���� �� �κ��丮 ���Ϳ� �ִ� �������� ���� ����� 
	deleteInvenItems();

	for (int i = 0; i < ITEMMENU->getInventory()->getItem().size(); i++)
	{
		//���� ���Կ� �ִ� �������� �����ϰ� �κ��丮 ���Կ� �ִ� ������ �������� 
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
	//�����¿� Ű �Է� �޾Ƽ� Ŀ�� �����̱� 
	//Ŀ���� ������ ������ Ŀ�� �ִϸ��̼� ���� 
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

	//Ŀ���� ��ġ�� ���������ǿ� �ֳ� �ƴϳĿ� ���� Ű �Է� �޸� �ޱ� 
	switch (_cursor->getSlotIdx())
	{
		//Ŀ���� ��ġ�� ���������ǿ� ���� �� jŰ �Է½� ���� �Է� ��Ʈ�ѷ��� ���� 
		case 12: case 13: case 26: case 27:
			if (INPUT->GetKeyDown('J'))
			{
				setShopCtrl(CTRL_PRICE);
			}
			break;

			//�׿� ��� ��Ȳ������ ������ ��� �� ���� ����
		default:
			if (INPUT->GetKey('J'))
			{
				_cursor->setClickTime(_cursor->getClickTime() + 1);

				//�� ������ ������ �Ѳ����� ��� ���� 
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
				//�Ѳ����� ��⸦ ������ ������ ��� ������ �ʰ� ���� �� ���
				//1���� �������� ���� �� �ֵ��� �Լ� ���� 
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
	//�¿� Ű �Է����� ���� Ŀ�� �¿�� �̵��ϱ� 
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

	//���� Ű �Է����� ���� �����ϱ�  
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

	//���� Ȯ���ϱ�(Ȯ�� �� ��Ʈ�ѷ� �κ��丮�� ����)
	if (INPUT->GetKeyDown('J'))
	{
		saveLastPrice();
		setShopCtrl(CTRL_INVENTORY);
		SOUNDMANAGER->play("cursor_move", 0.2f);
	}
}

void displayStand::leftKeyDown()
{
	//���� Ű �Է��� ���� �� Ŀ���� �ε��� ����
	//������ �� ���ʿ� ���� ��� ���� Ű�� �Է��ϸ� ������ �� ���������� �̵��ϹǷ� 
	//switch���� ���� �ش� ���̽��� ������ ����� ����. 

	switch (_cursor->getSlotIdx())
	{
		case 0:
			_cursor->setSlotIdx(6);
			break;

		case 7:
			//���� �������� ������� �ʰ�, 6�� ���Կ� �������� ���� �� 
			if (!_isGrabbingItem && !_shopSlot[6].isEmpty) _cursor->setSlotIdx(13);
			else _cursor->setSlotIdx(6);
			break;

		case 13:
			//���� �������� ������� �ʰ�, 5�� ���Կ� �������� ���� �� 
			if (!_isGrabbingItem && !_shopSlot[5].isEmpty) _cursor->setSlotIdx(12);
			else _cursor->setSlotIdx(5);
			break;

		case 14:
			_cursor->setSlotIdx(20);
			break;

		case 21:
			//���� �������� ������� �ʰ�, 20�� ���Կ� �������� ���� �� 
			if (!_isGrabbingItem && !_shopSlot[20].isEmpty) _cursor->setSlotIdx(27);
			else _cursor->setSlotIdx(20);
			break;

		case 27:
			//���� �������� ������� �ʰ�, 19�� ���Կ� �������� ���� �� 
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
			//���� �������� ������� �ʰ�, 5�� ���Կ� �������� ���� �� 
			if (!_isGrabbingItem && !_shopSlot[5].isEmpty) _cursor->setSlotIdx(12);
			else _cursor->setSlotIdx(5);
			break;

		case 12:
			//���� �������� ������� �ʰ�, 6�� ���Կ� �������� ���� �� 
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
			//���� �������� ������� �ʰ�, 19�� ���Կ� �������� ���� �� 
			if (!_isGrabbingItem && !_shopSlot[19].isEmpty) _cursor->setSlotIdx(26);
			else _cursor->setSlotIdx(19);
			break;

		case 26:
			//���� �������� ������� �ʰ�, 20�� ���Կ� �������� ���� �� 
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
			//���� �������� ������� �ʰ�, 19�� ���Կ� �������� ���� �� 
			if (!_isGrabbingItem && !_shopSlot[19].isEmpty) _cursor->setSlotIdx(26);
			else _cursor->setSlotIdx(19);
			break;

		case 6:
			//���� �������� ������� �ʰ�, 20�� ���Կ� �������� ���� �� 
			if (!_isGrabbingItem && !_shopSlot[20].isEmpty) _cursor->setSlotIdx(27);
			else _cursor->setSlotIdx(20);
			break;

		case 19:
			//���� �������� ������� �ʰ�, 5�� ���Կ� �������� ���� �� 
			if (!_isGrabbingItem && !_shopSlot[5].isEmpty) _cursor->setSlotIdx(12);
			else _cursor->setSlotIdx(5);
			break;

		case 20:
			//���� �������� ������� �ʰ�, 6�� ���Կ� �������� ���� �� 
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
			//���� �������� ������� �ʰ�, 5�� ���Կ� �������� ���� �� 
			if (!_isGrabbingItem && !_shopSlot[5].isEmpty) _cursor->setSlotIdx(12);
			else _cursor->setSlotIdx(19);
			break;

		case 6:
			//���� �������� ������� �ʰ�, 6�� ���Կ� �������� ���� �� 
			if (!_isGrabbingItem && !_shopSlot[6].isEmpty) _cursor->setSlotIdx(13);
			else _cursor->setSlotIdx(20);
			break;

		case 19:
			//���� �������� ������� �ʰ�, 19�� ���Կ� �������� ���� �� 
			if (!_isGrabbingItem && !_shopSlot[19].isEmpty) _cursor->setSlotIdx(26);
			else _cursor->setSlotIdx(5);
			break;

		case 20:
			//���� �������� ������� �ʰ�, 20�� ���Կ� �������� ���� �� 
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

	//���� ��� �ִ� �������� �ְų� �󽽷��� ��� �Լ� ���� 
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

	//Ŭ���ϰ� �ִ� �ð��� ��� �Ѳ����� ��� 
	if (_cursor->getClickTime() >= _grabTime)
	{
		_isGrabbingItem = true;
		grabAllItem();
		SOUNDMANAGER->play("cursor_pick", 0.4f);
	}
	else //ª�ٸ� 1���� ��� 
	{
		_isGrabbingItem = true;
		grabOneItem();
		SOUNDMANAGER->play("cursor_pick", 0.4f);
	}
}

void displayStand::grabOneItem()
{
	//Ŀ���� ����Ű�� �κ��丮 �ε����� ��ġ�ϴ� �������� itemGrabbed�� ��´�.
	for (int i = 0; i < _vShopInven.size(); i++)
	{
		if (_vShopInven[i]->getInvenPosIdx() != _cursor->getSlotIdx()) continue;

		//�������� �� ���� �����״ϱ� ������ Ŭ������ �������ֵ�, ������ 1���� �����Ѵ�.
		_itemGrabbed = *(_vShopInven[i]);
		_itemGrabbed.setCount(1);

		//���� �������� ������ŭ �κ��丮 �ε����� ������ ī��Ʈ�� ���ش�. 
		_vShopInven[i]->setCount(_vShopInven[i]->getCount() - 1);

		//���Կ� �ִ� �������� 0���� �۾��� ��� �ش� �����ʹ� �������ش�. 
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
	//Ŀ���� ����Ű�� �κ��丮 �ε����� ��ġ�ϴ� �������� itemGrabbed�� ��´�.
	for (int i = 0; i < _vShopInven.size(); i++)
	{
		if (_vShopInven[i]->getInvenPosIdx() != _cursor->getSlotIdx()) continue;

		//�������� �Ѳ����� ���� ���̹Ƿ� ���� �״�� ������ Ŭ������ �������ش�. 
		_itemGrabbed = *(_vShopInven[i]);

		//�������� �Ѳ����� ���� ���̹Ƿ� ���Կ� �ִ� �������� ���� 
		_shopSlot[_cursor->getSlotIdx()].isEmpty = true;
		SAFE_DELETE(_vShopInven[i]);
		_vShopInven.erase(_vShopInven.begin() + i);
		return;
	}
}

void displayStand::grabOneMoreItem()
{
	//���� �������� ������ŭ �κ��丮 �ε����� ������ ī��Ʈ�� ���ش�. 
	for (int i = 0; i < _vShopInven.size(); i++)
	{
		if (_vShopInven[i]->getInvenPosIdx() != _cursor->getSlotIdx()) continue;

		//���� ���� �����۰� ������ �������� �ٸ��ٸ� �ٲ�ġ���ϱ� 
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
			//�̹� ������ ���������� �ִ밳���̰ų�, ���� ���� ���� �ִ� ������ �ʰ��� �� ����Ī
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

			//������ ��� �ִ� �����۰� ���� ������ �������� �ϳ� �� ��´�. 
			_itemGrabbed.plusOneCount();
			_vShopInven[i]->minusOneCount();

			//���Կ� �ִ� �������� 0���� �۾��� ��� �ش� �����ʹ� �������ش�. 
			if (_vShopInven[i]->getCount() <= 0)
			{
				//������ �����ߴ� ������ ��������Ƿ� ���� ����ִ� �������� ��ġ����
				//���� ����Ű�� �ִ� ������ ��ġ������ �����Ѵ�.

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
	//��� �ִ� �������� ���� ��� �Լ� ���� 
	if (!_isGrabbingItem) return;

	//�� ������ Ŭ������ �� �ش� ���Կ� ����ִ� �������� ���´�.
	if (_shopSlot[_cursor->getSlotIdx()].isEmpty)
	{
		_isPuttingItem = true;
		putItemOnEmptySlot();
		loadLastPrice(_cursor->getSlotIdx());
		_isGrabbingItem = false;
		SOUNDMANAGER->play("cursor_drop", 0.4f);
	}
	else //������ ������� ���� �� 
	{
		//1.ó�� �������� ��Ҵ� ��ġ�� �ٽ� Ŭ������ �� �������� �� ���� ��´�.  
		if (_itemGrabbed.getInvenPosIdx() == _cursor->getSlotIdx())
		{
			_isPuttingItem = true;
			grabOneMoreItem();
		}
		else //2.ó�� �������� ���� ��ġ�� �ƴ� �� 
		{
			_isPuttingItem = true;
			putItemOnOccupiedSlot();
		}
	}
}

void displayStand::putItemOnEmptySlot()
{
	//�� ���Ӿ������� �����ؼ� ���� ��� �ִ� �������� �����ϰ� 
	//�ش� ���Ӿ������� ���� Ŀ���� ����Ű�� �ִ� ��ġ�� �����Ͽ� Ǫ�ù��Ѵ�. 
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

		//1.���� ���� ��� �ִ� �����۰� ���Կ� �ִ� �������� ������ ���� �� 
		if (_itemGrabbed.getItemIdx() == _vShopInven[i]->getItemIdx())
		{
			SOUNDMANAGER->play("cursor_drop", 0.4f);

			//�̹� ������ ���������� �ִ밳���� ���� ����Ī
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

			//���� ��� �ִ� �������� ���Կ� ��ġ�µ�, �� �������� ���Կ� ���� ������
			//�ִ� ������ ������ �ѱ� ��� �ִ� �������� ���Կ� �����ϰ�, �������� �ٽ� ��´�. 
			int maxCount = _itemGrabbed.getMaxCount();
			int sum = _itemGrabbed.getCount() + _vShopInven[i]->getCount();

			if (sum > maxCount) //�ִ� ���� ������ �Ѱ��� �� 
			{
				_itemGrabbed.setCount(_itemGrabbed.getCount() - (maxCount - _vShopInven[i]->getCount()));
				_vShopInven[i]->setCount(maxCount);
				_itemGrabbed.setInvenPosIdx(i);
				return;
			}
			else //�ִ� ���� ������ �ѱ��� �ʾ��� �� 
			{
				_vShopInven[i]->setCount(sum);
				_itemGrabbed = _itemEmpty;
				_isGrabbingItem = false;
				return;
			}
		}
		else //2.���� ���� ��� �ִ� �����۰� ���Կ� �ִ� ������ ������ �ٸ� �� 
		{
			SOUNDMANAGER->play("cursor_drop", 0.4f);

			//���� �������� ��Ҵ� �����ڸ��� ����ִ� ���
			if (_shopSlot[_itemGrabbed.getInvenPosIdx()].isEmpty)
			{
				//���� ����ִ� �������� �����ڸ��� ������, 
				//�����ڸ��� �ִ� �������� ���� ����ִ� �������� ����Ű�� �ε����� ������. 
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
			else //������� ���� ��� ���� ����ִ� �����۰� ���Կ� �ִ� ������ �ٲ�ġ�� 
			{
				gameItem *item = new gameItem;
				item->init(&_itemGrabbed);
				item->setInvenPosIdx(_cursor->getSlotIdx());

				_itemGrabbed = *(_vShopInven[i]);
				SAFE_DELETE(_vShopInven[i]);
				_vShopInven.erase(_vShopInven.begin() + i);
				_vShopInven.push_back(item);
				loadLastPrice(_cursor->getSlotIdx());
				//push_back�� ���ְ��� �� for���� ���� �ߺ������� �� �� �����Ƿ�
				//�ݵ�� return�Ͽ� �Լ��� ��������ش�. 
				return;
			}
		}
	}//end of for 
}

void displayStand::putGrabbingItem()
{
	//��� �ִ� �������� �ִٸ� ���� �ڸ��� �������� 
	if (!_isGrabbingItem) return;

	//���� �������� ���� ������ �ڸ��� ����ִٸ�
	if (_shopSlot[_itemGrabbed.getInvenPosIdx()].isEmpty)
	{
		//����ִ� ������ �����ؼ� ���Ϳ� Ǫ���ϰ� ���� �������� ���ֱ� 
		gameItem *item = new gameItem;
		item->init(&_itemGrabbed);
		_vShopInven.push_back(item);

		_shopSlot[item->getInvenPosIdx()].isEmpty = false;
		_itemGrabbed = _itemEmpty;
		_isGrabbingItem = false;
	}
	else // ������� �ʴٸ� ��� �ִ� �������� ī��Ʈ�� ���Կ� ���ϱ� 
	{
		for (int i = 0; i < _vShopInven.size(); i++)
		{
			//���� ��� �ִ� �������� ���� �ִ� �ڸ��� �ε��� ã�� 
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
	//���� Ŀ���� ��ġ�� ���� å�� ��ġ�� �� 
	switch (_cursor->getSlotIdx())
	{
		case 12: case 13: case 26: case 27:
			for (int i = 0; i < _vShopInven.size(); i++)
			{
				//���� Ŀ�� ��ġ�� ����Ű�� ������ �ε����� �ƴϸ� �ǳʶٱ� 
				if (_vShopInven[i]->getInvenPosIdx() != (_cursor->getSlotIdx() - 7)) continue;

				//���� ������ �ƽ����ݺ��� ���� �� ���� �ø��� 
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
	//���� Ŀ���� ��ġ�� ���� å�� ��ġ�� �� 
	switch (_cursor->getSlotIdx())
	{
		case 12: case 13: case 26: case 27:
			for (int i = 0; i < _vShopInven.size(); i++)
			{
				//���� Ŀ�� ��ġ�� ����Ű�� ������ �ε����� �ƴϸ� �ǳʶٱ� 
				if (_vShopInven[i]->getInvenPosIdx() != (_cursor->getSlotIdx() - 7)) continue;

				//���� ������ �ƽ����ݺ��� ���� �� ���� �ø��� 
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
				//���� Ŀ�� ��ġ�� ����Ű�� ������ �ε����� �ƴϸ� �ǳʶٱ� 
				if (_vShopInven[i]->getInvenPosIdx() != (_cursor->getSlotIdx() - 7)) continue;

				//���� å���� ������ ������ �������� �����ϱ� 
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
	//�κ��丮 �޴� ���� 
	IMAGEMANAGER->render("menu_shopInventory", getMemDC(), _shopInvenPos.x, _shopInvenPos.y);

	//���Ǵ� �޴� ���� 
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
		//����ִ� ������ �ǳʶٱ� 
		if (_shopSlot[i].isEmpty) continue;

		//������� ���� ������ �κ��丮 ���Ϳ��� �ش� ������ �ε����� ��ġ�ϴ� �������� ��� 
		for (int j = 0; j < _vShopInven.size(); j++)
		{
			//�κ��丮 ���Ϳ��� ������ �ε����� ��ġ���� �ʴ� �������� �ǳʶٱ� 
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
	//���� ������ ���(Ŀ���� ���)
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
	//Ŀ���� ����Ű�� �κ��丮�� ������ ����ִ� ��� �Լ��� ���� 
	if (_shopSlot[_cursor->getSlotIdx()].isEmpty) return;

	//Ŀ���� ����Ű�� �κ��丮 �ε����� ��ġ�ϴ� �������� ã�´�. 
	for (int i = 0; i < _vShopInven.size(); i++)
	{
		if (_vShopInven[i]->getInvenPosIdx() != _cursor->getSlotIdx()) continue;

		IMAGEMANAGER->render("nameBase", getMemDC(), 468, 630);
		_vShopInven[i]->getNameImg()->render(getMemDC(), 544, 635);
	}
}

void displayStand::displayCountRender()
{
	//������� ��� 0���� ��� 
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

	//������� ���� ��� �ش� �������� ī��Ʈ�� ��� 
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
	//������� ��� 0������ ��� 
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

	//������� ���� ��� ������ �� ���� ������ ��� 
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
	//������� ��� 0������ ��� 
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

	//������� ���� ��� �ش� �������� ������ ������ ���� �� ������ ��� 
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
	//count�� 0�� �� ��µǴ� �ڸ���(7�ڸ�)��ŭ 0 ǥ�� 
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

		//����ϰ��� �ϴ� ���ں��� ū �ڸ����� ȸ���� 0���� ä��� 
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

