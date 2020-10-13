#include "stdafx.h"
#include "maptoolLoading.h"

HRESULT maptoolLoading::init()
{
	//로딩클래스 초기화
	_loading = new loading;
	_loading->init();

	//로딩 이미지 및 사운드 초기화
	this->loadingImage();
	this->loadingSound();

	return S_OK;
}

void maptoolLoading::release()
{
	//로딩클래스 해제
	if (_loading)
	{
		_loading->release();
		SAFE_DELETE(_loading);
	}
}

void maptoolLoading::update()
{
	//로딩클래스 업데이트
	_loading->update();

	//로딩완료후 화면 변경
	if (_loading->loadingDone())
	{
		SCENEMANAGER->loadScene("맵툴화면");
	}

}

void maptoolLoading::render()
{
	//로딩클래스 렌더
	_loading->render();

}

void maptoolLoading::loadingImage()
{
	_loading->loadImage("tileMenu", "Images/맵툴/tileMenu.bmp", 550, 720, true, RGB(255, 0, 255));
	_loading->loadImage("townBack", "Images/맵툴/townBackground74X60.bmp", 2590, 2100);
	_loading->loadImage("dungeonBack", "Images/맵툴/dungeon_default37X21.bmp", 1280, 720);
	_loading->loadImage("gotoDungeon", "Images/던전/goToDungeon.bmp", 2380, 1995);

	//맵툴메뉴 이미지 초기화 
	_loading->loadImage("menu_size", "Images/맵툴/menu_size.bmp", 408, 490, true, RGB(255, 0, 255));
	_loading->loadImage("menu_tile", "Images/맵툴/menu_tile.bmp", 408, 490, true, RGB(255, 0, 255));
	_loading->loadImage("menu_tile_state", "Images/맵툴/menu_tile_state.bmp", 408, 490, true, RGB(255, 0, 255));
	_loading->loadImage("menu_data", "Images/맵툴/menu_data.bmp", 408, 490, true, RGB(255, 0, 255));
	_loading->loadFrameImage("menu_eraser", "Images/맵툴/menu_eraser.bmp", 88, 54, 2, 1);

	//이미지 추가
	_loading->loadFrameImage("강아지npc", "Images/npc/doberman10x4.bmp", 380, 232, 10, 4);
	_loading->loadFrameImage("마사", "Images/npc/Snot_Kid_walking(4x6).bmp", 360, 272, 6, 4); //마사
	_loading->loadFrameImage("토모", "Images/npc/Irish_Woman_walking(4x8).bmp", 512, 320, 8, 4); // 토모
	_loading->loadFrameImage("에리스", "Images/npc/NPC_Witch_64x1.bmp", 3712, 77, 64, 1); // 에리스
	_loading->loadFrameImage("모리", "Images/npc/대머리아저씨 8 X 4.bmp", 400, 328, 8, 4); // 모리
	_loading->loadFrameImage("레드먼드", "Images/npc/NPC_Irish_Dad(4x8).bmp", 512, 304, 8, 4); //레드먼드
	_loading->loadFrameImage("원형탈모아저씨", "Images/npc/원형아저씨8X4.bmp", 352, 304, 8, 4);

	/*_loading->loadFrameImage("빨랫줄집", "Images/맵툴/빨랫줄집.bmp", 7840, 181, 32, 1);
	_loading->loadFrameImage("눈있는집", "Images/맵툴/눈있는집.bmp", 201, 200, 1, 1);
	_loading->loadFrameImage("주인공집", "Images/맵툴/주인공집40.bmp", 9800, 200, 40, 1);
	_loading->loadFrameImage("포션상점", "Images/맵툴/포션상점.bmp", 208, 264, 1, 1);
	_loading->loadFrameImage("노란지붕집", "Images/맵툴/노란지붕집.bmp", 231, 210, 1, 1);
	_loading->loadFrameImage("판자집", "Images/맵툴/판자집.bmp", 159, 209, 1, 1);
	_loading->loadFrameImage("상점보자기", "Images/맵툴/상점보자기.bmp", 2774, 150, 40, 1);
	_loading->loadFrameImage("던전돌", "Images/맵툴/던전돌.bmp", 35, 35, 1, 1);
	_loading->loadFrameImage("던전돌2", "Images/맵툴/던전돌2.bmp", 35, 35, 1, 1);
	_loading->loadFrameImage("던전돌3", "Images/맵툴/던전돌3.bmp", 35, 35, 1, 1);
	_loading->loadFrameImage("던전돌4", "Images/맵툴/던전돌4.bmp", 35, 35, 1, 1);
	_loading->loadFrameImage("던전돌5", "Images/맵툴/던전돌5.bmp", 35, 35, 1, 1);
	_loading->loadFrameImage("나무뿌리", "Images/맵툴/나무뿌리.bmp", 105, 84, 1, 1);
	_loading->loadFrameImage("나무뿌리2", "Images/맵툴/나무뿌리2.bmp", 84, 126, 1, 1);
	_loading->loadFrameImage("던전텐트", "Images/맵툴/던전텐트.bmp", 280, 210, 1, 1);
	_loading->loadFrameImage("던전책", "Images/맵툴/던전책.bmp", 35, 35, 1, 1);
	_loading->loadFrameImage("던전의자", "Images/맵툴/던전의자.bmp", 70, 70, 1, 1);
	_loading->loadFrameImage("던전통", "Images/맵툴/던전통.bmp", 35, 35, 1, 1);
	_loading->loadFrameImage("던전보물상자", "Images/맵툴/던전보물상자14.bmp", 420, 38, 14, 1);*/

	_loading->loadFrameImage("빨랫줄집", "Images/맵툴/빨랫줄집.bmp", 15680, 362, 32, 1);
	_loading->loadFrameImage("눈있는집", "Images/맵툴/눈있는집.bmp", 304, 340, 1, 1);
	_loading->loadFrameImage("주인공집", "Images/맵툴/주인공집40.bmp", 19600, 400, 40, 1);
	_loading->loadFrameImage("해먹집", "Images/맵툴/해먹집.bmp", 410, 340, 1, 1);
	_loading->loadFrameImage("이건무슨집", "Images/맵툴/ratailer.bmp", 546, 458, 1, 1);
	_loading->loadFrameImage("포션상점", "Images/맵툴/potionShop.bmp", 498, 688, 1, 1);
	_loading->loadFrameImage("노란지붕집", "Images/맵툴/노란지붕집.bmp", 462, 420, 1, 1);
	_loading->loadFrameImage("판자집", "Images/맵툴/판자집.bmp", 318, 418, 1, 1);
	_loading->loadFrameImage("상점보자기", "Images/맵툴/상점보자기.bmp", 2774, 150, 40, 1);
	_loading->loadFrameImage("던전돌", "Images/맵툴/던전돌.bmp", 35, 35, 1, 1);
	_loading->loadFrameImage("던전돌2", "Images/맵툴/던전돌2.bmp", 35, 35, 1, 1);
	_loading->loadFrameImage("던전돌3", "Images/맵툴/던전돌3.bmp", 35, 35, 1, 1);
	_loading->loadFrameImage("던전돌4", "Images/맵툴/던전돌4.bmp", 35, 35, 1, 1);
	_loading->loadFrameImage("던전돌5", "Images/맵툴/던전돌5.bmp", 35, 35, 1, 1);
	_loading->loadFrameImage("나무뿌리", "Images/맵툴/나무뿌리.bmp", 105, 84, 1, 1);
	_loading->loadFrameImage("나무뿌리2", "Images/맵툴/나무뿌리2.bmp", 84, 126, 1, 1);
	_loading->loadFrameImage("던전텐트", "Images/맵툴/던전텐트.bmp", 280, 210, 1, 1);
	_loading->loadFrameImage("던전책", "Images/맵툴/던전책.bmp", 35, 35, 1, 1);
	_loading->loadFrameImage("던전의자", "Images/맵툴/던전의자.bmp", 70, 70, 1, 1);
	_loading->loadFrameImage("던전통", "Images/맵툴/던전통.bmp", 35, 35, 1, 1);
	_loading->loadFrameImage("던전보물상자", "Images/맵툴/던전보물상자14.bmp", 420, 38, 14, 1);



	_loading->loadFrameImage("해골전사", "Images/몬스터/skullHammerMove14X4.bmp", 2982, 688, 14, 4);

	// 새로 추가한 것들
	_loading->loadFrameImage("던전타일", "Images/맵툴/던전타일.bmp", 175, 175, SAMPLETILEX, SAMPLETILEY);
	_loading->loadFrameImage("잔디타일", "Images/맵툴/잔디타일.bmp", 175, 175, SAMPLETILEX, SAMPLETILEY);
	_loading->loadFrameImage("쩌리타일", "Images/맵툴/쩌리타일2.bmp", 175, 175, SAMPLETILEX, SAMPLETILEY);

	_loading->loadFrameImage("의자", "Images/맵툴/bench.bmp", 102, 52, 1, 1);
	_loading->loadFrameImage("대각선의자", "Images/맵툴/bench_diagonal.bmp", 100, 100, 1, 1);
	_loading->loadFrameImage("대장간", "Images/맵툴/blacksmith.bmp", 588, 506, 1, 1);
	_loading->loadFrameImage("게시판", "Images/맵툴/board.bmp", 178, 116, 1, 1);
	_loading->loadFrameImage("고장난분수", "Images/맵툴/fountain.bmp", 82, 102, 1, 1);
	_loading->loadFrameImage("울타리집", "Images/맵툴/울타리집.bmp", 486, 454, 1, 1);

	_loading->loadFrameImage("가스총맨", "Images/맵툴/gas_suit_12.bmp", 696, 77, 12, 1);
	_loading->loadFrameImage("집샘플", "Images/맵툴/objectHouse.bmp", 250, 250, 2, 2);
	_loading->loadFrameImage("나무샘플", "Images/맵툴/objectTree.bmp", 250, 250, 2, 2);
	_loading->loadFrameImage("건물샘플", "Images/맵툴/object2.bmp", 250, 250, 2, 2);
	_loading->loadFrameImage("건물샘플2", "Images/맵툴/object3.bmp", 250, 250, 2, 2);
	_loading->loadFrameImage("건물샘플3", "Images/맵툴/object4.bmp", 250, 250, 2, 2);

	_loading->loadFrameImage("던전샘플1", "Images/맵툴/dungeonObj.bmp", 250, 250, 2, 2);
	_loading->loadFrameImage("던전샘플2", "Images/맵툴/dungeonObj2.bmp", 250, 250, 2, 2);
	_loading->loadFrameImage("던전샘플3", "Images/맵툴/dungeonObj3.bmp", 250, 250, 2, 2);
	_loading->loadFrameImage("몬스터샘플", "Images/맵툴/sampleMonster.bmp", 250, 250, 2, 2);
	_loading->loadFrameImage("몬스터샘플2", "Images/맵툴/sampleMonster2.bmp", 250, 250, 2, 2);

	_loading->loadFrameImage("NPC샘플", "Images/맵툴/objectNPC.bmp", 250, 250, 2, 2);
	_loading->loadFrameImage("NPC샘플2", "Images/맵툴/objectNPC2.bmp", 250, 250, 2, 2);
	_loading->loadFrameImage("분수대", "Images/맵툴/분수대9.bmp", 378, 48, 9, 1);

	_loading->loadFrameImage("주황나무", "Images/맵툴/주황나무14.bmp", 2352, 190, 14, 1);
	_loading->loadFrameImage("진한주황나무", "Images/맵툴/진한주황나무13.bmp", 1976, 162, 13, 1);
	_loading->loadFrameImage("초록나무", "Images/맵툴/초록35.bmp", 7315, 180, 35, 1);

	_loading->loadFrameImage("작은나무", "Images/맵툴/tree_small.bmp", 50, 58, 1, 1);
	_loading->loadFrameImage("왼나이테", "Images/맵툴/stump_left.bmp", 48, 42, 1, 1);
	_loading->loadFrameImage("오나이테", "Images/맵툴/stump_right.bmp", 48, 42, 1, 1);

	_loading->loadImage("아래스크롤버튼", "Images/맵툴/scrolldown.bmp", 18, 10, true, RGB(255, 0, 255));
	_loading->loadImage("위스크롤버튼", "Images/맵툴/scrollup.bmp", 18, 10, true, RGB(255, 0, 255));
	_loading->loadImage("arrow_up", "Images/맵툴/arrow_up.bmp", 18, 10, true, RGB(255, 0, 255));
	_loading->loadImage("arrow_down", "Images/맵툴/arrow_down.bmp", 18, 10, true, RGB(255, 0, 255));
	_loading->loadImage("scrollBar", "Images/맵툴/scrollBar.bmp", 10, 47, true, RGB(255, 0, 255));

	_loading->loadFrameImage("babySlime", "Images/몬스터/babySlime9x4(2).bmp", 1152, 512, 9, 4);
	_loading->loadFrameImage("bigSlime", "Images/몬스터/BigSlime12x2.bmp", 984, 526, 12, 2);
	_loading->loadFrameImage("골렘공격", "Images/몬스터/GolemAttack13x4.bmp", 2470, 760, 13, 4);
	_loading->loadFrameImage("골렘전사", "Images/몬스터/GolemMove8x4.bmp", 1520, 760, 8, 4);
	_loading->loadFrameImage("항아리", "Images/몬스터/pot11x4.bmp", 519, 235, 11, 4);
	
	_loading->loadSound("맵툴브금", "Sound/shopBGM.mp3", true, true);
	






}

void maptoolLoading::loadingSound()
{
}
