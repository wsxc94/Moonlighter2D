#pragma once
// 'I' 버튼을 눌러서 메뉴 on/off 
// 'Q(좌)' 'E(우)' 버튼을 눌러서 메뉴 이동 
#include "calendar.h"
#include "wishList.h"
#include "inventory.h"
#include "noteBook.h"
#include "fadeManager.h"
#include "singletonBase.h"

#define TAGPOSX			490
#define TAGPOSY			12
#define TAGINITPOSY		50
//#define CALENDARPOSX	130
//#define CALENDARPOSY	92
//#define WISHLISTPOSX	232
//#define WISHLISTPOSY	130
//#define INVENPOSX		120
//#define INVENPOSY		86
//#define NOTEBOOKPOSX	230
//#define NOTEBOOKPOSY	98

enum CTRL_MENU
{
	CTRL_CALENDAR,
	CTRL_WISHLIST,
	CTRL_INVEN,
	CTRL_NOTEBOOK
};

class itemMenu : public singletonBase <itemMenu>
{
private:
	calendar *_calendar;		//캘린더 메뉴 클래스 
	wishList *_wishList;		//위시리스트 메뉴 클래스 
	inventory *_inventory;		//인벤토리 메뉴 클래스
	noteBook *_noteBook;		//노트북 메뉴 클래스 
	fadeManager *_fadeManager;	//페이드 매니져 클래스 
	CTRL_MENU _ctrlState;		//현재 컨트롤러 상태 

	POINT _tagPos;			//상단의 태그메뉴(캘린더부터 노트북까지 나열된 메뉴)의 위치 
	int _tagIdx;			//상단의 태그메뉴 인덱스(컨트롤러에 맞추면 속도가 좀 느림) 
	float _menuMoveSpeed;	//메뉴창 이동하는 속도 
	float _tagMoveSpeed;	//태그메뉴 이동하는 속도 

private:
	bool _menuOn;			//메뉴창 toggle용 변수 
	bool _calendarOn;		//캘린더메뉴 랜더용 toggle 변수
	bool _wishListOn;		//위시리스트메뉴 랜더용 toggle 변수
	bool _invenOn;			//인벤토리메뉴 랜더용 toggle 변수 
	bool _noteBookOn;		//노트북메뉴 랜더용 toggle 변수 

	bool _openMenu;			//메뉴창 열기용 변수 
	bool _closeMenu;		//메뉴창 닫기용 변수 
	bool _openTagMenu;		//태그메뉴 열기용 변수
	bool _closeTagMenu;		//태그메뉴 닫기용 변수 
	bool _canKeyInput;		//키 입력 받기여부 변수 
	bool _movingLeft;		//메뉴가 왼쪽으로 움직이는지 확인하는 변수 
	bool _movingRight;		//메뉴가 오른쪽으로 움직이는지 확인하는 변수 

	bool _goToTown_Pentant;		//상인의 펜던트 사용여부 확인(마을로 돌아가기)
	bool _goToTown_Emblem;

public:
	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	//get함수 
	inventory *getInventory() { return _inventory; }				//인벤토리 클래스 가져오기 
	bool getInvenOn() { return _invenOn; }
	int getMaxItemSlot() { return _inventory->getMaxItemSlot(); }	//소지 가능한 최대 아이템 개수 반환
	bool getCanKeyInput() { return _canKeyInput; }					//아이템 메뉴 키 입력 받기변수 가져오기 
	bool getGoToTownPendant() { return _goToTown_Pentant; }
	bool getGoToTownEmblem() { return _goToTown_Emblem; }
	
	//set함수 
	void setCurItemCount();			//소지하고 있는 아이템 개수를 데이터에 업데이트 
	void setPotionEquipped();		//현재 장착 중인 포션 아이템을 데이터에 업데이트
	void setWeaponIdx();			//현재 장착 상태인 무기 인덱스를 데이터에 업데이트
	void setWeaponEquipped();		//현재 장착 중인 무기 아이템을 데이터에 업데이트 
	void setCanKeyInput(bool value) { _canKeyInput = value; }	//아이템 메뉴 키 입력받기 변수 설정
	void setGoToTownPendant(bool value) { _goToTown_Pentant = value; }
	void SetGoToTownEmblem(bool value) { _goToTown_Emblem = value; }

	void toggleMenu();						//메뉴창 껐다 켰다하기 
	void DoCloseMenu();						//메뉴창을 닫기를 실행하는 함수(외부에서 접근해서 끄기 가능)
	void openMenu();						//메뉴창 열기 동작
	void closeMenu();						//메뉴창 닫기 동작 
	void openTagMenu();						//태그메뉴 열기 동작
	void closeTagMenu();					//태그메뉴 닫기 동작 
	void initMenuPos();						//모든메뉴 위치 초기화 
	void setCtrlState(CTRL_MENU state);		//컨트롤러상태 셋팅 
	void keyInput();						//키 입력을 받아서 메뉴창 컨트롤
	void leftKeyDown();						//왼쪽(Q) 키 입력  
	void rightKeyDown();					//오른쪽(E) 키 입력
	void ctrlCalendarInput();				//컨트롤러가 캘린더 상태일 때 키 입력값에 따른 이동 
	void ctrlWishListInput();				//컨트롤러가 위시리스트 상태일 때 키 입력값에 따른 이동 
	void ctrlInvenInput();					//컨트롤러가 인벤토리 상태일 때 키 입력값에 따른 이동 
	void ctrlNoteBookInput();				//컨트롤러가 노트북 상태일 때 키 입력값에 따른 이동
	void menuMoveLeft(POINT *pos, const int destPos);
	void menuMoveRight(POINT *pos, const int destPos);

	//랜더함수
	void menuRender(HDC hdc);				//아이템메뉴 랜더 
	void tagMenuRender(HDC hdc);			//상단의 태그메뉴 랜더 
	bool& getOpenMenu() { return _menuOn; }

};

