#pragma once
#include "gameNode.h"
#include "ShopNpcManager.h"
#include "displayStand.h"

#define ITEMDESKCOUNT 4 //좌판 개수

class shopScene : public gameNode
{
private:
	ShopNpcManager* _npc; // npc매니저
	displayStand* _displayStand; //아이템 좌판 정보

	RECT GoTownPortal; // 마을로가는 포탈

	bool _visit; //플레이어 방문여부
	bool _disMenuOn; // 좌판메뉴가 켜져있나 안켜져있나
	int _doorTime;
	vector <pair<int, int>> v_itemPos; //아이템위치
	vector <pair<int, int>> v_itemShadowPos;//아이템그림자위치

	vector < pair<float, float>> v_itemMoveSpeed; // 아이템 움직임 속도, 가속도 벡터
	
	vector<bool> b; // 아이템 움직임 true false 

	RECT _desk; // 계산대
	RECT _stand; // 좌판
	RECT _doorRect; // 문열고 닫는 렉트

	bool _shopDoorCheck; 
	bool _doorOpen;
	void ItemPosSet(); // 아이템 위치 세팅
	bool _isShopOpen;

	RECT _itemText[4]; // 아이템 드로우 텍스트 렉트
	HFONT hFont; // 아이템 가격 폰트
	HFONT oldFont; // 아이템 가격 폰트


	unique_ptr<animation> _cashRegister; // 책상 위 금고
	unique_ptr<animation> _button; // 좌판 배치 버튼
	unique_ptr<animation> _door; // 문
	unique_ptr<animation> _cauldron; //꿀단지
	unique_ptr<animation> _sellButton; // 판매 버튼

	bool _sellNpcCheck;
	bool _pixelBackGround;


public:
	HRESULT init();
	void release();
	void update();
	void render();

	void PortaltoTown(); // 마을행 포탈
	void PlayerCol(); // 플레이어 픽셀충돌 함수 
	void PlayerSell(); // 아이템 파는 함수
	void itemInfoUpdate(); // 좌판의 아이템정보를 검사해 npc를 초기화시키는 함수

	void npcAI(); // npc update 여기다 넣음
	void npcInit(int idx); //npc 초기화함수
	void doorOpen(); //문여는 조건 함수
	void doorPlay(); //문 애니메이션 사운드 ON 함수
	void doorReverseplay(); // 문 애니메이션 반대로

	void sellStandAction(); // 좌판 액션
	void sellDeskAction(); // 계산대 액션

	void itemMove(); // 좌판에 설치된 아이템 움직임 함수

	void SoundUpdate(); // 사운드 업데이트

	void backGroundRender(); // 배경 및 가구 렌더함수
	void InterfaceRender(); //버튼 및 인터페이스 , 아이템 렌더 함수
};

