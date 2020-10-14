#pragma once
#include "gameNode.h"
#include "itemManager.h"
#include "cursor.h"
#define SHOPINVENPOSX	100
#define SHOPINVENPOSY	86
#define DISPLAYPOSX		658
#define DISPLAYPOSY		142
#define MAXSHOPSLOT		28
#define MAXDIGIT		7
#define MAXPRICE		9999999

//상점 컨트롤러(인벤토리 or 가격책정) 
enum SHOP_CTRL
{
	CTRL_INVENTORY,
	CTRL_PRICE
};

//숫자 이미지 색상 
enum COLOR_TYPE
{
	COLOR_BLACK,
	COLOR_WHITE
};

//상점 인벤토리 슬롯의 타입 
enum SHOPSLOT
{
	SLOT_INVEN,
	SLOT_DISPLAY,
	SLOT_PRICE
};

//상점 슬롯 구조체 
typedef struct
{
	SHOPSLOT slotType;
	ITEM type;
	int slotIdx;
	bool isEmpty;

}shopSlot;

//가격 커서 구조체 
typedef struct
{
	image *img;				//커서 이미지 
	int posIdx;				//커서의 위치 인덱스 
	int digit[MAXDIGIT];	//자리수마다의 숫자 

}priceCursor;

class displayStand : public gameNode
{
private:
	typedef vector<gameItem*> vShopInven;
	typedef vector<gameItem*>::iterator viShopInven;

	vShopInven _vShopInven;
	viShopInven _viShopInven;

private:
	cursor *_cursor;					//메뉴 이동 커서 
	shopSlot _shopSlot[MAXSHOPSLOT];	//슬롯 구조체 
	priceCursor _priceCursor;			//가격책정 시 이동 커서 
	SHOP_CTRL _shopCtrl;				//상점 컨트롤러(인벤토리/가격책정)

	gameItem _itemEmpty;				//빈 아이템 구조체 
	gameItem _itemGrabbed;				//잡고 있는 아이템 구조체 
	gameItem _displayItem[4];			//상점 디스플레이 아이템 배열 

	POINT _shopInvenPos;				//상점 인벤토리 위치 
	POINT _displayPos;					//상점 디스플레이 위치 

	float _menuMoveSpeed;				//메뉴 이동속도 
	int _grabTime;						//아이템을 잡는 기준 시간(1개 잡을지/전부 잡을지)
	int _lastPrice[MAXITEM];			//아이템 가격()

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
	void initShopSlot();				//최초의 상점 슬롯 초기화 
	void initInvenSlot();				//인벤토리 슬롯 초기화(가판대를 열 때마다 실행)
	void initInvenItem();				//인벤토리 아이템 초기화(가판대를 열 때마다 실행)
	void deleteInvenItems();			//인벤토리에 있는 아이템 전부 삭제 
	gameItem findItemByIdx(int index);	//해당 인벤토리 인덱스에 있는 아이템 찾기 
	gameItem getDisplayOne();			//가판대의 물건1 반환
	gameItem getDisplayTwo();			//가판대의 물건2 반환 
	gameItem getDisplayThree();			//가판대의 물건3 반환 
	gameItem getDisplayFour();			//가판대의 물건4 반환 
	gameItem *getDisplayItem();			//가판대의 물건 반환 

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
	void grabItem();					//전체 아이템 잡기 함수 
	void grabOneItem();					//아이템 한 개 잡기 함수 
	void grabAllItem();					//아이템 전체 잡기 함수 
	void grabOneMoreItem();				//잡고 있는 아이템과 같은 아이템을 슬롯에서 한 개 더 잡는 함수 
	void putItem();						//전체 아이템 놓기 함수 
	void putItemOnEmptySlot();			//빈 곳에 아이템 놓기 함수 
	void putItemOnOccupiedSlot();		//비어있지 않은 곳에 아이템 놓기 함수 

	//가격책정 관련 함수 
	void setPriceUp();				//가격 올리기
	void setPriceDown();			//가격 내리기 
	void saveLastPrice();			//해당 아이템에 마지막으로 책정한 가격값 저장하기 
	void loadLastPrice(int index);	//인덱스 위치의 아이템에 마지막으로 책정한 가격값 받아오기 

	//렌더 함수 
	void menuRender();				//메뉴 이미지 출력 
	void cursorRender();			//커서 이미지 출력 
	void priceCursorRender();		//가격 커서 이미지 출력(화살표)
	void itemRender();				//아이템 이미지 출력 
	void itemGrabbedRender();		//현재 잡고 있는 아이템 이미지 출력 
	void itemNameRender();			//현재 커서가 가리키는 아이템 이름 출력 
	void displayCountRender();		//가판대에 있는 각 아이템의 숫자 출력 
	void unitPriceRender();			//아이템 한 개당 가격 출력 
	void totalPriceRender();		//아이템 가격 x 개수를 계산한 총 가격 출력 
	void countRender(int count, int destX, int destY, COLOR_TYPE colorIdx);	//아이템 카운트 출력 
	void greenCountRender(int count, int destX, int destY);					//아이템 카운트 출력(연두색)
	void priceCountRender(int count, int destX, int destY);					//아이템 가격 출력 

};

