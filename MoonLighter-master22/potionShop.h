#pragma once
#include "fadeManager.h"
#include "movingObject.h"
#include "selectMenu.h"
#include "animation.h"
#include "cursor.h"
#include "recipe.h"
#include "gameNode.h"
#define POTION_MENUPOSX			76
#define POTION_MENUPOSY			18
#define POTION_BANNERPOSX		400
#define POTION_BANNERPOSY		0
#define POTION_EKEYPOSX			1212
#define POTION_EKEYPOSY			320
#define POTION_CURSORX			476
#define POTION_CURSORY			182
#define POTION_CURSOR_DISTANCE	74
#define POTION_MAXSLOT			4
#define POTION_MAXPRODUCE		99

//포션 제조타입(구입,직접제조)
enum POTION_TYPE
{
	POTION_BUY,
	POTION_CRAFT
};

//포션메뉴 컨트롤러 
enum POTION_CTRL
{
	POTION_MENU,
	POTION_MESSAGE,
	POTION_SET_COUNT,
	POTION_SELECTMENU
};

//포션메뉴 슬롯 구조체 
typedef struct
{
	gameItem item;						//제조 할 아이템
	POTION_TYPE type;					//제조 유형(완제품 구매/재료로 제조)
	int price;							//제조 비용
	const char* description[2];			//아이템 설명
	recipe *mixRecipe;					//제조 레시피
	int slotIdx;						//슬롯의 인덱스

}potionSlot;

class potionShop : public gameNode
{
private:
	fadeManager *_fadeManager;
	animation *_animation;
	movingObject *_potionMenu;
	movingObject *_shopBanner;
	movingObject *_eKeyIcon;
	selectMenu *_selectMenu;
	cursor *_cursor;

	potionSlot _potionSlot[POTION_MAXSLOT];
	POTION_CTRL _potionCtrl;
	MESSAGE_TYPE _messageType;

private:
	bool _menuOn;			//메뉴 on/off 여부 확인용 변수 
	bool _openMenu;			//메뉴 열기 동작 실행여부 
	bool _closeMenu;		//메뉴 닫기 동작 실행여부 
	bool _showBannerTxt;	//상점배너 텍스트 출력여부 
	bool _animPlayed;		//애니메이션 플레이 1회 시키기 
	bool _playAnim;			//애니메이션이 현재 플레이 중인지 확인 

	int _produceCount;			//제조 할 아이템 개수 
	int _maxProduceCount;		//인벤토리의 빈 공간을 참조 해서 생성 가능한 최대 아이템 개수 
	float _potionPosY;			//제조 시 변경시켜줄 포션 위치 
	int _frameCount;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	//get함수 
	bool getMenuOn() { return _menuOn; }

	//set함수 

	//포션상점 열고닫기 함수 
	void toggleMenu();					//테스트용 메뉴 on/off 키 입력을 받는 함수 
	void initMenuPos();					//각 메뉴의 위치를 초기화하는 함수
	void openPotionMenu();				//포션상점 메뉴 열기 함수 
	void closePotionMenu();				//포션상점 메뉴 닫기 함수 
	void openMenu();					//메뉴 열기 동작 함수 
	void closeMenu();					//메뉴 닫기 동작 함수 

	//포션메뉴 관련 함수 
	void initPotionSlot();					//포션메뉴 슬롯 초기화 함수(최초 1회 실행)
	void initCursor();						//커서 인덱스 및 위치 초기화(상점 오픈마다 실행)
	void initPotionShop();					//포션상점 변수값 초기화 
	void setPotionCtrl(POTION_CTRL state);	//포션상점 컨트롤러 설정 
	bool checkRequirements();				//포션 제조에 필요한 요건이 충족되는지 여부 확인
	void checkMaxProduceCount();			//생성 가능한 최대 아이템 개수 확인 
	void playCraftingAnim();				//포션 제조 애니메이션 실행 
	void applyCraftingResult();

	//키입력 함수 
	void keyInput();					//전체 키입력 함수 
	void menuKeyInput();				//메뉴 컨트롤러 키입력 함수 
	void messageKeyInput();				//메시지 컨트롤러 키입력 함수 
	void setCountKeyInput();			//카운트 세팅 컨트롤러 키입력 함수 
	void selectMenuKeyInput();			//네,아니오 선택메뉴 컨트롤러 키입력 함수 
	void leftKeyDown();					//왼쪽 키(A) 입력 함수 
	void rightKeyDown();				//오른쪽 키(D) 입력 함수 
	void upKeyDown();					//위쪽 키(W) 입력 함수 
	void downKeyDown();					//아래쪽 키(S) 입력 함수 

	//렌더 함수(메뉴 및 포션정보) 
	void bannerTextRender();			//상점이름 '나무 모자' 출력 
	void potionMenuRender();			//포션제조 메뉴 관련 출력
	void potionMenuTextRender();		//포션메뉴 텍스트 출력(물약 목록, 레시피) 
	void potionInfoRender();			//현재 커서가 가리키는 포션 정보 전체 출력 
	void potionIconRender();			//커서가 가리키는 포션 아이콘 출력 
	void potionNameRender();			//커서가 가리키는 포션 이름 출력 
	void potionDesRender();				//커서가 가리키는 포션 설명 출력 
	void potionPriceRender();			//커서가 가리키는 포션의 제조비용 출력 
	void possessCountRender();			//커서가 가리키는 포션의 소지개수 출력 
	void recipeInfoRender();			//커서가 가리키는 레시피 정보 출력 

	//렌더 함수(메시지)
	void printMessage();				//필요요건 관련 메시지 띄우기 
	void printLackOfGold();				//골드 부족 메시지 띄우기 
	void printLackOfMaterial();			//재료 부족 메시지 띄우기 
	void printLackOfBoth();				//골드와 재료 부족 메시지 띄우기 
	void printLackOfRoom();				//공간 부족 메시지 띄우기 

	//렌더 함수(제작 개수 세팅)
	void setCountRender();				//제작개수 세팅 렌더 전체 함수  
	void setCountTextRender();			//제작개수 세팅 텍스트 렌더 
	void setCountIconRender();			//제작개수 화살표(커서) 출력 
	void produceCountRender();			//제작개수(숫자 이미지) 렌더 
	void totalPriceRender();			//제작개수*제작금액 = 총 제작금액 렌더 
	void countRender(int count, int destX, int destY, COLOR_TYPE color);//공통 숫자 이미지 출력 함수 

	//렌더 함수(네/아니오 선택메뉴)
	void selectMenuRender();			//네,아니오 선택메뉴 렌더 전체 함수 
	void selectMenuTextRender();		//선택메뉴 텍스트 렌더 
	void selectMenuAnimRender();		//선택메뉴 아이콘 애니메이션 렌더 

	//렌더 함수(포션 제작)
	void potionCraftRender();			//포션 제작 렌더 전체 함수 

};

