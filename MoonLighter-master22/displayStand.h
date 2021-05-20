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

//���� ��Ʈ�ѷ�(�κ��丮 or ����å��) 
enum SHOP_CTRL
{
	CTRL_INVENTORY,
	CTRL_PRICE
};

//���� �κ��丮 ������ Ÿ�� 
enum SHOPSLOT
{
	SLOT_INVEN,
	SLOT_DISPLAY,
	SLOT_PRICE
};

//���� ���� ����ü 
typedef struct
{
	SHOPSLOT slotType;
	ITEM type;
	int slotIdx;
	bool isEmpty;

}shopSlot;

//���� Ŀ�� ����ü 
typedef struct
{
	image *img;				//Ŀ�� �̹��� 
	int posIdx;				//Ŀ���� ��ġ �ε��� 
	int digit[MAXDIGIT];	//�ڸ��������� ���� 

}priceCursor;

class displayStand : public gameNode
{
private:
	typedef vector<gameItem*> vShopInven;
	typedef vector<gameItem*>::iterator viShopInven;

	vShopInven _vShopInven;
	viShopInven _viShopInven;

private:
	unique_ptr<fadeManager> _fadeManager;			//���̵� �Ŵ��� 
	unique_ptr<cursor> _cursor;					//�޴� �̵� Ŀ�� 
	shopSlot _shopSlot[MAXSHOPSLOT];	//���� ����ü 
	priceCursor _priceCursor;			//����å�� �� �̵� Ŀ�� 
	SHOP_CTRL _shopCtrl;				//���� ��Ʈ�ѷ�(�κ��丮/����å��)

	gameItem _itemEmpty;				//�� ������ ����ü 
	gameItem _itemGrabbed;				//��� �ִ� ������ ����ü 
	gameItem _displayItem[4];			//���� ���÷��� ������ �迭 

	POINT _shopInvenPos;				//���� �κ��丮 ��ġ 
	POINT _displayPos;					//���� ���÷��� ��ġ 

	float _menuMoveSpeed;				//�޴� �̵��ӵ� 
	int _grabTime;						//�������� ��� ���� �ð�(1�� ������/���� ������)
	int _lastPrice[MAXITEM];			//������ ����()

	bool _menuOn;						//�޴� on/off ���� Ȯ���ϴ� ���� 
	bool _openMenu;						//�޴��� ���� ������ �� �� ���θ� Ȯ���ϴ� ���� 
	bool _closeMenu;					//�޴��� �ݴ� ������ �� �� ���θ� Ȯ���ϴ� ���� 

	bool _canGrab;						//�޴� ���� �� �ߺ� Ű�Է����� ������ ��� ���� ���� ���� 
	bool _isGrabbingItem;				//���� �������� ��� �ִ��� ���θ� Ȯ���ϴ� ���� 
	bool _isPuttingItem;				//���� �������� ���� ������ �ϰ� �ִ��� Ȯ���ϴ� ���� 
	bool _grabSoundPlayed;				//�������� ��� ���尡 �÷��̵Ǿ����� Ȯ���ϴ� ����(�ߺ� ����)

public:

	displayStand() {};
	~displayStand();

	HRESULT init();
	void release();
	void update();
	void render();

	//get�Լ�
	bool getCanGrab() { return _canGrab; }

	//set�Լ� 
	void setCanGrab(bool value) { _canGrab = value; }

	//�޴� ���� �ݴ� �Լ� 
	void openDisplayStand();								//�������Ǵ� ���� 
	void closeDisplayStand();								//�������Ǵ� �ݱ� 
	void toggleMenu();										//�׽�Ʈ�� �޴� on/off Ű �Է��� �޴� �Լ� 
	void initMenuPos();										//�� �޴��� ��ġ�� �ʱ�ȭ�ϴ� �Լ�
	void openMenu();										//�޴� ���� �Լ� 
	void closeMenu();										//�޴� �ݱ� �Լ� 
	void menuMoveLeft(POINT *pos, const int destPos);		//��ġ���� �޾� �������� ���� �̵�
	void menuMoveRight(POINT *pos, const int destPos);		//��ġ���� �޾� �������� ������ �̵�
	void menuMoveUp(POINT *pos, const int destPos);			//��ġ���� �޾� �������� ���� �̵�
	void menuMoveDown(POINT *pos, const int destPos);		//��ġ���� �޾� �������� �Ʒ��� �̵� 

	//���� �κ��丮 ���� �Լ� 
	void initDisplayStand();			//������ �ʱ�ȭ �Լ� 
	void initShopInven();				//���� �κ��丮�� �ʿ��� ��� �ʱ�ȭ ����(���Ǵ븦 �������� ����)
	void initShopSlot();				//������ ���� ���� �ʱ�ȭ 
	void initInvenSlot();				//�κ��丮 ���� �ʱ�ȭ(���Ǵ븦 �� ������ ����)
	void initInvenItem();				//�κ��丮 ������ �ʱ�ȭ(���Ǵ븦 �� ������ ����)
	void deleteInvenItems();			//�κ��丮�� �ִ� ������ ���� ���� 
	gameItem findItemByIdx(int index);	//�ش� �κ��丮 �ε����� �ִ� ������ ã�� 
	gameItem *getDisplayItem();			//���Ǵ��� ���� ��ȯ 
	void deleteDisplayItem(int index);	//���Ǵ��� �ε����� �Ѱܹ޾� �ش� �ε����� �ش��ϴ� ������ ���� 
	void deleteItemByIdx(int index);	//�ε����� �޾� �ش� �ε��� ��ġ�� �ִ� ������ ���� 

	//Ű �Է� �Լ� 
	void setShopCtrl(SHOP_CTRL state);	//���� ��Ʈ�ѷ� ���� �Լ� 
	void keyInput();					//��ü Ű �Է� ���� 
	void invenKeyInput();				//�κ��丮 Ű �Է� ���� 
	void priceKeyInput();				//����å�� Ű �Է� ���� 
	void leftKeyDown();					//���� Ű(A) �Է� �Լ�
	void rightKeyDown();				//������ Ű(D) �Է� �Լ� 
	void upKeyDown();					//���� Ű(W) �Է� �Լ� 
	void downKeyDown();					//�Ʒ��� Ű(S) �Է� �Լ� 

	//������ ��� �� ���� �Լ� 
	void grabItem();					//��ü ������ ��� �Լ� 
	void grabOneItem();					//������ �� �� ��� �Լ� 
	void grabAllItem();					//��ü ������ ��� �Լ� 
	void grabOneMoreItem();				//��� �ִ� �����۰� ���� �������� ���Կ��� �� �� �� ��� �Լ� 
	void putItem();						//��ü ������ ���� �Լ� 
	void putItemOnEmptySlot();			//�� ���� ������ ���� �Լ� 
	void putItemOnOccupiedSlot();		//������� ���� ���� ������ ���� �Լ� 
	void putGrabbingItem();

	//����å�� ���� �Լ� 
	void setPriceUp();				//���� �ø���
	void setPriceDown();			//���� ������ 
	void saveLastPrice();			//�ش� �����ۿ� ���������� å���� ���ݰ� �����ϱ� 
	void loadLastPrice(int index);	//�ε��� ��ġ�� �����ۿ� ���������� å���� ���ݰ� �޾ƿ��� 

	//���� �Լ� 
	void menuRender();				//�޴� �̹��� ��� 
	void cursorRender();			//Ŀ�� �̹��� ��� 
	void priceCursorRender();		//���� Ŀ�� �̹��� ���(ȭ��ǥ)
	void itemRender();				//������ �̹��� ��� 
	void itemGrabbedRender();		//���� ��� �ִ� ������ �̹��� ��� 
	void itemNameRender();			//���� Ŀ���� ����Ű�� ������ �̸� ��� 
	void displayCountRender();		//���Ǵ뿡 �ִ� �� �������� ���� ��� 
	void unitPriceRender();			//������ �� ���� ���� ��� 
	void totalPriceRender();		//������ ���� x ������ ����� �� ���� ��� 
	void countRender(int count, int destX, int destY, COLOR_TYPE colorIdx);	//������ ���� ��� 
	void greenCountRender(int count, int destX, int destY);					//������ ���� ���(���λ�)
	void priceCountRender(int count, int destX, int destY);					//������ ���� ��� 

};

