#pragma once
#include "gameNode.h"
#include "itemManager.h"
#include "cursor.h"
#define SHOPINVENPOSX	100
#define SHOPINVENPOSY	86
#define DISPLAYPOSX		658
#define DISPLAYPOSY		142
#define MAXSHOPSLOT		28

enum SHOP_CTRL
{
	CTRL_INVENTORY,
	CTRL_PRICE
};

enum COLOR_TYPE
{
	COLOR_BLACK,
	COLOR_WHITE
};

enum SHOPSLOT
{
	SLOT_INVEN,
	SLOT_DISPLAY,
	SLOT_PRICE
};

typedef struct
{
	SHOPSLOT slotType;
	ITEM type;
	int slotIdx;
	bool isEmpty;

}shopSlot;

class displayStand : public gameNode
{
private:
	typedef vector<gameItem*> vShopInven;
	typedef vector<gameItem*>::iterator viShopInven;

	vShopInven _vShopInven;
	viShopInven _viShopInven;

private:
	cursor *_cursor;
	shopSlot _shopSlot[MAXSHOPSLOT];
	SHOP_CTRL _shopCtrl;

	gameItem _tempItem;
	gameItem _itemEmpty;
	gameItem _itemGrabbed;

	gameItem _displayItem1;
	gameItem _displayItem2;
	gameItem _displayItem3;
	gameItem _displayItem4;

	POINT _shopInvenPos;
	POINT _displayPos;

	float _menuMoveSpeed;
	int _grabTime;
	int _itemPrice[MAXITEM];

	bool _menuOn;
	bool _openMenu;
	bool _closeMenu;

	bool _isGrabbingItem;
	bool _isPuttingItem;
	bool _grabSoundPlayed;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	//메뉴 열고 닫는 함수 
	void openDisplayStand();								//상점가판대 열기 
	void closeDisplayStand();								//상점가판대 닫기 
	void toggleMenu();										//테스트용 메뉴 on/off 키 입력을 받는 함수 
	void initMenuPos();										//각 메뉴의 위치를 초기화하는 함수
	void openMenu();										//메뉴 열기 함수 
	void closeMenu();										//메뉴 닫기 함수 
	void menuMoveLeft(POINT *pos, const int destPos);		//위치값을 받아 목적지로 왼쪽 이동
	void menuMoveRight(POINT *pos, const int destPos);		//위치값을 받아 목적지로 오른쪽 이동
	void menuMoveUp(POINT *pos, const int destPos);			//위치값을 받아 목적지로 위쪽 이동
	void menuMoveDown(POINT *pos, const int destPos);		//위치값을 받아 목적지로 아래쪽 이동 

	//상점 인벤토리 관련 함수 
	void initShopSlot();			//최초의 상점 슬롯 초기화 
	void initInvenSlot();			//인벤토리 슬롯 초기화(가판대를 열 때마다 실행)
	void initInvenItem();			//인벤토리 아이템 포기화(가판대를 열 때마다 실행)
	void deleteInvenItems();		//인벤토리에 있는 아이템 전부 삭제 

	//키 입력 함수 
	void setShopCtrl(SHOP_CTRL state);	//상점 컨트롤러 세팅 함수 
	void keyInput();					//전체 키 입력 관리 
	void invenKeyInput();				//인벤토리 키 입력 관리 
	void priceKeyInput();				//가격책정 키 입력 관리 
	void leftKeyDown();					//왼쪽 키(A) 입력 함수
	void rightKeyDown();				//오른쪽 키(D) 입력 함수 
	void upKeyDown();					//위쪽 키(W) 입력 함수 
	void downKeyDown();					//아래쪽 키(S) 입력 함수 

	//아이템 잡기 및 놓기 함수 
	void grabItem();
	void grabOneItem();
	void grabAllItem();
	void grabOneMoreItem();
	void putItem();
	void putItemOnEmptySlot();
	void putItemOnOccupiedSlot();

	//렌더 함수 
	void menuRender();
	void cursorRender();
	void itemRender();
	void itemGrabbedRender();
	void itemNameRender();
	void itemCountRender(int count, int destX, int destY, COLOR_TYPE colorIdx);
	void greenCountRender(int count, int destX, int destY);
	void displayCount();
};

