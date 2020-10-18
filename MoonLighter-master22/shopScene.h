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

	vector <pair<int, int>> v_itemPos; //아이템위치
	vector <pair<int, int>> v_itemShadowPos;//아이템그림자위치

	RECT _desk; // 계산대
	RECT _stand; // 좌판
	void ItemPosSet(); // 아이템 위치 세팅


	RECT _itemText[4]; // 아이템 드로우 텍스트 렉트
	HFONT hFont; // 아이템 가격 폰트
	HFONT oldFont; // 아이템 가격 폰트

	animation* _cashRegister; // 책상 위 금고
	animation* _button; // 좌판 배치 버튼


	vector < pair<float, float>> v_itemMoveSpeed; // 아이템 움직임 속도, 가속도 벡터
	
	bool b[4] = { false, }; // 아이템 움직임 true false 

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

	void itemMove(); // 좌판에 설치된 아이템 움직임 함수

	void SoundUpdate(); // 사운드 업데이트

	void backGroundRender(); // 배경 및 가구 렌더함수
	void InterfaceRender(); //버튼 및 인터페이스 , 아이템 렌더 함수
};

