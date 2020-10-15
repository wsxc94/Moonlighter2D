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

	//아이템메뉴 이미지 초기화 
	_loading->loadImage("menu_calendar", "Images/ItemMenu/menu_calendar.bmp", 1002, 592, true, RGB(255, 0, 255));
	_loading->loadImage("menu_wishlist", "Images/ItemMenu/menu_wishlist.bmp", 818, 518, true, RGB(255, 0, 255));
	_loading->loadImage("menu_inventory", "Images/ItemMenu/menu_inventory.bmp", 1044, 598, true, RGB(255, 0, 255));
	_loading->loadImage("menu_inventory_inDungeon", "Images/ItemMenu/menu_inventory_inDungeon.bmp", 1044, 598, true, RGB(255, 0, 255));
	_loading->loadImage("menu_notebook", "Images/ItemMenu/menu_notebook.bmp", 827, 574, true, RGB(255, 0, 255));
	_loading->loadImage("btn_qe", "Images/ItemMenu/btn_qe.bmp", 300, 38, true, RGB(255, 0, 255));
	_loading->loadFrameImage("toggle_calendar", "Images/ItemMenu/toggle_calendar.bmp", 92, 50, 2, 1);
	_loading->loadFrameImage("toggle_wishlist", "Images/ItemMenu/toggle_wishlist.bmp", 92, 50, 2, 1);
	_loading->loadFrameImage("toggle_inventory", "Images/ItemMenu/toggle_inventory.bmp", 92, 50, 2, 1);
	_loading->loadFrameImage("toggle_notebook", "Images/ItemMenu/toggle_notebook.bmp", 92, 50, 2, 1);
	_loading->loadFrameImage("icon_euip", "Images/ItemMenu/icon_euip.bmp", 64, 30, 2, 1);
	_loading->loadFrameImage("icon_equip_idx", "Images/ItemMenu/icon_equip_idx.bmp", 60, 30, 2, 1);
	_loading->loadFrameImage("icon_equip_idx_empty", "Images/ItemMenu/icon_equip_idx_empty.bmp", 60, 30, 2, 1);
	_loading->loadImage("nameBase", "Images/ItemMenu/nameBase.bmp", 310, 34, true, RGB(255, 0, 255));
	_loading->loadImage("grabBase", "Images/ItemMenu/grabBase.bmp", 72, 72, true, RGB(255, 0, 255));
	_loading->loadImage("item_slot", "Images/ItemMenu/item_slot.bmp", 60, 60, true, RGB(255, 0, 255));
	_loading->loadImage("inventory_selectBubble", "Images/ItemMenu/inventory_selectBubble.bmp", 220, 144, true, RGB(255, 0, 255));
	_loading->loadFrameImage("select_yes", "Images/ItemMenu/select_yes.bmp", 312, 78, 4, 1);
	_loading->loadFrameImage("select_no", "Images/ItemMenu/select_no.bmp", 312, 78, 4, 1);
	_loading->loadImage("bag_pendant", "Images/ItemMenu/bag_pendant.bmp", 70, 48, true, RGB(255, 0, 255));
	_loading->loadImage("bag_emblem", "Images/ItemMenu/bag_emblem.bmp", 68, 66, true, RGB(255, 0, 255));


	//커서 이미지 초기화 
	_loading->loadFrameImage("cursor_move", "Images/ItemMenu/cursor_move.bmp", 304, 76, 4, 1);
	_loading->loadFrameImage("cursor_click", "Images/ItemMenu/cursor_click.bmp", 380, 76, 5, 1);
	_loading->loadFrameImage("cursor_price", "Images/Shop/cursor_price.bmp", 408, 94, 2, 1);
	_loading->loadFrameImage("cursor_select", "Images/ItemMenu/cursor_select.bmp", 510, 102, 5, 1);

	//아이템 이미지 초기화 
	_loading->loadImage("richJelly", "Images/Item/item_richJelly.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("venomJelly", "Images/Item/item_venomJelly.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("crystal", "Images/Item/item_crystal.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("vine", "Images/Item/item_vine.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("teethStone", "Images/Item/item_teethStone.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("root", "Images/Item/item_root.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("ironBar", "Images/Item/item_ironBar.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("foundryRests", "Images/Item/item_foundryRests.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("brokenSword", "Images/Item/item_brokenSword.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("fabric", "Images/Item/item_fabric.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("hardenedSteel", "Images/Item/item_hardenedSteel.bmp", 48, 48, true, RGB(255, 0, 255));

	_loading->loadImage("trainingShortSword", "Images/Item/item_trainingShortSword.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("trainingBow", "Images/Item/item_trainingBow.bmp", 48, 48, true, RGB(255, 0, 255));

	_loading->loadImage("hpPotion1", "Images/Item/item_hpPotion1.bmp", 48, 48, true, RGB(255, 0, 255));
	_loading->loadImage("hpPotion2", "Images/Item/item_hpPotion2.bmp", 48, 48, true, RGB(255, 0, 255));

	//아이템 이름 이미지 초기화
	_loading->loadImage("name_richJelly", "Images/Item/name_richJelly.bmp", 160, 24);
	_loading->loadImage("name_venomJelly", "Images/Item/name_venomJelly.bmp", 160, 24);
	_loading->loadImage("name_crystal", "Images/Item/name_crystal.bmp", 160, 24);
	_loading->loadImage("name_vine", "Images/Item/name_vine.bmp", 160, 24);
	_loading->loadImage("name_teethStone", "Images/Item/name_teethStone.bmp", 160, 24);
	_loading->loadImage("name_root", "Images/Item/name_root.bmp", 160, 24);
	_loading->loadImage("name_ironBar", "Images/Item/name_ironBar.bmp", 160, 24);
	_loading->loadImage("name_foundryRests", "Images/Item/name_foundryRests.bmp", 160, 24);
	_loading->loadImage("name_brokenSword", "Images/Item/name_brokenSword.bmp", 160, 24);
	_loading->loadImage("name_fabric", "Images/Item/name_fabric.bmp", 160, 24);
	_loading->loadImage("name_hardenedSteel", "Images/Item/name_hardenedSteel.bmp", 160, 24);

	_loading->loadImage("name_trainingShortSword", "Images/Item/name_trainingShortSword.bmp", 160, 24);
	_loading->loadImage("name_trainingBow", "Images/Item/name_trainingBow.bmp", 160, 24);

	_loading->loadImage("name_hpPotion1", "Images/Item/name_hpPotion1.bmp", 160, 24);
	_loading->loadImage("name_hpPotion2", "Images/Item/name_hpPotion2.bmp", 160, 24);

	//아이템 설명 이미지 초기화 
	_loading->loadImage("des_richJelly", "Images/Item/des_richJelly.bmp", 304, 66);
	_loading->loadImage("des_venomJelly", "Images/Item/des_venomJelly.bmp", 314, 42);
	_loading->loadImage("des_crystal", "Images/Item/des_crystal.bmp", 280, 20);
	_loading->loadImage("des_vine", "Images/Item/des_vine.bmp", 292, 42);
	_loading->loadImage("des_teethStone", "Images/Item/des_teethStone.bmp", 310, 42);
	_loading->loadImage("des_root", "Images/Item/des_root.bmp", 316, 42);
	_loading->loadImage("des_ironBar", "Images/Item/des_ironBar.bmp", 312, 64);
	_loading->loadImage("des_foundryRests", "Images/Item/des_foundryRests.bmp", 278, 64);
	_loading->loadImage("des_brokenSword", "Images/Item/des_brokenSword.bmp", 276, 64);
	_loading->loadImage("des_fabric", "Images/Item/des_fabric.bmp", 306, 64);
	_loading->loadImage("des_hardenedSteel", "Images/Item/des_hardenedSteel.bmp", 270, 42);

	_loading->loadImage("des_trainingShortSword", "Images/Item/des_trainingShortSword.bmp", 286, 64);
	_loading->loadImage("des_trainingBow", "Images/Item/des_trainingBow.bmp", 306, 66);

	_loading->loadImage("des_hpPotion1", "Images/Item/des_hpPotion1.bmp", 310, 42);
	_loading->loadImage("des_hpPotion2", "Images/Item/des_hpPotion2.bmp", 304, 42);

	//인터페이스 이미지 초기화 
	_loading->loadImage("interface_key", "Images/Interface/interface_key.bmp", 174, 152, true, RGB(255, 0, 255));
	_loading->loadImage("icon_coin", "Images/Interface/icon_coin.bmp", 18, 18, true, RGB(255, 0, 255));
	_loading->loadImage("icon_goldPouch", "Images/Interface/icon_goldPouch.bmp", 70, 70, true, RGB(255, 0, 255));
	_loading->loadImage("icon_bag", "Images/Interface/icon_bag.bmp", 84, 84, true, RGB(255, 0, 255));
	_loading->loadImage("icon_hp", "Images/Interface/icon_hp.bmp", 56, 40, true, RGB(255, 0, 255));
	_loading->loadImage("hp_barBack", "Images/Interface/hp_barBack.bmp", 130, 24, true, RGB(255, 0, 255));
	_loading->loadImage("hp_barFront", "Images/Interface/hp_barFront.bmp", 130, 24, true, RGB(255, 0, 255));
	_loading->loadFrameImage("weapon_slot", "Images/Interface/weapon_slot.bmp", 212, 70, 2, 1);
	_loading->loadFrameImage("weapon_frontIdx", "Images/Interface/weapon_frontIdx.bmp", 60, 30, 2, 1);
	_loading->loadFrameImage("weapon_backIdx", "Images/Interface/weapon_backIdx.bmp", 60, 30, 2, 1);

	//상점 이미지 
	_loading->loadImage("menu_shopInventory", "Images/Shop/menu_shopInventory.bmp", 482, 544, true, RGB(255, 0, 255));
	_loading->loadImage("menu_display", "Images/Shop/menu_display.bmp", 500, 464, true, RGB(255, 0, 255));
	_loading->loadImage("arrow_price", "Images/Shop/arrow_price.bmp", 14, 46, true, RGB(255, 0, 255));

	//숫자(흰색) 이미지 초기화 
	_loading->loadImage("0", "Images/Number/0.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("1", "Images/Number/1.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("2", "Images/Number/2.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("3", "Images/Number/3.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("4", "Images/Number/4.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("5", "Images/Number/5.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("6", "Images/Number/6.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("7", "Images/Number/7.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("8", "Images/Number/8.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("9", "Images/Number/9.bmp", 10, 14, true, RGB(255, 0, 255));

	//숫자(검정) 이미지 초기화 
	_loading->loadImage("0_black", "Images/Number/0_black.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("1_black", "Images/Number/1_black.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("2_black", "Images/Number/2_black.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("3_black", "Images/Number/3_black.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("4_black", "Images/Number/4_black.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("5_black", "Images/Number/5_black.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("6_black", "Images/Number/6_black.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("7_black", "Images/Number/7_black.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("8_black", "Images/Number/8_black.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("9_black", "Images/Number/9_black.bmp", 10, 14, true, RGB(255, 0, 255));

	//숫자(초록색) 이미지 초기화 
	_loading->loadImage("0_green", "Images/Number/0_green.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("1_green", "Images/Number/1_green.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("2_green", "Images/Number/2_green.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("3_green", "Images/Number/3_green.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("4_green", "Images/Number/4_green.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("5_green", "Images/Number/5_green.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("6_green", "Images/Number/6_green.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("7_green", "Images/Number/7_green.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("8_green", "Images/Number/8_green.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("9_green", "Images/Number/9_green.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("x_green", "Images/Number/x_green.bmp", 10, 14, true, RGB(255, 0, 255));
	_loading->loadImage("0_gray", "Images/Number/0_gray.bmp", 10, 14, true, RGB(255, 0, 255));

	//작은 숫자(흰색) 이미지 초기화 
	_loading->loadImage("0_white_small", "Images/Number/0_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
	_loading->loadImage("1_white_small", "Images/Number/1_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
	_loading->loadImage("2_white_small", "Images/Number/2_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
	_loading->loadImage("3_white_small", "Images/Number/3_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
	_loading->loadImage("4_white_small", "Images/Number/4_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
	_loading->loadImage("5_white_small", "Images/Number/5_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
	_loading->loadImage("6_white_small", "Images/Number/6_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
	_loading->loadImage("7_white_small", "Images/Number/7_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
	_loading->loadImage("8_white_small", "Images/Number/8_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
	_loading->loadImage("9_white_small", "Images/Number/9_white_small.bmp", 6, 12, true, RGB(255, 0, 255));
	_loading->loadImage("slash_white_small", "Images/Number/slash_white_small.bmp", 6, 12, true, RGB(255, 0, 255));

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

	//플레이어 사운드 
	_loading->loadSound("heal", "Sound/playerSound/heal.wav");
	_loading->loadSound("weaponChange", "Sound/playerSound/weaponChange.wav");
	_loading->loadSound("doorBGM", "Sound/door.wav");

	_loading->loadSound("상점입장0", "Sound/shopSound/shopDoorOpen0.wav");
	_loading->loadSound("상점입장1", "Sound/shopSound/shopDoorOpen1.wav");
	_loading->loadSound("문닫아", "Sound/shopSound/문닫힘.wav");

	_loading->loadSound("화살발사", "Sound/playerSound/bow.wav");			//적용
	_loading->loadSound("화살맞음", "Sound/playerSound/bow_Attack.wav");	//적용
	_loading->loadSound("화살스킬", "Sound/playerSound/bow_Charge.wav");	
	_loading->loadSound("구르기", "Sound/playerSound/roll.wav");			//적용
	_loading->loadSound("검휘두르기", "Sound/playerSound/Sword.wav");		//애매
	_loading->loadSound("플레이어걷기", "Sound/playerSound/walk.wav");		//적용
	_loading->loadSound("침대", "Sound/playerSound/침대.wav");
	_loading->loadSound("온천", "Sound/playerSound/회복_샘.wav");			//적용

	
	//마을 사운드
	_loading->loadSound("개소리" , "Sound/townSound/개소리.wav");

}
