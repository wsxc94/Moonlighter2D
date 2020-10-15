#include "stdafx.h"
#include "inventory.h"

HRESULT inventory::init()
{
	//클래스 초기화 
	_itemManager = new itemManager;
	_itemManager->init();

	_cursor = new cursor;
	_cursor->init();

	_selectMenu = new selectMenu;
	_selectMenu->init();

	//인벤토리메뉴 위치 초기화 
	initPos();

	//인벤토리 슬롯 설정 초기화 
	initInvenSlot();
	_invenCtrl = INVEN_INVENTORY;

	//아이템 초기화 
	initItem();

	//아이템 잡기 관련 변수 
	_isGrabbingItem = false;
	_isPuttingItem = false;
	_canGrab = true; 
	_grabTime = 12;

	//무기 장착 관련 변수 
	_curWeaponIdx = 1;
	_itemCount = 0;

	//메뉴 이동속도 
	_menuMoveSpeed = 90.f;

	//사운드 관련 변수 
	_grabSoundPlayed = false;

	return S_OK;
}

void inventory::release()
{
	_itemManager->release();
	SAFE_DELETE(_itemManager);

	_cursor->release();
	SAFE_DELETE(_cursor);

	_selectMenu->release();
	SAFE_DELETE(_selectMenu);
}

void inventory::update()
{
	_cursor->update();
	keyInput();

	if (_invenCtrl == INVEN_MERCHANT_PENDANT ||
		_invenCtrl == INVEN_MERCHANT_EMBLEM)
	{
		_selectMenu->update();
	}
}

void inventory::render(HDC hdc)
{
	//던전에 있는지 여부에 따라 메뉴이미지 다르게 출력 
	if (PLAYERDATA->getIsInDungeon())
	{
		IMAGEMANAGER->findImage("menu_inventory_inDungeon")->render(hdc, _invenPos.x, _invenPos.y);
	}
	else
	{
		IMAGEMANAGER->findImage("menu_inventory")->render(hdc, _invenPos.x, _invenPos.y);
	}

	//메뉴가 현재 고정된 상태일 때 출력 
	if (_cursor->getShowCursor())
	{
		merchantIconRender(hdc);
		itemRender(hdc);
		itemNameRender(hdc);
		statusRender(hdc);

		if (_itemGrabbed.getType() != ITEM_EMPTY) itemGrabbedRender(hdc);

		cursorRender(hdc);
		weaponIconRender(hdc);

		switch (_invenCtrl)
		{
			case INVEN_MERCHANT_MIRROR:
				break;

			case INVEN_MERCHANT_PENDANT:
				pendantCtrlRender(hdc);
				break;

			case INVEN_MERCHANT_EMBLEM:
				emblemCtrlRender(hdc);
				break;
		}//end of switch 
	}

	//char str[128];

	//wsprintf(str, "invenCtrl : %d", _invenCtrl);
	//TextOut(hdc, 10, 130, str, strlen(str));

	//wsprintf(str, "canGrab : %d", _canGrab);
	//TextOut(hdc, 10, 150, str, strlen(str));

	//wsprintf(str, "isGrabbingItem : %d", _isGrabbingItem);
	//TextOut(hdc, 10, 170, str, strlen(str));

	//for (int i = 0; i < _vInvenItem.size(); i++)
	//{
	//	wsprintf(str, "inven[%d].invenIdx : %d", i, _vInvenItem[i]->getInvenPosIdx());
	//	TextOut(hdc, 10, 170 + (i * 20), str, strlen(str));
	//}

}

int inventory::getCurItemCount()
{
	int itemCount = 0;

	for (int i = 0; i < _vInven.size(); i++)
	{
		switch (_vInven[i]->getInvenPosIdx())
		{
			case 5: case 6: case 12: case 13:
			case 19: case 20: case 26: case 27:
			case 28: case 29: case 30:
				break;

			default:
				if (_vInven[i]->getType() != ITEM_EMPTY) itemCount++;
				break;

		}//end of switch
	}//end of for 

	return itemCount;
}

gameItem inventory::getPotionEquipped()
{
	if (_invenSlot[20].isEmpty) return _itemEmpty;
	else
	{
		for (int i = 0; i < _vInven.size(); i++)
		{
			if (_vInven[i]->getInvenPosIdx() != 20) continue;

			return *(_vInven[i]);
		}
	}
}

gameItem inventory::getWeaponEquipped()
{
	switch (_curWeaponIdx)
	{
		case 1:
			if (_invenSlot[5].isEmpty) return _itemEmpty;
			else
			{
				for (int i = 0; i < _vInven.size(); i++)
				{
					if (_vInven[i]->getInvenPosIdx() != 5) continue;

					return *(_vInven[i]);
				}
			}

		case 2:
			if (_invenSlot[6].isEmpty) return _itemEmpty;
			else
			{
				for (int i = 0; i < _vInven.size(); i++)
				{
					if (_vInven[i]->getInvenPosIdx() != 6) continue;

					return *(_vInven[i]);
				}
			}
	}//end of switch
}

void inventory::initPos()
{
	_invenPos.x = INVENPOSX;
	_invenPos.y = WINSIZEY;
}

void inventory::moveInvenLeft(int destPos)
{
	_invenPos.x -= _menuMoveSpeed;

	if (_invenPos.x <= destPos)
	{
		_invenPos.x = destPos;
	}
}

void inventory::moveInvenRight(int destPos)
{
	_invenPos.x += _menuMoveSpeed;

	if (_invenPos.x >= destPos)
	{
		_invenPos.x = destPos;
	}
}

void inventory::initInven()
{
	_isGrabbingItem = false; 
	_isPuttingItem = false; 
	_grabSoundPlayed = false;
	_canGrab = true;

	_cursor->setSlotIdx(0);
	setInvenCtrl(INVEN_INVENTORY);
	putGrabbingItem();
}

void inventory::initInvenSlot()
{
	memset(_invenSlot, 0, sizeof(_invenSlot));

	for (int i = 0; i < MAXSLOT; i++)
	{
		_invenSlot[i].slotIdx = i;
		_invenSlot[i].isEmpty = true;

		//슬롯의 인덱스에 맞게 슬롯에 들어갈 수 있는 아이템의 타입을 정해준다. 
		switch (_invenSlot[i].slotIdx)
		{
			case 0: case 1: case 2: case 3: case 4:
			case 7: case 8: case 9: case 10: case 11:
			case 14: case 15: case 16: case 17: case 18:
			case 21: case 22: case 23: case 24: case 25:
				_invenSlot[i].slotType = SLOT_ITEM;
				_invenSlot[i].type = ITEM_NORMAL;
				break;

			case 5:
				_invenSlot[i].slotType = SLOT_WEAPON_LEFT;
				_invenSlot[i].type = ITEM_WEAPON;
				break;

			case 6:
				_invenSlot[i].slotType = SLOT_WEAPON_RIGHT;
				_invenSlot[i].type = ITEM_WEAPON;
				break;

			case 12:
				_invenSlot[i].slotType = SLOT_HELMET;
				_invenSlot[i].type = ITEM_HELMET;
				break;

			case 13:
				_invenSlot[i].slotType = SLOT_RING_UP;
				_invenSlot[i].type = ITEM_RING;
				break;

			case 19:
				_invenSlot[i].slotType = SLOT_ARMOR;
				_invenSlot[i].type = ITEM_ARMOR;
				break;

			case 20:
				_invenSlot[i].slotType = SLOT_POTION;
				_invenSlot[i].type = ITEM_POTION;
				break;

			case 26:
				_invenSlot[i].slotType = SLOT_BOOTS;
				_invenSlot[i].type = ITEM_BOOTS;
				break;

			case 27:
				_invenSlot[i].slotType = SLOT_RING_DOWN;
				_invenSlot[i].type = ITEM_RING;
				break;

			case 28:
				_invenSlot[i].slotType = SLOT_MIRROR;
				break;

			case 29:
				_invenSlot[i].slotType = SLOT_PENDANT;
				break;

			case 30:
				_invenSlot[i].slotType = SLOT_EMBLEM;
				break;
		}
	}
}

void inventory::initItemSlot()
{
	for (int i = 0; i < MAXSLOT; i++)
	{
		//아이템 슬롯은 전부 비워주기  
		switch (_invenSlot[i].slotIdx)
		{
			case 0: case 1: case 2: case 3: case 4:
			case 7: case 8: case 9: case 10: case 11:
			case 14: case 15: case 16: case 17: case 18:
			case 21: case 22: case 23: case 24: case 25:
				_invenSlot[i].isEmpty = true;
				break;

			default:
				break;
		}
	}//end of for 
}

void inventory::initItem()
{
	//아이템 초기화 
	memset(&_itemEmpty, 0, sizeof(_itemEmpty));
	memset(&_tempItem, 0, sizeof(_tempItem));
	memset(&_itemGrabbed, 0, sizeof(_itemGrabbed));

	for (int i = 0; i < _itemManager->getItem().size(); i++)
	{
		//새로운 게임 아이템을 생성해서 아이템매니져에 있는 아이템목록을 복사해서 넣어주기 

		gameItem *item = new gameItem;
		item->init(_itemManager->getItem()[i]);

		for (int j = 0; j < MAXSLOT; j++)
		{
			if (!_invenSlot[j].isEmpty || _invenSlot[j].slotType != SLOT_ITEM) continue;
	
			if (i == 15)
			{
				_invenSlot[5].isEmpty = false;
				item->setInvenPosIdx(5);
				_vInven.push_back(item);
				break;
			}
			else
			{
				_invenSlot[j].isEmpty = false;
				item->setInvenPosIdx(j);
				_vInven.push_back(item);
				break;
			}
		}
	}

	_vInven[0]->setCount(5);
	_vInven[1]->setCount(10);
	_vInven[2]->setCount(5);
	_vInven[3]->setCount(5);
	_vInven[13]->setCount(5);
}

bool inventory::addItemToInven(gameItem item)
{
	for (int i = 0; i < _vInven.size(); i++)
	{
		//추가하고자 하는 아이템과 같은 종류의 아이템이 인벤토리 슬롯에 있을 경우 
		if (_vInven[i]->getItemIdx() == item.getItemIdx())
		{
			//슬롯의 아이템이 이미 최대개수 이상일 때는 건너뛰기 
			if (_vInven[i]->getCount() >= _vInven[i]->getMaxCount()) continue;

			//슬롯에 있는 아이템에 개수 + 1
			_vInven[i]->plusOneCount();
			SOUNDMANAGER->play("cursor_pick", 0.6f);
			return true;
		}

		//같은 종류의 아이템이 인벤토리 슬롯에 없는 경우 인벤토리에 새롭게 추가하기 
		if (i == _vInven.size() - 1)
		{
			//인벤토리의 빈 자리로 위치값을 설정하여 인벤토리 벡터에 푸시백 
			for (int j = 0; j < MAXSLOT; j++)
			{
				if (!_invenSlot[j].isEmpty || _invenSlot[j].slotType != SLOT_ITEM) continue;

				gameItem *tempItem = new gameItem;
				tempItem->init(&item);
				tempItem->setCount(1);
				tempItem->setInvenPosIdx(j);

				_vInven.push_back(tempItem);
				_invenSlot[j].isEmpty = false;
				SOUNDMANAGER->play("cursor_pick", 0.6f);
				return true;

			}//end of for(j)

			//인벤토리가 꽉 차있는 경우 아이템을 줍지 않음 
			return false;
		}
	}//end of for(i)
}

void inventory::deleteInvenItem()
{
	for (int i = 0; i < _vInven.size(); )
	{
		switch (_vInven[i]->getInvenPosIdx())
		{
		case 5: case 6: case 12: case 13:
		case 19: case 20: case 26: case 27:
		case 28: case 29: case 30:
			i++;
			break;

		default:
			SAFE_DELETE(_vInven[i]);
			_vInven.erase(_vInven.begin() + i);
			break;
		}
	}
}

void inventory::syncWithShopInven(vector<gameItem*> vShopInven)
{
	//아이템 슬롯에 있던 아이템 지우고, 비우기 
	deleteInvenItem();
	initItemSlot();

	for (int i = 0; i < vShopInven.size(); i++)
	{
		//장착 슬롯에 있는 아이템을 제외하고 인벤토리 슬롯에 있던 아이템 가져오기 
		switch (vShopInven[i]->getInvenPosIdx())
		{
		case 5: case 6: case 12: case 13:
		case 19: case 20: case 26: case 27:
			break;

		default:
			gameItem *item = new gameItem;
			item->init(vShopInven[i]);
			_vInven.push_back(item);

			_invenSlot[item->getInvenPosIdx()].isEmpty = false;
			break;

		}//end of switch 
	}//end of for 
}

void inventory::keyInput()
{
	switch (_invenCtrl)
	{
		case INVEN_INVENTORY:
			invenKeyInput();
			break;

		case INVEN_MERCHANT_MIRROR:
			mirrorKeyInput();
			break;

		case INVEN_MERCHANT_PENDANT:
			pendantKeyInput();
			break;

		case INVEN_MERCHANT_EMBLEM:
			emblemKeyInput();
			break;
	}

	//키 입력을 받아서 무기 바꾸기 
	if (INPUT->GetKeyDown('Z'))
	{
		switchWeapon();
	}
}

void inventory::setInvenCtrl(INVEN_CTRL state)
{
	switch (state)
	{
		case INVEN_INVENTORY:
			_invenCtrl = state;
			_cursor->setCursorState(CURSOR_MOVE);
			break;

		case INVEN_MERCHANT_MIRROR:
			_invenCtrl = state;
			_selectMenu->setSelectIdx(SELECT_NO);
			_cursor->setCursorState(CURSOR_SELECT_MOVE);
			break;

		case INVEN_MERCHANT_PENDANT:
			_invenCtrl = state;
			_selectMenu->setSelectIdx(SELECT_NO);
			_cursor->setCursorState(CURSOR_SELECT_MOVE);
			break;

		case INVEN_MERCHANT_EMBLEM:
			_invenCtrl = state;
			_selectMenu->setSelectIdx(SELECT_NO);
			_cursor->setCursorState(CURSOR_SELECT_MOVE);
			break;
	}
}

void inventory::setMerchantCtrl()
{
	switch (_cursor->getSlotIdx())
	{
		case 28:
			setInvenCtrl(INVEN_MERCHANT_MIRROR);
			SOUNDMANAGER->play("cursor_move", 0.2f);
			break;

		case 29:
			if (PLAYERDATA->getIsInDungeon() && PLAYERDATA->getGold() >= 200)
				setInvenCtrl(INVEN_MERCHANT_PENDANT);
			SOUNDMANAGER->play("cursor_move", 0.2f);
			break;

		case 30:
			if (PLAYERDATA->getIsInDungeon() && PLAYERDATA->getGold() >= 1000)
				setInvenCtrl(INVEN_MERCHANT_EMBLEM);
			SOUNDMANAGER->play("cursor_move", 0.2f);
			break;
	}
}

void inventory::invenKeyInput()
{
	//상하좌우 키 입력 받아서 커서 움직이기 
	//커서가 움직일 때마다 커서 애니메이션 실행 
	if (INPUT->GetKeyDown('W'))
	{
		upKeyDown();
		_cursor->setCursorState(CURSOR_MOVE);
		SOUNDMANAGER->play("cursor_move", 0.2f);
	}
	if (INPUT->GetKeyDown('S'))
	{
		downKeyDown();
		_cursor->setCursorState(CURSOR_MOVE);
		SOUNDMANAGER->play("cursor_move", 0.2f);
	}
	if (INPUT->GetKeyDown('A'))
	{
		leftKeyDown();
		_cursor->setCursorState(CURSOR_MOVE);
		SOUNDMANAGER->play("cursor_move", 0.2f);
	}
	if (INPUT->GetKeyDown('D'))
	{
		rightKeyDown();
		_cursor->setCursorState(CURSOR_MOVE);
		SOUNDMANAGER->play("cursor_move", 0.2f);
	}

	//버튼을 누르고 있는 시간에 따라 
	//아이템을 한 개씩 잡거나 모두 잡는 동작 실행 
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
		setMerchantCtrl();
	}
	if (INPUT->GetKeyUp('J'))
	{
		//한꺼번에 잡기를 실행할 정도로 길게 누르지 않고 손을 뗄 경우
		//1개씩 아이템을 잡을 수 있도록 함수 실행 
		if (!_isPuttingItem) grabItem();

		_isPuttingItem = false;
		_canGrab = true; 
		_grabSoundPlayed = false;
		_cursor->setClickTime(0);
	}
}

void inventory::mirrorKeyInput()
{
}

void inventory::pendantKeyInput()
{
	//좌우 키 입력 시 selectIdx값 변경하기(네,아니오)
	if (INPUT->GetKeyDown('A') || INPUT->GetKeyDown('D'))
	{
		if (_selectMenu->getSelectIdx() == SELECT_NO)
		{
			_selectMenu->setSelectIdx(SELECT_YES);
			_selectMenu->setMenuState(SELECT_YES);
			_cursor->setCursorState(CURSOR_SELECT_MOVE);
			SOUNDMANAGER->play("cursor_move", 0.2f);
		}
		else
		{
			_selectMenu->setSelectIdx(SELECT_NO);
			_selectMenu->setMenuState(SELECT_NO);
			_cursor->setCursorState(CURSOR_SELECT_MOVE);
			SOUNDMANAGER->play("cursor_move", 0.2f);
		}
	}

	//네,아니요 중 선택하기 
	if (INPUT->GetKeyDown('J'))
	{
		//아니요 선택 시 인벤토리 컨트롤러로 변경 
		if (_selectMenu->getSelectIdx() == SELECT_NO)
		{
			setInvenCtrl(INVEN_INVENTORY);
			SOUNDMANAGER->play("cursor_move", 0.2f);
		}
		else
		{
			//네 선택 시 마을로 돌아가기 
			//1. gototown변수 true로 설정 
			//2. 아이템메뉴 종료하기(메뉴 닫기)
			//3. 사용비 골드에서 차감하기(200원)
			//4. 인벤토리 컨트롤러 초기화 
			//5. 선택메뉴의 상태 초기화(NO)
			//6. 플레이어 상태를 팬던트사용으로 변경해줌

			ITEMMENU->SetGoToTownEmblem(true);
			ITEMMENU->DoCloseMenu();
			PLAYERDATA->subGold(200);
			_selectMenu->setMenuState(SELECT_NO);
			//if (_selectMenu->getSelectIdx() == SELECT_YES) _cursor->setSlotIdx(0);
			SOUNDMANAGER->play("cursor_move", 0.2f);
		}
	}
	if (INPUT->GetKeyUp('J'))
	{
		_canGrab = false;
	}

	//뒤로가기 
	if (INPUT->GetKeyDown('L'))
	{
		setInvenCtrl(INVEN_INVENTORY);
		_selectMenu->setMenuState(SELECT_NO);
		SOUNDMANAGER->play("cursor_move", 0.2f);
	}
}

void inventory::emblemKeyInput()
{
	//좌우 키 입력 시 selectIdx값 변경하기(네,아니오)
	if (INPUT->GetKeyDown('A') || INPUT->GetKeyDown('D'))
	{
		if (_selectMenu->getSelectIdx() == SELECT_NO)
		{
			_selectMenu->setSelectIdx(SELECT_YES);
			_selectMenu->setMenuState(SELECT_YES);
			_cursor->setCursorState(CURSOR_SELECT_MOVE);
			SOUNDMANAGER->play("cursor_move", 0.2f);
		}
		else
		{
			_selectMenu->setSelectIdx(SELECT_NO);
			_selectMenu->setMenuState(SELECT_NO);
			_cursor->setCursorState(CURSOR_SELECT_MOVE);
			SOUNDMANAGER->play("cursor_move", 0.2f);
		}
	}

	//네,아니오 중 선택하기 
	if (INPUT->GetKeyDown('J'))
	{
		//아니오 선택 시 인벤토리 컨트롤러로 변경 
		if (_selectMenu->getSelectIdx() == SELECT_NO) setInvenCtrl(INVEN_INVENTORY);
		else
		{
			//네 선택 시 마을로 돌아가기 
			//1. gototown변수 true로 설정 
			//2. 아이템메뉴 종료하기(메뉴 닫기)
			//3. 사용비 골드에서 차감하기(200원)
			//4. 인벤토리 컨트롤러 초기화 
			//5. 선택메뉴의 상태 초기화(NO)
			//6. 플레이어 상태를 팬던트사용으로 변경해줌

			ITEMMENU->SetGoToTownEmblem(true);
			ITEMMENU->DoCloseMenu();
			PLAYERDATA->subGold(1000);
			_selectMenu->setMenuState(SELECT_NO);
			//if (_selectMenu->getSelectIdx() == SELECT_YES) _cursor->setSlotIdx(0);
			SOUNDMANAGER->play("cursor_move", 0.2f);
		}
	}
	if (INPUT->GetKeyUp('J'))
	{
		_canGrab = false;
	}

	//뒤로가기 
	if (INPUT->GetKeyDown('L'))
	{
		setInvenCtrl(INVEN_INVENTORY);
		_selectMenu->setMenuState(SELECT_NO);
		SOUNDMANAGER->play("cursor_move", 0.2f);
	}
}

void inventory::leftKeyDown()
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
		_cursor->setSlotIdx(13);
		break;

	case 14:
		_cursor->setSlotIdx(20);
		break;

	case 21:
		_cursor->setSlotIdx(27);
		break;

	case 28:
		_cursor->setSlotIdx(26);
		break;

	default:
		_cursor->setSlotIdx(_cursor->getSlotIdx() - 1);
		break;
	}
}

void inventory::rightKeyDown()
{
	switch (_cursor->getSlotIdx())
	{
	case 6:
		_cursor->setSlotIdx(0);
		break;

	case 13:
		_cursor->setSlotIdx(7);
		break;

	case 20:
		_cursor->setSlotIdx(14);
		break;

	case 27:
		_cursor->setSlotIdx(21);
		break;

	case 30:
		_cursor->setSlotIdx(26);
		break;

	default:
		_cursor->setSlotIdx(_cursor->getSlotIdx() + 1);
		break;
	}
}

void inventory::upKeyDown()
{
	switch (_cursor->getSlotIdx())
	{
	case 0: case 1:
		_cursor->setSlotIdx(28);
		break;

	case 2:
		_cursor->setSlotIdx(29);
		break;

	case 3: case 4:
		_cursor->setSlotIdx(30);
		break;

	case 5: case 6:
		_cursor->setSlotIdx(27);
		break;

	case 13:
		_cursor->setSlotIdx(5);
		break;

	case 29:
		_cursor->setSlotIdx(23);
		break;

	case 30:
		_cursor->setSlotIdx(25);
		break;

	default:
		_cursor->setSlotIdx(_cursor->getSlotIdx() - 7);
		break;
	}
}

void inventory::downKeyDown()
{
	switch (_cursor->getSlotIdx())
	{
		case 5: case 6:
			_cursor->setSlotIdx(13);
			break;

		case 21: case 22:
			_cursor->setSlotIdx(28);
			break;

		case 23:
			_cursor->setSlotIdx(29);
			break;

		case 24: case 25:
			_cursor->setSlotIdx(30);
			break;

		case 26: case 27:
			_cursor->setSlotIdx(5);
			break;

		case 28:
			_cursor->setSlotIdx(0);
			break;

		case 29:
			_cursor->setSlotIdx(2);
			break;

		case 30:
			_cursor->setSlotIdx(4);
			break;

		default:
			_cursor->setSlotIdx(_cursor->getSlotIdx() + 7);
			break;
	}
}

void inventory::switchWeapon()
{
	switch (_curWeaponIdx)
	{
		case 1:
			if (!_invenSlot[5].isEmpty) //5번 슬롯에 무기를 이미 착용하고 있고, 
			{
				//6번 슬롯에 무기가 장착되어있지 않다면 return (바꿀 무기가 없음)
				if (_invenSlot[6].isEmpty) return;
				else //5번과 6번 슬롯 모두 무기를 장착하고 있다면, 
				{
					switchWeaponIdx();
					SOUNDMANAGER->play("weaponChange", 0.4f);
				}
			}
			else //5번 슬롯에 장착한 무기가 없고, 
			{
				//6번 슬롯에도 장착한 무기가 없다면 return
				if (_invenSlot[6].isEmpty) return;
				else //5번에 장착한 무기는 없지만 6번에 장착한 무기가 있다면 변경 
				{
					switchWeaponIdx();
				}
			}
			break;

		case 2:
			if (!_invenSlot[6].isEmpty) //6번 슬롯에 무기를 이미 착용하고 있고, 
			{
				//5번 슬롯에 무기가 장착되어있지 않다면 return (바꿀 무기가 없음)
				if (_invenSlot[5].isEmpty) return;
				else //5번과 6번 슬롯 모두 무기를 장착하고 있다면, 
				{
					switchWeaponIdx();
					SOUNDMANAGER->play("weaponChange", 0.4f);
				}
			}
			else //6번 슬롯에 장착한 무기가 없고, 
			{
				//5번 슬롯에도 장착한 무기가 없다면 return
				if (_invenSlot[5].isEmpty) return;
				else //6번에 장착한 무기는 없지만 5번에 장착한 무기가 있다면 변경 
				{
					switchWeaponIdx();
				}
			}
			break;
	}
}

void inventory::switchWeaponIdx()
{
	if (_curWeaponIdx == 1) _curWeaponIdx = 2;
	else if (_curWeaponIdx == 2)_curWeaponIdx = 1;
}

void inventory::usePotionEquipped()
{
	//현재 장착 중인 포션이 없다면 함수 종료
	if (_invenSlot[20].isEmpty) return;

	for (int i = 0; i < _vInven.size(); i++)
	{
		if (_vInven[i]->getInvenPosIdx() != 20) continue;

		//해당 포션의 회복력만큼 체력 회복 
		PLAYERDATA->plusInDungeonHp(_vInven[i]->getPotionValue());

		//사용한 포션의 개수를 1개 감소  
		_vInven[i]->minusOneCount();

		//포션의 개수가 0개가 되면 인벤토리에서 삭제 
		if (_vInven[i]->getCount() <= 0)
		{
			_invenSlot[20].isEmpty = true;
			SAFE_DELETE(_vInven[i]);
			_vInven.erase(_vInven.begin() + i);
		}
	}
}

void inventory::grabItem()
{
	if (!_canGrab) return; 

	//현재 잡고 있는 아이템이 있거나 빈슬롯일 경우 함수 종료 
	if (_invenSlot[_cursor->getSlotIdx()].isEmpty && !_isGrabbingItem)
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

void inventory::grabOneItem()
{
	//커서가 가리키는 인벤토리 인덱스와 일치하는 아이템을 itemGrabbed에 담는다.
	for (int i = 0; i < _vInven.size(); i++)
	{
		if (_vInven[i]->getInvenPosIdx() != _cursor->getSlotIdx()) continue;

		//아이템을 한 개만 집을테니까 아이템 클래스를 복사해주되, 개수는 1개로 수정한다.
		_itemGrabbed = *(_vInven[i]);
		_itemGrabbed.setCount(1);

		//담은 아이템의 개수만큼 인벤토리 인덱스의 아이템 카운트를 빼준다. 
		_vInven[i]->setCount(_vInven[i]->getCount() - 1);

		//슬롯에 있는 아이템이 0보다 작아질 경우 해당 데이터는 삭제해준다. 
		if (_vInven[i]->getCount() <= 0)
		{
			_invenSlot[_cursor->getSlotIdx()].isEmpty = true;
			SAFE_DELETE(_vInven[i]);
			_vInven.erase(_vInven.begin() + i);
			return;
		}
	}
}

void inventory::grabAllItem()
{
	//커서가 가리키는 인벤토리 인덱스와 일치하는 아이템을 itemGrabbed에 담는다.
	for (int i = 0; i < _vInven.size(); i++)
	{
		if (_vInven[i]->getInvenPosIdx() != _cursor->getSlotIdx()) continue;

		//아이템을 한꺼번에 잡을 것이므로 개수 그대로 아이템 클래스를 복사해준다. 
		_itemGrabbed = *(_vInven[i]);

		//아이템을 한꺼번에 잡은 것이므로 슬롯에 있던 아이템은 삭제 
		_invenSlot[_cursor->getSlotIdx()].isEmpty = true;
		SAFE_DELETE(_vInven[i]);
		_vInven.erase(_vInven.begin() + i);
		return;
	}
}

void inventory::grabOneMoreItem()
{
	//담은 아이템의 개수만큼 인벤토리 인덱스의 아이템 카운트를 빼준다. 
	for (int i = 0; i < _vInven.size(); i++)
	{
		if (_vInven[i]->getInvenPosIdx() != _cursor->getSlotIdx()) continue;

		//만약 잡은 아이템과 슬롯의 아이템이 다르다면 바꿔치기하기 
		if (_itemGrabbed.getItemIdx() != _vInven[i]->getItemIdx())
		{
			SOUNDMANAGER->play("cursor_drop", 0.4f);

			gameItem *item = new gameItem;
			item->init(&_itemGrabbed);
			item->setInvenPosIdx(_cursor->getSlotIdx());

			_itemGrabbed = *(_vInven[i]);
			SAFE_DELETE(_vInven[i]);
			_vInven.erase(_vInven.begin() + i);
			_vInven.push_back(item);
			return;
		}
		else
		{
			//이미 슬롯의 소지개수가 최대개수이거나, 둘을 더한 값이 최대 개수를 초과할 때 스위칭
			if (_vInven[i]->getCount() == _vInven[i]->getMaxCount() ||
				_vInven[i]->getCount() + _itemGrabbed.getCount() > _vInven[i]->getMaxCount())
			{
				SOUNDMANAGER->play("cursor_drop", 0.4f);

				gameItem *item = new gameItem;
				item->init(&_itemGrabbed);
				item->setInvenPosIdx(_cursor->getSlotIdx());

				_itemGrabbed = *(_vInven[i]);
				SAFE_DELETE(_vInven[i]);
				_vInven.erase(_vInven.begin() + i);
				_vInven.push_back(item);
				return;
			}

			SOUNDMANAGER->play("cursor_pick", 0.4f);

			//기존에 쥐고 있는 아이템과 같은 종류의 아이템을 하나 더 잡는다. 
			_itemGrabbed.plusOneCount();
			_vInven[i]->minusOneCount();

			//슬롯에 있는 아이템이 0보다 작아질 경우 해당 데이터는 삭제해준다. 
			if (_vInven[i]->getCount() <= 0)
			{
				//기존에 선택했던 슬롯이 사라졌으므로 이제 잡고있는 아이템의 위치값은
				//현재 가리키고 있는 슬롯의 위치값으로 수정한다.

				_itemGrabbed.setInvenPosIdx(_cursor->getSlotIdx());
				_invenSlot[_cursor->getSlotIdx()].isEmpty = true;
				SAFE_DELETE(_vInven[i]);
				_vInven.erase(_vInven.begin() + i);
			}
			return;
		}
	}//end of for 
}

void inventory::putItem()
{
	//잡고 있는 아이템이 없는 경우 함수 종료 
	if (!_isGrabbingItem) return;

	//장비 및 포션 슬롯은 해당 타입이 같은 아이템만 장착 가능 
	switch (_cursor->getSlotIdx())
	{
	case 5: case 6: case 12: case 13:
	case 19: case 20: case 26: case 27:
		if (_itemGrabbed.getType() != _invenSlot[_cursor->getSlotIdx()].type)
		{
			SOUNDMANAGER->play("cursor_error", 0.6f);
			return;
		}
		break;

	case 28: case 29: case 30:
		SOUNDMANAGER->play("cursor_error", 0.6f);
		return;

	default:
		break;
	}

	//빈 슬롯은 클릭했을 때 해당 슬롯에 잡고있던 아이템을 놓는다.
	if (_invenSlot[_cursor->getSlotIdx()].isEmpty)
	{
		_isPuttingItem = true;
		putItemOnEmptySlot();
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

void inventory::putItemOnEmptySlot()
{
	//새 게임아이템을 생성해서 현재 잡고 있는 아이템을 복사하고 
	//해당 게임아이템을 현재 커서가 가리키고 있는 위치로 설정하여 푸시백한다. 
	gameItem *item = new gameItem;
	item->init(&_itemGrabbed);

	item->setInvenPosIdx(_cursor->getSlotIdx());
	_vInven.push_back(item);
	_invenSlot[_cursor->getSlotIdx()].isEmpty = false;

	_itemGrabbed = _itemEmpty;
}

void inventory::putItemOnOccupiedSlot()
{
	for (int i = 0; i < _vInven.size(); i++)
	{
		if (_vInven[i]->getInvenPosIdx() != _cursor->getSlotIdx()) continue;

		//1.내가 현재 잡고 있는 아이템과 슬롯에 있는 아이템의 종류가 같을 때 
		if (_itemGrabbed.getItemIdx() == _vInven[i]->getItemIdx())
		{
			SOUNDMANAGER->play("cursor_drop", 0.4f);

			//이미 슬롯의 소지개수가 최대개수일 때는 스위칭
			if (_vInven[i]->getCount() == _vInven[i]->getMaxCount())
			{
				gameItem *item = new gameItem;
				item->init(&_itemGrabbed);
				item->setInvenPosIdx(_cursor->getSlotIdx());

				_itemGrabbed = *(_vInven[i]);
				SAFE_DELETE(_vInven[i]);
				_vInven.erase(_vInven.begin() + i);
				_vInven.push_back(item);
				return;
			}

			//현재 잡고 있는 아이템을 슬롯에 합치는데, 그 과정에서 슬롯에 소지 가능한
			//최대 아이템 개수를 넘길 경우 최대 개수까지 슬롯에 저장하고, 나머지는 다시 잡는다. 
			int maxCount = _itemGrabbed.getMaxCount();
			int sum = _itemGrabbed.getCount() + _vInven[i]->getCount();

			if (sum > maxCount) //최대 소지 개수를 넘겼을 때 
			{
				_itemGrabbed.setCount(_itemGrabbed.getCount() - (maxCount - _vInven[i]->getCount()));
				_vInven[i]->setCount(maxCount);
				_itemGrabbed.setInvenPosIdx(i);
				return;
			}
			else //최대 소지 개수를 넘기지 않았을 때 
			{
				_vInven[i]->setCount(sum);
				_itemGrabbed = _itemEmpty;
				_isGrabbingItem = false;
				return;
			}
		}
		else //2.내가 현재 잡고 있는 아이템과 슬롯에 있는 아이템 종류가 다를 떄 
		{
			//장비 및 포션 슬롯의 아이템을 일반 아이템과 바꾸려고 할 경우 함수 종료 
			switch (_itemGrabbed.getInvenPosIdx())
			{
			case 5: case 6: case 12: case 13:
			case 19: case 20: case 26: case 27:
				if (_itemGrabbed.getType() != _invenSlot[_cursor->getSlotIdx()].type)
				{
					SOUNDMANAGER->play("cursor_error", 0.6f);
					return;
				}
				break;

			case 28: case 29: case 30:
				SOUNDMANAGER->play("cursor_error", 0.6f);
				return;

			default:
				break;
			}

			SOUNDMANAGER->play("cursor_drop", 0.4f);

			//내가 아이템을 잡았던 슬롯자리가 비어있는 경우
			if (_invenSlot[_itemGrabbed.getInvenPosIdx()].isEmpty)
			{
				//현재 잡고있는 아이템은 슬롯자리로 보내고, 
				//슬롯자리에 있던 아이템은 현재 잡고있는 아이템이 가리키는 인덱스로 보낸다. 
				gameItem *item = new gameItem;
				item->init(&_itemGrabbed);
				item->setInvenPosIdx(_cursor->getSlotIdx());
				_vInven.push_back(item);

				_vInven[i]->setInvenPosIdx(_itemGrabbed.getInvenPosIdx());
				_invenSlot[_itemGrabbed.getInvenPosIdx()].isEmpty = false;

				_itemGrabbed = _itemEmpty;
				_isGrabbingItem = false;
				return;
			}
			else //비어있지 않은 경우 현재 들고있는 아이템과 슬롯에 있는 아이템 바꿔치기 
			{
				gameItem *item = new gameItem;
				item->init(&_itemGrabbed);
				item->setInvenPosIdx(_cursor->getSlotIdx());

				_itemGrabbed = *(_vInven[i]);
				SAFE_DELETE(_vInven[i]);
				_vInven.erase(_vInven.begin() + i);
				_vInven.push_back(item);
				//push_back을 해주고나서 또 for문을 돌면 중복실행이 될 수 있으므로
				//반드시 return하여 함수를 종료시켜준다. 
				return;
			}
		}
	}//end of for 
}

void inventory::putGrabbingItem()
{
	//잡고 있는 아이템이 있다면 원래 자리로 돌려놓기 
	if (!_isGrabbingItem) return; 
	
	//내가 아이템을 잡은 슬롯의 자리가 비어있다면
	if (_invenSlot[_itemGrabbed.getInvenPosIdx()].isEmpty)
	{
		//잡고있는 아이템 복사해서 벡터에 푸시하고 잡은 아이템은 없애기 
		gameItem *item = new gameItem;
		item->init(&_itemGrabbed);
		_vInven.push_back(item);

		_invenSlot[item->getInvenPosIdx()].isEmpty = false; 
		_itemGrabbed = _itemEmpty;
		_isGrabbingItem = false; 
	}
	else // 비어있지 않다면 잡고 있는 아이템의 카운트를 슬롯에 더하기 
	{
		for (int i = 0; i < _vInven.size(); i++)
		{
			//현재 잡고 있는 아이템이 원래 있던 자리의 인덱스 찾기 
			if (_vInven[i]->getInvenPosIdx() != _itemGrabbed.getInvenPosIdx()) continue;

			_vInven[i]->setCount(_vInven[i]->getCount() + _itemGrabbed.getCount());

			_itemGrabbed = _itemEmpty;
			_isGrabbingItem = false;
			return;

		}//end of for 
	}
}

void inventory::cursorRender(HDC hdc)
{
	//커서의 상태에 따라 출력을 달리한다. 
	switch (_cursor->getState())
	{
		case CURSOR_SELECT_IDLE : case CURSOR_SELECT_MOVE:
			selectCursorRender(hdc);
			break;

		default:
			invenCursorRender(hdc);
			break;
	}
}

void inventory::invenCursorRender(HDC hdc)
{
	//커서 출력(슬롯이 불규칙하게 퍼져있어서 예외처리) 
	for (int i = 0; i < MAXSLOT; i++)
	{
		int columnIdx = _cursor->getSlotIdx() % 7;
		int rowIdx = _cursor->getSlotIdx() / 7;

		switch (_cursor->getSlotIdx())
		{
			case 0: case 1: case 2: case 3: case 4:
				_cursor->getImg()->frameRender(hdc,
					204 + (columnIdx * 72), 158, _cursor->getIdx(), 0);
				break;

			case 7: case 8: case 9: case 10: case 11:
			case 14: case 15: case 16: case 17: case 18:
			case 21: case 22: case 23: case 24: case 25:
				_cursor->getImg()->frameRender(hdc,
					204 + (columnIdx * 72), 244 + ((rowIdx - 1) * 72), _cursor->getIdx(), 0);
				break;

			case 5:
				_cursor->getImg()->frameRender(hdc, 734, 154, _cursor->getIdx(), 0);
				break;

			case 6:
				_cursor->getImg()->frameRender(hdc, 908, 154, _cursor->getIdx(), 0);
				break;

			case 12:
				_cursor->getImg()->frameRender(hdc, 650, 244, _cursor->getIdx(), 0);
				break;

			case 13:
				_cursor->getImg()->frameRender(hdc, 722, 244, _cursor->getIdx(), 0);
				break;

			case 19:
				_cursor->getImg()->frameRender(hdc, 650, 316, _cursor->getIdx(), 0);
				break;

			case 20:
				_cursor->getImg()->frameRender(hdc, 722, 316, _cursor->getIdx(), 0);
				break;

			case 26:
				_cursor->getImg()->frameRender(hdc, 650, 388, _cursor->getIdx(), 0);
				break;

			case 27:
				_cursor->getImg()->frameRender(hdc, 722, 388, _cursor->getIdx(), 0);
				break;

			case 28:
				_cursor->getImg()->frameRender(hdc, 204, 506, _cursor->getIdx(), 0);
				break;

			case 29:
				_cursor->getImg()->frameRender(hdc, 356, 484, _cursor->getIdx(), 0);
				break;

			case 30:
				_cursor->getImg()->frameRender(hdc, 470, 484, _cursor->getIdx(), 0);
				break;
		}
	}
}

void inventory::merchantIconRender(HDC hdc)
{
	//던전에 있을 때 아이템 사용금 이상의 골드가 있을 때만 상인의 아이템 출력 
	if (PLAYERDATA->getIsInDungeon())
	{
		if (PLAYERDATA->getGold() >= 200) IMAGEMANAGER->render("bag_pendant", hdc, 360, 496);
		if (PLAYERDATA->getGold() >= 1000) IMAGEMANAGER->render("bag_emblem", hdc, 474, 488);
	}
}

void inventory::selectCursorRender(HDC hdc)
{
	switch (_cursor->getSlotIdx())
	{
		case 28:
			break;

		case 29:
			break;

		case 30:
			break;

		default:
			break;
	}
}

void inventory::pendantCtrlRender(HDC hdc)
{
	IMAGEMANAGER->frameRender("cursor_move", hdc, 356, 484, 3, 0);
	IMAGEMANAGER->render("inventory_selectBubble", hdc, 404, 332);

	//selectIdx에 따라 알맞은 이미지 출력하기 
	if (_selectMenu->getSelectIdx() == SELECT_YES)
	{
		_selectMenu->getImg()->frameRender(hdc, 426, 356, _selectMenu->getIdx(), 0);
		_cursor->getImg()->frameRender(hdc, 414, 344, _cursor->getIdx(), 0);

		RECT txtRC = RectMake(444, 438, 42, 26);
		HFONT font = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
			0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("JejuGothic"));
		HFONT oFont = (HFONT)SelectObject(hdc, font);
		SetTextColor(hdc, RGB(227, 212, 184));
		DrawText(hdc, "네", -1, &txtRC, DT_CENTER | DT_WORDBREAK | DT_VCENTER);
		SelectObject(hdc, oFont);
		DeleteObject(font);
	}
	else
	{
		_selectMenu->getImg()->frameRender(hdc, 524, 356, _selectMenu->getIdx(), 0);
		_cursor->getImg()->frameRender(hdc, 512, 344, _cursor->getIdx(), 0);

		RECT txtRC = RectMake(534, 438, 58, 26);
		HFONT font = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
			0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("JejuGothic"));
		HFONT oFont = (HFONT)SelectObject(hdc, font);
		SetTextColor(hdc, RGB(227, 212, 184));
		DrawText(hdc, "아니요", -1, &txtRC, DT_CENTER | DT_WORDBREAK | DT_VCENTER);
		SelectObject(hdc, oFont);
		DeleteObject(font);
	}
}

void inventory::emblemCtrlRender(HDC hdc)
{
	IMAGEMANAGER->frameRender("cursor_move", hdc, 356 + 114, 484, 3, 0);
	IMAGEMANAGER->render("inventory_selectBubble", hdc, 404 + 114, 332);

	//selectIdx에 따라 알맞은 이미지 출력하기 
	if (_selectMenu->getSelectIdx() == SELECT_YES)
	{
		_selectMenu->getImg()->frameRender(hdc, 426 + 114, 356, _selectMenu->getIdx(), 0);
		_cursor->getImg()->frameRender(hdc, 414 + 114, 344, _cursor->getIdx(), 0);

		RECT txtRC = RectMake(444 + 114, 438, 42, 26);
		HFONT font = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
			0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("JejuGothic"));
		HFONT oFont = (HFONT)SelectObject(hdc, font);
		SetTextColor(hdc, RGB(227, 212, 184));
		DrawText(hdc, "네", -1, &txtRC, DT_CENTER | DT_WORDBREAK | DT_VCENTER);
		SelectObject(hdc, oFont);
		DeleteObject(font);
	}
	else
	{
		_selectMenu->getImg()->frameRender(hdc, 524 + 114, 356, _selectMenu->getIdx(), 0);
		_cursor->getImg()->frameRender(hdc, 512 + 114, 344, _cursor->getIdx(), 0);

		RECT txtRC = RectMake(534 + 114, 438, 58, 26);
		HFONT font = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
			0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("JejuGothic"));
		HFONT oFont = (HFONT)SelectObject(hdc, font);
		SetTextColor(hdc, RGB(227, 212, 184));
		DrawText(hdc, "아니요", -1, &txtRC, DT_CENTER | DT_WORDBREAK | DT_VCENTER);
		SelectObject(hdc, oFont);
		DeleteObject(font);
	}
}

void inventory::statusRender(HDC hdc)
{
	//플레이어 데이터에서 디폴트 스탯값 가져오기 
	int hp = PLAYERDATA->getDefaultHp();
	int atk = PLAYERDATA->getDefaultAtk();
	int def = PLAYERDATA->getDefaultDef();
	int spd = PLAYERDATA->getDefaultSpd();

	//착용한 무기의 스테이터스값을 가져와서 더하기 
	switch (_curWeaponIdx)
	{
		case 1:
			if (!_invenSlot[5].isEmpty)
			{
				for (int i = 0; i < _vInven.size(); i++)
				{
					if (_vInven[i]->getInvenPosIdx() != 5) continue;

					hp += *(_vInven[i]->getAbility() + ABILITY_HP);
					atk += *(_vInven[i]->getAbility() + ABILITY_ATK);
					def += *(_vInven[i]->getAbility() + ABILITY_DEF);
					spd += *(_vInven[i]->getAbility() + ABILITY_SPD);
				}
			}
			break;

		case 2:
			if (!_invenSlot[6].isEmpty)
			{
				for (int i = 0; i < _vInven.size(); i++)
				{
					if (_vInven[i]->getInvenPosIdx() != 6) continue;

					hp += *(_vInven[i]->getAbility() + ABILITY_HP);
					atk += *(_vInven[i]->getAbility() + ABILITY_ATK);
					def += *(_vInven[i]->getAbility() + ABILITY_DEF);
					spd += *(_vInven[i]->getAbility() + ABILITY_SPD);
				}
			}
			break;
	}

	//장비 스탯값이 반영된 스테이터스값을 플레이어 데이터에 반영하기 
	PLAYERDATA->setHp(hp);
	PLAYERDATA->setAtk(atk);
	PLAYERDATA->setDef(def);
	PLAYERDATA->setSpd(spd);

	//장비 스탯값이 반영된 스테이터스값 출력하기 
	statusNumRender(hdc, hp, 292);
	statusNumRender(hdc, atk, 352);
	statusNumRender(hdc, def, 412);
	statusNumRender(hdc, spd, 472);
}

void inventory::weaponIconRender(HDC hdc)
{
	switch (_curWeaponIdx)
	{
		case 1:
			IMAGEMANAGER->findImage("icon_equip_idx_empty")->frameRender(hdc,
				962, 206, 1, 0);
			IMAGEMANAGER->findImage("icon_equip_idx")->frameRender(hdc,
				788, 206, 0, 0);
			IMAGEMANAGER->findImage("icon_euip")->frameRender(hdc,
				806, 176, 0, 0);
			break;

		case 2:
			IMAGEMANAGER->findImage("icon_equip_idx_empty")->frameRender(hdc,
				788, 206, 0, 0);
			IMAGEMANAGER->findImage("icon_equip_idx")->frameRender(hdc,
				962, 206, 1, 0);
			IMAGEMANAGER->findImage("icon_euip")->frameRender(hdc,
				880, 176, 1, 0);
			break;
	}
}

void inventory::itemRender(HDC hdc)
{
	for (int i = 0; i < MAXSLOT; i++)
	{
		//비어있는 슬롯은 건너뛰기 
		if (_invenSlot[i].isEmpty) continue;

		//비어있지 않은 슬롯은 인벤토리 벡터에서 해당 슬롯의 인덱스와 일치하는 아이템을 출력 
		for (int j = 0; j < _vInven.size(); j++)
		{
			//인벤토리 벡터에서 슬롯의 인덱스와 일치하지 않는 아이템은 건너뛰기 
			if (_vInven[j]->getInvenPosIdx() != i) continue;

			int columnIdx = _vInven[j]->getInvenPosIdx() % 7;
			int rowIdx = _vInven[j]->getInvenPosIdx() / 7;

			//인벤토리 인덱스와 맞는 위치에 아이템 이미지와 개수 이미지 출력 
			switch (_vInven[j]->getInvenPosIdx())
			{
				case 0: case 1: case 2: case 3: case 4:
					_vInven[j]->getItemImg()->render(hdc,
						218 + (columnIdx * 72), 172);
					itemCountRender(hdc, _vInven[j]->getCount(),
						258 + (columnIdx * 72), 208);
					break;

				case 7: case 8: case 9: case 10: case 11:
				case 14: case 15: case 16: case 17: case 18:
				case 21: case 22: case 23: case 24: case 25:
					_vInven[j]->getItemImg()->render(hdc,
						218 + (columnIdx * 72), 258 + ((rowIdx - 1) * 72));
					itemCountRender(hdc, _vInven[j]->getCount(),
						258 + (columnIdx * 72), 294 + ((rowIdx - 1) * 72));
					break;

				case 5:
					IMAGEMANAGER->findImage("item_slot")->render(hdc, 742, 162);
					_vInven[j]->getItemImg()->render(hdc, 748, 168);
					break;

				case 6:
					IMAGEMANAGER->findImage("item_slot")->render(hdc, 916, 162);
					_vInven[j]->getItemImg()->render(hdc, 922, 168);
					break;

				case 12:
					_vInven[j]->getItemImg()->render(hdc, 664, 258);
					break;

				case 13:
					_vInven[j]->getItemImg()->render(hdc, 736, 258);
					break;

				case 19:
					_vInven[j]->getItemImg()->render(hdc, 664, 330);
					break;

				case 20:
					IMAGEMANAGER->findImage("item_slot")->render(hdc, 730, 324);
					_vInven[j]->getItemImg()->render(hdc, 736, 330);
					itemCountRender(hdc, _vInven[j]->getCount(), 776, 366);
					break;

				case 26:
					_vInven[j]->getItemImg()->render(hdc, 664, 402);
					break;

				case 27:
					_vInven[j]->getItemImg()->render(hdc, 736, 402);
					break;
			}//end of switch 

			//슬롯의 인덱스와 일치하는 아이템을 출력했다면 for문 탈출 
			break;

		}//end of for(j)
	}//end of for(i)

}

void inventory::itemGrabbedRender(HDC hdc)
{
	//잡은 아이템 출력(커서에 기반)
	for (int i = 0; i < MAXSLOT; i++)
	{
		int columnIdx = _cursor->getSlotIdx() % 7;
		int rowIdx = _cursor->getSlotIdx() / 7;

		switch (_cursor->getSlotIdx())
		{
			case 0: case 1: case 2: case 3: case 4:
				IMAGEMANAGER->render("grabBase", hdc,
					206 + (columnIdx * 72), 90);
				_itemGrabbed.getItemImg()->render(hdc,
					218 + (columnIdx * 72), 100);
				itemCountRender(hdc, _itemGrabbed.getCount(), 258 + (columnIdx * 72), 136);
				break;

			case 7: case 8: case 9: case 10: case 11:
			case 14: case 15: case 16: case 17: case 18:
			case 21: case 22: case 23: case 24: case 25:
				IMAGEMANAGER->render("grabBase", hdc,
					206 + (columnIdx * 72), 176 + ((rowIdx - 1) * 72));
				_itemGrabbed.getItemImg()->render(hdc,
					218 + (columnIdx * 72), 186 + ((rowIdx - 1) * 72));
				itemCountRender(hdc, _itemGrabbed.getCount(),
					258 + (columnIdx * 72), 222 + ((rowIdx - 1) * 72));
				break;

			case 5:
				IMAGEMANAGER->render("grabBase", hdc, 736, 86);
				_itemGrabbed.getItemImg()->render(hdc, 748, 96);
				itemCountRender(hdc, _itemGrabbed.getCount(), 788, 132);
				break;

			case 6:
				IMAGEMANAGER->render("grabBase", hdc, 910, 86);
				_itemGrabbed.getItemImg()->render(hdc, 922, 96);
				itemCountRender(hdc, _itemGrabbed.getCount(), 962, 132);
				break;

			case 12:
				IMAGEMANAGER->render("grabBase", hdc, 652, 176);
				_itemGrabbed.getItemImg()->render(hdc, 664, 186);
				itemCountRender(hdc, _itemGrabbed.getCount(), 704, 222);
				break;

			case 13:
				IMAGEMANAGER->render("grabBase", hdc, 724, 176);
				_itemGrabbed.getItemImg()->render(hdc, 736, 186);
				itemCountRender(hdc, _itemGrabbed.getCount(), 776, 222);
				break;

			case 19:
				IMAGEMANAGER->render("grabBase", hdc, 652, 248);
				_itemGrabbed.getItemImg()->render(hdc, 664, 258);
				itemCountRender(hdc, _itemGrabbed.getCount(), 704, 294);
				break;

			case 20:
				IMAGEMANAGER->render("grabBase", hdc, 724, 248);
				_itemGrabbed.getItemImg()->render(hdc, 736, 258);
				itemCountRender(hdc, _itemGrabbed.getCount(), 776, 294);
				break;

			case 26:
				IMAGEMANAGER->render("grabBase", hdc, 652, 320);
				_itemGrabbed.getItemImg()->render(hdc, 664, 330);
				itemCountRender(hdc, _itemGrabbed.getCount(), 704, 366);
				break;

			case 27:
				IMAGEMANAGER->render("grabBase", hdc, 724, 320);
				_itemGrabbed.getItemImg()->render(hdc, 736, 330);
				itemCountRender(hdc, _itemGrabbed.getCount(), 776, 366);
				break;

			case 28:
				IMAGEMANAGER->render("grabBase", hdc, 206, 438);
				_itemGrabbed.getItemImg()->render(hdc, 218, 448);
				itemCountRender(hdc, _itemGrabbed.getCount(), 258, 484);
				break;

			case 29:
				IMAGEMANAGER->render("grabBase", hdc, 358, 414);
				_itemGrabbed.getItemImg()->render(hdc, 370, 424);
				itemCountRender(hdc, _itemGrabbed.getCount(), 410, 460);
				break;

			case 30:
				IMAGEMANAGER->render("grabBase", hdc, 470, 414);
				_itemGrabbed.getItemImg()->render(hdc, 482, 424);
				itemCountRender(hdc, _itemGrabbed.getCount(), 522, 460);
				break;
		}
	}
}

void inventory::itemNameRender(HDC hdc)
{
	//커서가 가리키는 인벤토리의 슬롯이 비어있는 경우 함수를 종료 
	if (_invenSlot[_cursor->getSlotIdx()].isEmpty) return;

	//커서가 가리키는 인벤토리 인덱스와 일치하는 아이템을 찾는다. 
	for (int i = 0; i < _vInven.size(); i++)
	{
		if (_vInven[i]->getInvenPosIdx() != _cursor->getSlotIdx()) continue;

		IMAGEMANAGER->render("nameBase", hdc, 468, 630);
		_vInven[i]->getNameImg()->render(hdc, 544, 635);
	}
}

void inventory::itemCountRender(HDC hdc, int count, int destX, int destY)
{
	for (int i = 1, distance = 0; i <= count; i *= 10)
	{
		int number = (count / i) % 10;

		char keyName[16];
		wsprintf(keyName, "%d_black", number);
		IMAGEMANAGER->render(keyName, hdc, destX - (distance * 12), destY);

		distance++;
	}
}

void inventory::statusNumRender(HDC hdc, int number, int destY)
{
	// number < 10  
	if (number < 10)
	{
		for (int i = 1; i <= number; i *= 10)
		{
			int num = (number / i) % 10;

			char keyName[16];
			wsprintf(keyName, "%d", num);
			IMAGEMANAGER->render(keyName, hdc, 1114, destY);
		}
	}
	else if (number < 100) // 10 <= number < 100
	{
		for (int i = 1, distance = 0; i <= number; i *= 10)
		{
			int num = (number / i) % 10;

			char keyName[16];
			wsprintf(keyName, "%d", num);
			IMAGEMANAGER->render(keyName, hdc, 1120 - (distance * 12), destY);

			distance++;
		}
	}
	else // number >= 100 
	{
		for (int i = 1, distance = 0; i <= number; i *= 10)
		{
			int num = (number / i) % 10;

			char keyName[16];
			wsprintf(keyName, "%d", num);
			IMAGEMANAGER->render(keyName, hdc, 1126 - (distance * 12), destY);

			distance++;
		}
	}
}

