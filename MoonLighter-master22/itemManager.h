#pragma once
#include "csvload.h"

#define MAXITEM	16

//아이템의 종류 
enum ITEM
{
	ITEM_EMPTY,		//빈 아이템 
	ITEM_NORMAL,	//노멀 아이템 
	ITEM_WEAPON,	//무기 아이템 
	ITEM_HELMET,	//투구 아이템 
	ITEM_ARMOR,		//갑옷 아이템 
	ITEM_BOOTS,		//부츠 아이템 
	ITEM_RING,		//반지 아이템	
	ITEM_POTION		//포션 아이템 
};

//아이템의 능력치  
enum ABILITY
{
	ABILITY_HP,		//체력 
	ABILITY_ATK,	//공격력
	ABILITY_DEF,	//방어력 
	ABILITY_SPD		//스피드
};

//아이템의 가격범위 
enum PRICE_RANGE
{
	PRICE_CHEAP,		//저렴한
	PRICE_REASONABLE,	//합리적인
	PRICE_EXPENSIVE,	//비싼
	PRICE_OUTRAGEOUS	//터무니없는
};

//아이템의 인기수준 
enum POPULARITY
{
	POPULARITY_LOW = -1,		//인기 없음
	POPULARITY_AVERAGE = 0,		//평범함
	POPULARITY_HIGH = 1,		//인기 있음  
};

enum ITEMMOVESTATE
{
	ITEM_INIT,ITEM_FALL,ITEM_FOLLOW,ITEM_IDLE,ITEM_CHECKSTATE
};

typedef struct
{
	image *imgItem;			//아이템 이미지 
	image *imgName;			//아이템 이름 이미지 
	image *imgDes;			//아이템 설명 이미지 
	RECT rc;				//아이템 RECT
	ITEM type;				//아이템 종류
	const char* name;		//아이템 이름(출력 시에 사용)
	POINT itemPos;			//아이템 좌표값(던전 등에서 움직일 때 사용)
	int invenPosIdx;		//아이템 인벤토리 인덱스값(인벤토리 내에 있을 때의 위치값)
	int itemIdx;			//아이템의 인덱스값(랜덤하게 아이템을 뽑아내거나 노트북에 사용)
	int potionValue;		//아이템이 포션일 때 체력 회복값
	int ability[4];			//아이템이 장비일 때 아이템의 능력치값 (0.hp/1.atk/2.def/3.spd) 
	int priceRange[4];		//아이템의 가격 범위 (0.저렴한/1.합리적인/2.비싼/3.터무니없는)
	int price;				//아이템의 가격(상점에서 판매하는 가격)
	int popularity;			//아이템 판매 시의 인기수준(-1.인기없는/0.평범한/1.인기있는)
	int count;				//아이템 개수 
	int maxCount;			//한 칸의 슬롯에 소지 가능한 최대 아이템 개수 
	bool isObtained;		//아이템의 최초 획득여부 (false.획득하지않은/true.획득한) 
	//===========================================================================
	ITEMMOVESTATE moveState;	// 아이템 움직임상태
	int holeAlpha;				// 아이템 알파렌더
	float initSpeed;			// 통통튈때 스피드
	float initAngle;			// 통통이 각도
	bool isPop;					// 홀에 떨어진거 팝할거냐?

}itemUnit;

class gameItem
{
private:
	itemUnit _item;

public:
	//일반 아이템 초기화 
	HRESULT init(string itemKey, string nameKey, string desKey, const char* name, int itemIdx,
		int cheapPrice, int reasonablePrice, int expensivePrice, int outrageousPrice, int maxCount);
	//포션 아이템 초기화 
	HRESULT init(string itemKey, string nameKey, string desKey, const char* name, int itemIdx, int potionValue,
		int cheapPrice, int reasonablePrice, int expensivePrice, int outrageousPrice, int maxCount);
	//장비 아이템 초기화 
	HRESULT init(string itemKey, string nameKey, string desKey, const char* name,
		ITEM type, int itemIdx, int hpValue, int atkValue, int defValue, int spdValue,
		int cheapPrice, int reasonablePrice, int expensivePrice, int outrageousPrice, int maxCount);
	//클래스 복사하기 
	HRESULT init(gameItem *copyItem);

	void render(HDC hdc);
	void moveUpdate(float x, float y);				//아이템 이동 함수 
	bool checkCollision(RECT rc);					//아이템 충돌 확인 함수 

	//get함수 
	itemUnit getItem() { return _item; }
	image *getItemImg() { return _item.imgItem; }
	image *getNameImg() { return _item.imgName; }
	image *getDesImg() { return _item.imgDes; }
	RECT getRect() { return _item.rc; }
	ITEM getType() { return _item.type; }
	const char* getName() { return _item.name; }
	POINT getPos() { return _item.itemPos; }

	int& getInvenPosIdx() { return _item.invenPosIdx; }
	int& getItemIdx() { return _item.itemIdx; }
	int& getPotionValue() { return _item.potionValue; }
	int *getAbility() { return _item.ability; }
	int *getPriceRange() { return _item.priceRange; }
	int& getPrice() { return _item.price; }
	int& getPopularity() { return _item.popularity; }
	int& getCount() { return _item.count; }
	int& getMaxCount() { return _item.maxCount; }
	bool getIsObtained() { return _item.isObtained; }

	//set함수 
	void setType(ITEM type) { _item.type = type; }
	void setInvenPosIdx(int index) { _item.invenPosIdx = index; }
	void setPrice(int price) { _item.price = price; }
	void addPrice(int addValue) { _item.price += addValue; }
	void subPrice(int subValue) { _item.price -= subValue; }	
	void setCount(int count) { _item.count = count; }
	void plusOneCount() { _item.count++; }				//아이템 개수 +1
	void minusOneCount() { _item.count--; }				//아이템 개수 -1
	void setItemPos(POINT pt) { _item.itemPos = pt; }
	void setItemMoveState(ITEMMOVESTATE state) { _item.moveState = state; }
};

class itemManager
{
private:
	typedef vector<gameItem*> vItem;
	typedef vector<gameItem*>::iterator viItem;
	int _cnt;

private:
	vItem _vItem;
	unordered_map<int, ::Item> m_itemData;

public:
	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	//일반 아이템 추가 
	void addNormalItem(string itemKey, string nameKey, string desKey, const char* name, int itemIdx,
		int cheapPrice, int reasonablePrice, int expensivePrice, int outrageousPrice, int maxCount);
	//포션 아이템 추가 
	void addPotionItem(string itemKey, string nameKey, string desKey, const char* name, int itemIdx,
		int potionValue, int cheapPrice, int reasonablePrice, int expensivePrice, int outrageousPrice, int maxCount);
	//장비 아이템 추가 
	void addEquipmentItem(string itemKey, string nameKey, string desKey, const char* name,
		ITEM type, int itemIdx, int hpValue, int atkValue, int defValue, int spdValue,
		int cheapPrice, int reasonablePrice, int expensivePrice, int outrageousPrice, int maxCount);

	void csvItemDataLoad(string path, CSVITEMTYPE _type);

	//게임에 사용하는 아이템들 추가하는 함수 
	void addGameItems();
	gameItem getItemByIdx(int idx);

	//아이템 클래스 담겨있는 벡터 가져오기
	vector<gameItem*> getItem() { return _vItem; }
};

