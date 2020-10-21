#include "stdafx.h"
#include "potionShop.h"

HRESULT potionShop::init()
{
	//페이드매니져 초기화 
	_fadeManager = new fadeManager;
	_fadeManager->init();

	//셀렉트메뉴 초기화 
	_selectMenu = new selectMenu;
	_selectMenu->init();

	//애니메이션 초기화 
	_animation = new animation;
	_animation->init(IMAGEMANAGER->findImage("craftPotionAnim"), 0, 5, false, false);
	_animation->aniStop();

	//커서 초기화 
	_cursor = new cursor;
	_cursor->init();

	//포션상점 초기화 
	initPotionSlot();
	initPotionShop();

	//이동메뉴 클래스 초기화 
	_potionMenu = new movingObject;
	_potionMenu->init(IMAGEMANAGER->findImage("menu_potion"),
		POTION_MENUPOSX, WINSIZEY, 90.f);

	_shopBanner = new movingObject;
	_shopBanner->init(IMAGEMANAGER->findImage("shop_nameBase"),
		POTION_BANNERPOSX, -IMAGEMANAGER->findImage("shop_nameBase")->getHeight(), 20.f);

	_eKeyIcon = new movingObject;
	_eKeyIcon->init(IMAGEMANAGER->findImage("icon_potionKeyE"),
		WINSIZEX, POTION_EKEYPOSY, 20.f);

	_frameCount = 0;

	//메뉴 열고 닫기 관련 변수 초기화 
	_menuOn = false; 
	_openMenu = false; 
	_closeMenu = false; 
	_showBannerTxt = false; 
	_animPlayed = false; 
	_playAnim = false; 
	
	return S_OK;
}

void potionShop::release()
{
	SAFE_DELETE(_fadeManager);
	SAFE_DELETE(_potionMenu);
	SAFE_DELETE(_shopBanner);
	SAFE_DELETE(_eKeyIcon);
	SAFE_DELETE(_selectMenu);
	SAFE_DELETE(_cursor);
}

void potionShop::update()
{
	_fadeManager->update();
	_selectMenu->update();
	_animation->update();
	_cursor->update();

	//메뉴 열고 닫기 
	toggleMenu();
	openMenu();
	closeMenu();

	if (_menuOn)
	{
		//키입력 함수 
		keyInput();
	}
}

void potionShop::render()
{
	_fadeManager->render(getMemDC());

	//char str[100];
	//wsprintf(str, "produceCount : %d", _produceCount);
	//TextOut(getMemDC(), 10, 130, str, strlen(str));
	//
	//wsprintf(str, "frameCount : %d", _frameCount);
	//TextOut(getMemDC(), 10, 150, str, strlen(str));

	if (!_menuOn) return; 

	//상점메뉴 이미지 출력 
	potionMenuRender();

	//상점이름 텍스트 출력 
	if (_showBannerTxt) bannerTextRender();

	//메뉴가 고정되어있을 때만 출력 
	if (_cursor->getShowCursor())
	{
		potionMenuTextRender();
		potionInfoRender();
		recipeInfoRender();

		switch (_potionCtrl)
		{
			case POTION_MENU:
				_cursor->render(getMemDC());
				if (_playAnim) potionCraftRender();
				break;

			case POTION_MESSAGE:
				printMessage();
				break;

			case POTION_SET_COUNT:
				setCountRender();
				break;

			case POTION_SELECTMENU:
				selectMenuRender();
				_cursor->render(getMemDC());
				break;
		}
	}
}

void potionShop::toggleMenu()
{
	if (INPUT->GetKeyDown('O'))
	{
		//현재 메뉴를 열고 있거나 닫고 있는 중이라면 함수 종료 
		if (_openMenu || _closeMenu) return;

		if (_menuOn)
		{
			_closeMenu = true;
			_showBannerTxt = false; 
			_fadeManager->fadeInit(16, FADE_IN);
			_cursor->setShowCursor(false);
		}
		else
		{
			_menuOn = true;
			_openMenu = true;
			initPotionShop();

			//인터페이스 렌더여부 설정 
			PLAYERDATA->setRenderHp(false);
			PLAYERDATA->setRenderKeyInfo(false);
			PLAYERDATA->setRenderItemCount(false);

			//아이템메뉴의 키입력을 받지 않는다.(메뉴의 키값이 서로 충돌하기 때문)
			ITEMMENU->setCanKeyInput(false);
			_fadeManager->fadeInit(16, FADE_OUT, 205);
			SOUNDMANAGER->play("witch_open", 0.5f);
		}
	}
}

void potionShop::initMenuPos()
{
	//포션메뉴 위치 초기화 
	_potionMenu->initPos(POTION_MENUPOSX, WINSIZEY);
	_shopBanner->initPos(POTION_BANNERPOSX, -(_shopBanner->getImg()->getHeight()));
	_eKeyIcon->initPos(WINSIZEX, POTION_EKEYPOSY);

	//강화메뉴 위치 초기화 
}

void potionShop::openPotionMenu()
{
	//현재 메뉴를 열고 있거나 닫고 있는 중이라면 함수 종료 
	if (_openMenu || _closeMenu) return;

	_menuOn = true;
	_openMenu = true;
	
	//인터페이스 렌더여부 설정 
	PLAYERDATA->setRenderHp(false);
	PLAYERDATA->setRenderKeyInfo(false);
	PLAYERDATA->setRenderItemCount(false);

	//아이템메뉴의 키입력을 받지 않는다.(메뉴의 키값이 서로 충돌하기 때문)
	ITEMMENU->setCanKeyInput(false);

	_fadeManager->fadeInit(16, FADE_OUT, 205);
	SOUNDMANAGER->play("witch_Open", 0.5f);
}

void potionShop::closePotionMenu()
{
	//현재 메뉴를 열고 있거나 닫고 있는 중이라면 함수 종료 
	if (_openMenu || _closeMenu) return;

	_closeMenu = true;
	_fadeManager->fadeInit(16, FADE_IN);
	_cursor->setShowCursor(false);
}

void potionShop::openMenu()
{
	if (!_openMenu) return;

	_potionMenu->moveUp(POTION_MENUPOSY);
	_shopBanner->moveDown(POTION_BANNERPOSY);
	_eKeyIcon->moveLeft(POTION_EKEYPOSX);

	if (_shopBanner->getIsArrived()) _showBannerTxt = true; 

	//모두 제 위치로 도착했을 때 
	if (_potionMenu->getIsArrived() && _shopBanner->getIsArrived() && _eKeyIcon->getIsArrived())
	{
		_openMenu = false; 
		_potionMenu->setIsArrived(false);
		_shopBanner->setIsArrived(false);
		_eKeyIcon->setIsArrived(false);
		
		initCursor();
		_cursor->setShowCursor(true);
		_cursor->setCursorState(CURSOR_MOVE);
	}
}

void potionShop::closeMenu()
{
	if (!_closeMenu) return;

	_potionMenu->moveDown(WINSIZEY);
	_shopBanner->moveUp(-(_shopBanner->getImg()->getHeight()));
	_eKeyIcon->moveRight(WINSIZEX);

	//모두 제 위치로 도착했을 때 
	if (_potionMenu->getIsArrived() && _shopBanner->getIsArrived() && _eKeyIcon->getIsArrived())
	{
		_menuOn = false; 
		_closeMenu = false;

		//포션메뉴 설정값 변경 
		_potionMenu->setIsArrived(false);
		_shopBanner->setIsArrived(false);
		_eKeyIcon->setIsArrived(false);

		//인터페이스 렌더여부 설정 
		PLAYERDATA->setRenderHp(true);
		PLAYERDATA->setRenderKeyInfo(true);
		PLAYERDATA->setRenderItemCount(true);

		//아이템메뉴 키입력 받기 
		ITEMMENU->setCanKeyInput(true);
	}
}

void potionShop::initPotionSlot()
{
	memset(_potionSlot, 0, sizeof(_potionSlot));

	//슬롯 1번(HP물약1 구매)
	_potionSlot[0].slotIdx = 0;
	_potionSlot[0].type = POTION_BUY;
	_potionSlot[0].item = ITEMMENU->getItemManager()->getItemByIdx(POTION1_IDX);
	_potionSlot[0].price = 400;
	_potionSlot[0].description[0] = "체력을 40 회복한다. 한두 개";
	_potionSlot[0].description[1] = "정도는 항상 휴대하는 게 좋다.";
	
	//슬롯 2번(HP물약1 제조)
	_potionSlot[1].slotIdx = 1;
	_potionSlot[1].type = POTION_CRAFT;
	_potionSlot[1].item = ITEMMENU->getItemManager()->getItemByIdx(POTION1_IDX);
	_potionSlot[1].price = 125;
	_potionSlot[1].description[0] = "체력을 40 회복한다. 한두 개";
	_potionSlot[1].description[1] = "정도는 항상 휴대하는 게 좋다.";
	_potionSlot[1].mixRecipe = new recipe;
	_potionSlot[1].mixRecipe->addMaterial(RICHJELLY_IDX, 1);
	
	//슬롯 3번(HP물약2 구매)
	_potionSlot[2].slotIdx = 2;
	_potionSlot[2].type = POTION_BUY;
	_potionSlot[2].item = ITEMMENU->getItemManager()->getItemByIdx(POTION2_IDX);
	_potionSlot[2].price = 2500;
	_potionSlot[2].description[0] = "체력을 75 회복한다. 제값을";
	_potionSlot[2].description[1] = "톡톡히 하는 제품이다.";

	//슬롯 2번(HP물약2 제조)
	_potionSlot[3].slotIdx = 3;
	_potionSlot[3].type = POTION_CRAFT;
	_potionSlot[3].item = ITEMMENU->getItemManager()->getItemByIdx(POTION2_IDX);
	_potionSlot[3].price = 800;
	_potionSlot[3].description[0] = "체력을 75 회복한다. 제값을";
	_potionSlot[3].description[1] = "톡톡히 하는 제품이다.";
	_potionSlot[3].mixRecipe = new recipe;
	_potionSlot[3].mixRecipe->addMaterial(RICHJELLY_IDX, 1);
	_potionSlot[3].mixRecipe->addMaterial(VENOMJELLY_IDX, 1);
}

void potionShop::initCursor()
{
	_cursor->setSlotIdx(0);
	_cursor->setDestX(POTION_CURSORX);
	_cursor->setDestY(POTION_CURSORY);
}

void potionShop::initPotionShop()
{
	setPotionCtrl(POTION_MENU);
	_messageType = LACK_OF_GOLD;
	initCursor();

	_produceCount = 1;
	_maxProduceCount = 1;
}

void potionShop::setPotionCtrl(POTION_CTRL state)
{
	switch (state)
	{
		case POTION_MENU:
			_cursor->setDestX(POTION_CURSORX + (_cursor->getSlotIdx()*POTION_CURSOR_DISTANCE));
			_cursor->setDestY(POTION_CURSORY);
			_cursor->setCursorState(CURSOR_MOVE);
			_potionCtrl = state;
			break;

		case POTION_MESSAGE:
			_potionCtrl = state;
			break;

		case POTION_SET_COUNT:
			_produceCount = 1;
			_potionCtrl = state;
			break;

		case POTION_SELECTMENU:
			_cursor->setDestPos(718, 138);
			_cursor->setCursorState(CURSOR_SELECT_MOVE);
			_potionPosY = 484;
			_selectMenu->init();
			_potionCtrl = state;
			break;
	}
}

bool potionShop::checkRequirements()
{
	//가방에 공간이 없다면 구매,제조 상관없이 false
	int itemIdx = _potionSlot[_cursor->getSlotIdx()].item.getItemIdx();
	if (!ITEMMENU->getInventory()->checkRoomForItem(itemIdx))
	{
		_messageType = LACK_OF_ROOM;
		setPotionCtrl(POTION_MESSAGE);
		SOUNDMANAGER->play("cursor_error", 0.6f);
		return false; 
	}

	//포션 구매 시 골드 충족여부만 확인 
	if (_potionSlot[_cursor->getSlotIdx()].type == POTION_BUY)
	{
		//현재 소지중인 골드가 이용요금보다 적을 경우 경고 메시지 띄우고 return
		if (PLAYERDATA->getGold() < _potionSlot[_cursor->getSlotIdx()].price)
		{
			_messageType = LACK_OF_GOLD;
			setPotionCtrl(POTION_MESSAGE);
			SOUNDMANAGER->play("cursor_error", 0.6f);
			return false; 
		}
		else return true;
	}
	else //포션 제조 시 골드충족여부 + 재료충족여부 확인 
	{
		//재료가 부족한 경우 
		if (!_potionSlot[_cursor->getSlotIdx()].mixRecipe->checkRequirements())
		{
			//골드가 부족하다면 골드,재료부족 메시지 띄우기 
			if (PLAYERDATA->getGold() < _potionSlot[_cursor->getSlotIdx()].price)
			{
				_messageType = LACK_OF_BOTH;
				setPotionCtrl(POTION_MESSAGE);
				SOUNDMANAGER->play("cursor_error", 0.6f);
				return false;
			}
			else
			{
				_messageType = LACK_OF_MATERIAL;
				setPotionCtrl(POTION_MESSAGE);
				SOUNDMANAGER->play("cursor_error", 0.6f);
				return false; 
			}
		}
		else //재료가 부족하지 않은 경우 
		{
			//골드가 부족하다면 골드부족 메시지 띄우기 
			if (PLAYERDATA->getGold() < _potionSlot[_cursor->getSlotIdx()].price)
			{
				_messageType = LACK_OF_GOLD;
				setPotionCtrl(POTION_MESSAGE);
				SOUNDMANAGER->play("cursor_error", 0.6f);
				return false;
			}
			else return true; 
		}
	}
}

void potionShop::checkMaxProduceCount()
{
	//1.인벤토리 슬롯에 기반한 최대제조개수 구하기 
	_maxProduceCount = ITEMMENU->getInventory()->getMaxProduceBasedOnRoom(
		_potionSlot[_cursor->getSlotIdx()].item.getItemIdx(),
		_potionSlot[_cursor->getSlotIdx()].item.getMaxCount());

	//2.골드에 기반한 최대제조개수 구하기 
	int maxGoldCount = PLAYERDATA->getGold() / _potionSlot[_cursor->getSlotIdx()].price;

	//1. 2번 중에서 최종적으로 더 적은 경우를 최대제조개수로 설정 
	if (maxGoldCount < _maxProduceCount) _maxProduceCount = maxGoldCount;

	//3.포션을 직접 만들 경우 재료에 기반한 최대제조개수까지 구해서 비교  
	if (_potionSlot[_cursor->getSlotIdx()].type == POTION_CRAFT)
	{
		int maxMaterialCount = _potionSlot[_cursor->getSlotIdx()].mixRecipe->getMaxProduceBasedOnMaterial();
		if (maxMaterialCount < _maxProduceCount) _maxProduceCount = maxMaterialCount;
	}
}

void potionShop::playCraftingAnim()
{
	if (!_animPlayed)
	{
		int totalPrice = (_produceCount * _potionSlot[_cursor->getSlotIdx()].price);
		DAMAGEFONT->init(90, 112, -totalPrice);
		PLAYERDATA->startChangeGold(totalPrice, GOLD_SUBTRACT);

		_animation->aniRestart();
		applyCraftingResult();
		_animPlayed = true;

		SOUNDMANAGER->play("witch_craft", 0.4f);
		SOUNDMANAGER->play("gold_decrement", 0.3f);
	}
}

void potionShop::applyCraftingResult()
{	
	//1. 제조한 만큼 인벤토리에 재료 삭감(크래프팅일 경우 )
	if (_potionSlot[_cursor->getSlotIdx()].type == POTION_CRAFT)
	{
		_potionSlot[_cursor->getSlotIdx()].mixRecipe->deleteUsedMaterial(_produceCount);
	}

	//2. 제조한 만큼 인벤토리에 생성 
	ITEMMENU->getInventory()->addItemByCount(_potionSlot[_cursor->getSlotIdx()].item, _produceCount);
}

void potionShop::printMessage()
{
	switch (_messageType)
	{
		case LACK_OF_GOLD:
			printLackOfGold();
			break;

		case LACK_OF_MATERIAL:
			printLackOfMaterial();
			break;

		case LACK_OF_BOTH:
			printLackOfBoth();
			break;

		case LACK_OF_ROOM:
			printLackOfRoom();
			break;
	}
}

void potionShop::printLackOfGold()
{
	IMAGEMANAGER->render("shop_bubble", getMemDC(), 328, 120);

	RECT txtRC = RectMake(356, 168, 400, 54);
	HFONT font = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
		0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("JejuGothic"));
	HFONT oFont = (HFONT)SelectObject(getMemDC(), font);
	SetTextColor(getMemDC(), RGB(230, 215, 187));
	DrawText(getMemDC(), "골드가 충분하지 않아.",
		-1, &txtRC, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	SelectObject(getMemDC(), oFont);
	DeleteObject(font);
}

void potionShop::printLackOfMaterial()
{
	IMAGEMANAGER->render("shop_bubble", getMemDC(), 328, 120);

	char tempStr[64];
	strcpy(tempStr, _potionSlot[_cursor->getSlotIdx()].mixRecipe->getShortMaterialName());
	strcat(tempStr, " 아이템이 없어.");

	RECT txtRC = RectMake(356, 168, 448, 54);
	HFONT font = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
		0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("JejuGothic"));
	HFONT oFont = (HFONT)SelectObject(getMemDC(), font);
	SetTextColor(getMemDC(), RGB(230, 215, 187));
	DrawText(getMemDC(), tempStr,
		-1, &txtRC, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	SelectObject(getMemDC(), oFont);
	DeleteObject(font);
}

void potionShop::printLackOfBoth()
{
	IMAGEMANAGER->render("shop_bubble", getMemDC(), 328, 120);

	RECT txtRC = RectMake(356, 168, 400, 54);
	HFONT font = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
		0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("JejuGothic"));
	HFONT oFont = (HFONT)SelectObject(getMemDC(), font);
	SetTextColor(getMemDC(), RGB(230, 215, 187));
	DrawText(getMemDC(), "필요한 재료나 골드가 부족해.",
		-1, &txtRC, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	SelectObject(getMemDC(), oFont);
	DeleteObject(font);
}

void potionShop::printLackOfRoom()
{
	IMAGEMANAGER->render("shop_bubble", getMemDC(), 328, 120);

	RECT txtRC = RectMake(356, 168, 400, 54);
	HFONT font = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
		0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("JejuGothic"));
	HFONT oFont = (HFONT)SelectObject(getMemDC(), font);
	SetTextColor(getMemDC(), RGB(230, 215, 187));
	DrawText(getMemDC(), "가방에 아이템을 넣을 공간이 부족해.",
		-1, &txtRC, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	SelectObject(getMemDC(), oFont);
	DeleteObject(font);
}

void potionShop::keyInput()
{
	switch (_potionCtrl)
	{
		case POTION_MENU:
			menuKeyInput();
			break;

		case POTION_MESSAGE:
			messageKeyInput();

		case POTION_SET_COUNT:
			setCountKeyInput();
			break;

		case POTION_SELECTMENU:
			selectMenuKeyInput();
			break;

		//case POTION_CRAFTING:
		//	break;
	}
}

void potionShop::menuKeyInput()
{
	if (_playAnim)
	{
		playCraftingAnim();
		_potionPosY -= 0.3f;
		_frameCount++;

		if (_frameCount >= 85 && _animPlayed)
		{
			_playAnim = false;
			_animPlayed = false;
			_frameCount = 0;
		}
		return;
	}

	//상하좌우 키입력 
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

	//J키 입력(포션 제조)
	if (INPUT->GetKeyDown('J'))
	{
		_cursor->setCursorState(CURSOR_CLICK);
		SOUNDMANAGER->play("cursor_move", 0.2f);

		//빈 슬롯은 건너뛰기 
		if (_cursor->getSlotIdx() >= POTION_MAXSLOT) return; 

		//필요충분조건을 확인하고 생성 가능하면 개수 선택화면으로 이동 
		if (checkRequirements())
		{
			//생성 가능한 최대 개수를 세팅하고 개수 선택창으로 이동 
			checkMaxProduceCount();
			setPotionCtrl(POTION_SET_COUNT);
		}
	}
}

void potionShop::messageKeyInput()
{
	if (INPUT->GetKeyDown('J') || INPUT->GetKeyDown('L'))
	{
		setPotionCtrl(POTION_MENU);
		SOUNDMANAGER->play("cursor_move", 0.2f);
	}
}

void potionShop::setCountKeyInput()
{
	//상하좌우 키 입력에 따라 만들고자 하는 개수 변경 
	if (INPUT->GetKeyDown('W'))
	{
		if(_produceCount < _maxProduceCount)_produceCount++;
		SOUNDMANAGER->play("cursor_move", 0.2f);
	}
	if (INPUT->GetKeyDown('S'))
	{
		if(_produceCount > 1) _produceCount--;
		SOUNDMANAGER->play("cursor_move", 0.2f);
	}
	if (INPUT->GetKeyDown('A'))
	{
		if (_produceCount <= 5) _produceCount = 1;
		else _produceCount -= 5;
		SOUNDMANAGER->play("cursor_move", 0.2f);
	}
	if (INPUT->GetKeyDown('D'))
	{
		if (_produceCount + 5 > _maxProduceCount) _produceCount = _maxProduceCount;
		else _produceCount += 5;
		SOUNDMANAGER->play("cursor_move", 0.2f);
	}

	//J키를 입력해서 제조 결정 
	if (INPUT->GetKeyDown('J'))
	{
		SOUNDMANAGER->play("cursor_move", 0.2f);
		
		setPotionCtrl(POTION_SELECTMENU);
	}
}

void potionShop::selectMenuKeyInput()
{
	//좌우 키 입력 시 selectIdx값 변경하기(네,아니오)
	if (INPUT->GetKeyDown('A') && _selectMenu->getSelectIdx() != SELECT_YES)
	{
		_selectMenu->setMenuState(SELECT_YES);
		_cursor->setCursorState(CURSOR_SELECT_MOVE);
		_cursor->setDestX(620);
		SOUNDMANAGER->play("cursor_move", 0.2f);
	}
	if (INPUT->GetKeyDown('D') && _selectMenu->getSelectIdx() != SELECT_NO)
	{
		_selectMenu->setMenuState(SELECT_NO);
		_cursor->setCursorState(CURSOR_SELECT_MOVE);
		_cursor->setDestX(718);
		SOUNDMANAGER->play("cursor_move", 0.2f);
	}

	if (INPUT->GetKeyDown('J'))
	{
		if (_selectMenu->getSelectIdx() == SELECT_YES)
		{
			//네
			SOUNDMANAGER->play("cursor_move", 0.2f);
			setPotionCtrl(POTION_MENU);
			_playAnim = true; 
		}
		else
		{
			//아니요 
			SOUNDMANAGER->play("cursor_move", 0.2f);
			setPotionCtrl(POTION_MENU);
		}
	}
}

void potionShop::leftKeyDown()
{
	switch (_cursor->getSlotIdx())
	{
		case 0: case 4:
			_cursor->setSlotIdx(_cursor->getSlotIdx() + 3);
			_cursor->setDestX(_cursor->getDestX() + POTION_CURSOR_DISTANCE * 3);
			break;

		default:
			_cursor->setSlotIdx(_cursor->getSlotIdx() - 1);
			_cursor->setDestX(_cursor->getDestX() - POTION_CURSOR_DISTANCE);
			break;
	}
}

void potionShop::rightKeyDown()
{
	switch (_cursor->getSlotIdx())
	{
		case 3: case 7:
			_cursor->setSlotIdx(_cursor->getSlotIdx() - 3);
			_cursor->setDestX(_cursor->getDestX() - POTION_CURSOR_DISTANCE * 3);
			break;

		default:
			_cursor->setSlotIdx(_cursor->getSlotIdx() + 1);
			_cursor->setDestX(_cursor->getDestX() + POTION_CURSOR_DISTANCE);
			break;
	}
}

void potionShop::upKeyDown()
{
	switch (_cursor->getSlotIdx())
	{
		//case 0: case 1: case 2: case 3:
		//	_cursor->setSlotIdx(_cursor->getSlotIdx() + 4);
		//	_cursor->setDestY(_cursor->getDestY() + POTION_CURSOR_DISTANCE);
		//	break;

		case 4: case 5: case 6: case 7:
			_cursor->setSlotIdx(_cursor->getSlotIdx() - 4);
			_cursor->setDestY(_cursor->getDestY() - POTION_CURSOR_DISTANCE);
			break;
	}
}

void potionShop::downKeyDown()
{
	switch (_cursor->getSlotIdx())
	{
		case 0: case 1: case 2: case 3:
			_cursor->setSlotIdx(_cursor->getSlotIdx() + 4);
			_cursor->setDestY(_cursor->getDestY() + POTION_CURSOR_DISTANCE);
			break;

		//default:
		//	_cursor->setSlotIdx(_cursor->getSlotIdx() - 4);
		//	_cursor->setDestY(_cursor->getDestY() - POTION_CURSOR_DISTANCE);
		//	break;
	}
}

void potionShop::bannerTextRender()
{
	//상호명 '나무 모자' 출력 
	RECT txtRC = RectMake(538, 30, 126, 40);
	HFONT font = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
		0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("JejuGothic"));
	HFONT oFont = (HFONT)SelectObject(getMemDC(), font);
	SetTextColor(getMemDC(), RGB(236, 221, 182));
	DrawText(getMemDC(), "나무 모자", -1, &txtRC, DT_CENTER | DT_WORDBREAK | DT_VCENTER);
	SelectObject(getMemDC(), oFont);
	DeleteObject(font);
}

void potionShop::potionMenuRender()
{
	_shopBanner->render(getMemDC());
	_potionMenu->render(getMemDC());
	_eKeyIcon->render(getMemDC());
}

void potionShop::potionMenuTextRender()
{
	//포션메뉴 '물약 목록:' 출력  
	RECT txtRC1 = RectMake(425, 132, 92, 30);
	HFONT font = CreateFont(19, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
		0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("JejuGothic"));
	HFONT oFont = (HFONT)SelectObject(getMemDC(), font);
	SetTextColor(getMemDC(), RGB(81, 75, 64));
	DrawText(getMemDC(), "물약 목록:", -1, &txtRC1, DT_CENTER | DT_WORDBREAK | DT_VCENTER);
	SelectObject(getMemDC(), oFont);
	DeleteObject(font);

	//포션메뉴 '레시피' 출력  
	RECT txtRC2 = RectMake(890, 372, 60, 30);
	oFont = (HFONT)SelectObject(getMemDC(), font);
	SetTextColor(getMemDC(), RGB(50, 50, 50));
	DrawText(getMemDC(), "레시피", -1, &txtRC2, DT_CENTER | DT_WORDBREAK | DT_VCENTER);
	SelectObject(getMemDC(), oFont);
	DeleteObject(font);
}

void potionShop::potionInfoRender()
{
	if (_cursor->getSlotIdx() >= POTION_MAXSLOT) return;

	potionIconRender();
	potionNameRender();
	potionDesRender();
	potionPriceRender();
	possessCountRender();
}

void potionShop::potionIconRender()
{
	_potionSlot[_cursor->getSlotIdx()].item.getItemImg()->render(getMemDC(), 1104, 80);
}

void potionShop::potionNameRender()
{
	RECT txtRC = RectMake(960, 24, 144, 24);
	HFONT font = CreateFont(19, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
		0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("JejuGothic"));
	HFONT oFont = (HFONT)SelectObject(getMemDC(), font);
	SetTextColor(getMemDC(), RGB(81, 75, 64));
	DrawText(getMemDC(), _potionSlot[_cursor->getSlotIdx()].item.getName(),
		-1, &txtRC, DT_CENTER | DT_WORDBREAK | DT_VCENTER);
	SelectObject(getMemDC(), oFont);
	DeleteObject(font);
}

void potionShop::potionDesRender()
{
	for (int i = 0; i < 2; i++)
	{
		RECT txtRC = RectMake(914, 180 + (i * 28), 260, 36);
		HFONT font = CreateFont(19, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
			0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("JejuGothic"));
		HFONT oFont = (HFONT)SelectObject(getMemDC(), font);
		SetTextColor(getMemDC(), RGB(223, 209, 181));
		DrawText(getMemDC(), _potionSlot[_cursor->getSlotIdx()].description[i],
			-1, &txtRC, DT_LEFT | DT_WORDBREAK | DT_VCENTER);
		SelectObject(getMemDC(), oFont);
		DeleteObject(font);
	}
}

void potionShop::potionPriceRender()
{
	for (int i = 1, distance = 0; i <= _potionSlot[_cursor->getSlotIdx()].price; i *= 10)
	{
		int number = (_potionSlot[_cursor->getSlotIdx()].price / i) % 10;

		char keyName[16];
		wsprintf(keyName, "%d_black", number);
		IMAGEMANAGER->render(keyName, getMemDC(), 1120 - (distance * 12), 378);

		distance++;
	}
}

void potionShop::possessCountRender()
{
	RECT txtRC = RectMake(914, 146, 44, 24);
	HFONT font = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
		0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("JejuGothic"));
	HFONT oFont = (HFONT)SelectObject(getMemDC(), font);
	SetTextColor(getMemDC(), RGB(220, 206, 180));
	DrawText(getMemDC(), "소지:", -1, &txtRC, DT_LEFT | DT_WORDBREAK | DT_VCENTER);
	SelectObject(getMemDC(), oFont);
	DeleteObject(font);

	int multiplyCount = 0;
	int potionCount = ITEMMENU->getInventory()->getCountByIdx(
		_potionSlot[_cursor->getSlotIdx()].item.getItemIdx());
	int count = potionCount;

	while (true)
	{
		if (count / 10 != 0)
		{
			count /= 10;
			multiplyCount++;
		}
		else break;
	}

	for (int i = 1; i <= potionCount; i *= 10)
	{
		int number = (potionCount / i) % 10;

		char keyName[16];
		wsprintf(keyName, "%d", number);
		IMAGEMANAGER->render(keyName, getMemDC(), 962 + (multiplyCount * 12), 146);

		multiplyCount--;
	}
}

void potionShop::recipeInfoRender()
{
	if (_cursor->getSlotIdx() >= POTION_MAXSLOT ||
		_potionSlot[_cursor->getSlotIdx()].type == POTION_BUY) return;

	_potionSlot[_cursor->getSlotIdx()].mixRecipe->render(getMemDC(), 892, 430);
}

void potionShop::setCountRender()
{
	//카운트 세팅 메뉴 렌더 
	IMAGEMANAGER->render("menu_potionMake", getMemDC(), 396, 236);
	setCountTextRender();

	//카운트 세팅 화살표 렌더 
	setCountIconRender();

	//카운트 세팅 아이템 렌더 
	_potionSlot[_cursor->getSlotIdx()].item.getItemImg()->render(getMemDC(), 470, 354);

	//카운트 렌더 
	countRender(_produceCount, 505, 394, COLOR_BLACK);
	totalPriceRender();
}

void potionShop::setCountTextRender()
{
	//'몇 개나 필요하세요?' 출력  
	RECT txtRC1 = RectMake(414, 254, 174, 24);
	HFONT font = CreateFont(19, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
		0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("JejuGothic"));
	HFONT oFont = (HFONT)SelectObject(getMemDC(), font);
	SetTextColor(getMemDC(), RGB(224, 205, 169));
	DrawText(getMemDC(), "몇 개나 필요하세요?", -1, &txtRC1, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	SelectObject(getMemDC(), oFont);
	DeleteObject(font);

	//'필요:' 출력  
	RECT txtRC2 = RectMake(604, 304, 46, 24);
	DrawText(getMemDC(), "필요:", -1, &txtRC2, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	SelectObject(getMemDC(), oFont);
	DeleteObject(font);
}

void potionShop::setCountIconRender()
{
	if (_produceCount > 1)
	{
		IMAGEMANAGER->frameRender("potion_arrow", getMemDC(), 439, 372, 0, 0);
		IMAGEMANAGER->frameRender("icon_five", getMemDC(), 424, 356, 0, 0);
	}

	if (_produceCount < _maxProduceCount)
	{
		IMAGEMANAGER->frameRender("potion_arrow", getMemDC(), 487, 324, 1, 0);
		IMAGEMANAGER->frameRender("icon_five", getMemDC(), 545, 356, 1, 0);
	}
}

void potionShop::produceCountRender()
{

}

void potionShop::totalPriceRender()
{
	int totalPrice = _produceCount * _potionSlot[_cursor->getSlotIdx()].price;

	countRender(totalPrice, 769, 330, COLOR_WHITE);
}

void potionShop::countRender(int count, int destX, int destY, COLOR_TYPE color)
{
	for (int i = 1, distance = 0; i <= count; i *= 10)
	{
		int number = (count / i) % 10;

		char keyName[16];

		switch (color)
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

void potionShop::selectMenuRender()
{
	//말풍선 출력 
	IMAGEMANAGER->render("shop_bubble", getMemDC(), 328, 120);
	selectMenuTextRender();

	selectMenuAnimRender();
}

void potionShop::selectMenuTextRender()
{
	char tempStr[72];
	strcpy(tempStr, _potionSlot[_cursor->getSlotIdx()].item.getName());
	strcat(tempStr, "을(를) 만들고 싶어?");

	RECT txtRC = RectMake(352, 168, 448, 54);
	HFONT font = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
		0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("JejuGothic"));
	HFONT oFont = (HFONT)SelectObject(getMemDC(), font);
	SetTextColor(getMemDC(), RGB(230, 215, 187));
	DrawText(getMemDC(), tempStr,
		-1, &txtRC, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	SelectObject(getMemDC(), oFont);
	DeleteObject(font);
}

void potionShop::selectMenuAnimRender()
{
	if (_selectMenu->getSelectIdx() == SELECT_YES)
	{
		IMAGEMANAGER->frameRender("select_no", getMemDC(), 730, 150, 0, 0);
		_selectMenu->render(getMemDC(), 632, 150);
	}
	else
	{
		IMAGEMANAGER->frameRender("select_yes", getMemDC(), 632, 150, 0, 0);
		_selectMenu->render(getMemDC(), 730, 150);
	}
}

void potionShop::potionCraftRender()
{
	//포션 그림자 띄우기 
	IMAGEMANAGER->render("potion_shadow", getMemDC(), 586, 512);

	//제작한 포션개수 출력 
	countRender(_produceCount, 616, 518, COLOR_WHITE);

	IMAGEMANAGER->findImage(_potionSlot[_cursor->getSlotIdx()].item.getName())->stretchRender(
		getMemDC(), 602, (int)_potionPosY, 1.5f);

	//애니메이션 출력 
	if (_animation->getAniState() == ANIMATION_PLAY) 
		_animation->render(getMemDC(), 418, 314);
}





