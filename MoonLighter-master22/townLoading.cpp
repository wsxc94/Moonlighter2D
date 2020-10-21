#include "stdafx.h"
#include "townLoading.h"

HRESULT townLoading::init()
{
	_loading = new loading;
	_loading->init();

	this->loadingImage();
	this->loadingSound();

	PLAYERDATA->setIsActivate(false);

	return S_OK;
}

void townLoading::release()
{
	if (_loading)
	{
		_loading->release();
		SAFE_DELETE(_loading);
	}
}

void townLoading::update()
{
	_loading->update();
	if (_loading->loadingDone())
	{
		SCENEMANAGER->loadScene("타운화면");
		PLAYERDATA->setIsActivate(true);
	}
}

void townLoading::render()
{
	_loading->render();
}

void townLoading::loadingImage()
{
	/*for (int i = 0; i < 2000; i++)
	{
		_loading->loadImage("test", 0, 0);
	}*/
	_loading->loadImage("tileMenu", "Images/맵툴/tileMenu.bmp", 550, 720, true, RGB(255, 0, 255));
	_loading->loadImage("townBack", "Images/맵툴/townBackground74X60.bmp", 2590, 2100);
	_loading->loadImage("gotoDungeon", "Images/던전/goToDungeon.bmp", 2380, 1995);

	//맵툴메뉴 이미지 초기화 
	_loading->loadImage("menu_size", "Images/맵툴/menu_size.bmp", 408, 490, true, RGB(255, 0, 255));
	_loading->loadImage("menu_tile", "Images/맵툴/menu_tile.bmp", 408, 490, true, RGB(255, 0, 255));
	_loading->loadImage("menu_tile_state", "Images/맵툴/menu_tile_state.bmp", 408, 490, true, RGB(255, 0, 255));
	_loading->loadImage("menu_data", "Images/맵툴/menu_data.bmp", 408, 490, true, RGB(255, 0, 255));
	_loading->loadFrameImage("menu_eraser", "Images/맵툴/menu_eraser.bmp", 88, 54, 2, 1);

	//이미지 추가
	_loading->loadFrameImage("강아지npc", "Images/npc/doberman10x4.bmp", 380, 232, 10, 4);
	_loading->loadFrameImage("마사", "Images/npc/Snot_Kid_walking(4x6).bmp", 360, 272, 6, 4);
	_loading->loadFrameImage("토모", "Images/npc/Irish_Woman_walking(4x8).bmp", 512, 320, 8, 4);
	_loading->loadFrameImage("에리스", "Images/npc/NPC_Witch_64x1.bmp", 3712, 77, 64, 1);
	_loading->loadFrameImage("모리", "Images/npc/대머리아저씨 8 X 4.bmp", 400, 328, 8, 4);
	_loading->loadFrameImage("레드먼드", "Images/npc/NPC_Irish_Dad(4x8).bmp", 512, 304, 8, 4);
	_loading->loadFrameImage("원형탈모아저씨", "Images/npc/원형아저씨8X4.bmp", 352, 304, 8, 4);

	_loading->loadFrameImage("마사일러", "Images/npc/마사일러.bmp", 233, 200, 1,1);
	_loading->loadFrameImage("토모일러", "Images/npc/토모일러.bmp", 175, 200, 1,1);
	_loading->loadFrameImage("에리스일러", "Images/npc/에리스일러.bmp", 184, 200, 1,1);
	_loading->loadFrameImage("레드먼드일러", "Images/npc/레드먼드일러.bmp", 228, 200, 1, 1);
	_loading->loadFrameImage("모리일러", "Images/npc/모리일러.bmp", 188, 200, 1, 1);

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

	_loading->loadFrameImage("분수대", "Images/맵툴/분수대9.bmp", 378, 48, 9, 1);

	_loading->loadFrameImage("주황나무", "Images/맵툴/주황나무14.bmp", 2352, 190, 14, 1);
	_loading->loadFrameImage("진한주황나무", "Images/맵툴/진한주황나무13.bmp", 1976, 162, 13, 1);
	_loading->loadFrameImage("초록나무", "Images/맵툴/초록35.bmp", 7315, 180, 35, 1);

	_loading->loadFrameImage("작은나무", "Images/맵툴/tree_small.bmp", 50, 58, 1, 1);
	_loading->loadFrameImage("왼나이테", "Images/맵툴/stump_left.bmp", 48, 42, 1, 1);
	_loading->loadFrameImage("오나이테", "Images/맵툴/stump_right.bmp", 48, 42, 1, 1);

	//던전가는길에 쓸 이미지 추가
	_loading->loadFrameImage("플레이어던전입장", "Images/플레이어/TeleportIn20X1.bmp", 1040, 70, 20, 1);
	_loading->loadFrameImage("던전가는길문", "Images/던전가는길/던전가는길문.bmp", 1280*2, 128*2, 10, 1);

}

void townLoading::loadingSound()
{
	_loading->loadSound("상점브금", "Sound/shopBGM.mp3", true, true);
	_loading->loadSound("마을브금", "Sound/townBGM.mp3", true, true);

	//아이템메뉴 사운드 
	_loading->loadSound("openInven", "Sound/UI&inventorySound/openInven.wav");
	_loading->loadSound("moveMenu", "Sound/UI&inventorySound/moveMenu.wav");
	_loading->loadSound("cursor_move", "Sound/UI&inventorySound/cursor_move.wav");
	_loading->loadSound("cursor_pick", "Sound/UI&inventorySound/cursor_pick.wav");
	_loading->loadSound("cursor_drop", "Sound/UI&inventorySound/cursor_drop.wav");
	_loading->loadSound("cursor_error", "Sound/UI&inventorySound/cursor_error.wav");

	//상점(포션,대장간) 사운드
	_loading->loadSound("witch_Open", "Sound/witch&blacksmith/witch_open.wav");

	//플레이어 사운드 
	_loading->loadSound("heal", "Sound/playerSound/heal.wav");
	_loading->loadSound("weaponChange", "Sound/playerSound/weaponChange.wav");
	_loading->loadSound("doorBGM", "Sound/door.wav");

	_loading->loadSound("상점입장0", "Sound/shopSound/shopDoorOpen0.wav");
	_loading->loadSound("상점입장1", "Sound/shopSound/shopDoorOpen1.wav");
	_loading->loadSound("문닫아", "Sound/shopSound/문닫힘.wav");

	_loading->loadSound("화살발사", "Sound/playerSound/bow.wav");			//적용
	_loading->loadSound("화살맞음", "Sound/playerSound/bow_Attack.wav");		//적용
	_loading->loadSound("화살스킬", "Sound/playerSound/bow_Charge.wav");	
	_loading->loadSound("구르기", "Sound/playerSound/roll.wav");				//적용
	_loading->loadSound("검휘두르기", "Sound/playerSound/Sword.wav");		//애매
	_loading->loadSound("플레이어걷기", "Sound/playerSound/walk.wav");		//적용
	_loading->loadSound("침대", "Sound/playerSound/침대.wav");
	_loading->loadSound("온천", "Sound/playerSound/회복_샘.wav");			//적용

	//마을 사운드
	_loading->loadSound("개소리" , "Sound/townSound/개소리.wav");

}
