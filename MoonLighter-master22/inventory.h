#pragma once
#include "itemManager.h"
#include "cursor.h"
#include "selectMenu.h"
#define INVENPOSX	120
#define INVENPOSY	86
#define MAXSLOT		31
#define MAXITEMSLOT	20

//인벤토리 슬롯별 타입정의 
enum SLOT
{
	SLOT_ITEM,
	SLOT_WEAPON_LEFT,
	SLOT_WEAPON_RIGHT,
	SLOT_HELMET,
	SLOT_ARMOR,
	SLOT_BOOTS,
	SLOT_RING_UP,
	SLOT_RING_DOWN,
	SLOT_POTION,
	SLOT_MIRROR,
	SLOT_PENDANT,
	SLOT_EMBLEM
};

//인벤토리 컨트롤러 타입정의
enum INVEN_CTRL
{
	INVEN_INVENTORY,
	INVEN_MERCHANT_MIRROR,
	INVEN_MERCHANT_PENDANT,
	INVEN_MERCHANT_EMBLEM
};

//슬롯 구조체 
typedef struct
{
	SLOT slotType;
	ITEM type;
	int slotIdx;
	bool isEmpty;

}slotUnit;

class inventory
{
private:
	typedef vector<gameItem*> vInven;
	typedef vector<gameItem*>::iterator viInven;

	vInven _vInven;
	viInven _viInven;

private:
	itemManager *_itemManager;				//아이템 매니저 클래스 
	selectMenu *_selectMenu;				//선택메뉴 클래스 
	cursor *_cursor;						//커서 클래스 

	gameItem _tempItem;						//임시 게임아이템 구조체 
	gameItem _itemEmpty;					//비어있는 아이템 구조체 
	gameItem _itemGrabbed;					//현재 잡고있는 아이템 구조체 

	slotUnit _invenSlot[MAXSLOT];			//인벤토리 슬롯용 구조체 
	INVEN_CTRL _invenCtrl;					//인벤토리 컨트롤러 
	POINT _invenPos;						//인벤토리메뉴 위치 

private:
	float _menuMoveSpeed;					//메뉴가 이동하는 속도 
	int _curWeaponIdx;						//현재 착용 중인 무기 인덱스 
	int _grabTime;							//커서가 모든 아이템을 잡는 클릭시간 
	int _itemCount;							//현재 소지하고 있는 아이템 개수 

	bool _isGrabbingItem;					//현재 아이템을 잡고있는지 확인하는 변수 
	bool _isPuttingItem;					//현재 아이템을 놓고있는지 확인하는 변수 
	bool _grabSoundPlayed;					//잡는 사운드가 플레이되었는지 확인하는 변수(중복 출력 금지)
	bool _canGrab;							//메뉴창을 닫을 대 잡기버그 방지 변수 

public:
	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	//get함수 
	vector<gameItem*> getItem() { return _vInven; }			//인벤토리 아이템 반환 
	itemManager* getItemManager();							//아이템매니저 클래스 반환 
	cursor *getCursor() { return _cursor; }					//인벤토리 커서 반환 
	int getWeaponIdx() { return _curWeaponIdx; }			//현재의 무기인덱스 값을 반환 
	int getMaxItemSlot() { return MAXITEMSLOT; }			//인벤토리에 소지 가능한 최대 아이템 개수 반환 
	int getCurItemCount();									//현재 인벤토리에 소지하고 있는 아이템 수 반환
	gameItem getPotionEquipped();							//현재 장착하고 있는 포션 아이템 클래스를 반환 
	gameItem getWeaponEquipped();							//현재 장착하고 있는 무기 아이템 클래스를 반환 
	int getCountByIdx(int itemIdx);									//특정 아이템의 소지개수를 인덱스를 값을 통해 찾아서 반환 

	//set함수 
	void setWeaponIdx(int index) { _curWeaponIdx = index; }	//장착하는 무기의 인덱스 값을 설정하는 함수 
	void setCanGrab(bool value) { _canGrab = value; }

	//인벤토리메뉴 이동 관련 함수 
	POINT getInvenPos() { return _invenPos; }				//인벤토리 위치값 get함수
	void setInvenPosX(int posX) { _invenPos.x = posX; }		//인벤토리 x좌표 set함수
	void setInvenPosY(int posY) { _invenPos.y = posY; }		//인벤토리 y좌표 set함수 
	void initPos();											//인벤토리 위치값 초기화 
	void moveInvenLeft(int destPos);						//인벤토리를 destPos까지 좌측으로 이동시킴
	void moveInvenRight(int destPos);						//인벤토리를 destPos까지 우측으로 이동시킴

	//인벤토리 초기화 및 아이템 획득 관련 함수 
	void initInven();						//인벤토리 창을 열때마다 초기화하는 함수 
	void initInvenSlot();					//인벤토리 슬롯 설정값 초기화 	
	void initItemSlot();					//아이템 슬롯 초기화(전부 비우기)
	void initItem();						//아이템 초기화(임의로 아이템을 넣어놓음)  
	bool addItemToInven(gameItem item);		//던전에서 획득한 아이템을 인벤토리에 추가 
	void deleteInvenItem();					//인벤토리 슬롯에 있는 아이템들 전부 제거 
	void syncWithShopInven(vector<gameItem*> vShopInven);	//상점의 인벤토리와 동기화하는 함수 

	//키입력 관련 함수 
	void keyInput();						//w,a,s,d(상하좌우) 키 값을 받아서 커서 움직이기 
	void setInvenCtrl(INVEN_CTRL state);	//인벤토리 컨트롤러 세팅 함수 
	void setMerchantCtrl();					//현재 커서가 가리키는 상인아이템에 맞게 컨트롤러 변경
	void invenKeyInput();					//인벤토리 컨트롤러 상태의 키 입력 
	void mirrorKeyInput();					//상인의 거울 컨트롤러 상태의 키 입력 
	void pendantKeyInput();					//상인의 펜던트 컨트롤러 상태의 키 입력 
	void emblemKeyInput();					//상인의 엠블렘 컨트롤러 상태의 키 임력
	void leftKeyDown();						//왼쪽 키 입력 시 커서의 위치값 변경 
	void rightKeyDown();					//오른쪽 키 입력 시 커서의 위치값 변경 
	void upKeyDown();						//위쪽 키 입력 시 커서의 위치값 변경 
	void downKeyDown();						//아래쪽 키 입력 시 커서의 위치값 변경
	void switchWeapon();					//무기 바꾸기(무기를 바꿀 수 있는 상황인지 확인해서 변경) 
	void switchWeaponIdx();					//현재 무기 인덱스값 바꾸기 
	void usePotionEquipped();				//현재 장착 중인 포션 사용하기 

	//아이템 잡기 함수 
	void grabItem();							//전체 아이템 잡기 함수 
	void grabOneItem();							//빈손일 때 한 개의 아이템 잡기 
	void grabAllItem();							//빈손일 때 한꺼번에 아이템 잡기 
	void grabOneMoreItem();						//아이템을 쥐고 있을 때 한 개의 아이템 더 잡기
	void putItem();								//전체 아이템 놓기 함수 
	void putItemOnEmptySlot();					//빈 슬롯에 잡고있는 아이템 놓기 
	void putItemOnOccupiedSlot();				//이미 채워진 슬롯에 잡고있는 아이템 놓기 
	void putGrabbingItem();						//잡고있는 아이템이 있다면 원래 자리로 돌려놓기 

	//렌더 함수 
	void cursorRender(HDC hdc);					//커서 출력 함수
	void invenCursorRender(HDC hdc);			//인벤토리 메뉴의 커서 출력 
	void merchantIconRender(HDC hdc);			//상인아이템의 아이콘 이미지 출력 
	void selectCursorRender(HDC hdc);			//선택 메뉴(네,아니오)의 커서 출력 
	void pendantCtrlRender(HDC hdc);			//펜던트 컨트롤러 상태에 필요한 이미지 출력 
	void emblemCtrlRender(HDC hdc);				//엠블렘 컨트롤러 상태에 필요한 이미지 출력
	void statusRender(HDC hdc);					//플레이어 스테이터스 출력 함수 
	void weaponIconRender(HDC hdc);				//무기 장착 관련 아이콘 출력 함수 
	void itemRender(HDC hdc);					//아이템 이미지 출력 함수 
	void itemGrabbedRender(HDC hdc);			//잡고있는 아이템 이미지 출력 함수 
	void itemNameRender(HDC hdc);				//인벤토리 하단에 아이템 이름을 출력하는 함수  
	void itemCountRender(HDC hdc, int count, int destX, int destY);	//아이템의 개수를 출력하는 함수 
	void statusNumRender(HDC hdc, int number, int destY);			//스테이터스 숫자 출력 함수 

};

